/**
  ******************************************************************************
  * @file    drp1m1_bus.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for BUS hardware resources.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRP1M1_BUS_H
#define DRP1M1_BUS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "drp1m1_conf.h"
#if defined(USE_STM32G4XX_NUCLEO)
#include "stm32g4xx_nucleo_conf.h"
#include "stm32g4xx_nucleo_errno.h"
#elif defined(USE_STM32G0XX_NUCLEO)
/* STM32G0 series using BSP V1 framework, no conf template file, no error definitions, ... */
#include "stm32g0xx_nucleo.h"
#include "drp1m1_errno.h"
#else
#warning "Please select include files according to HW setup"
#endif /* USE_STM32G4XX_NUCLEO */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup DRP1M1
  * @{
  */

/** @addtogroup DRP1M1_BUS
  * @{
  */

/** @defgroup DRP1M1_BUS_Exported_Constants Exported Constants
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

/** @defgroup DRP1M1_BUS_Private_Types Private Types
  * @{
  */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
typedef struct
{
  pI2C_CallbackTypeDef  pMspInitCb;
  pI2C_CallbackTypeDef  pMspDeInitCb;
} BSP_I2C_Cb_t;
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */

/**
  * @}
  */

/** @addtogroup DRP1M1_BUS_Exported_Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup DRP1M1_BUS_Exported_Functions Exported Functions
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
int32_t BSP_I2C_RegisterDefaultMspCallbacks(void);
int32_t BSP_I2C_RegisterMspCallbacks(BSP_I2C_Cb_t *Callback);
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

#endif /* DRP1M1_BUS_H */

