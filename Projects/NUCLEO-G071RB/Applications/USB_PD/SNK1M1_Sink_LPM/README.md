
## <b>SNK1M1_Sink_LPM Application Description</b>

This application provides an example for managing the Port 0 as a Consumer Only port with LPM (Low Power Management).

LPM(Low power management) is enabled, and for consumption improvement the STM32CubeMonUCPD graphical user interface is not enabled.

The user can measure the consumption by connecting the application STM32 power monitor on the nucleo PIN IDD of the connector JP3.

User can use the STM32CubeMonUCPD running on a computer to control the STM32G0XX to send some USB power delivery messages through the integrated UCPD peripheral.

This application has been certified with USB-PD Test Conformance Tools (Ellisys, MQP, GRL and LeCroy).

Vendor Information File, used for Conformance testing and describing Port capabilities and supported options is provided in application directory in STMicroelectronics_SNK1M1_Sink_LPM_STM32G071RB_VIF.xml file.



For debug purpose, TRACER_EMB and GUI_INTERFACE utilities are enabled in this application. 

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
  - This application has been tested with STMicroelectronics NUCLEO-G071RB board with X-NUCLEO-SNK1M1 expansion board
    and can be easily tailored to any other supported device and development board.

- X-NUCLEO-SNK1M1 Set-up
    - JP3/JP4 jumper setting : [RESET] and [LDO_OUT] jumpers OFF
    - Solders bridges J1 and J2 connected in position 1/2
	- Consumption measurement on the IDD pin of JP3
	
### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application

