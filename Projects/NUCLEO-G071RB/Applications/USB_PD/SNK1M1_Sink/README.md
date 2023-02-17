
## <b>SNK1M1_Sink Application Description</b>

This application provides an example for managing the Port 0 as a Consumer Only port.

STM32CubeMonUCPD graphical user interface is enabled.

User can use the STM32CubeMonUCPD running on a computer to control the STM32G0XX to send some USB power delivery messages through the integrated UCPD peripheral.

This application has been certified with USB-PD Test Conformance Tools (Ellisys, MQP, GRL and LeCroy).

Vendor Information File, used for Conformance testing and describing Port capabilities and supported options is provided in application directory in STMicroelectronics_SNK1M1_Sink_STM32G071RB_VIF.xml file.

This application is based on the following wiki: [STM32_Advance_USB-Power_Delivery_Sink](https://wiki.st.com/stm32mcu/wiki/STM32StepByStep:STM32_Advance_USB-Power_Delivery_Sink)

STM32CubeMX is used to configure the project.
The following sink PDOs are configured in STM32CubeMX:

  - 5V 1.5A
  - 9V 3A
  - 15V 2A
  - 20V 1A

By default, the sink asks for the highest power matching PDO of the connected source. User can change 
this behaviour by changing the USER_SERV_PDO_Sel_Method variable in usbpd_user_services.c.
For example, if your source supports 5V/2A, 9V/2A, 15V/2A and 20V/2A, the sink will ask for the 15V profile,
as it is the highest power profile the sink and the source can support.


| Sink PDO | Source capabilities | Sink drawing pwr | Supported by source ?                                |
|----------|---------------------|------------------|------------------------------------------------------|
| 5V 1.5A  | 5V 2A (10W)         | 7.5W             | YES                                                  |
| 9V 3A    | 9V 2A (18W)         | 9W               | NO                                                   |
| 15V 2A   | 15V 2A (30W)        | 30W              | YES <------- Chosen profile (highest supported power)|
| 20V 3A   | 20V 2A (40W)        | 60W              | NO                                                   |

You can change the PDO selection method with the USER_SERV_PDO_Sel_Method variable. Availables methods are:

  - Max power
  - Min power
  - Max voltage
  - Min voltage
  - Max current
  - Min current

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
  - This application has been tested with STMicroelectronics NUCLEO-G071RB board with X-NUCLEO-SNK1M1 shield
    and can be easily tailored to any other supported device and development board.

- X-NUCLEO-SNK1M1 Set-up
    - JP3/JP4 jumper setting : [RESET] and [LDO_OUT] jumpers OFF
    - Solders bridges J1 and J2 connected in position 1/2
	
### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application

