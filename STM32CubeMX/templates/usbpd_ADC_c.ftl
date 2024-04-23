[#ftl]
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ADC
  * @brief   ADC USBPD Config C file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) ${year} STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *  ******************************************************************************
  */

#include "usbpd_ADC.h"

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

[#list BspIpDatas as SWIP]
  [#if SWIP.bsp??]
    [#-- edit le contenu --]
    [#list SWIP.bsp as bsp]
        [#assign DmaIsTrue = bsp.dmaUsed]
        [#if DmaIsTrue]
            [#if bsp.bspName == "ADC VBUS"]
                [#assign ADC_VBUS = bsp.bspInstanceName]
                [#assign ADC_VBUS_Channel = bsp.solution?keep_after(":IN")]
                [#assign ADC_DMA_Num = bsp.dmaChannel?keep_before("_")]
                [#assign ADC_DMA_Chan = bsp.dmaChannel]
                [#assign ADC_DMA_Mod = bsp.dmaModeUsed]
                [#assign ADC_DMA_Irq = bsp.dmaChannelIrqNb]
                [#if (ADC_VBUS_Channel?length > 2)]
                     [#assign ADC_VBUS_Channel = ADC_VBUS_Channel?keep_before(" Single-ended")]
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
                [#if (ADC_VBUS_Channel?length > 2)]
                     [#assign ADC_VBUS_Channel = ADC_VBUS_Channel?keep_before(" Single-ended")]
                [/#if]
            [/#if]
        [/#if]
    [/#list]
  [/#if]
[/#list] 

[#assign MCU_FAMILY = FamilyName?lower_case]
 
void ADC_Start(void)
{
#if defined ADC_DIFF_CAPABLE

[#if MCU_FAMILY == "stm32u5"]
  HAL_ADCEx_Calibration_Start(&h${ADC_VBUS?lower_case},ADC_CALIB_OFFSET_LINEARITY,ADC_SINGLE_ENDED);
[#else]
  HAL_ADCEx_Calibration_Start(&h${ADC_VBUS?lower_case},ADC_SINGLE_ENDED);
[/#if]

#else

[#if MCU_FAMILY == "stm32u5"]
  HAL_ADCEx_Calibration_Start(&h${ADC_VBUS?lower_case},ADC_CALIB_OFFSET_LINEARITY,ADC_SINGLE_ENDED);
[#else]
  HAL_ADCEx_Calibration_Start(&h${ADC_VBUS?lower_case});
[/#if]

#endif /* ADC_DIFF_CAPABLE */

[#if ADC_ISENSE != "" || ADC_VPROV != "" || ADC_VCONS != ""]
  HAL_ADC_Start_DMA(&h${ADC_VBUS?lower_case}, (uint32_t*)usbpd_pwr_adcx_buff, VISENSE_ADC_BUFFER_SIZE);
[#else]
  HAL_ADC_Start(&h${ADC_VBUS?lower_case}); 
[/#if]
}
/**
  *
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
