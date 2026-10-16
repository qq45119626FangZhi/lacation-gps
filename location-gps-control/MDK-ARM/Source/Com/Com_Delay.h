#ifndef __COM_DELAY_H__
#define __COM_DELAY_H__

#include "stdio.h"
#include "stm32f1xx_hal.h"

void Com_Delay_us(uint16_t us);
void Com_Delay_ms(uint16_t ms);
void Com_Delay_s(uint16_t s);

#endif /* __COM_DELAY_H__ */
