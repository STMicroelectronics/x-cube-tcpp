[#ftl]
/**
  ******************************************************************************
  * @file    GPIO_SNK_CONF.h
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
  */#n  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GPIO_SNK_CONF_H
#define GPIO_SNK_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
[#if includes??]
[#list includes as include]
#include "${include}"
[/#list]
[/#if]
#include "${FamilyName?lower_case}xx_ll_gpio.h"

/* Exported Defines ----------------------------------------------------------*/
[#assign IpInstance = ""]
[#assign IpName = ""]

[#assign TCPP01_VCC_OUT_PORT = ""]
[#assign TCPP01_VCC_OUT_PIN = ""]
[#assign TCPP01_DB_OUT_PORT = ""]
[#assign TCPP01_DB_OUT_PIN = ""]

[#list BspIpDatas as SWIP] 
    [#if SWIP.variables??]
        [#list SWIP.variables as variables]
            [#if variables.name?contains("IpInstance")]
                [#assign IpInstance = variables.value]
            [/#if]
            [#if variables.name?contains("IpName")]
                [#assign IpName = variables.value]
            [/#if]
            [#if variables.value?contains("TCPP01 VCC OUT")]
                [#assign TCPP01_VCC_OUT_PORT = IpName]
                [#assign TCPP01_VCC_OUT_PIN = IpInstance]
            [/#if]
            [#if variables.value?contains("TCPP01 DB OUT")]
                [#assign TCPP01_DB_OUT_PORT = IpName]
                [#assign TCPP01_DB_OUT_PIN = IpInstance]
            [/#if]
        [/#list]
    [/#if]
[/#list] 

/* GPIO ----------------------------------------------------------------------*/
[#if TCPP01_VCC_OUT_PORT == ""]
/* TCPP01_VCC_OUT is considered to be connected to VCC instead of a GPIO */
#define TCPP01_PORT0_VCC_OUT_TO_VCC       
[#else]
//TCPP01 VCC OUT GPIO Configuration
#define TCPP01_PORT0_VCC_OUT_GPIO_PORT              ${TCPP01_VCC_OUT_PORT}
#define TCPP01_PORT0_VCC_OUT_GPIO_PIN               ${TCPP01_VCC_OUT_PIN}
#define TCPP01_PORT0_VCC_OUT_GPIO_DEFVALUE()        HAL_GPIO_WritePin(TCPP01_PORT0_VCC_OUT_GPIO_PORT,TCPP01_PORT0_VCC_OUT_GPIO_PIN,GPIO_PIN_SET);
#define TCPP01_PORT0_VCC_OUT_GPIO_SET()             HAL_GPIO_WritePin(TCPP01_PORT0_VCC_OUT_GPIO_PORT,TCPP01_PORT0_VCC_OUT_GPIO_PIN,GPIO_PIN_SET);
#define TCPP01_PORT0_VCC_OUT_GPIO_RESET()           HAL_GPIO_WritePin(TCPP01_PORT0_VCC_OUT_GPIO_PORT,TCPP01_PORT0_VCC_OUT_GPIO_PIN,GPIO_PIN_RESET);
[/#if]

[#if TCPP01_DB_OUT_PORT == ""]
/* TCPP01_DB_OUT is considered to be connected to VCC instead of a GPIO */
#define TCPP01_PORT0_DB_OUT_TO_VCC
[#else]
//TCPP01 DB OUT GPIO Configuration
#define TCPP01_PORT0_DB_OUT_GPIO_PORT               ${TCPP01_DB_OUT_PORT}
#define TCPP01_PORT0_DB_OUT_GPIO_PIN                ${TCPP01_DB_OUT_PIN}
#define TCPP01_PORT0_DB_OUT_GPIO_DEFVALUE()         HAL_GPIO_WritePin(TCPP01_PORT0_DB_OUT_GPIO_PORT,TCPP01_PORT0_DB_OUT_GPIO_PIN,GPIO_PIN_SET);
#define TCPP01_PORT0_DB_OUT_GPIO_SET()              HAL_GPIO_WritePin(TCPP01_PORT0_DB_OUT_GPIO_PORT,TCPP01_PORT0_DB_OUT_GPIO_PIN,GPIO_PIN_SET);
#define TCPP01_PORT0_DB_OUT_GPIO_RESET()            HAL_GPIO_WritePin(TCPP01_PORT0_DB_OUT_GPIO_PORT,TCPP01_PORT0_DB_OUT_GPIO_PIN,GPIO_PIN_RESET);
[/#if]

#ifdef __cplusplus
}
#endif

#endif /* GPIO_SNK_CONF_H */