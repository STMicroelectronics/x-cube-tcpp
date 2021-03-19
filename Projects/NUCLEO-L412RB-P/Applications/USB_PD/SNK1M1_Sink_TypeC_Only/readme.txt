/**
  @page USB-C Power Delivery SNK1M1_Sink_TypeC_Only application
  
  @verbatim
  ******************************************************************************
  * @file    SNK1M1_Sink_TypeC_Only/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB-C Power Delivery SNK1M1_Sink_TypeC_Only application.
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
Use of Type-C Consumer application running on STM32L4XX devices, 
      with X-NUCLEO-SNK1M1 shield plugged.
      USB device application based on the Mass Storage Class (MSC) is enabled.

This application provides an example for managing the Port 0 as a Consumer Type-C connector.

Goal is to identify a Type-C connection to power supply and to determine the RP Resistor
      exposed by it (thanks to ADC).

When Type-C cable is plugged: LED1 on X-NUCLEO-SNK1M1 shield switches on which
      means that VBUS is provided by the port partner.
      The Rp resistor exposed by port partner will be indicated thanks to the blinking of 
      LED4 on NUCLEO-L412RB-P board:
      - 1 blink every 2sec: Default USB, allows up to 500mA
      - 2 blinks every 2sec: 1.5A
      - 3 blinks every 2sec: 3.0A

In this application, USB Device MSC (Mass Storage Class) is enabled and shows how USB on
      Type-C should cohabit (you should listen to the USB2.0 pop-up sound when you
      connect the L4 application to your laptop)

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct operation.

@par Directory contents

  - SNK1M1_Sink_TypeC_Only/Binary/SNK1M1_Sink_TypeC_Only.bin     Application binary file
  - SNK1M1_Sink_TypeC_Only/Src/app_freertos.c                    FreeRTOS application file
  - SNK1M1_Sink_TypeC_Only/Src/main.c                            Main program
  - SNK1M1_Sink_TypeC_Only/Src/stm32l4xx_hal_msp.c               MSP file
  - SNK1M1_Sink_TypeC_Only/Src/stm32l4xx_it.c                    Interrupt handlers
  - SNK1M1_Sink_TypeC_Only/Src/system_stm32f4xx.c                STM32L4XX system clock configuration file
  - SNK1M1_Sink_TypeC_Only/Src/usb_device.c                      USB Device application code
  - SNK1M1_Sink_TypeC_Only/Src/usbd_conf.c                       General low level driver configuration
  - SNK1M1_Sink_TypeC_Only/Src/usb_desc.c                        USB device descriptor
  - SNK1M1_Sink_TypeC_Only/Src/usbd_storage_if.c                 Internal flash memory management
  - SNK1M1_Sink_TypeC_Only/Src/usbpd_usb_if.c                    USB-C -> USB Data interface
  - SNK1M1_Sink_TypeC_Only/Inc/FreeRTOSConfig.h                  FreeRTOS module configuration file
  - SNK1M1_Sink_TypeC_Only/Inc/main.h                            Main program header file
  - SNK1M1_Sink_TypeC_Only/Inc/stm32l4xx_hal_conf.h              HAL configuration file
  - SNK1M1_Sink_TypeC_Only/Inc/stm32l4xx_it.h                    Interrupt handlers header file
  - SNK1M1_Sink_TypeC_Only/Inc/usb_device.h                      USB Device application header file
  - SNK1M1_Sink_TypeC_Only/Inc/usbd_desc.h                       USB device descriptor header file
  - SNK1M1_Sink_TypeC_Only/Inc/usbd_storage_if.h                 Internal flash memory management header file
  - SNK1M1_Sink/Inc/usbpd_usb_if.h                               USB-C -> USB Data interface header file
  - SNK1M1_Sink_TypeC_Only/Inc/usbd_conf.h                       USB device driver Configuration file

@par Hardware and Software environment

  - This application runs on STM32L4XX devices.
  
  - This example has been tested with STMicroelectronics NUCLEO-L412RB-P board with X-NUCLEO-SNK1M1 shield.

  - X-NUCLEO-SNK1M1 Set-up
    JP3/JP4 jumper setting : [RESET] and [LDO_OUT] jumpers OFF
    Solders bridges J1 and J2 connected in position 2/3

@par How to use it ?

In order to make the program work, you must do the following:
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 