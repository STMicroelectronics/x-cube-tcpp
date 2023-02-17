
## <b>SNK1M1_Sink_TypeC_Only Application Description</b>

This application provides an example for managing the Port 0 as a Consumer Only Type-C connector.

USB device application based on the Mass Storage Class (MSC) is enabled.

Goal is to identify a Type-C connection to power supply and to determine the RP Resistor exposed by it (thanks to ADC).

In this application, USB Device MSC (Mass Storage Class) is enabled and shows how USB on Type-C should cohabit.

####  <b>Expected success behavior</b>
When Type-C cable is plugged: LED1 on X-NUCLEO-SNK1M1 shield switches on which means that VBUS is provided by the port partner.
The Rp resistor exposed by port partner will be indicated thanks to the blinking of 
LED4 on NUCLEO-L412RB-P board:

  - 1 blink every 2sec: Default USB, allows up to 500mA
  - 2 blinks every 2sec: 1.5A
  - 3 blinks every 2sec: 3.0A

USB2.0 device should be recognized when you connect the L4 application to a computer.

#### <b>Error behaviors</b>
Errors are detected and the corresponding message is displayed on the STM32CubeMonUCPD.

#### <b>Assumptions if any</b>
User is familiar with USB-PD 3.1 "Universal Serial BUS Power Delivery" Specification.

#### <b>Known limitations</b>
None.

### <b>Keywords</b>

Connectivity, UART/USART, USB_PD_Lib, UCPD, Type C, USBPD

### <b>Hardware and Software environment</b>

  - This application runs on STM32L4XX devices
  - This application has been tested with STMicroelectronics NUCLEO-L412RB-P board with X-NUCLEO-SNK1M1 shield
    and can be easily tailored to any other supported device and development board.

- X-NUCLEO-SNK1M1 Set-up
    - JP3/JP4 jumper setting : [RESET] and [LDO_OUT] jumpers OFF
    - Solders bridges J1 and J2 connected in position 2/3
	
### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application

