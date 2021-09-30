/**
  @page USB-C Power Delivery DRP1M1_DRP application
  
  @verbatim
  ******************************************************************************
  * @file    DRP1M1_DRP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB-C Power Delivery DRP1M1_DRP application.
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
Use of USB Power Delivery (USB-PD) DRP application on STM32G4XX devices, 
      with X-NUCLEO-DRP1DM1 shield plugged.
      Project configuration is based on USB Power Delivery Specification revision 3.0.

This application provides an example for managing the Port 0 as a Dual Role Power port.
      STM32CubeMonUCPD graphical user interface is enabled.
      User can use the STM32CubeMonUCPD running on a computer to control the STM32G0XX to send some USB power
      delivery messages through the integrated UCPD peripheral.

      This application has been certified with USB-PD Test Conformance Tools (Ellisys, MQP, GRL and LeCroy).
      Vendor Information File, used for Conformance testing and describing Port capabilities and supported 
      options is provided in application directory in STMicroelectronics_DRP1M1_DRP_STM32G474RE_VIF.xml file.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct operation.

@par Directory contents

  - DRP1M1_DRP/Src/main.c                      Main program
  - DRP1M1_DRP/Src/system_stm32g0xx.c          STM32G0xx system clock configuration file
  - DRP1M1_DRP/Src/usbpd.c                     Device defines for GUI
  - DRP1M1_DRP/Src/stm32g4xx_it.c              Interrupt handlers
  - DRP1M1_DRP/Src/stm32g4xx_hal_msp.c         HAL MSP Initialization file
  - DRP1M1_DRP/Src/app_FreeRTOS.c              Code for freertos applications file 
  - DRP1M1_DRP/Src/usbpd_dpm_user.c            DPM layer implementation
  - DRP1M1_DRP/Src/usbpd_dpm_core.c            DPM core file
  - DRP1M1_DRP/Src/usbpd_vdm_user.c            Vendo Defined Message management implementation
  - DRP1M1_DRP/Src/usbpd_pwr_if.c              General power interface configuration
  - DRP1M1_DRP/Src/usbpd_pwr_user.c            Default power management service file
  - DRP1M1_DRP/Inc/usbpd_pwr_user.h            Default power management service header file
  - DRP1M1_DRP/Inc/main.h                      Main program header file
  - DRP1M1_DRP/Inc/stm32g4xx_it.h              Interrupt handlers header file
  - DRP1M1_DRP/Inc/stm32g4xx_hal_conf.h        HAL configuration file
  - DRP1M1_DRP/Inc/usbpd_dpm_conf.h            USB-C Power Delivery application Configuration file
  - DRP1M1_DRP/Inc/usbpd_dpm_user.h            DPM Layer header file
  - DRP1M1_DRP/Inc/usbpd_dpm_core.c            DPM core header file
  - DRP1M1_DRP/Inc/usbpd_vdm_user.h            Vendo Defined Message management header file
  - DRP1M1_DRP/Inc/usbpd_pdo_defs.h            PDO definition central header file
  - DRP1M1_DRP/Inc/drp1m1_conf.h               Defines for the DRP1M1 shield configuration
  - DRP1M1_DRP/Inc/usbpd_pwr_if.h              Defines for PDOs
  - DRP1M1_DRP/Inc/usbpd_gui_memmap.h          GUI flash memory mapping
  - DRP1M1_DRP/Inc/usbpd_devices_conf.h        Device settings (Timers, DMA...)
  - DRP1M1_DRP/Inc/usbpd.h                     USBPD application includes
  - DRP1M1_DRP/Inc/tracer_emb_conf.h           Debug trace UART settings
  - DRP1M1_DRP/Inc/FreeRTOSConfig.h            FreeRTOS module configuration file
 
@par Hardware and Software environment

  - This application runs on STM32G4XX devices.
  
  - This example has been tested with STMicroelectronics NUCLEO-G474RE board with X-NUCLEO-DRP1M1 shield.

  - X-NUCLEO-DRP1M1 Set-up
    No specific Jumpers required
	In order to be able to act with Source Power Role, a power supply should be connected to CN3 connector.

@par How to use it ?

In order to make the program work, you must do the following:
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 