/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd.c
  * @author  MCD Application Team
  * @brief   This file contains the device define.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbpd.h"
#if defined(_GUI_INTERFACE)
#include "gui_api.h"
#endif /* _GUI_INTERFACE */

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/* USER CODE BEGIN 1 */
/* USER CODE END 1 */

/* Private variables ---------------------------------------------------------*/
const uint8_t HWBoardVersionName[] = "STM32G071-NUCLEO";
const uint8_t PDTypeName[] = "MB1360A";

/* Private functions ---------------------------------------------------------*/
static const uint8_t*          GetHWBoardVersionName(void);
static const uint8_t*          GetPDTypeName(void);

/* USER CODE BEGIN 2 */
/* USER CODE END 2 */

/* USBPD init function */
void MX_USBPD_Init(void)
{

  /* Global Init of USBPD HW */
  USBPD_HW_IF_GlobalHwInit();

  /* Initialize the Device Policy Manager */
  if (USBPD_OK != USBPD_DPM_InitCore())
  {
    while(1);
  }

#if defined(_GUI_INTERFACE)
  /* Initialize GUI before retrieving PDO from RAM */
  GUI_Init(GetHWBoardVersionName, GetPDTypeName, HW_IF_PWR_GetVoltage, HW_IF_PWR_GetCurrent);
#endif /* _GUI_INTERFACE */

  /* Initialise the DPM application */
  if (USBPD_OK != USBPD_DPM_UserInit())
  {
    while(1);
  }

  /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */

  if (USBPD_OK != USBPD_DPM_InitOS())
  {
    while(1);
  }

  /* USER CODE BEGIN EnableIRQ */
  /* Enable IRQ which has been disabled by FreeRTOS services */
  __enable_irq();
  /* USER CODE END EnableIRQ */

}
/**
  * @brief  This method returns HW board version name
  * @retval HW Board version name
  */
static const uint8_t* GetHWBoardVersionName(void)
{
  return HWBoardVersionName;
}

/**
  * @brief  This method returns HW PD Type name
  * @retval HW Board version name
  */
static const uint8_t* GetPDTypeName(void)
{
  return PDTypeName;
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @}
  */

/**
  * @}
  */
