/**
  ******************************************************************************
  * @file    usbpd_usb_if.c
  * @author  MCD Application Team
  * @brief   This file contains the usb if functions.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbpd_usb_if.h"
#include "usb_device.h"
#include "usbd_core.h"
/* USB include files ----------------------------------------------------------*/

/** @addtogroup STM32_USBPD_USBIF
  * @{
  */

/* Private enums -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
extern USBD_HandleTypeDef hUsbDeviceFS;
/**
  * @brief  Initialize billboard driver
  * @retval status
  */
int32_t USBPD_USBIF_Init(void)
{
  return 0;
}

/** @addtogroup USBPD_CORE_USBIF_Exported_Functions
  * @{
  */
void USBPD_USBIF_DeviceStart(uint32_t PortNum)
{
  if (USBD_Start(&hUsbDeviceFS) != USBD_OK) 
  {
    Error_Handler();
  }
}

void USBPD_USBIF_DeviceStop(uint32_t PortNum)
{
  USBD_Stop(&hUsbDeviceFS);
}

void USBPD_USBIF_HostStart(uint32_t PortNum)
{
}

void USBPD_USBIF_HostStop(uint32_t PortNum)
{
}

void USBPD_USBIF_DeviceBillboard(uint32_t PortNum)
{
}

void USBPD_USBIF_Swap2Host(uint32_t PortNum)
{
}

void USBPD_USBIF_Swap2Device(uint32_t PortNum)
{
}

void USBPD_USBIF_DeviceSetBOSInfo(uint32_t PortNum, void *DataPtr)
{
}

void USBPD_USBIF_DeviceSetVDMInfo(uint32_t PortNum, void *DataPtr)
{
}

/**
  * @}
  */

/** @addtogroup USBPD_CORE_USBIF_Private_Functions
  * @{
  */

/**
  * @}
  */

