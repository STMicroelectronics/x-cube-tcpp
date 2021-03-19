/**
  @page USB-C Power Delivery SNK1M1_Sink application
  
  @verbatim
  ******************************************************************************
  * @file    SNK1M1_Sink/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB-C Power Delivery SNK1M1_Sink application.
  ******************************************************************************
  *
  * Copyright (c) 2021 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim
  
@par Application Description
Use of USB Power Delivery (USB-PD) Consumer application with PPS running on STM32G0XX devices, 
      with X-NUCLEO-SNK1M1 shield plugged.
      Project configuration is based on USB Power Delivery Specification revision 3.0.

This application provides an example for managing the Port 0 as a Consumer Only port.
      PPS(Programmable Power Supply) is enabled, as well as the STM32CubeMonUCPD graphical user interface.
      User can use the STM32CubeMonUCPD running on a computer to send control the STM32GXX to send some USB power
      delivery messages through the integrated UCPD peripheral.

      This application has been certified with USB-PD Test Conformance Tools (Ellisys, MQP, GRL and LeCroy) with TID : 5205.
      Vendor Information File, used for Conformance testing and  describing Port capabilities and supported 
      options is provided in application directory in STMicroelectronics_SNK1M1_Sink_STM32G071RB_VIF.xml file.

Goal is to identify a Type-C connection to power supply and to determine:
      - if PD capable: LED1 from Shield turn-on + LED2 from NUCLEO board turn-on
      - if no PD capable: LED1 from Shield turn-on + LED2 will blink to expose RP resistor. 

When Type-C cable is plugged to NO PD port partner: LED1 on X-NUCLEO-SNK1M1 shield switches on which
      means that VBUS is provided by the port partner.
      The Rp resistor exposed by port partner will be indicated thanks to the blinking of 
      LED4 on NUCLEO-G071RB board:
      - 1 blink every 2sec: Default USB, allows up to 500mA
      - 2 blinks every 2sec: 1.5A
      - 3 blinks every 2sec: 3.0A

For debug purpose, TRACER_EMB and GUI_INTERFACE utilities are enabled in this application. 
      STM32CubeMonUCPD can be used to display USB-PD traces and to interact with the port partner.
      Tool can be found at the following link: https://www.st.com/en/development-tools/stm32cubemonucpd.html.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct operation.

@par Directory contents

  - SNK1M1_Sink/Binary/G0_SNK1M1_Sink.bin       Application binary file
  - SNK1M1_Sink/Src/app_freertos.c              FreeRTOS application file
  - SNK1M1_Sink/Src/main.c                      Main program
  - SNK1M1_Sink/Src/stm32g0xx_hal_msp.c         STM32G0XX HAL MSP functions
  - SNK1M1_Sink/Src/stm32g0xx_it.c              Interrupt handlers
  - SNK1M1_Sink/Src/system_stm32g0xx.c          STM32G0XX system clock configuration file
  - SNK1M1_Sink/Src/usbpd.c                     Main USBPD file for initialization
  - SNK1M1_Sink/Src/usbpd_dpm_core.c            DPM CORE layer implementation
  - SNK1M1_Sink/Src/usbpd_dpm_user.c            DPM USER layer implementation
  - SNK1M1_Sink/Src/usbpd_pwr_if.c              General power interface configuration
  - SNK1M1_Sink/Src/usbpd_pwr_user.c            User power implementation
  - SNK1M1_Sink/Src/usbpd_vdm_user.c            User VDM implementation
  - SNK1M1_Sink/Inc/FreeRTOSConfig.h            FreeRTOS module configuration file
  - SNK1M1_Sink/Inc/main.h                      Main program header file
  - SNK1M1_Sink/Inc/stm32_assert.h              Assert header file
  - SNK1M1_Sink/Inc/stm32g0xx_hal_conf.h        HAL configuration file
  - SNK1M1_Sink/Inc/stm32g0xx_it.h              Interrupt handlers header file
  - SNK1M1_Sink/Inc/tracer_emb_conf.h           Debug trace UART settings
  - SNK1M1_Sink/Inc/usbpd.h                     USBPD application includes
  - SNK1M1_Sink/Inc/usbpd_devices_conf.h        Device settings (Timers, DMA...)
  - SNK1M1_Sink/Inc/usbpd_dpm_conf.h            USB-C Power Delivery application Configuration file
  - SNK1M1_Sink/Inc/usbpd_dpm_user.h            DPM Layer header file
  - SNK1M1_Sink/Inc/usbpd_gui_memmap.h          Memory mapping for GUI interface
  - SNK1M1_Sink/Inc/usbpd_pdo_defs.h            PDO definition central header file
  - SNK1M1_Sink/Inc/usbpd_pwr_if.h              Defines for PDOs
  - SNK1M1_Sink/Inc/usbpd_pwr_user.h            User power header file
  - SNK1M1_Sink/Inc/usbpd_vdm_user.h            User VDM header file

@par Hardware and Software environment

  - This application runs on STM32G0XX devices.
  
  - This example has been tested with STMicroelectronics NUCLEO-G071RE board with X-NUCLEO-SNK1M1 shield.

  - X-NUCLEO-SNK1M1 Set-up
    JP3/JP4 jumper setting : [RESET] and [LDO_OUT] jumpers OFF
    Solders bridges J1 and J2 connected in position 1/2

@par How to use it ?

In order to make the program work, you must do the following:
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 