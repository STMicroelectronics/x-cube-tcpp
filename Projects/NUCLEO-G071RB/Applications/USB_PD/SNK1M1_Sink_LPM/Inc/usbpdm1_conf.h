/**
  ******************************************************************************
  * @file    usbpdm1_conf.h
  * @author  MCD Application Team
  * @brief   USBPDM1 board configuration file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Replace the header file names with the ones of the target platform */
#include "main.h"

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


