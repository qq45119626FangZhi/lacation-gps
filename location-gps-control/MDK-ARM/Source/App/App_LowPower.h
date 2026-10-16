#ifndef __APP_LOWPOWER_H__
#define __APP_LOWPOWER_H__

#include "stm32f1xx_hal.h"
#include "Int_DS3553.h"
#include "Int_AT6558R.h"
#include "Int_QS100.h"
#include <stdio.h>
#include "Com_Debug.h"
#include "Com_Delay.h"

/**
 * @brief 进入低功耗模式
 * 
 */
void App_LowPower_Enter(void);

/**
 * @brief 退出低功耗模式
 * 
 */
void App_LowPower_Exit(void);

#endif /* __APP_LOWPOWER_H__ */

