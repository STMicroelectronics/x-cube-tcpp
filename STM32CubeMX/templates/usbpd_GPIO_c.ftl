[#ftl]
/**
  ******************************************************************************
  * @file    GPIO_CONF.C
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
#ifndef GPIO_CONF_C
#define GPIO_CONF_C

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
[#if includes??]
[#list includes as include]
#include "${include}"
[/#list]
[/#if]
#include "usbpd_GPIO.h"
#ifndef _SRCnoPD
#include "usbpd.h"
#endif

/* Exported Defines ----------------------------------------------------------*/
[#assign IpInstance = ""]
[#assign IpName = ""]
[#assign IrqNumber = ""]
[#assign ExtiLine = ""]

[#assign TCPP0203_FLG_EXTI_PORT = ""]
[#assign TCPP0203_FLG_EXTI_PIN = ""]
[#assign TCPP0203_FLG_EXTI_IRQn = ""]
[#assign TCPP0203_FLG_EXTI_LINE = ""]

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
        [/#list]
    [/#if]
[/#list] 

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _SRCnoPD
#ifndef TCPP0203_PORT0_FLG_NOT_USED
/**
  * @brief  This function handles the external line interrupt request.
  *         (Associated to FLGn line in case of TCPP0203 management)
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin==TCPP0203_PORT0_FLG_GPIO_PIN)
  {
    BSP_USBPD_PWR_EventCallback(USBPD_PWR_TYPE_C_PORT_1);
  }
}
#endif /* TCPP0203_PORT0_FLG_NOT_USED */
#endif /* _SRCnoPD */

#ifdef __cplusplus
}
#endif
#endif /* GPIO_CONF_C */
