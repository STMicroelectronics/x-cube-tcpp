[#ftl]
/**
  ******************************************************************************
  * @file    USBnoPD_Tim.h
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
#ifndef USBNOPD_TIM_H
#define USBNOPD_TIM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported Defines ----------------------------------------------------------*/
[#assign IpInstance = ""]
[#assign IpName = ""]

[#assign USBNOPD_TIM_INSTANCE = ""]
[#assign USBNOPD_TIM_NAME = ""]

[#list BspIpDatas as SWIP] 
    [#if SWIP.variables??]
        [#list SWIP.variables as variables]
            [#if variables.name?contains("IpInstance")]
                [#assign IpInstance = variables.value]
            [/#if]
            [#if variables.name?contains("IpName")]
                [#assign IpName = variables.value]
            [/#if]
            [#if variables.value?contains("TIM NoPD")]
                [#assign USBNOPD_TIM_NAME = IpName]
                [#assign USBNOPD_TIM_INSTANCE = IpInstance]
            [/#if]
        [/#list]
    [/#if]
[/#list] 

#ifdef __cplusplus
extern "C" {
#endif

/* TIMER -----------------------------------------------------------------------*/
[#if USBNOPD_TIM_INSTANCE == ""]
#error Please select a Timer
[#else]
#define USBNOPD_TIM_INST                      ${USBNOPD_TIM_INSTANCE}
[/#if]

void TIM_Base_Start_IT();
void TIM_Base_Stop_IT();

#ifdef __cplusplus
}
#endif
#endif /* USBNOPD_TIM_H */
