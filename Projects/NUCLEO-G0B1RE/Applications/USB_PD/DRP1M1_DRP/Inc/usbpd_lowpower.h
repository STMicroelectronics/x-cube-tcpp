/**
  ******************************************************************************
  * @file    usbpd_lowpower.h
  * @author  MCD Application Team
  * @brief   Header file for usbpd_lowpower.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
