/**
  @page USB-C Power Delivery USBPDM1_Consumer_LPM application
  
  @verbatim
  ******************************************************************************
  * @file    USBPDM1_Consumer_LPM/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB-C Power Delivery USBPDM1_Consumer_LPM application.
  ******************************************************************************
  *
  * Copyright (c) 2018 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim
  
@par Application Description
Use of USB Power Delivery (USB-PD) Consumer application with LPM running on STM32G0XX devices, 
     with X-NUCLEO-USBPDM1 shield plugged.
     Project configuration is based on USB Power Delivery Specification revision 3.0.

This application provides an example for managing the Port 0 as a Consumer Only port with LPM.
     LPM(Low power managment) is enabled, and for consumption improvement the STM32CubeMonUCPD graphical user interface is not enabled.
     The user can measure the consumption by connecting the application STM32 power monitor on the nucleo PIN IDD of the connector JP3.

     An application to create a simple application from STM32CubeMX is available at this link :
     https://www.st.com/resource/en/application_note/dm00663511-how-to-build-a-simple-usbpd-sink-application-with-stm32cubemx-stmicroelectronics.pdf
     and the corresponding video here : https://www.youtube.com/watch?v=Z_Sn6CBbz-k

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct operation.

@par Directory contents

  - USBPDM1_Consumer_LPM/Src/adc.c                       ADC configuration
  - USBPDM1_Consumer_LPM/Src/app_freertos.c              FreeRTOS configuration
  - USBPDM1_Consumer_LPM/Src/dma.c                       DMA configuration
  - USBPDM1_Consumer_LPM/Src/gpio.c                      GPIO configuration
  - USBPDM1_Consumer_LPM/Src/main.c                      Main program
  - USBPDM1_Consumer_LPM/Src/rtc.c                       RTC configuration (RTC is used to exit low power mode)
  - USBPDM1_Consumer_LPM/Src/stm32_lpm_if.c              hardware interface of the tiny_lpm utility
  - USBPDM1_Consumer_LPM/Src/stm32_timer_if.c            hardware interface of the timer utility
  - USBPDM1_Consumer_LPM/Src/stm32_hal_msp.c             msp interface for hal driver
  - USBPDM1_Consumer_LPM/Src/stm32g0xx_it.c              Interrupt handlers
  - USBPDM1_Consumer_LPM/Src/system_stm32g0xx.c          STM32G0xx system clock configuration file
  - USBPDM1_Consumer_LPM/Src/ucpd.c                      UCPD configuration
  - USBPDM1_Consumer_LPM/Src/usart.c                     UART configuration
  - USBPDM1_Consumer_LPM/Src/usbpd.c                     Device defines for GUI
  - USBPDM1_Consumer_LPM/Src/usbpd_dpm_core.c            DPM core layer implementation
  - USBPDM1_Consumer_LPM/Src/usbpd_dpm_user.c            DPM layer implementation
  - USBPDM1_Consumer_LPM/Src/usbpd_lowpower.c            management of the low power
  - USBPDM1_Consumer_LPM/Src/usbpd_pwr_if.c              General power interface configuration
  - USBPDM1_Consumer_LPM/Src/usbpd_pwr_user.c            BSP power definition
  - USBPDM1_Consumer_LPM/Src/usbpd_vdm_user.c            VDM definition
  - USBPDM1_Consumer_LPM/Inc/adc.h                       ADC configuration
  - USBPDM1_Consumer_LPM/Inc/dma.c                       DMA configuration
  - USBPDM1_Consumer_LPM/Inc/FreeRTOSConfig.h            FreeRTOS module configuration file
  - USBPDM1_Consumer_LPM/Inc/gpio.h                      GPIO configuration
  - USBPDM1_Consumer_LPM/Inc/main.h                      Main program header file
  - USBPDM1_Consumer_LPM/Inc/rtc.h                       RTC configuration (RTC is used to exit low power mode)
  - USBPDM1_Consumer_LPM/Inc/stm32_lpm_if.h              hardware interface of the tiny_lpm utility
  - USBPDM1_Consumer_LPM/Inc/stm32_timer_if.h            hardware interface of the timer utility
  - USBPDM1_Consumer_LPM/Inc/stm32g0xx_hal_conf.h        HAL configuration file
  - USBPDM1_Consumer_LPM/Inc/stm32g0xx_it.h              Interrupt handlers header file
  - USBPDM1_Consumer_LPM/Inc/tracer_emb_conf.h           Debug trace UART settings
  - USBPDM1_Consumer_LPM/Inc/ucpd.h                      UCPD configuration
  - USBPDM1_Consumer_LPM/Inc/usart.h                     UART configuration
  - USBPDM1_Consumer_LPM/Inc/usbpd.h                     Device defines for GUI
  - USBPDM1_Consumer_LPM/Inc/usbpd_devices_conf.h        Device settings (Timers, DMA...)
  - USBPDM1_Consumer_LPM/Inc/usbpd_dpm_conf.h            USB-C Power Delivery application Configuration file
  - USBPDM1_Consumer_LPM/Inc/usbpd_dpm_core.h            DPM core layer implementation
  - USBPDM1_Consumer_LPM/Inc/usbpd_dpm_user.h            DPM Layer header file
  - USBPDM1_Consumer_LPM/Inc/usbpd_lowpower.h            management of the low power
  - USBPDM1_Consumer_LPM/Inc/usbpd_pdo_defs.h            PDO definition central header file
  - USBPDM1_Consumer_LPM/Inc/usbpd_pwr_if.h              Defines for PDOs
  - USBPDM1_Consumer_LPM/Inc/usbpd_pwr_user.h            BSP power definition
  - USBPDM1_Consumer_LPM/Inc/usbpd_vdm_user.h            VDM definition
  - USBPDM1_Consumer_LPM/Inc/usbpdm1_conf.h              Defines for the usbpdm1 shield (VSENSE ADC Channel)
  - USBPDM1_Consumer_LPM/Inc/utilities_conf.h            configuration ot the utilities
 
@par Hardware and Software environment

  - This application runs on STM32G0XX devices.
  
  - This example has been tested with STMicroelectronics NUCLEO-G071RB board with X-NUCLEO-USBPDM1 shield.

  - X-NUCLEO-USBPDM1 Set-up
    JP2 jumper setting : [VCC-VCC_OUT] and [DB-DB_3V3] jumpers set 
	
  - consumption measurement on the IDD pin of JP3

@par How to use it ?

In order to make the program work, you must do the following:
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 