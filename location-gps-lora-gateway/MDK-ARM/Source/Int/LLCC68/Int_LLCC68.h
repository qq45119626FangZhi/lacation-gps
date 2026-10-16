#ifndef __INT_LLCC68_H__
#define __INT_LLCC68_H__

#include "stm32f1xx_hal.h"
#include "driver_llcc68_interface.h"


/**
 * @brief llcc68 lora example default definition
 */
#define LLCC68_LORA_DEFAULT_STOP_TIMER_ON_PREAMBLE      LLCC68_BOOL_FALSE                 /**< disable stop timer on preamble */
#define LLCC68_LORA_DEFAULT_REGULATOR_MODE              LLCC68_REGULATOR_MODE_DC_DC_LDO   /**< only ldo */
#define LLCC68_LORA_DEFAULT_PA_CONFIG_DUTY_CYCLE        0x02                              /**< set +17dBm power */
#define LLCC68_LORA_DEFAULT_PA_CONFIG_HP_MAX            0x03                              /**< set +17dBm power */
#define LLCC68_LORA_DEFAULT_TX_DBM                      17                                /**< +17dBm */
#define LLCC68_LORA_DEFAULT_RAMP_TIME                   LLCC68_RAMP_TIME_10US             /**< set ramp time 10 us */
#define LLCC68_LORA_DEFAULT_SF                          LLCC68_LORA_SF_9                  /**< sf9 */
#define LLCC68_LORA_DEFAULT_BANDWIDTH                   LLCC68_LORA_BANDWIDTH_125_KHZ     /**< 125khz */
#define LLCC68_LORA_DEFAULT_CR                          LLCC68_LORA_CR_4_5                /**< cr4/5 */
#define LLCC68_LORA_DEFAULT_LOW_DATA_RATE_OPTIMIZE      LLCC68_BOOL_FALSE                 /**< disable low data rate optimize */
#define LLCC68_LORA_DEFAULT_RF_FREQUENCY                480000000U                        /**< 480000000Hz */
#define LLCC68_LORA_DEFAULT_SYMB_NUM_TIMEOUT            0                                 /**< 0 */
#define LLCC68_LORA_DEFAULT_SYNC_WORD                   0x3444U                           /**< public network */
#define LLCC68_LORA_DEFAULT_RX_GAIN                     0x94                              /**< common rx gain */
#define LLCC68_LORA_DEFAULT_OCP                         0x38                              /**< 140 mA */
#define LLCC68_LORA_DEFAULT_PREAMBLE_LENGTH             12                                /**< 12 */
#define LLCC68_LORA_DEFAULT_HEADER                      LLCC68_LORA_HEADER_EXPLICIT       /**< explicit header */
#define LLCC68_LORA_DEFAULT_BUFFER_SIZE                 255                               /**< 255 */
#define LLCC68_LORA_DEFAULT_CRC_TYPE                    LLCC68_LORA_CRC_TYPE_ON           /**< crc on */
#define LLCC68_LORA_DEFAULT_INVERT_IQ                   LLCC68_BOOL_FALSE                 /**< disable invert iq */
#define LLCC68_LORA_DEFAULT_CAD_SYMBOL_NUM              LLCC68_LORA_CAD_SYMBOL_NUM_2      /**< 2 symbol */
#define LLCC68_LORA_DEFAULT_CAD_DET_PEAK                24                                /**< 24 */
#define LLCC68_LORA_DEFAULT_CAD_DET_MIN                 10                                /**< 10 */
#define LLCC68_LORA_DEFAULT_START_MODE                  LLCC68_START_MODE_WARM            /**< warm mode */
#define LLCC68_LORA_DEFAULT_RTC_WAKE_UP                 LLCC68_BOOL_TRUE                  /**< enable rtc wake up */


/**
 * @brief     lora example init
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t Int_LLCC68_Init(void);

/**
 * @brief  lora example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t Int_LLCC68_DeInit(void);

/**
 * @brief  lora example enter to the continuous receive mode
 * @return status code
 *         - 0 success
 *         - 1 enter failed
 * @note   none
 */
uint8_t Int_LLCC68_Set_Continuous_Receive_Mode(void);

// /**
//  * @brief  lora example enter to the shot receive mode
//  * @return status code
//  *         - 0 success
//  *         - 1 enter failed
//  * @note   none
//  */
// uint8_t Int_LLCC68_set_shot_receive_mode(double us);

/**
 * @brief  lora example enter to the send mode
 * @return status code
 *         - 0 success
 *         - 1 enter failed
 * @note   none
 */
uint8_t Int_LLCC68_Set_Send_Mode(void);

/**
 * @brief     lora example send lora data
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 send failed
 * @note      none
 */
uint8_t Int_LLCC68_Send(uint8_t *buf, uint16_t len);

uint8_t Int_LLCC68_Receive(uint8_t *buf, uint16_t *len);

// /**
//  * @brief      lora example run the cad
//  * @param[out] *enable points to a enable buffer
//  * @return     status code
//  *             - 0 success
//  *             - 1 run failed
//  * @note       none
//  */
// uint8_t Int_LLCC68_run_cad(llcc68_bool_t *enable);

// /**
//  * @brief      lora example get the status
//  * @param[out] *rssi points to a rssi buffer
//  * @param[out] *snr points to a snr buffer
//  * @return     status code
//  *             - 0 success
//  *             - 1 get status failed
//  * @note       none
//  */
// uint8_t Int_LLCC68_get_status(float *rssi, float *snr);

// /**
//  * @brief      lora example check packet error
//  * @param[out] *enable points to a bool value buffer
//  * @return     status code
//  *             - 0 success
//  *             - 1 check packet error failed
//  * @note       none
//  */
// uint8_t Int_LLCC68_check_packet_error(llcc68_bool_t *enable);

// /**
//  * @brief  lora example enter to the sleep mode
//  * @return status code
//  *         - 0 success
//  *         - 1 sleep failed
//  * @note   none
//  */
// uint8_t Int_LLCC68_sleep(void);

// /**
//  * @brief  lora example wake up the chip
//  * @return status code
//  *         - 0 success
//  *         - 1 wake up failed
//  * @note   none
//  */
// uint8_t Int_LLCC68_wake_up(void);



#endif /* __INT_LLCC68_H__ */
