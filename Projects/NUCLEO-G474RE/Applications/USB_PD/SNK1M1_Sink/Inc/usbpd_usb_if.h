/**
  ******************************************************************************
  * @file    usbpd_usb_if.h
  * @author  MCD Application Team
  * @brief   This file contains the usb if API.
    ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
#ifndef __USBPD_USBIF_H_
#define __USBPD_USBIF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/** @addtogroup STM32_USBPD_USB_IF
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup USBPD_CORE_USBIF_Exported_Functions USBPD USB IF Exported Functions
  * @{
  */
    
/**
  * @brief  Initialize the usb layer
  * @param  None
  * @retval 0 if initailization is success
  */
int32_t USBPD_USBIF_Init(void);

/**
  * @brief  start the USB device
  * @param  PortNum Index of current used port
  * @retval None
  */
void USBPD_USBIF_DeviceStart(uint32_t PortNum);
/**
  * @brief  stop the USB device
  * @param  PortNum Index of current used port
  * @retval None
  */
void USBPD_USBIF_DeviceStop(uint32_t PortNum);

/**
  * @brief  start the USB host
  * @param  PortNum Index of current used port
  * @retval None
  */
void USBPD_USBIF_HostStart(uint32_t PortNum);

/**
  * @brief  stop the USB host
  * @param  PortNum Index of current used port
  * @retval None
  */
void USBPD_USBIF_HostStop(uint32_t PortNum);

/**
  * @brief  start the billboard class
  * @param  PortNum Index of current used port
  * @retval None
  */
void USBPD_USBIF_DeviceBillboard(uint32_t PortNum);

/**
  * @brief  swap from device to host
  * @param  PortNum Index of current used port
  * @retval None
  */
void USBPD_USBIF_Swap2Host(uint32_t PortNum);

/**
  * @brief  swap from host to device
  * @param  PortNum Index of current used port
  * @retval None
  */
void USBPD_USBIF_Swap2Device(uint32_t PortNum);

/**
  * @brief  update the BOS information of the device
  * @param  PortNum Index of current used port
  * @param  DataPtr pointer on the data
  * @retval None
  */
void USBPD_USBIF_DeviceSetBOSInfo(uint32_t PortNum, void *DataPtr);

/**
  * @brief  update the VDM information presented by the billboard class
  * @param  PortNum Index of current used port
  * @param  DataPtr pointer on the data
  * @retval None
  */
void USBPD_USBIF_DeviceSetVDMInfo(uint32_t PortNum, void *DataPtr);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_USBIF_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
