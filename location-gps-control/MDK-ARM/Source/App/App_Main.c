#define DEBUG_ENABE

#include "App_Main.h"
#include "App_Work.h"
#include "App_LowPower.h"
#include "Int_LLCC68.h"

/**
 * @brief 项目工程自定义程序入口
 * 
 */
void App_Main(void)
{
  Com_Debug_println("App_Main");
  
  // 退出低功耗
  App_LowPower_Exit();
  //初始化工作模式
  App_Work_Init();
  //执行任务
  App_Work_Execute();
  //进入低功耗
  App_LowPower_Enter();

  // Int_LLCC68_Init();

  while (1) {
    Int_LLCC68_Send("fz123", 5);
    HAL_Delay(3000);
  }

}
