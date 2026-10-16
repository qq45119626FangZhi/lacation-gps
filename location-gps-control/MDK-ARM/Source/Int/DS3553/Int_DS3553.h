
#ifndef __INT_DS3553_H__
#define __INT_DS3553_H__

#include "stdio.h"
#include "stm32f1xx_hal.h"

void Int_DS3553_Init(void);
uint32_t Int_DS3553_GetStepCount(void);
#endif /* __INT_DS3553_H__ */
