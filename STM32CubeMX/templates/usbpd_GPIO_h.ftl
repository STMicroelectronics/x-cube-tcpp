[#ftl]
/**
  ******************************************************************************
  * @file    GPIO_CONF.h
  * @author  DFD Application Team
  * @brief   Header file for TCPP_Conf.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) ${year} STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GPIO_CONF_H
#define GPIO_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
[#if includes??]
[#list includes as include]
#include "${include}"
[/#list]
[/#if]
#include "${FamilyName?lower_case}xx_ll_bus.h"
#include "${FamilyName?lower_case}xx_ll_gpio.h"
#include "${FamilyName?lower_case}xx_ll_system.h"
#include "${FamilyName?lower_case}xx_ll_adc.h"
#include "${FamilyName?lower_case}xx_ll_dma.h"
#include "${BoardName?lower_case}_bus.h"
#include "${BoardName?lower_case}_errno.h"
#include "tcpp0203.h"

/* Exported Defines ----------------------------------------------------------*/
[#assign IpInstance = ""]
[#assign IpName = ""]

[#assign TCPP0X_I2C_INSTANCE = ""]
[#assign TCPP0X_ENABLE_PORT = ""]
[#assign TCPP0X_ENABLE_PIN = ""]
[#assign TCPP0X_FLG_EXTI_PORT = ""]
[#assign TCPP0X_FLG_EXTI_PIN = ""]
[#assign TCPP0X_FLG_EXTI_IRQn = ""]
[#assign TCPP0X_FLG_EXTI_LINE = ""]

[#list BspIpDatas as SWIP] 
    [#if SWIP.variables??]
        [#list SWIP.variables as variables]
            [#if variables.name?contains("IpInstance")]
                [#assign IpInstance = variables.value]
            [/#if]
            [#if variables.name?contains("IpName")]
                [#assign IpName = variables.value]
            [/#if]
            [#if variables.name?contains("GPIO_INT_NUM")]
                [#assign IrqNumber = variables.value]
            [/#if]
            [#if variables.name?contains("EXTI_LINE_NUMBER")]
                [#assign ExtiLine = variables.value]
            [/#if]
            [#if variables.value?contains("TCPP0203 BUS IO driver")]
                [#assign TCPP0X_I2C_INSTANCE = IpInstance]
            [/#if]
            [#if variables.value?contains("TCPP0203 ENABLE")]
                [#assign TCPP0X_ENABLE_PORT = IpName]
                [#assign TCPP0X_ENABLE_PIN = IpInstance]
            [/#if]
            [#if variables.value?contains("TCPP0203 FLG")]
                [#assign TCPP0X_FLG_EXTI_PORT = IpName]
                [#assign TCPP0X_FLG_EXTI_PIN = IpInstance]
                [#assign TCPP0X_FLG_EXTI_IRQn = IrqNumber]
                [#assign TCPP0X_FLG_EXTI_LINE = ExtiLine]
            [/#if]
        [/#list]
    [/#if]
[/#list] 

#ifdef __cplusplus
extern "C" {
#endif

#define ${FamilyName?upper_case}XX

/* I2C -----------------------------------------------------------------------*/
[#if TCPP0X_I2C_INSTANCE == ""]
#error Please select an I2C
[#else]
#define TCPP0203_I2C_Init                           BSP_${TCPP0X_I2C_INSTANCE}_Init
#define TCPP0203_I2C_DeInit                         BSP_${TCPP0X_I2C_INSTANCE}_DeInit
#define TCPP0203_I2C_IsReady                        BSP_${TCPP0X_I2C_INSTANCE}_IsReady
#define TCPP0203_I2C_WriteReg                       BSP_${TCPP0X_I2C_INSTANCE}_WriteReg
#define TCPP0203_I2C_ReadReg                        BSP_${TCPP0X_I2C_INSTANCE}_ReadReg
#define TCPP0203_I2C_WriteReg16                     BSP_${TCPP0X_I2C_INSTANCE}_WriteReg16
#define TCPP0203_I2C_ReadReg16                      BSP_${TCPP0X_I2C_INSTANCE}_ReadReg16
#define TCPP0203_I2C_Send                           BSP_${TCPP0X_I2C_INSTANCE}_Send
#define TCPP0203_I2C_Recv                           BSP_${TCPP0X_I2C_INSTANCE}_Recv
#define TCPP0203_I2C_SendRecv                       BSP_${TCPP0X_I2C_INSTANCE}_SendRecv
#define TCPP0203_GetTick                            BSP_GetTick
#define BUS_I2C_POLL_TIMEOUT                        0x1000U
[/#if]

/* GPIO ----------------------------------------------------------------------*/
[#if TCPP0X_ENABLE_PORT == ""]
#error Please select a GPIO Output for Enable
[#else]
#define TCPP0203_PORT0_ENABLE_GPIO_PORT           ${TCPP0X_ENABLE_PORT}
#define TCPP0203_PORT0_ENABLE_GPIO_PIN            ${TCPP0X_ENABLE_PIN}
#define TCPP0203_PORT0_ENABLE_GPIO_DEFVALUE()     HAL_GPIO_WritePin(TCPP0203_PORT0_ENABLE_GPIO_PORT,TCPP0203_PORT0_ENABLE_GPIO_PIN,GPIO_PIN_RESET);
#define TCPP0203_PORT0_ENABLE_GPIO_SET()          HAL_GPIO_WritePin(TCPP0203_PORT0_ENABLE_GPIO_PORT,TCPP0203_PORT0_ENABLE_GPIO_PIN,GPIO_PIN_SET);
#define TCPP0203_PORT0_ENABLE_GPIO_RESET()        HAL_GPIO_WritePin(TCPP0203_PORT0_ENABLE_GPIO_PORT,TCPP0203_PORT0_ENABLE_GPIO_PIN,GPIO_PIN_RESET);
[/#if]

[#if TCPP0X_FLG_EXTI_PORT == ""]
/* TCPP0X Alarm Flag not connected to the MCU */
#define TCPP0203_PORT0_FLG_NOT_USED
[#else]
#define TCPP0203_PORT0_FLG_GPIO_PORT                ${TCPP0X_FLG_EXTI_PORT}
#define TCPP0203_PORT0_FLG_GPIO_PIN                 ${TCPP0X_FLG_EXTI_PIN}
#define TCPP0203_PORT0_FLG_EXTI_IRQN                ${TCPP0X_FLG_EXTI_IRQn}
#define TCPP0203_PORT0_FLG_EXTI_LINE                ${TCPP0X_FLG_EXTI_LINE}
#define TCPP0203_PORT0_FLG_EXTI_IRQHANDLER          ${TCPP0X_FLG_EXTI_IRQn}_IRQHandler

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
[/#if]



#ifdef __cplusplus
}
#endif
#endif /* GPIO_CONF_H */
