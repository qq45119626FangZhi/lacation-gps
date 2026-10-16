#ifndef __INT_AT6558R_H__
#define __INT_AT6558R_H__

#include "stm32f1xx_hal.h"

void Int_AT6558R_Init(void);
void Int_AT6558R_GetGPS(uint8_t *gps);

void Int_AT6558R_Enter_LowPower(void);
void Int_AT6558R_Exit_LowPower(void);

#endif /* __INT_AT6558R_H__ */
