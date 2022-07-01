/**
  @page USB Type-C SRC1M1_Source application

  @verbatim
  ******************************************************************************
  * @file    SRC1M1_Source_TypeC_Only/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB Type-C SRC1M1_Source_TypeC_Only application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description
Use of Type-C Source application on STM32F4XX devices,
      with X-NUCLEO-SRC1M1 shield plugged.
      Project configuration is based on USB Type-C Specification revision 0.88.

This application provides an example for managing the Port 0 as a Type-C Source port.

This application has been certified with USB type-C Test Conformance Tools (Ellisys, MQP, GRL and LeCroy).
      Vendor Information File, used for Conformance testing and describing Port capabilities and supported
      options is provided in application directory in STMicroelectronics_SRC1M1_Source_STM32F446RE_VIF.xml file.

Goal is to identify a Type-C connection to a sink device and to determine the RD Resistor
      exposed by it (with the help of an ADC).

When a sink device is connected through a Type-C cable: GREEN LED on NUCLEO-F446RE switches on
      which means that VBUS is provided to the port partner.

@par Directory contents

      SRC1M1_Source
      ├──── Binary
      |     └──── SRC1M1_Source_TypeC_Only.bin   Application binary file
      ├──── Src
      |     └──── main.c                         Main program
      |     └──── stm32f4xx_hal_msp.c            HAL MSP Initialization file
      |     └──── stm32f4xx_it.c                 Interrupt handlers
      |     └──── system_stm32f4xx.c             STM32F4xx system clock configuration file
      └──── Inc
            └──── main.h                         Main program header file
            └──── src1m1_conf.h                  Defines for the SRC1M1 shield configuration
            └──── stm32f4xx_hal_conf.h           HAL configuration file
            └──── stm32f4xx_it.h                 Interrupt handlers header file

@par Hardware and Software environment

  - This application runs on STM32F4XX devices.

  - This example has been tested with STMicroelectronics NUCLEO-F446RE board with X-NUCLEO-SRC1M1 shield.

  - X-NUCLEO-SRC1M1 Set-up
    Move CC1 (J2) Jumper to the CC1 no UCPD position 2/3
    Move CC2 (J3) Jumper to the CC2 no UCPD position 2/3
    JP1 1-2 [LDO_OUT] jumper ON
    JP1 3-4 [RESET] jumper ON
    Supply CN3 with a 5V power supply
    Connect PA3 (CN10 - 37) and PC4 (CN10 - 34) together.

    Optional, for up to 3.0A
     - Remove R35 and place it on SH19
     - Remove R39 and place it on SH21
     - Change R5 shunt resistor to a 10mOhms one

@par How to use it ?

In order to make the program work, you must do the following:
  - Open your preferred toolchain
  - Rebuild all files and load your image into target memory
  - Run the application
 */
