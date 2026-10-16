#include "Int_AT6558R.h"
#include "stdio.h"
#include "string.h"
#include "usart.h"
#include "gpio.h"

/**
 * @brief 封装发送得gps数据
 *
 * @param data
 */
void __Int_AT6558R_TransmitData(uint8_t *data)
{
  uint8_t xor = data[0];
  uint8_t length = strlen((char *)data);
  for (uint16_t i = 0; i < length; i++) {
    xor ^= data[i];
  }

  uint8_t cmd[100] = {0};
  sprintf((char *)cmd, "$%s*%02X\r\n", data, xor);
  HAL_UART_Transmit(&huart2, data, strlen((char *)cmd), 1000);
}

/**
 * @brief gps初始化 使用usart2通信
 *
 */
void Int_AT6558R_Init(void)
{
  //设置串口通信波特率，1=9600bps，
  __Int_AT6558R_TransmitData("PCAS01,1");
  //设置位置定位更新频率，默认1Hz/s，
  __Int_AT6558R_TransmitData("PCAS02,1000");
  //设置工作模式，1gps，2bds(北斗)
  __Int_AT6558R_TransmitData("PCAS04,1");
}

/**
 * @brief 获取gps数据
 *
 */
void Int_AT6558R_GetGPS(uint8_t *gps)
{
  uint16_t data_length = 0;
  HAL_UARTEx_ReceiveToIdle(&huart2, gps, 512, &data_length, 1000);
}

/**
 * @brief 进入低功耗
 * 
 */
void Int_AT6558R_Enter_LowPower(void){
  HAL_GPIO_WritePin(GPS_EN_GPIO_Port, GPS_EN_Pin, GPIO_PIN_RESET);
}

/**
 * @brief 退出低功耗
 * 
 */
void Int_AT6558R_Exit_LowPower(void){
  HAL_GPIO_WritePin(GPS_EN_GPIO_Port, GPS_EN_Pin, GPIO_PIN_SET);
}

