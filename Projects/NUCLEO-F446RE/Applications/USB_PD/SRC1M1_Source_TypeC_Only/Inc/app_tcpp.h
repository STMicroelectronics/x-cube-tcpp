/**
 ******************************************************************************
 * @file    STMicroelectronics.X-CUBE-TCPP.1.0.0
 * @brief   APP SOURCE H file
 ******************************************************************************
 * @attention
 *
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_SOURCE_APPLICATION_H
#define APP_SOURCE_APPLICATION_H

 #ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* Exported functions --------------------------------------------------------*/
void MX_TCPP_Init(void);
void MX_TCPP_Process(void);

void USBnoPD_TCPPFaultHandling(void);
void TIM2_IRQHandler(void);
void DMA2_Stream0_IRQHandler(void);

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  USBnoPD_CC1 = 0u,
  USBnoPD_CC2
} USBnoPD_CCTypeDef;

typedef enum
{
  USBnoPD_State_DETACHED = 0u, /* IDLE, nothing connected         */
  USBnoPD_State_ATTACHING,     /* Attachment ongoing - debouncing */
  USBnoPD_State_ATTACHED,      /* Attached                        */
  USBnoPD_State_DETACHING,     /* Detachment ongoing - debouncing */
  USBnoPD_State_DISCHARGING,   /* Vbus discharge ongoing          */
  USBnoPD_State_FAULT          /* Hardware fault                  */
} USBnoPD_StatesTypeDef;

typedef enum
{
  USBnoPD_ADC_Index_CC1 = 0u,  /* CC1 index in adc buffer    */
  USBnoPD_ADC_Index_CC2,       /* CC2 index in adc buffer    */
  USBnoPD_ADC_Index_VBUSC,     /* VBus index in adc buffer   */
  USBnoPD_ADC_Index_ISENSE,    /* Isense index in adc buffer */
  USBnoPD_ADC_Index_VPROV      /* Vprov index in adc buffer  */
} USBnoPD_ADCBufIDTypeDef;

/* Exported constants --------------------------------------------------------*/
#define USBNOPD_ADC_USED_CHANNELS     5u      /* Number of used ADC channels                                   */

#define USBNOPD_CC_VOLTAGE_MAXRA      800u    /* CC line Max voltage when Ra is connected (in mV)              */
#define USBNOPD_CC_VOLTAGE_MINRD      850u    /* CC line Min voltage when connected to Rd (in mV)              */
#define USBNOPD_CC_VOLTAGE_MAXRD      2450u   /* CC line Max voltage when connected to Rd (in mV)              */
#define USBNOPD_CC_VOLTAGE_MINOPEN    2750u   /* CC line Minimum voltage when not connected (in mV)            */
#define USBNOPD_VBUS_VOLTAGE_MAX      5500u   /* Vbus Maximum allowed voltage (in mV)                          */
#define USBNOPD_VPROV_VOLTAGE_MIN     4500u   /* Vprov Minimum voltage (in mV)                                 */
#define USBNOPD_VSAFE_VOLTAGE_MAX     100u    /* Vbus safe voltage to end vbus discharge (in mV)               */

#define USBNOPD_SRC1M1_NORA           0u      /* No voltage divider on CC lines                                */
#define USBNOPD_SRC1M1_NORB           0u      /* No voltage divider on CC lines                                */

#define USBNOPD_DEBOUNCE_ATTACH_TICKS 120u    /* Number of ticks needed to complete attaching state debouncing */
#define USBNOPD_DEBOUNCE_DETACH_TICKS 10u      /* Number of ticks needed to complete detaching state debouncing*/

#define BSP_USBPD_PWR_DONT_WAIT_VBUSOFF_DISCHARGE 1u

#define GREEN_LED_GPIO_Port           GPIOA
#define GREEN_LED_Pin                 GPIO_PIN_5

#ifdef __cplusplus
}
#endif

#endif /* APP_SOURCE_APPLICATION_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
