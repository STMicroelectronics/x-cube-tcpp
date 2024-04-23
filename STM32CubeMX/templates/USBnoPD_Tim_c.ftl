[#ftl]
/**
  ******************************************************************************
  * @file    USBnoPD_Tim.c
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
#ifndef USBNOPD_TIM_C
#define USBNOPD_TIM_C

#include "${FamilyName?lower_case}xx.h"
#include "USBnoPD_Tim.h"

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

extern TIM_HandleTypeDef h${USBNOPD_TIM_INSTANCE?lower_case};

void TIM_Base_Start_IT()
{
  HAL_TIM_Base_Start_IT(&h${USBNOPD_TIM_INSTANCE?lower_case});
}

void TIM_Base_Stop_IT()
{
  HAL_TIM_Base_Stop_IT(&h${USBNOPD_TIM_INSTANCE?lower_case});
}


#ifdef __cplusplus
}
#endif
#endif /* USBNOPD_TIM_H */