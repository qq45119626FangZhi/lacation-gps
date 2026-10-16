#include "Com_Delay.h"

/**
 * @brief 延迟函数 微秒
 *
 */
void Com_Delay_us(uint16_t us)
{
  // 不准确，粗略计算
  uint16_t count = HAL_RCC_GetSysClockFreq() / 7200000 * us;
  while (count > 0) {
    __NOP();
    count--;
  }
}

/**
 * @brief 延迟函数 毫秒
 *
 */
void Com_Delay_ms(uint16_t ms) { HAL_Delay(ms); }

/**
 * @brief 延迟函数 秒
 *
 */
void Com_Delay_s(uint16_t s) { HAL_Delay(s * 1000); }
