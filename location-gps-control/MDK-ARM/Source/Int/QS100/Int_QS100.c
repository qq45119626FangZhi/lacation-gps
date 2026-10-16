#include "Int_QS100.h"
#include "gpio.h"
#include "usart.h"

#define SERVER_IP "115.120.239.161"
#define SERVER_PORT 23399

#define AT_CMD_OK 0
#define AT_CMD_ERR_POWERON 1
#define AT_CMD_ERR_SIM 2
#define AT_CMD_ERR_ATTACHMENT 3
#define AT_CMD_ERR_SOCKET_CREATE 4
#define AT_CMD_ERR_SOCKET_CONNECT 5
#define AT_CMD_ERR_SEND_DATA 6
#define AT_CMD_ERR_SOCKET_CLOSE 7
#define AT_CMD_ERR_ENTER_LOWPOWER 8

// 分段接收数据的缓冲区
#define AT_CMD_ACK_SIZE 128
uint8_t at_cmd_ack_buffer[AT_CMD_ACK_SIZE] = {0};
uint16_t at_cmd_ack_buffer_length;

// 拼接当前请求的所有分段接收数据的缓冲区
#define AT_CMD_ACK_FULL_SIZE 512
uint8_t at_cmd_ack_full_buffer[AT_CMD_ACK_FULL_SIZE] = {0};
uint16_t at_cmd_ack_full_buffer_length;

/**
 * @brief 物联网芯片唤醒
 */
void __Int_QS100_WakeUp(void)
{
  HAL_GPIO_WritePin(QS100_WU_GPIO_Port, QS100_WU_Pin, GPIO_PIN_SET);
  Com_Delay_ms(5);
  HAL_GPIO_WritePin(QS100_WU_GPIO_Port, QS100_WU_Pin, GPIO_PIN_RESET);
  Com_Delay_ms(5);
}

/**
 * @brief 发送命令
 *
 * @param at_cmd at命令
 */
void __Int_QS100_SendCMD(uint8_t *at_cmd)
{
  // 清空缓存区
  memset(at_cmd_ack_full_buffer, 0, AT_CMD_ACK_FULL_SIZE);
  at_cmd_ack_full_buffer_length = 0;
  // 发送指令
  HAL_UART_Transmit(&huart3, at_cmd, strlen((char *)at_cmd), 1000);
}

/**
 * @brief 接收分段数据包，并将每段数据包，填充到大的缓冲区
 */
uint8_t __Int_QS100_FillBuffer()
{
  HAL_UARTEx_ReceiveToIdle(&huart3, at_cmd_ack_buffer, AT_CMD_ACK_SIZE, &at_cmd_ack_buffer_length, 1000);
  if (at_cmd_ack_buffer_length > 0) {
    // 将接收到的数据拷贝到缓冲区, 缓冲区起始位置每次后移
    memcpy(at_cmd_ack_full_buffer + at_cmd_ack_full_buffer_length, at_cmd_ack_buffer, at_cmd_ack_buffer_length);
    at_cmd_ack_full_buffer_length += at_cmd_ack_buffer_length;
    // 清空每次接收的数据
    memset(at_cmd_ack_buffer, 0, at_cmd_ack_buffer_length);
    at_cmd_ack_buffer_length = 0;
    return 1;
  }
  return 0;
}

/**
 * @brief 检测接收数据的命令是否有效
 * @return uint8_t 0查找命令失败，非0有效
 */
uint8_t __Int_QS100_CheckReceiveCMD(char *cmd) { return strstr((char *)at_cmd_ack_full_buffer, cmd) != NULL; }

/**
 * @brief 软复位
 *
 * @return uint8_t
 */
uint8_t __Int_QS100_ATCMD_SoftReset(void)
{
  __Int_QS100_SendCMD("AT+RB\r\n");
  while (1) {
    // 接收返回数据
    if (!__Int_QS100_FillBuffer()) {
      continue;
    }
    // 判断指令
    if (!__Int_QS100_CheckReceiveCMD("POWERON")) {
      continue;
    }
    if (__Int_QS100_CheckReceiveCMD("POWERON:2")) {
      if (!__Int_QS100_CheckReceiveCMD("SIMST")) {
        continue;
      }
      if (__Int_QS100_CheckReceiveCMD("SIMST:1")) {
        return AT_CMD_OK;
      } else {
        return AT_CMD_ERR_SIM;
      }
    } else {
      return AT_CMD_ERR_POWERON;
    }
  }
}

