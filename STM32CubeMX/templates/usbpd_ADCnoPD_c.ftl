[#ftl]
/**
  ******************************************************************************
  * File Name          : ADCnoPD.c
  * Description        : ADC USB-no-Power-Delivery Config C file
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

#include "usbpd_ADCnoPD.h"

[#assign ADC_DMA_Num = ""]
[#assign ADC_DMA_Chan = ""]
[#assign ADC_DMA_Mod = ""]
[#assign ADC_DMA_Irq = ""]
[#assign ADC_CC1 = ""]
[#assign ADC_CC1_Channel = ""]
[#assign ADC_CC2 = ""]
[#assign ADC_CC2_Channel = ""]
[#assign ADC_VBUS_NOPD = ""]
[#assign ADC_VBUS_NOPD_Channel = ""]
[#assign ADC_VPROV_NOPD = ""]
[#assign ADC_VPROV_NOPD_Channel = ""]
[#assign ADC_ISENSE_NOPD = ""]
[#assign ADC_ISENSE_NOPD_Channel = ""]
[#assign DmaIsTrue = false]
[#assign ADC_DIFF = ""]

[#list BspIpDatas as SWIP]
  [#if SWIP.bsp??]
    [#-- edit le contenu --]
    [#list SWIP.bsp as bsp]
        [#assign DmaIsTrue = bsp.dmaUsed]
        [#if DmaIsTrue]
            [#if bsp.bspName == "ADC CC1"]
                [#assign ADC_CC1 = bsp.bspInstanceName]
                [#assign ADC_CC1_Channel = bsp.solution?keep_after(":IN")]
                [#assign ADC_DMA_Num = bsp.dmaChannel?keep_before("_")]
                [#assign ADC_DMA_Chan = bsp.dmaChannel]
                [#assign ADC_DMA_Mod = bsp.dmaModeUsed]
                [#assign ADC_DMA_Irq = bsp.dmaChannelIrqNb]
                [#if (ADC_CC1_Channel?length > 2)]
                    [#assign ADC_CC1_Channel = ADC_CC1_Channel?keep_before(" Single-ended")]
                    [#assign ADC_DIFF = "1"]
                [/#if]
            [#elseif bsp.bspName == "ADC CC2"]
                [#assign ADC_CC2 = bsp.bspInstanceName]
                [#assign ADC_CC2_Channel = bsp.solution?keep_after(":IN")]
                [#if (ADC_CC2_Channel?length > 2)]
                    [#assign ADC_CC2_Channel = ADC_CC2_Channel?keep_before(" Single-ended")]
                [/#if]
            [#elseif bsp.bspName == "ADC VBUS NOPD"]
                [#assign ADC_VBUS_NOPD = bsp.bspInstanceName]
                [#assign ADC_VBUS_NOPD_Channel = bsp.solution?keep_after(":IN")]
                [#if (ADC_VBUS_NOPD_Channel?length > 2)]
                    [#assign ADC_VBUS_NOPD_Channel = ADC_VBUS_NOPD_Channel?keep_before(" Single-ended")]
                [/#if]
            [#elseif bsp.bspName == "ADC VPROV NOPD"]
                [#assign ADC_VPROV_NOPD = bsp.bspInstanceName]
                [#assign ADC_VPROV_NOPD_Channel = bsp.solution?keep_after(":IN")]
                [#if (ADC_VPROV_NOPD_Channel?length > 2)]
                    [#assign ADC_VPROV_NOPD_Channel = ADC_VPROV_NOPD_Channel?keep_before(" Single-ended")]
                [/#if]
            [#elseif bsp.bspName == "ADC ISENSE NOPD"]
                [#assign ADC_ISENSE_NOPD = bsp.bspInstanceName]
                [#assign ADC_ISENSE_NOPD_Channel = bsp.solution?keep_after(":IN")]
                [#if (ADC_ISENSE_NOPD_Channel?length > 2)]
                    [#assign ADC_ISENSE_NOPD_Channel = ADC_ISENSE_NOPD_Channel?keep_before(" Single-ended")]
                [/#if]
            [/#if]
        [/#if]
    [/#list]
  [/#if]
[/#list]

/* Check DMA Usage */
[#if DmaIsTrue == false]
#error DMA should be activated for ADC
[/#if]

/* Check same ADC for all */
[#if ADC_CC2 != ADC_CC1]
#error All Channels must belong to the same ADC
[/#if]
[#if ADC_VBUS_NOPD != ADC_CC1]
#error All Channels must belong to the same ADC
[/#if]
[#if ADC_VPROV_NOPD != ADC_CC1]
#error All Channels must belong to the same ADC
[/#if]
[#if ADC_ISENSE_NOPD != ADC_CC1]
#error All Channels must belong to the same ADC
[/#if]
/* Common ADC for All 3(SRC) or 4(DRP) Channels : ${ADC_CC1} */
#define	ADC_INSTANCE                ${ADC_CC1}

#define ADC_CC1_CHANNEL	            ADC_CHANNEL_${ADC_CC1_Channel}
#define ADC_CC2_CHANNEL	            ADC_CHANNEL_${ADC_CC2_Channel}
#define ADC_VBUS_NOPD_CHANNEL       ADC_CHANNEL_${ADC_VBUS_NOPD_Channel}
#define ADC_VPROV_NOPD_CHANNEL      ADC_CHANNEL_${ADC_VPROV_NOPD_Channel}
#define ADC_ISENSE_NOPD_CHANNEL     ADC_CHANNEL_${ADC_ISENSE_NOPD_Channel}

#define ADC_DMA                     ${ADC_DMA_Num}
#define ADC_DMA_CHANNEL             ${ADC_DMA_Chan}

extern ADC_HandleTypeDef            h${ADC_CC1?lower_case};
extern DMA_HandleTypeDef            hdma_${ADC_CC1?lower_case};

/**
  *
  */
 
void ADC_Channels_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;
   /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CC1_CHANNEL;
  sConfig.Rank = 1;
  sConfig.SamplingTime = LL_ADC_GetChannelSamplingTime(${ADC_CC1?upper_case},1);
  HAL_ADC_ConfigChannel(&h${ADC_CC1?lower_case}, &sConfig);

  sConfig.Channel = ADC_CC2_CHANNEL;
  sConfig.Rank = 2;
  HAL_ADC_ConfigChannel(&h${ADC_CC1?lower_case}, &sConfig);

  sConfig.Channel = ADC_VBUS_NOPD_CHANNEL;
  sConfig.Rank = 3;
  HAL_ADC_ConfigChannel(&h${ADC_CC1?lower_case}, &sConfig);

  sConfig.Channel = ADC_ISENSE_NOPD_CHANNEL;
  sConfig.Rank = 4;
  HAL_ADC_ConfigChannel(&h${ADC_CC1?lower_case}, &sConfig);
  
  sConfig.Channel = ADC_VPROV_NOPD_CHANNEL;
  sConfig.Rank = 5;
  HAL_ADC_ConfigChannel(&h${ADC_CC1?lower_case}, &sConfig);
}

void ADC_Start(void)
{
  HAL_ADC_Start_DMA(&h${ADC_CC1?lower_case},(uint32_t *)&USBnoPD_adc_buffer, USBNOPD_ADC_USED_CHANNELS); 
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
