[#ftl]
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ADC
  * @brief   ADC USBPD Config H file
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

#ifndef ADC_H
#define ADC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "${FamilyName?lower_case}xx.h"
#include "${FamilyName?lower_case}xx_hal.h"

[#assign ADC_DMA_Num = ""]
[#assign ADC_DMA_Chan = ""]
[#assign ADC_DMA_Mod = ""]
[#assign ADC_DMA_Irq = ""]
[#assign ADC_VBUS = ""]
[#assign ADC_VBUS_Channel = ""]
[#assign ADC_VPROV = ""]
[#assign ADC_VPROV_Channel = ""]
[#assign ADC_VCONS = ""]
[#assign ADC_VCONS_Channel = ""]
[#assign ADC_ISENSE = ""]
[#assign ADC_ISENSE_Channel = ""]
[#assign DmaIsTrue = false]
[#assign ADC_DIFF = ""]

[#list BspIpDatas as SWIP]
  [#if SWIP.bsp??]
    [#-- edit le contenu --]
    [#list SWIP.bsp as bsp]
        [#assign DmaIsTrue = bsp.dmaUsed]
        [#if DmaIsTrue]
            [#if bsp.bspName == "ADC VBUS"]
                [#assign ADC_VBUS = bsp.bspInstanceName]
                [#assign ADC_VBUS_Channel = bsp.solution?keep_after(":IN")]
                [#assign ADC_DMA_Num	= bsp.dmaChannel?keep_before("_")]
                [#assign ADC_DMA_Chan = bsp.dmaChannel]
                [#assign ADC_DMA_Mod = bsp.dmaModeUsed]
                [#assign ADC_DMA_Irq = bsp.dmaChannelIrqNb]
                [#if (ADC_VBUS_Channel?length > 2)]
                    [#assign ADC_VBUS_Channel = ADC_VBUS_Channel?keep_before(" Single-ended")]
                    [#assign ADC_DIFF = "1"]
                [/#if]
            [#elseif bsp.bspName == "ADC VCONS"]
                [#assign ADC_VCONS = bsp.bspInstanceName]
                [#assign ADC_VCONS_Channel = bsp.solution?keep_after(":IN")]
                [#if (ADC_VCONS_Channel?length > 2)]
                    [#assign ADC_VCONS_Channel = ADC_VCONS_Channel?keep_before(" Single-ended")]
                [/#if]
            [#elseif bsp.bspName == "ADC VPROV"]
                [#assign ADC_VPROV = bsp.bspInstanceName]
                [#assign ADC_VPROV_Channel = bsp.solution?keep_after(":IN")]
                [#if (ADC_VPROV_Channel?length > 2)]
                    [#assign ADC_VPROV_Channel = ADC_VPROV_Channel?keep_before(" Single-ended")]
                [/#if]
            [#elseif bsp.bspName == "ADC ISENSE"]
                [#assign ADC_ISENSE = bsp.bspInstanceName]
                [#assign ADC_ISENSE_Channel = bsp.solution?keep_after(":IN")]
                [#if (ADC_ISENSE_Channel?length > 2)]
                    [#assign ADC_ISENSE_Channel = ADC_ISENSE_Channel?keep_before(" Single-ended")]
                [/#if]
            [/#if]
        [#else]
            [#if bsp.bspName == "ADC VBUS"]
                [#assign ADC_VBUS = bsp.bspInstanceName]
                [#assign ADC_VBUS_Channel = bsp.solution?keep_after(":IN")]		
                [#if (ADC_VBUS_Channel?length > 2)]
                    [#assign ADC_VBUS_Channel = ADC_VBUS_Channel?keep_before(" Single-ended")]
                    [#assign ADC_DIFF = "1"]
                [/#if]
            [/#if]
        [/#if]
    [/#list]
  [/#if]
[/#list]

[#if ADC_DIFF == "1"]
#define ADC_DIFF_CAPABLE
[/#if]

[#if ADC_VBUS != ""]
/* ADC Vbus is defined */
#define	VISENSE_ADC_INSTANCE                        ${ADC_VBUS}	
#define ADC_VBUS_CHANNEL                            ADC_CHANNEL_${ADC_VBUS_Channel}
extern ADC_HandleTypeDef                            h${ADC_VBUS?lower_case};

  [#if ADC_ISENSE != ""]

    [#if ADC_ISENSE != ADC_VBUS]
/* ADC Isense is defined but not from same ADC than VBUS */
#error All Channels must belong to the same ADC
    [#else]
/* ADC Isense is defined and from same ADC than VBUS */
#define ADC_ISENSE_USE                              1
#define ADC_ISENSE_CHANNEL                          ADC_CHANNEL_${ADC_ISENSE_Channel}
    [/#if]
  [#else]
/* ADC Isense not used */
#define ADC_ISENSE_USE                              0
  [/#if]

  [#if ADC_VPROV != ""]
    [#if ADC_VPROV != ADC_VBUS]
/* ADC Vprov is defined but not from same ADC than VBUS */
#error All Channels must belong to the same ADC	
    [#else]
/* ADC Vprov is defined and from same ADC than VBUS */
#define ADC_VPROV_USE                               1
#define ADC_VPROV_CHANNEL                           ADC_CHANNEL_${ADC_VPROV_Channel}
    [/#if]
  [#else]
/* ADC Vprov not used */
#define ADC_VPROV_USE                               0
  [/#if]

  [#if ADC_VCONS !=""]
    [#if ADC_VCONS != ADC_VBUS]
/* ADC Vcons is defined but not from same ADC than VBUS	*/
#error All Channels must belong to the same ADC	
    [#else]
/* ADC Vcons is defined and from same ADC than VBUS */
#define ADC_VCONS_USE                               1
#define ADC_VCONS_CHANNEL                           ADC_CHANNEL_${ADC_VCONS_Channel}
    [/#if]
  [#else]
/* ADC Vcons not used */
#define ADC_VCONS_USE                               0
  [/#if]

  [#if ADC_ISENSE == ""]
    [#if ADC_VPROV == ""]
      [#if ADC_VCONS = ""]
#define VISENSE_ADC_BUFFER_SIZE                     1
#define ADC_VBUS_ONLY
      [#else]
#define VISENSE_ADC_BUFFER_SIZE                     2
       [/#if]
    [#else]
      [#if ADC_VCONS == ""]
#define VISENSE_ADC_BUFFER_SIZE                     2  
      [#else]
#define VISENSE_ADC_BUFFER_SIZE                     3
      [/#if]
    [/#if]
  [#else]
    [#if ADC_VPROV == ""]
      [#if ADC_VCONS == ""]
#define VISENSE_ADC_BUFFER_SIZE                     2
      [#else]
#define VISENSE_ADC_BUFFER_SIZE                     3
      [/#if]
    [#else]
      [#if ADC_VCONS == ""]
#define VISENSE_ADC_BUFFER_SIZE                     3
      [#else]
#define VISENSE_ADC_BUFFER_SIZE                     4
      [/#if]
    [/#if]
  [/#if]

  [#if ADC_ISENSE != "" || ADC_VPROV != "" || ADC_VCONS != ""]
/* Check DMA Usage */
    [#if DmaIsTrue == false]
#error DMA should be activated for ADC
    [#else]
#define ADC_DMA                                     ${ADC_DMA_Num}
#define ADC_DMA_CHANNEL                             ${ADC_DMA_Chan}
[#-- #define ADC_DMA_MODE                           ${ADC_DMA_Mod} --]
extern DMA_HandleTypeDef                            hdma_${ADC_VBUS?lower_case};
    [/#if]
[#--    [#if ADC_DMA_Mod != "DMA_CIRCULAR"] --]
[#-- #error DMA Mode should be Circular --]
[#--    [/#if] --]
  [/#if]

[#else]
#error Please define at least an ADC for VBUS
[/#if]

/* ADC_Buffer values */
extern uint16_t usbpd_pwr_adcx_buff[];

void ADC_Start(void);

/**
  * @}
  */

/**
  * @}
  */

#endif /* ADC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
