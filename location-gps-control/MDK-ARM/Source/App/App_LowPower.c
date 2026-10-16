#include "App_LowPower.h"
#include "rtc.h"

/**
 * @brief 设备进入低功耗
 * 
 */
void App_LowPower_Enter(void) {
  //联网芯片
  Int_QS100_Enter_LowPower();
  //GPS芯片
  Int_AT6558R_Enter_LowPower();

  //STM32 
  //获取当前时间
  RTC_TimeTypeDef rtc_time;
  HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
  //采用RTC闹钟唤醒，10秒后自动唤醒
  RTC_AlarmTypeDef rtc_alarm;
  rtc_alarm.AlarmTime.Hours = rtc_time.Hours;
  rtc_alarm.AlarmTime.Minutes = rtc_time.Minutes;
  rtc_alarm.AlarmTime.Seconds = rtc_time.Seconds + 10;
  HAL_RTC_SetAlarm(&hrtc, &rtc_alarm, RTC_FORMAT_BIN);
 
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
  // HAL_PWR_EnableWakeUpPin()//引脚唤醒需要配置
  HAL_PWR_EnterSTANDBYMode();
}

/**
 * @brief 设备退出低功耗模式
 * 
 */
void App_LowPower_Exit(void) {
  //联网芯片
  Int_QS100_Exit_LowPower();
  //GPS芯片
  Int_AT6558R_Exit_LowPower();
}
