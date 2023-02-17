
## <b>SRC1M1_Source_TypeC_Only Application Description</b>

This application provides an example for managing an USB Type C port as Source, without Power Delivery.

Goal is to identify a Type-C connection to a sink device and to determine the RD Resistor exposed by it (with the help of an ADC).

When a sink device is connected through a Type-C cable: GREEN LED on NUCLEO-F446RE switches on which means that VBUS is provided to the port partner.

This application has been certified with USB Type-C Test Conformance Tools (Ellisys, MQP, GRL and LeCroy).

Vendor Information File, used for Conformance testing and describing Port capabilities and supported options is provided in application directory in STMicroelectronics_SRC1M1_Source_STM32F446RE_VIF.xml file.

This application is based on the following wiki: [Getting_started_with_USB_Type-C_only_Source](https://wiki.st.com/stm32mcu/wiki/STM32StepByStep:Getting_started_with_USB_Type-C_only_Source)

####  <b>Expected success behavior</b>
Vbus is active 120 ms after plugging an USB Type-C source device.

Vbus is immediately shut down and placed in safe mode (0V) when unplugging the sink device.

During connection, if an overcurrent or malfunction is detected, the Source is placed in safe mode (0V) and needs a disconnection/reconnection to restart.

#### <b>Error behaviors</b>
Errors are detected and the corresponding message is displayed on the STM32CubeMonUCPD.

#### <b>Assumptions if any</b>
None.

#### <b>Known limitations</b>
None.

### <b>Keywords</b>

Connectivity, UART/USART, Type C

### <b>Hardware and Software environment</b>

  - This application runs on STM32F4XX devices
  - This application has been tested with STMicroelectronics NUCLEO-F446RE board with X-NUCLEO-SRC1M1 shield
    and can be easily tailored to any other supported device and development board.

- X-NUCLEO-SRC1M1 Set-up
    - Move CC1 (J2) Jumper to the CC1 no UCPD position 2/3
    - Move CC2 (J3) Jumper to the CC2 no UCPD position 2/3
    - JP1 1-2 (LDO_OUT) jumper ON
    - JP1 3-4 (RESET) jumper ON
    - Supply CN3 with a 5V power supply
    - Connect PA3 (CN10 - 37) and PC4 (CN10 - 34) together	
    - Optional, for up to 3.0A:
      - Remove R35 and place it on SH19
       - Remove R39 and place it on SH21
       - Change R5 shunt resistor to a 10mOhms one
	
### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application

