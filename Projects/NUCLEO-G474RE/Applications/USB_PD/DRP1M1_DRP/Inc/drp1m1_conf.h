/**
  ******************************************************************************
  * @file    drp1m1_conf.h
  * @author  MCD Application Team
  * @brief   X-NUCLEO-DRP1M1 board configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRP1M1_CONF_H__
#define DRP1M1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Replace the header file names with the ones of the target platform */
#include "stm32g4xx_nucleo.h"
#include "stm32g4xx_ll_adc.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_exti.h"
#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_nucleo_errno.h"


/** @addtogroup DRP1M1
  * @{
  */

/** @addtogroup DRP1M1_CONFIG
  * @{
  */

/** @addtogroup DRP1M1_CONFIG_Exported_Constants Exported Constants
  * @{
  */

/* I2C BUS definitions */
#define BUS_I2C_INSTANCE                 I2C1

#ifndef BUS_I2C_FREQUENCY
#define BUS_I2C_FREQUENCY                100000U /* Frequency of I2Cn = 100 kHz*/
#endif

#define BUS_I2C_CLK_ENABLE()             __HAL_RCC_I2C1_CLK_ENABLE()
#define BUS_I2C_CLK_DISABLE()            __HAL_RCC_I2C1_CLK_DISABLE()
#define BUS_I2C_FORCE_RESET()            __HAL_RCC_I2C1_FORCE_RESET()
#define BUS_I2C_RELEASE_RESET()          __HAL_RCC_I2C1_RELEASE_RESET()

#define BUS_I2C_SCL_GPIO_PIN             GPIO_PIN_8
#define BUS_I2C_SCL_GPIO_PORT            GPIOB
#define BUS_I2C_SCL_GPIO_AF              GPIO_AF4_I2C1
#define BUS_I2C_SCL_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C_SCL_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUS_I2C_SDA_GPIO_PIN             GPIO_PIN_9
#define BUS_I2C_SDA_GPIO_PORT            GPIOB
#define BUS_I2C_SDA_GPIO_AF              GPIO_AF4_I2C1
#define BUS_I2C_SDA_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C_SDA_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()

#define BUS_I2C_POLL_TIMEOUT             0x1000U

#define VSENSE_ADC_INSTANCE             ADC1
#define VSENSE_ADC_COMMON               ADC12_COMMON
#define VSENSE_ADC_RANK                 LL_ADC_REG_RANK_1
#define VSENSE_ADC_CHANNEL              LL_ADC_CHANNEL_1

/**
  * @brief TCPP0203_FLGn pin
  * To be defined for each Port, protected by a TCPP0203 component
  */
#define TCPP0203_PORT0_FLG_GPIO_CLK_ENABLE()        LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
#define TCPP0203_PORT0_FLG_GPIO_CLK_DISABLE()       LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
#define TCPP0203_PORT0_FLG_GPIO_PORT                GPIOC
#define TCPP0203_PORT0_FLG_GPIO_PIN                 LL_GPIO_PIN_5
#define TCPP0203_PORT0_FLG_GPIO_MODE                LL_GPIO_MODE_INPUT
#define TCPP0203_PORT0_FLG_GPIO_PUPD                LL_GPIO_PULL_UP
#define TCPP0203_PORT0_FLG_SET_EXTI()               LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE5);
#define TCPP0203_PORT0_FLG_EXTI_LINE                LL_EXTI_LINE_5
#define TCPP0203_PORT0_FLG_EXTI_ENABLE()            LL_EXTI_EnableIT_0_31(TCPP0203_PORT0_FLG_EXTI_LINE);
#define TCPP0203_PORT0_FLG_EXTI_DISABLE()           LL_EXTI_DisableIT_0_31(TCPP0203_PORT0_FLG_EXTI_LINE);
#define TCPP0203_PORT0_FLG_TRIG_ENABLE()            LL_EXTI_EnableFallingTrig_0_31(TCPP0203_PORT0_FLG_EXTI_LINE);
#define TCPP0203_PORT0_FLG_TRIG_DISABLE()           LL_EXTI_DisableFallingTrig_0_31(TCPP0203_PORT0_FLG_EXTI_LINE);
#define TCPP0203_PORT0_FLG_EXTI_IS_ACTIVE_FLAG()    LL_EXTI_IsActiveFlag_0_31(TCPP0203_PORT0_FLG_EXTI_LINE)
#define TCPP0203_PORT0_FLG_EXTI_CLEAR_FLAG()        LL_EXTI_ClearFlag_0_31(TCPP0203_PORT0_FLG_EXTI_LINE);
#define TCPP0203_PORT0_FLG_EXTI_IRQN                EXTI9_5_IRQn
#define TCPP0203_PORT0_FLG_EXTI_IRQHANDLER          EXTI9_5_IRQHandler
#define TCPP0203_PORT0_FLG_IT_PRIORITY              (12U)
#define TCPP0203_PORT0_FLG_GENERATE_IT()            LL_EXTI_GenerateSWI_0_31(TCPP0203_PORT0_FLG_EXTI_LINE);

/**
  * @brief TCPP0203_PWRENn pin
  */
