# X-CUBE-TCPP MCU Firmware Package

**X-CUBE-TCPP** is an STMicroelectronics original initiative. It uses for USB Type-C and Power Delivery software expansion for STM32Cube (UM2073).

**X-CUBE-TCPP** is a USB-IF certified Expansion Package and consists of libraries, drivers, sources, APIs and application examples running on STM32G0 and STM32G4 Series microcontrollers.

Check the ST web page : [USB Type-C and Power Delivery](https://www.st.com/en/applications/connectivity/usb-type-c-and-power-delivery.html#overview) for more details.

In this package, it contains different applications:
  * A [PPS Sink example on STM32G0](https://github.com/STMicroelectronics/x-cube-tcpp/tree/main/Projects/NUCLEO-G071RB/Applications/USB_PD/USBPDM1_Consumer_PPS) and a [Sink example on STM32G4](https://github.com/STMicroelectronics/x-cube-tcpp/tree/main/Projects/NUCLEO-G474RE/Applications/USB_PD/USBPDM1_Consumer) are provided to help to develop applications based on USB-PD.
The "Core" of the stack is delivered in library format while the "Device" part, in open source format, offers high level of flexibility to match the design considerations.
  * A [LPM sink example on STM32G0](https://github.com/STMicroelectronics/x-cube-tcpp/tree/main/Projects/NUCLEO-G071RB/Applications/USB_PD/USBPDM1_Consumer_LPM) is provided to explain how to use the USB-PD application in low power mode

This Expansion Package is demonstrated on one hardware implementation, but can easily be ported to any STM32 including the UCPD periperal.
Included features
* Debug tools for trace and power delivery configuration (using [ STM32CubeMonUCPD ](https://www.st.com/en/development-tools/stm32cubemonucpd.html)
* USB-PD 2.0/3.0+PPS compliant
* Tested with TCPP01
   
## Boards available
  * [X-NUCLEO-USBPDM1](https://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-power-drive-hw/x-nucleo-usbpdm1.html)

## Documentation
 * Application Note [AN5225 : USB Type-C™ Power Delivery using STM32xx Series MCUs and STM32xxx
Series MPUs](https://www.st.com/content/ccc/resource/technical/document/application_note/group1/38/94/1d/41/0e/ba/49/21/DM00536349/files/DM00536349.pdf/jcr:content/translations/en.DM00536349.pdf)
 
 * User manual [UM2552 : Managing USB power delivery stack with STM32 microcontrollers](https://www.st.com/content/ccc/resource/technical/document/user_manual/group1/aa/15/14/5d/f5/b8/4a/fc/DM00598101/files/DM00598101.pdf/jcr:content/translations/en.DM00598101.pdf)
 
 * An application to create a simple application from STM32CubeMX is available at this link : [AN5418](https://www.st.com/resource/en/application_note/dm00663511-how-to-build-a-simple-usbpd-sink-application-with-stm32cubemx-stmicroelectronics.pdf)
 * Corresponding video [here](https://www.youtube.com/watch?v=Z_Sn6CBbz-k)


## Troubleshooting

**Caution** : The **Issues** requests are strictly limited to submit problems or suggestions related to the software delivered in this repo 

**For any question** related to this solution, the hardware performance, the hardware characteristics, the tools, the environment, you can submit a topic on the [ST Community/STM32 MCUs forum](https://community.st.com/s/group/0F90X000000AXsASAW/stm32-mcus)
