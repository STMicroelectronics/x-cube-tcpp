[#ftl]
/**
  ******************************************************************************
  * @file    ADC_SNK.h
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
#ifndef ADC_SNK_H
#define ADC_SNK_H

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

[#assign ADC_VSENSE = ""]
[#assign ADC_VSENSE_Channel = ""]
[#assign ADC_DIFF = ""]

[#list BspIpDatas as SWIP]
  [#if SWIP.bsp??]
    [#-- edit le contenu --]
    [#list SWIP.bsp as bsp]
        [#if bsp.bspName == "ADC VSENSE"]
            [#assign ADC_VSENSE = bsp.bspInstanceName]
            [#assign ADC_VSENSE_Channel = bsp.solution?keep_after(":IN")]
            [#if (ADC_VSENSE_Channel?length > 2)]
                [#assign ADC_VSENSE_Channel = ADC_VSENSE_Channel?keep_before(" Single-ended")]
                [#assign ADC_DIFF = "1"]
            [/#if]
        [/#if]
    [/#list]
  [/#if]
[/#list]

#ifdef __cplusplus
extern "C" {
#endif

[#if ADC_DIFF == "1"]
#define ADC_DIFF_CAPABLE
[/#if]

[#if ADC_VSENSE == ""]
#error Please select an ADC for VSENSE
[#else]
#define TCPP01_PORT0_VSENSE_ADC_INSTANCE            ${ADC_VSENSE}
#define TCPP01_PORT0_VSENSE_ADC_CHANNEL             ADC_CHANNEL_${ADC_VSENSE_Channel}
[/#if]

void ADC_Start(void);

#ifdef __cplusplus
}
#endif
#endif /* ADC_SNK_H */
