/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpdm1_usbpd_pwr.h
  * @author  MCD Application Team
  * @brief   Header file for usbpdm1_usbpd_pwr.c file
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USBPDM1_USBPD_PWR_H
#define USBPDM1_USBPD_PWR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbpdm1_conf.h"
#include "usbpdm1_errno.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup USBPDM1
  * @{
  */

/** @defgroup USBPDM1_USBPD_PWR_USBPD_PWR STM32G071B-DISCO USBPD POWER
  * @{
  */

/** @defgroup USBPDM1_USBPD_PWR_USBPD_PWR_Exported_Types Exported Types
  * @{
  */

/**
  * @brief  VCC feature presence
  */
#define USBPDM1_VCC_FEATURE_ENABLED     1
#define USBPDM1_DB_OUT_FEATURE_ENABLED  1
  
/**
  * @brief  Power role
  */
typedef enum
{
  POWER_ROLE_SOURCE = 0,
  POWER_ROLE_SINK,
  POWER_ROLE_DUAL
}
USBPD_PWR_PowerRoleTypeDef;

/**
  * @brief  Voltage control mode
  */
typedef enum
{
  DCDC_CTRL_MODE_UNKNOWN = 0,
  DCDC_CTRL_MODE_GPIO,
  DCDC_CTRL_MODE_PWM,
} USBPD_PWR_DCDCCtrlModeTypeDef;

/**
  * @brief  VBUS connection status
  */
typedef enum
{
  VBUS_CONNECTED = 0,
  VBUS_NOT_CONNECTED
} USBPD_PWR_VBUSConnectionStatusTypeDef;

/**
  * @}
  */

/** @defgroup USBPDM1_USBPD_PWR_USBPD_PWR_Exported_Constants Exported Constants
  * @{
  */
/**
  * @brief  Number of TypeC ports
  */
#define USBPD_PWR_INSTANCES_NBR           2u

/**
  * @brief  Type-C port identifier
  */
#define USBPD_PWR_TYPE_C_PORT_1           0u
#define USBPD_PWR_TYPE_C_PORT_2           1u

/**
  * @brief  CC pin identifier
  */
#define USBPD_PWR_TYPE_C_CC1              1u
#define USBPD_PWR_TYPE_C_CC2              2u

/**
  * @brief  VBUS disconnection threshold values (in mV)
  */
#define USBPD_PWR_HIGH_VBUS_THRESHOLD     2700u
#define USBPD_PWR_LOW_VBUS_THRESHOLD      330u
#define USBPD_PWR_VBUS_THRESHOLD_5V       2150u
#define USBPD_PWR_VBUS_THRESHOLD_9V       7000u
#define USBPD_PWR_VBUS_THRESHOLD_15V      12500u
#define USBPD_PWR_VBUS_THRESHOLD_20V      17000u
#define USBPD_PWR_VBUS_THRESHOLD_APDO     2150u

/**
  * @brief  VBUS discharge parameters
  */
#define USBPD_PWR_DISCHARGE_MARGIN        (500u)
#define USBPD_PWR_DISCHARGE_TIME            (6u)

/**
  * @brief  Calibration settings
  */
#define USBPD_PWR_DCDC_PRECISION          (40u)     /* DCDC output precision set to 40mV (Noise)*/
#define USBPD_PWR_CALIBRATION_ENABLED     (1u)
#define USBPD_PWR_CALIBRATION_DISABLED    (0u)

/**
  * @brief  Standard VBUS voltage levels
  */
#define USBPD_PWR_VBUS_5V                 5000u
#define USBPD_PWR_VBUS_9V                 9000u
#define USBPD_PWR_VBUS_15V                15000u
             
/**
  * @brief  power timeout
  */
#define USBPD_PWR_TIMEOUT_PDO             250u         /* Timeout for PDO to PDO or PDO to APDO at 250ms*/
#define USBPD_PWR_TIMEOUT_APDO            25u          /* Timeout for APDO to APDO at 25ms*/

/**
  * @brief  Invalid value set during issue with voltage setting
  */
#define USBPD_PWR_INVALID_VALUE           0xFFFFFFFFu

/**
  * @}
  */

/** @defgroup USBPDM1_USBPD_PWR_USBPD_PWR_Exported_Functions Exported Functions
  * @{
  */
/* Common functions */
int32_t BSP_USBPD_PWR_Init(uint32_t Instance);

int32_t BSP_USBPD_PWR_Deinit(uint32_t Instance);

int32_t BSP_USBPD_PWR_VBUSInit(uint32_t Instance);

int32_t BSP_USBPD_PWR_VBUSDeInit(uint32_t Instance);

int32_t BSP_USBPD_PWR_VBUSGetVoltage(uint32_t Instance, uint32_t *pVoltage);

int32_t BSP_USBPD_PWR_VBUSGetCurrent(uint32_t Instance, int32_t *pCurrent);

int32_t BSP_USBPD_PWR_SetVBUSDisconnectionThreshold(uint32_t Instance,
                                                    uint32_t VoltageThreshold);

int32_t BSP_USBPD_PWR_VCCSetState(uint32_t Instance, uint32_t State);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* USBPDM1_USBPD_PWR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
