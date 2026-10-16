#include "Int_DS3553.h"
#include "Com_Delay.h"
#include "gpio.h"
#include "i2c.h"

#define DS3553_I2C_ADD_R 0x4F
#define DS3553_I2C_ADD_W 0x4E
#define DS3553_RED_ADD_ID 0x01
#define DS3553_RED_USER_SET 0xC3
#define DS3553_RED_SETP_CNT_L 0xC4
#define DS3553_RED_SETP_CNT_M 0xC5
#define DS3553_RED_SETP_CNT_H 0xC6

void __Int_DS3553_I2C_WriteData(uint16_t mem_adds, uint8_t data)
{
  // 片选拉低
  HAL_GPIO_WritePin(DS3553_CS_GPIO_Port, DS3553_CS_Pin, GPIO_PIN_RESET);
  Com_Delay_ms(5);
  // 读取
  HAL_I2C_Mem_Write(&hi2c1, DS3553_I2C_ADD_R, mem_adds, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
  // 片选拉高
  HAL_GPIO_WritePin(DS3553_CS_GPIO_Port, DS3553_CS_Pin, GPIO_PIN_SET);
  Com_Delay_ms(12);
}

uint8_t __Int_DS3553_I2C_ReadData(uint16_t mem_adds)
{
  uint8_t data = 0;
  // 片选拉低
  HAL_GPIO_WritePin(DS3553_CS_GPIO_Port, DS3553_CS_Pin, GPIO_PIN_RESET);
  Com_Delay_ms(5);
  // 读取
  HAL_I2C_Mem_Read(&hi2c1, DS3553_I2C_ADD_R, mem_adds, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
  // 片选拉高
  HAL_GPIO_WritePin(DS3553_CS_GPIO_Port, DS3553_CS_Pin, GPIO_PIN_SET);
  Com_Delay_ms(12);
  return data;
}

/**
 * @brief 计步器芯片 初始化 芯片8位
 * 时钟和引脚HAL已经配置，只需要配置 初始值
 */
void Int_DS3553_Init(void)
{
  uint8_t init_data = 0x18;
  init_data &= ~(4 << 1); // 第5位赋值0
  init_data |= (1 << 1);  // 第二位赋值1
  __Int_DS3553_I2C_WriteData(DS3553_RED_USER_SET, init_data);
}

/**
 * @brief 获取芯片计数步数
 * 
 */
uint32_t Int_DS3553_GetStepCount(void) {
  return __Int_DS3553_I2C_ReadData(DS3553_RED_SETP_CNT_H) << 16 |
        __Int_DS3553_I2C_ReadData(DS3553_RED_SETP_CNT_M) << 8 |
        __Int_DS3553_I2C_ReadData(DS3553_RED_SETP_CNT_L);
}

