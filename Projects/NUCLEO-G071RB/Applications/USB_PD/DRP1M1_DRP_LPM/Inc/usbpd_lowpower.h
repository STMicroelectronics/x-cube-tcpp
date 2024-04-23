/**
  ******************************************************************************
  * @file    usbpd_lowpower.h
  * @author  MCD Application Team
  * @brief   Header file for usbpd_lowpower.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef __USBPD_LOWPOWER_H_
#define __USBPD_LOWPOWER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_lpm.h"
#include "utilities_conf.h"  
/* Exported typedef ----------------------------------------------------------*/

/* Exported define -----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void USBPD_LOWPOWER_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_DPM_H_ */

