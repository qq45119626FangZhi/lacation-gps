#define DEBUG_ENABE

#include "App_Main.h"
#include "Int_LLCC68.h"
#include "stdio.h"
#include "Int_W5500.h"

/**
 * @brief 项目工程自定义程序入口
 *
 */
void App_Main(void)
{
  printf("App_Main");
  // //LLCC68初始化
  // Int_LLCC68_Init();
  // //定义接收数据缓冲区
  // uint8_t receive_buf[100] = {0};
  // uint16_t receive_length = 0;
  // while (1) {
  //   //接收数据
  //   Int_LLCC68_Receive(receive_buf, &receive_length);
  //   if (receive_length > 0) {
  //     printf("receive_buf=%s\n", receive_buf);
  //   }
  //   //重置接收缓冲区
  //   memset(receive_buf, 0, receive_length);
  //   receive_length = 0;
  //   HAL_Delay(3000);
  // }

  //发送到电脑服务器
  Int_W5500_Init();
  while (1) {
    Int_W5500_SendData("fangzhi", 7);
    HAL_Delay(3000);
  }
}
