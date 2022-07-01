/**
  @page USB-C Power Delivery SNK1M1_Sink application
  
  @verbatim
  ******************************************************************************
  * @file    SNK1M1_Sink/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB-C Power Delivery SNK1M1_Sink application.
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
Use of USB Power Delivery (USB-PD) Consumer application with PPS running on STM32G4XX devices, 
      with X-NUCLEO-SNK1M1 shield plugged.
      Project configuration is based on USB Power Delivery Specification revision 3.0.

This application provides an example for managing the Port 0 as a Consumer Only port.
      This application has been certified with USB-PD Test Conformance Tools (Ellisys, MQP, GRL and LeCroy).
      Vendor Information File, used for Conformance testing and  describing Port capabilities and supported 
      options is provided in application directory in STMicroelectronics_SNK1M1_Sink_STM32G474RE_VIF.xml file.

      This application is based on the following wiki:
      https://wiki.st.com/stm32mcu/wiki/STM32StepByStep:STM32_Advance_USB-Power_Delivery_Sink
      
      STM32CubeMX is used to configure the project.
      The following sink PDOs are configured in STM32CubeMX:
            - 5V 1.5A
            - 9V 3A
            - 15V 2A
            - 20V 1A

      By default, the sink asks for the highest power matching PDO of the connected source. User can change 
      this behaviour by changing the USED_PDO_SEL_METHOD define in usbpd_user_services.c.
      For example, if your source supports 5V/2A, 9V/2A, 15V/2A and 20V/2A, the sink will ask for the 15V profile,
      as it is the highest power profile the sink and the source can support.

      ╔══════════╦═════════════════════╦══════════════════╦═══════════════════════╗
      ║ Sink PDO ║ Source capabilities ║ Sink drawing pwr ║ Supported by source ? ║
      ╠══════════╬═════════════════════╬══════════════════╬═══════════════════════╣
      ║ 5V 1.5A  ║    5V  2A  (10W)    ║      7.5W        ║         YES           ║
      ║ 9V   3A  ║    9V  2A  (18W)    ║        9W        ║          NO           ║
      ║ 15V  2A  ║    15V 2A  (30W)    ║       30W        ║         YES           ║ <------- Chosen profile
      ║ 20V  3A  ║    20V 2A  (40W)    ║       60W        ║          NO           ║          (highest supported power)
      ╚══════════╩═════════════════════╩══════════════════╩═══════════════════════╝

      You can change the PDO selection method with the USED_PDO_SEL_METHOD define. Availables methods are:
            - Max power
            - Min power
            - Max voltage
            - Min voltage
            - Max current
            - Min current

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

      SNK1M1_Sink
      ├──── Binary
      |     └──── G4_SNK1M1_Sink.bin          Application binary file
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
      |     └──── usbpd_pwr_user.h            Default power management service implementation
      |     └──── usbpd_user_services.c       PDO handling services implementation
      |     └──── usbpd_vdm_user.c            Vendo Defined Message management implementation
      └──── Inc
            └──── FreeRTOSConfig.h            FreeRTOS module configuration file
            └──── main.h                      Main program header file
            └──── stm32_assert.h              Assert header file
            └──── stm32g4xx_hal_conf.h        HAL configuration file
            └──── stm32g4xx_it.h              Interrupt handlers header file
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
            └──── usbpd_user_services.c       PDO handling services header file
            └──── usbpd_vdm_user.h            Vendo Defined Message management header file

@par Hardware and Software environment

  - This application runs on STM32G4XX devices.
  
  - This example has been tested with STMicroelectronics NUCLEO-G474RE board with X-NUCLEO-SNK1M1 shield.

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
 