/**
 * @brief
 *
 * @param callback 执行的命令函数的回调函数
 * @param delay_ms 延迟时间 毫秒
 * @return uint8_t 状态码
 */
uint8_t __Int_QS100_CheckSendCMDState(uint8_t (*callback)(), uint16_t delay_ms)
{
  // 重试10次
  uint8_t try_count = 10;
  uint8_t status = 0;
  while (try_count > 0) {
    status = callback();
    if (status == AT_CMD_OK) {
      return AT_CMD_OK;
    }
    Com_Debug_println("__Int_QS100_CheckSendCMDState err status=%d", status);
    try_count--;
    Com_Delay_ms(delay_ms);
  }
  return status;
}

/**
 * @brief
 *
 * @param callback 执行的命令函数的回调函数
 * @param socket 客户端创建的socket id
 * @param delay_ms 延迟实际
 * @return uint8_t 状态
 */
uint8_t __Int_QS100_CheckSendCMDState1(uint8_t (*callback)(uint8_t), int8_t socket, uint16_t delay_ms)
{
  // 重试10次
  uint8_t try_count = 10;
  uint8_t status = 0;
  while (try_count > 0) {
    status = callback(socket);
    if (status == AT_CMD_OK) {
      return AT_CMD_OK;
    }
    Com_Debug_println("__Int_QS100_CheckSendCMDState1 err status=%d", status);
    try_count--;
    Com_Delay_ms(delay_ms);
  }
  return status;
}

/**
 * @brief 联网芯片初始化
 */
void Int_QS100_Init(void)
{
  Com_Debug_println("Int_QS100_Init ");
  // 开时钟，配置工作引脚,HAL自动配置
  // 软复位
  if (__Int_QS100_CheckSendCMDState(__Int_QS100_ATCMD_SoftReset, 2) != AT_CMD_OK) {
    Com_Debug_println("__Int_QS100_ATCMD_SoftReset init err ");
    return;
  }
  Com_Debug_println("__Int_QS100_ATCMD_SoftReset init succes ");
}

/**
 * QS100联网芯片 - 附着
 */
uint8_t __Int_QS100_ATCMD_Attachment(void)
{
  __Int_QS100_SendCMD("AT+CGATT?\r\n");
  while (1) {
    if (__Int_QS100_FillBuffer() && __Int_QS100_CheckReceiveCMD("OK") && __Int_QS100_CheckReceiveCMD("+CGATT")) {
      if (__Int_QS100_CheckReceiveCMD("+CGATT:1")) {
        return AT_CMD_OK;
      } else {
        return AT_CMD_ERR_ATTACHMENT;
      }
    }
  }
}

/**
 * @brief 发送命令，并返回结果
 *
 * @param at_cmd at命令
 */
uint8_t __Int_QS100_SendCMD_GetResult(uint8_t *at_cmd, uint8_t err_code)
{
  // 清空缓存区
  memset(at_cmd_ack_full_buffer, 0, AT_CMD_ACK_FULL_SIZE);
  at_cmd_ack_full_buffer_length = 0;
  // 发送指令
  HAL_UART_Transmit(&huart3, at_cmd, strlen((char *)at_cmd), 1000);
  while (1) {
    if (__Int_QS100_FillBuffer()) {
      // 判断指令的结果
      if (__Int_QS100_CheckReceiveCMD("OK")) {
        return AT_CMD_OK;
      }
      if (__Int_QS100_CheckReceiveCMD("ERROR")) {
        return err_code;
      }
    }
  }
}

/**
 * QS100联网芯片 -创建网络客户端（Socket）
 */
uint8_t __Int_QS100_ATCMD_CreateSocket(void) {
   return __Int_QS100_SendCMD_GetResult("AT+NSOCR=STREAM,6,10005,1\r\n", AT_CMD_ERR_SOCKET_CREATE); 
  }

