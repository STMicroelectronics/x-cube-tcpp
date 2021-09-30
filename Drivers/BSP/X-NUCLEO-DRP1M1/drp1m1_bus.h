/**
  ******************************************************************************
  * @file    usbpdm3_bus.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for BUS hardware resources.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USBPDM3_BUS_H
#define USBPDM3_BUS_H

#ifdef __cplusplus
 extern "C" {
#endif

 /* Includes ------------------------------------------------------------------*/
#include "drp1m1_conf.h"
#if defined(USE_STM32G4XX_NUCLEO)
#include "stm32g4xx_nucleo_conf.h"
#include "stm32g4xx_nucleo_errno.h"
#elif defined(USE_STM32G0XX_NUCLEO)
/* STM32G0 serie using BSP V1 framework, no conf template file, no error definitions, ... */
#include "stm32g0xx_nucleo.h"
#include "drp1m1_errno.h"
#else
##warning "Please select include files according to HW setup"
#endif /* USE_STM32G4XX_NUCLEO */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup USBPDM3
  * @{
  */

/** @addtogroup USBPDM3_BUS
  * @{
  */

/** @defgroup USBPDM3_BUS_Exported_Constants Exported Constants
  * @{
  */

/* Additional error codes */
   /* BSP BUS error codes */
#define BSP_ERROR_BUS_TRANSACTION_FAILURE    -100
#define BSP_ERROR_BUS_ARBITRATION_LOSS       -101
#define BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE    -102
#define BSP_ERROR_BUS_PROTOCOL_FAILURE       -103

/**
  * @}
  */

/** @defgroup USBPDM3_BUS_Private_Types Private Types
  * @{
  */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
typedef struct
{
  pI2C_CallbackTypeDef  pMspInitCb;
  pI2C_CallbackTypeDef  pMspDeInitCb;
}BSP_I2C_Cb_t;
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */

/**
  * @}
  */

/** @addtogroup USBPDM3_BUS_Exported_Variables
  * @{
  */
extern I2C_HandleTypeDef hbus_i2c;
/**
  * @}
  */

/** @defgroup USBPDM3_BUS_Exported_Functions Exported Functions
  * @{
  */
HAL_StatusTypeDef MX_I2C_Init(void);
int32_t BSP_I2C_Init(void);
int32_t BSP_I2C_DeInit(void);
int32_t BSP_I2C_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C_IsReady(uint16_t DevAddr, uint32_t Trials);
int32_t BSP_GetTick(void);
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
int32_t BSP_I2C_RegisterDefaultMspCallbacks (void);
int32_t BSP_I2C_RegisterMspCallbacks (BSP_I2C_Cb_t *Callback);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

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

#endif /* USBPDM3_BUS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
