
## <b>DRP1M1_DRP Application Description</b>

This application provides an example for managing the Port 0 as a Dual Role Power port.

STM32CubeMonUCPD graphical user interface is enabled.

User can use the STM32CubeMonUCPD running on a computer to control the STM32G0XX to send some USB power delivery messages through the integrated UCPD peripheral.

This application has been certified with USB-PD Test Conformance Tools (Ellisys, MQP, GRL and LeCroy).

Vendor Information File, used for Conformance testing and describing Port capabilities and supported options is provided in application directory in STMicroelectronics_DRP1M1_DRP_STM32G071RB_VIF.xml file.

####  <b>Expected success behavior</b>
Connect STM32CubeMonUCPD on the VCP associated to our board (only available if USB cable is connected)
The STM32CubeMonUCPD will be used to trace all the messages exchange between the ports partner.

#### <b>Error behaviors</b>
Errors are detected and the corresponding message is displayed on the STM32CubeMonUCPD.

#### <b>Assumptions if any</b>
User is familiar with USB-PD 3.1 "Universal Serial BUS Power Delivery" Specification.

#### <b>Known limitations</b>
None.

### <b>Keywords</b>

Connectivity, UART/USART, USB_PD_Lib, UCPD, Type C, USBPD

### <b>Hardware and Software environment</b>

  - This application runs on STM32G0XX devices
  - This application has been tested with STMicroelectronics NUCLEO-G071RB board with X-NUCLEO-DRP1M1 shield
    and can be easily tailored to any other supported device and development board.

- X-NUCLEO-DRP1M1 Set-up
    - No specific Jumpers required. In order to be able to act with Source Power Role, a power supply should be connected to CN3 connector.
	
### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application

