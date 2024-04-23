[#ftl]
/**
  ******************************************************************************
  * @file    ADC_SNK.c
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

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbpd_ADC_SNK.h"

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

extern ADC_HandleTypeDef            h${ADC_VSENSE?lower_case};

void ADC_Start()
{
  HAL_ADC_Start(&h${ADC_VSENSE?lower_case}); 
} 
  
#ifdef __cplusplus
}
#endif
