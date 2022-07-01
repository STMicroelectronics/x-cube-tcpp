/**
  @page USB-C Power Delivery SRC1M1_Source application
  
  @verbatim
  ******************************************************************************
  * @file    SRC1M1_Source/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB-C Power Delivery SRC1M1_Source application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim
  
@par Application Description
Use of USB Power Delivery (USB-PD) SRC application on STM32G4XX devices, 
      with X-NUCLEO-SRC1DM1 shield plugged.
      Project configuration is based on USB Power Delivery Specification revision 3.0.

This application provides an example for managing the Port 0 as a Source port.
      STM32CubeMonUCPD graphical user interface is enabled.
      User can use the STM32CubeMonUCPD running on a computer to control the STM32G0XX to send some USB power
      delivery messages through the integrated UCPD peripheral.

      This application has been certified with USB-PD Test Conformance Tools (Ellisys, MQP, GRL and LeCroy).
      Vendor Information File, used for Conformance testing and describing Port capabilities and supported 
      options is provided in application directory in STMicroelectronics_SRC1M1_Source_STM32G474RE_VIF.xml file.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct operation.

@par Directory contents

      SRC1M1_Source
      ├──── Binary
      |     └──── SRC1M1_Source.bin           Application binary file
      ├──── Src
      |     └──── app_freertos.c              Code for freertos applications file 
      |     └──── main.c                      Main program
      |     └──── stm32g4xx_hal_msp.c         HAL MSP Initialization file
      |     └──── stm32g4xx_it.c              Interrupt handlers
      |     └──── system_stm32g4xx.c          STM32G4xx system clock configuration file
      |     └──── usbpd.c                     Device defines for GUI
      |     └──── usbpd_dpm_core.c            DPM core file
      |     └──── usbpd_dpm_user.c            DPM layer implementation
      |     └──── usbpd_pwr_if.c              General power interface configuration
      |     └──── usbpd_pwr_user.c            Default power management service file
      |     └──── usbpd_vdm_user.c            Vendo Defined Message management implementation
      └──── Inc
            └──── FreeRTOSConfig.h            FreeRTOS module configuration file
            └──── main.h                      Main program header file
            └──── src1m1_conf.h               Defines for the SRC1M1 shield configuration
            └──── stm32_assert.h              Assert header file
            └──── stm32g4xx_hal_conf.h        HAL configuration file
            └──── stm32g4xx_it.h              Interrupt handlers header file
            └──── stm32g4xx_nucleo_conf.h     STM32G4xx_Nucleo board configuration file
            └──── tracer_emb_conf.h           Debug trace UART settings
            └──── usbpd.h                     USBPD application includes
            └──── usbpd_devices_conf.h        Device settings (Timers, DMA...)
            └──── usbpd_dpm_conf.h            USB-C Power Delivery application Configuration file
            └──── usbpd_dpm_core.c            DPM core header file
            └──── usbpd_dpm_user.h            DPM Layer header file
            └──── usbpd_gui_memmap.h          GUI flash memory mapping
            └──── usbpd_pdo_defs.h            PDO definition central header file
            └──── usbpd_pwr_if.h              Defines for PDOs
            └──── usbpd_pwr_user.h            Default power management service header file
            └──── usbpd_vdm_user.h            Vendo Defined Message management header file
 
@par Hardware and Software environment

  - This application runs on STM32G4XX devices.
  
  - This example has been tested with STMicroelectronics NUCLEO-G474RE board with X-NUCLEO-SRC1M1 shield.

  - X-NUCLEO-SRC1M1 Set-up
    No specific Jumpers required

@par How to use it ?

In order to make the program work, you must do the following:
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 