/**
  @page USB-C Power Delivery USBPDM1_Consumer_PPS application
  
  @verbatim
  ******************************************************************************
  * @file    USBPDM1_Consumer_PPS/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB-C Power Delivery USBPDM1_Consumer_PPS application.
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
Use of USB Power Delivery (USB-PD) Consumer application with PPS running on STM32G0XX devices, 
      with X-NUCLEO-USBPDM1 shield plugged.
      Project configuration is based on USB Power Delivery Specification revision 3.0.

This application provides an example for managing the Port 0 as a Consumer Only port.
      PPS(Programmable Power Supply) is enabled, as well as the STM32CubeMonUCPD graphical user interface.
      User can use the STM32CubeMonUCPD running on a computer to send control the STM32G0XX to send some USB power
      delivery messages through the integrated UCPD peripheral.

      This application has been certified with USB-PD Test Conformance Tools (Ellisys, MQP, and LeCroy) with TID : 3036
      Vendor Information File, used for Conformance testing and  describing Port capabilities and supported 
      options is provided in application directory in STMicroelectronics_USBPDM1_Consumer_PPS_STM32G071RB_VIF.xml file.

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

  - USBPDM1_Consumer_PPS/Src/main.c                      Main program
  - USBPDM1_Consumer_PPS/Src/system_stm32g0xx.c          STM32G0xx system clock configuration file
  - USBPDM1_Consumer_PPS/Src/usbpd.c                     Device defines for GUI
  - USBPDM1_Consumer_PPS/Src/stm32g0xx_it.c              Interrupt handlers
  - USBPDM1_Consumer_PPS/Src/usbpd_dpm_user.c            DPM layer implementation
  - USBPDM1_Consumer_PPS/Src/usbpd_pwr_if.c              General power interface configuration
  - USBPDM1_Consumer_PPS/Inc/main.h                      Main program header file
  - USBPDM1_Consumer_PPS/Inc/stm32g0xx_it.h              Interrupt handlers header file
  - USBPDM1_Consumer_PPS/Inc/stm32g0xx_hal_conf.h        HAL configuration file
  - USBPDM1_Consumer_PPS/Inc/usbpd_dpm_conf.h            USB-C Power Delivery application Configuration file
  - USBPDM1_Consumer_PPS/Inc/usbpd_dpm_user.h            DPM Layer header file
  - USBPDM1_Consumer_PPS/Inc/usbpd_pdo_defs.h            PDO definition central header file
  - USBPDM1_Consumer_PPS/Inc/usbpdm1_conf.h              Defines for the usbpdm1 shield (VSENSE ADC Channel)
  - USBPDM1_Consumer_PPS/Inc/usbpd_pwr_if.h              Defines for PDOs
  - USBPDM1_Consumer_PPS/Inc/usbpd_gui_memmap.h          GUI flash memory mapping
  - USBPDM1_Consumer_PPS/Inc/usbpd_devices_conf.h        Device settings (Timers, DMA...)
  - USBPDM1_Consumer_PPS/Inc/usbpd.h                     USBPD application includes
  - USBPDM1_Consumer_PPS/Inc/tracer_emb_conf.h           Debug trace UART settings
  - USBPDM1_Consumer_PPS/Inc/FreeRTOSConfig.h            FreeRTOS module configuration file
 
@par Hardware and Software environment

  - This application runs on STM32G0XX devices.
  
  - This example has been tested with STMicroelectronics NUCLEO-G071RB board with X-NUCLEO-USBPDM1 shield.

  - X-NUCLEO-USBPDM1 Set-up
    JP2 jumper setting : [VCC-VCC_OUT] and [DB-DB_OUT] jumpers set 

@par How to use it ?

In order to make the program work, you must do the following:
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 