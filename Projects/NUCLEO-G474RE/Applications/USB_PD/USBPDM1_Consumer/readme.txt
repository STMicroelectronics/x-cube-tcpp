/**
  @page USB-C Power Delivery TCPP01_Consumer application
  
  @verbatim
  ******************************************************************************
  * @file    TCPP01_Consumer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB-C Power Delivery TCPP01_Consumer application.
  ******************************************************************************
  *
  * Copyright (c) 2020 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim
  
@par Application Description
Use of USB Power Delivery (USB-PD) Consumer application with PPS running on STM32G4XX devices, 
      with X-NUCLEO-USBPDM1 shield plugged.
      Project configuration is based on USB Power Delivery Specification revision 3.0.

This application provides an example for managing the Port 0 as a Consumer Only port.
      User can use the STM32CubeMonUCPD running on a computer to send control the STM32G4XX to send some USB power
      delivery messages through the integrated UCPD peripheral.

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

  - TCPP01_Consumer/Src/main.c                      Main program
  - TCPP01_Consumer/Src/system_stm32g4xx.c          STM32G4XX system clock configuration file
  - TCPP01_Consumer/Src/usbpd.c                     Device defines for GUI
  - TCPP01_Consumer/Src/stm32g4xx_it.c              Interrupt handlers
  - TCPP01_Consumer/Src/usbpd_dpm_user.c            DPM layer implementation
  - TCPP01_Consumer/Src/usbpd_pwr_if.c              General power interface configuration
  - TCPP01_Consumer/Inc/main.h                      Main program header file
  - TCPP01_Consumer/Inc/stm32g4xx_it.h              Interrupt handlers header file
  - TCPP01_Consumer/Inc/stm32g4xx_hal_conf.h        HAL configuration file
  - TCPP01_Consumer/Inc/usbpd_dpm_conf.h            USB-C Power Delivery application Configuration file
  - TCPP01_Consumer/Inc/usbpd_dpm_user.h            DPM Layer header file
  - TCPP01_Consumer/Inc/usbpd_pdo_defs.h            PDO definition central header file
  - TCPP01_Consumer/Inc/usbpdm1_conf.h              Defines for the usbpdm1 shield (VSENSE ADC Channel)
  - TCPP01_Consumer/Inc/usbpd_pwr_if.h              Defines for PDOs
  - TCPP01_Consumer/Inc/usbpd_gui_memmap.h          GUI flash memory mapping
  - TCPP01_Consumer/Inc/usbpd_devices_conf.h        Device settings (Timers, DMA...)
  - TCPP01_Consumer/Inc/usbpd.h                     USBPD application includes
  - TCPP01_Consumer/Inc/tracer_emb_conf.h           Debug trace UART settings
  - TCPP01_Consumer/Inc/FreeRTOSConfig.h            FreeRTOS module configuration file
 
@par Hardware and Software environment

  - This application runs on STM32G4XX devices.
  
  - This example has been tested with STMicroelectronics NUCLEO-G474RE board with X-NUCLEO-USBPDM1 shield.

  - X-NUCLEO-USBPDM1 Set-up
    JP2 jumper setting : [VCC-VCC_OUT] and [DB-DB_OUT] jumpers set 

@par How to use it ?

In order to make the program work, you must do the following:
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 