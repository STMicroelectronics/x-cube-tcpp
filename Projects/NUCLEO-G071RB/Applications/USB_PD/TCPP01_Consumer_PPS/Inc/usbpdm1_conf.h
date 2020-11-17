/**
  ******************************************************************************
  * @file    usbpdm1_conf.h
  * @author  MCD Application Team
  * @brief   USBPDM1 board configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Replace the header file names with the ones of the target platform */
#include "stm32g0xx_nucleo.h"
#include "stm32g0xx_ll_adc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_rcc.h"
#include "usbpdm1_errno.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBPDM1_CONF_H__
#define __USBPDM1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 1 */

/* Enable ADC clock (core clock) */
#define VSENSE_ADC_INSTANCE               ADC1

#ifdef NUCLEO_MB1360A
#define VSENSE_ADC_CHANNEL                LL_ADC_CHANNEL_9
#else
#define VSENSE_ADC_CHANNEL                LL_ADC_CHANNEL_0
#endif

/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif

#endif /* __USBPDM1_CONF_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

