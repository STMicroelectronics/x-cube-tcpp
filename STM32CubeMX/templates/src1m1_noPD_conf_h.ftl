[#ftl]
/**
  ******************************************************************************
  * @file    src1m1_conf.h
  * @author  MCD Application Team
  * @brief   X-NUCLEO-SRC1M1 board configuration template file.
  *          This file should be copied to the application folder and renamed
  *          to src1m1_conf.h.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SRC1M1_CONF_H__
#define SRC1M1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "src1m1_errno.h"

/** @addtogroup SRC1M1
  * @{
  */

/** @addtogroup SRC1M1_CONFIG
  * @{
  */

/** @addtogroup SRC1M1_CONFIG_Exported_Constants Exported Constants
  * @{
  */

/* I2C BUS definitions */
#define BUS_I2C_INSTANCE                 I2C1

#if !defined(BUS_I2C_FREQUENCY)
#define BUS_I2C_FREQUENCY                400000U /* Frequency of I2Cn = 400 kHz*/
#endif /* !BUS_I2C_FREQUENCY */

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

#define VISENSE_ADC_BUFFER_SIZE          5

#define USBNOPD_PORT0_CC1_GPIO_PIN       GPIO_PIN_4
#define USBNOPD_PORT0_CC1_GPIO_PORT      GPIOA
#define USBNOPD_PORT0_CC2_GPIO_PIN       GPIO_PIN_0
#define USBNOPD_PORT0_CC2_GPIO_PORT      GPIOC
#define USBNOPD_PORT0_VBUS_GPIO_PIN      GPIO_PIN_0
#define USBNOPD_PORT0_VBUS_GPIO_PORT     GPIOA
#define USBNOPD_PORT0_VBUSPROV_GPIO_PIN  GPIO_PIN_1
#define USBNOPD_PORT0_VBUSPROV_GPIO_PORT GPIOA
#define USBNOPD_PORT0_IANA_GPIO_PIN      GPIO_PIN_11
#define USBNOPD_PORT0_IANA_GPIO_PORT     GPIOB

#define TCPP0203_PORT0_ENABLE_GPIO_PORT  GPIOC
#define TCPP0203_PORT0_ENABLE_GPIO_PIN   GPIO_PIN_8
#define TCPP0203_PORT0_FLG_GPIO_PORT     GPIOC
#define TCPP0203_PORT0_FLG_GPIO_PIN      GPIO_PIN_4
#define TCPP0203_PORT0_FLG_EXTI_IRQN     EXTI4_IRQn

#define SRC1M1_VSENSE_RA                 200u /* VBUS voltage divider RA in milliohm */
#define SRC1M1_VSENSE_RB                 40u  /* VBUS voltage divider RB in milliohm */

#define SRC1M1_ISENSE_GA                 42u  /* Current measure gain In V/V */
#define SRC1M1_ISENSE_RS                 7u   /* Current measure shunt resistor in milliohm */

#define BSP_USBPD_PWR_DONT_WAIT_VBUSOFF_DISCHARGE   1u /* Set to 1 to not wait for vbus discharge in VBUSOff function */

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

#endif /* SRC1M1_CONF_H__*/