#define TCPP0203_PORT0_ENABLE_GPIO_CLK_ENABLE()     LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
#define TCPP0203_PORT0_ENABLE_GPIO_CLK_DISABLE()    LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
#define TCPP0203_PORT0_ENABLE_GPIO_PORT             GPIOC
#define TCPP0203_PORT0_ENABLE_GPIO_PIN              LL_GPIO_PIN_8
#define TCPP0203_PORT0_ENABLE_GPIO_MODE             LL_GPIO_MODE_OUTPUT
#define TCPP0203_PORT0_ENABLE_GPIO_OUTPUT           LL_GPIO_OUTPUT_PUSHPULL
#define TCPP0203_PORT0_ENABLE_GPIO_PUPD             LL_GPIO_PULL_NO
#define TCPP0203_PORT0_ENABLE_GPIO_DEFVALUE()       LL_GPIO_ResetOutputPin(TCPP0203_PORT0_ENABLE_GPIO_PORT,\
                                                                           TCPP0203_PORT0_ENABLE_GPIO_PIN);
#define TCPP0203_PORT0_ENABLE_GPIO_SET()            LL_GPIO_SetOutputPin(TCPP0203_PORT0_ENABLE_GPIO_PORT,\
                                                                         TCPP0203_PORT0_ENABLE_GPIO_PIN);
#define TCPP0203_PORT0_ENABLE_GPIO_RESET()          LL_GPIO_ResetOutputPin(TCPP0203_PORT0_ENABLE_GPIO_PORT,\
                                                                           TCPP0203_PORT0_ENABLE_GPIO_PIN);

/* Definition of ADCx instance */
#define TCPP0203_PORT0_ADC_INSTANCE                 ADC2
#define TCPP0203_PORT0_ADC_CLK_ENABLE()            __HAL_RCC_ADC12_CLK_ENABLE()
#define TCPP0203_PORT0_ADC_CLK_DISABLE()           __HAL_RCC_ADC12_CLK_DISABLE()
#define TCPP0203_PORT0_ADC_FORCE_RESET()           __HAL_RCC_ADC12_FORCE_RESET()
#define TCPP0203_PORT0_ADC_RELEASE_RESET()         __HAL_RCC_ADC12_RELEASE_RESET()

/* Definition of ADCx channels */
#define TCPP0203_PORT0_IANA_ADC_CHANNEL             ADC_CHANNEL_11          /* PC5 : ADC2_IN11 */
#define TCPP0203_PORT0_VBUSC_ADC_CHANNEL            ADC_CHANNEL_1           /* PA0 : ADC12_IN1 */
#define TCPP0203_PORT0_VBUSPROV_ADC_CHANNEL         ADC_CHANNEL_2           /* PA1 : ADC12_IN2 */
#define TCPP0203_PORT0_VBUSCONS_ADC_CHANNEL         ADC_CHANNEL_7           /* PA4 : ADC2_IN7  */
#define TCPP0203_PORT0_ADCXCHANNELN                 (4U)

#define TCPP0203_PORT0_IANA_GPIO_CLK_ENABLE()       LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
#define TCPP0203_PORT0_IANA_GPIO_CLK_DISABLE()      LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
#define TCPP0203_PORT0_IANA_GPIO_PORT               GPIOC
#define TCPP0203_PORT0_IANA_GPIO_PIN                LL_GPIO_PIN_1
#define TCPP0203_PORT0_IANA_GPIO_MODE               LL_GPIO_MODE_ANALOG

#define TCPP0203_PORT0_VBUSC_GPIO_CLK_ENABLE()      LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
#define TCPP0203_PORT0_VBUSC_GPIO_CLK_DISABLE()     LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
#define TCPP0203_PORT0_VBUSC_GPIO_PORT              GPIOA
#define TCPP0203_PORT0_VBUSC_GPIO_PIN               LL_GPIO_PIN_0
#define TCPP0203_PORT0_VBUSC_GPIO_MODE              LL_GPIO_MODE_ANALOG

#define TCPP0203_PORT0_VBUSPROV_GPIO_CLK_ENABLE()   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
#define TCPP0203_PORT0_VBUSPROV_GPIO_CLK_DISABLE()  LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
#define TCPP0203_PORT0_VBUSPROV_GPIO_PORT           GPIOA
#define TCPP0203_PORT0_VBUSPROV_GPIO_PIN            LL_GPIO_PIN_1
#define TCPP0203_PORT0_VBUSPROV_GPIO_MODE           LL_GPIO_MODE_ANALOG

#define TCPP0203_PORT0_VBUSCONS_GPIO_CLK_ENABLE()   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
#define TCPP0203_PORT0_VBUSCONS_GPIO_CLK_DISABLE()  LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
#define TCPP0203_PORT0_VBUSCONS_GPIO_PORT           GPIOA
#define TCPP0203_PORT0_VBUSCONS_GPIO_PIN            LL_GPIO_PIN_4
#define TCPP0203_PORT0_VBUSCONS_GPIO_MODE           LL_GPIO_MODE_ANALOG

#define DRP1M1_VSENSE_RA                           200
#define DRP1M1_VSENSE_RB                           40
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* DRP1M1_CONF_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