/**
 * QS100联网芯片 - 连接服务器
 */
uint8_t __Int_QS100_ATCMD_ConnectServer(uint8_t socket)
{
  uint8_t *at_cmd_tmp = "AT+NSOCO=%d,%s,%d\r\n";
  uint8_t at_cmd[100] = {0};
  sprintf((char *)at_cmd, (char *)at_cmd_tmp, socket, SERVER_IP, SERVER_PORT);
  return __Int_QS100_SendCMD_GetResult(at_cmd, AT_CMD_ERR_SOCKET_CONNECT);
}

/**
 * QS100联网芯片 - 发送数据
 */
uint8_t __Int_QS100_ATCMD_SendData(uint8_t socket, uint8_t *data)
{
  uint16_t data_length = strlen((char *)data);
  uint16_t hex_string_data_length = data_length * 2 + 1;
  uint8_t hex_string[hex_string_data_length];
  // memset(hex_string, 0, hex_string_data_length);
  // !将数据中的每一个字符转换为16进制字符后拼接成一个完整的字符串
  for (uint16_t i = 0; i < data_length; i++) {
    sprintf((char *)&hex_string[i * 2], "%X", data[i]);
  }
  uint8_t *at_cmd_tmp = "AT+NSOSD=%d,%d,%s\r\n";
  uint8_t at_cmd[3000] = {0};
  sprintf((char *)at_cmd, (char *)at_cmd_tmp, socket, data_length, hex_string);
  return __Int_QS100_SendCMD_GetResult(at_cmd, AT_CMD_ERR_SEND_DATA);
}

/**
 * QS100联网芯片 - 关闭客户端
 */
uint8_t __Int_QS100_ATCMD_CloseSocket(uint8_t socket)
{
  uint8_t *at_cmd_tmp = "AT+NSOCL=%d\r\n";
  uint8_t at_cmd[100] = {0};
  sprintf((char *)at_cmd, (char *)at_cmd_tmp, socket);
  return __Int_QS100_SendCMD_GetResult(at_cmd, AT_CMD_ERR_SOCKET_CLOSE);
}

/**
 * @brief 发送数据
 * 
 * @param data 需要发送的字符数据
 * @param length 数据长度
 */
void Int_QS100_SendData(uint8_t *data)
{
  // 附着移动网络
  if (__Int_QS100_CheckSendCMDState(__Int_QS100_ATCMD_Attachment, 2000) != AT_CMD_OK) {
    Com_Debug_println("QS100 Attchment Fail !!!");
    return;
  }
  Com_Debug_println("QS100 Attchment Success !!!");

  // 创建网络客户端（Socket）
  if (__Int_QS100_CheckSendCMDState(__Int_QS100_ATCMD_CreateSocket, 1000) != AT_CMD_OK) {
    Com_Debug_println("__Int_QS100_ATCMD_CreateSocket Fail !!!");
    return;
  }
  Com_Debug_println("__Int_QS100_ATCMD_CreateSockett Success !!!");
  uint8_t socket = strchr((char *)at_cmd_ack_full_buffer, ':')[1] - 48;
  Com_Debug_println("QS100 Socket Id = %d !!!", socket);

  // 连接网络服务器
  if (__Int_QS100_CheckSendCMDState1(__Int_QS100_ATCMD_ConnectServer, socket, 1000) != AT_CMD_OK) {
    Com_Debug_println("__Int_QS100_ATCMD_ConnectServer Fail !!!");
    return;
  }
  Com_Debug_println("__Int_QS100_ATCMD_ConnectServer Success !!!");
  // 发送数据
  __Int_QS100_ATCMD_SendData(socket, data);
  // 关闭网络客户端（释放资源）
  __Int_QS100_ATCMD_CloseSocket(socket);
}

/**
 * @brief 进入低功耗
 * 
 */
void Int_QS100_Enter_LowPower(void){
  __Int_QS100_SendCMD_GetResult("AT+FASTOFF=?\r\n", AT_CMD_ERR_ENTER_LOWPOWER);
}

/**
 * @brief 退出低功耗
 * 
 */
void Int_QS100_Exit_LowPower(void){
  __Int_QS100_WakeUp();
}
