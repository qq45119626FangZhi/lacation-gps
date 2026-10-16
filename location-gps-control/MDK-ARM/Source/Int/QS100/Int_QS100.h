#ifndef __INT_QS100_H__
#define __INT_QS100_H__

#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "string.h"
#include "Com_Debug.h"
#include "Com_Delay.h"

void Int_QS100_Init(void);
/**
 * @brief 发送数据
 * 
 */
void Int_QS100_SendData(uint8_t *data);

/**
 * @brief 进入低功耗
 * 
 */
void Int_QS100_Enter_LowPower(void);

/**
 * @brief 退出低功耗
 * 
 */
void Int_QS100_Exit_LowPower(void);

#endif /* __INT_QS100_H__ */
