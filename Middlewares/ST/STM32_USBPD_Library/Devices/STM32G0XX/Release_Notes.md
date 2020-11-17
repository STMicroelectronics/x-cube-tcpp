---
pagetitle: Release Notes for STM32 USB-C Power Delivery G0 Device Driver
lang: en
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

::: {.card .fluid}
::: {.sectione .dark}
<center>
# <small>Release Notes for</small> STM32 USB-C Power Delivery G0 Device Driver
Copyright &copy; 2018 STMicroelectronics\
    
[![ST logo](_htmresc/st_logo.png)](https://www.st.com){.logo}
</center>
:::
:::

# License

This software component is licensed by ST under Ultimate Liberty license
SLA0044, the \"License\"; You may not use this component except in
compliance with the License. You may obtain a copy of the License at:

[http://www.st.com/SLA0044](http://www.st.com/SLA0044)

# Purpose

The USB-PD device driver provides a set of functions to manage the physical layer (i.e. low level of the type C state machine and low level of message transport). This includes :

- Type C state machine: SRC, SNK or DRP

- Physical layer : message handling SOP, SOP', SOP'', HARDRESET, ...

- Timer server to handle GOODCRC, PRL repetition timing

The USB-PD library is developed following the Universal Serial Bus Power Delivery Specification Revision 3.0, V2.0 (August, 2019) and Universal Serial Bus type-C Cable 
and Connector Specification, Revision 2.0 (August 2019). It has passed successfully the official certification.
:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section8" checked aria-hidden="true">
<label for="collapse-section8" aria-hidden="true">V3.0.0 / 07-Apr-2020</label>
<div>

## Main Changes

### Maintenance release

  Headline
  ----------------------------------------------------------
  Ticket 76935 - CAD power role overwritten in configuration _SRC & _SNK
  update to avoid issue in errorrecovery when prower role swap failed
  UCPD IT should be not enabled for NoPD configuration
  Fix a pb with NRTOS version on Keil environment (Ellisys test failed)
  TD.PD.C.E5 patch for NRTOS application
  Solve issue in DISCO configuration (SINK not working at all since integration of BSP V2.6)
  Ticket 70979 -  Ellisys 4.9.4 Source Vconn Swap (Testing Ufp) failed
  Solve randomness reloading of CC1 and CC2 values in local variables in CAD_Check_HW() function according to optimizations options, code sequences.
  Improve the disconnection timing to avoid issue with Ellisys (4.8.3)
  Align new BSP Power interfaces with BSP v2.6
  Initialize power in init function to avoid issues with safety

## Known Limitations

  Outstanding bugs list : None

  Requirements not met or planned in a forthcoming release : None

## Development Toolchains and Compilers


- IAR Embedded Workbench for ARM (EWARM) toolchain V8.32.3
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.27.1.0
- System Workbench STM32 (SW4STM32) toolchain V2.7.2

## Supported Devices and boards

  All STM32G0xx devices embedding USBPD IP. This tag has been used for official certification of G0 + TCPP01 (SINK PPS 100W - TID3036)

## Backward compatibility

  No compatibility break with previous version

## Dependencies

 This software release is compatible with USB-C Power Delivery Core Stack Library v2.11.0 or v3.0.0

</div>
:::



::: {.collapse}
<input type="checkbox" id="collapse-section7"  aria-hidden="true">
<label for="collapse-section7" aria-hidden="true">V2.6.0 / 10-Apr-2019</label>
<div>

## Main Changes

### Maintenance release

  Headline
  ----------------------------------------------------------
  [LOWPOWER] update to allow stop mode in attached cases
  [LOWPOWER] improvement link with lowpower management
  [LOWPOWER] rework aroun VBUS init/deinit TIMER Init/Deinit
  Ticket 64288 - USBPD_PHY_GetRetryTimerValue
  Udpate to improve the state machine : increase timing detection, symetrie VBUSInit, VBUSDeInit
  Change switch SRC to _SRC
  Conf_template.h updates.
  Add DMA LL Struct initialisation function call.
  Rework around CC disabled
  Add TIMDenit management - add VBUSDenit - add VCONNDeinit - state machine detection optimization
  Detach not detected in NRTOS in case of switch from SRC to RA (ellisys)
  Update to avoid wrong detection
  Trace in case of attachement read the CC state from register
  Change return value from uint32_t to uint16_t for HW_IF_PWR_GetVoltage (compatibility with F0)
  Add a workaround to fix a glitch issue in NRTOS version
  Udpate exported function description for CAD and timerserver
  Update PHY function description
  Remove USE_STM32G081B_EVAL_REVx switches

  : Fixed bugs list

## Known limitations

  Outstanding bugs list : None

  Requirements not met or planned in a forthcoming release : None

## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7.2

## Supported Devices and boards

  All STM32G0xx devices embedding USBPD IP

## Backward compatibility

  This version introduces a compatibility break compared to previous version (HW IF PWR interface update)

## Dependencies

 This software release is compatible with USB-C Power Delivery Core Stack Library v2.6.0 

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section6" aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">V2.5.0 / 10-Dec-2018</label>
<div>			

## Main Changes

### Maintenance release

  Headline
  ----------------------------------------------------------
  Editorial correction (renaming toogle by toggle)
  Wrong parameter for errorrecovery

  
  : Fixed bugs list

## Known limitations

  Outstanding bugs list : None

  Requirements not met or planned in a forthcoming release : None

## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7.2

## Supported Devices and boards

  All STM32G0xx devices embedding USBPD IP

## Backward compatibility

  This version introduces a compatibility break compared to previous version (HW IF PWR interface update)

## Dependencies

  NA
  
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">V2.4.0 / 15-Nov-2018</label>
<div>			

## Main Changes

### Maintenance release

  Headline
  ----------------------------------------------------------
  Update for cubemx integration
  Remove BSP reference, now managed trough the header file usbpd_devices_conf.h
  Remove reference to disco board
  Update for error recovery management

  
  : Fixed bugs list
## Known limitations

  Outstanding bugs list : None

  Requirements not met or planned in a forthcoming release : None

## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7.2

## Supported Devices and boards

  All STM32G0xx devices embedding USBPD IP

## Backward compatibility

  This version introduces a compatibility break compared to previous version (HW IF PWR interface update)

## Dependencies

  NA
  
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">V2.3.0 / 09-Oct-2018</label>
<div>			

## Main Changes

### Maintenance release

  Headline
  ----------------------------------------------------------
  LL_UCPD_Init prototype misalignment
  
  : Fixed bugs list
## Known limitations

  Outstanding bugs list : None

  Requirements not met or planned in a forthcoming release : None

## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7.2

## Supported Devices and boards

  All STM32G0xx devices embedding USBPD IP

## Backward compatibility

  This version introduces a compatibility break compared to previous version (HW IF PWR interface update)

## Dependencies

  NA
  
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">V2.2.0 / 07-Sep-2018</label>
<div>			

## Main Changes

### Maintenance release

  Headline
  ----------------------------------------------------------
  Minor update to avoid detection behavior in case of vconn management
  
  : Fixed bugs list
## Known limitations

  Outstanding bugs list : None

  Requirements not met or planned in a forthcoming release : None

## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7.2

## Supported Devices and boards

  All STM32G0xx devices embedding USBPD IP

## Backward compatibility

  This version introduces a compatibility break compared to previous version (HW IF PWR interface update)

## Dependencies

  NA
  
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">V2.1.1 / 15-Jun-2018</label>
<div>			

## Main Changes

### Maintenance release

  Headline
  --------
  Main change add a file usbpd_devices_conf.h to configure the device
  
  : Fixed bugs list

## Known limitations

  Outstanding bugs list : None

  Requirements not met or planned in a forthcoming release : None

## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7.2

## Supported Devices and boards

  All STM32G0xx devices embedding USBPD IP

## Backward compatibility

  This version introduces a compatibility break compared to previous version (HW IF PWR interface update)

## Dependencies

  NA
  
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">V1.0.0 / 13-Sep-2017</label>
<div>			

## Main Changes

### Initial release

  Headline
  ----------------------------------------------------------
  First official version for STM32G0xx device (source code available).

## Known limitations

  Outstanding bugs list : None

  Requirements not met or planned in a forthcoming release : None

## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7.2

## Supported Devices and boards

  All STM32G0xx devices embedding USBPD IP

## Backward compatibility

  This version introduces a compatibility break compared to previous version (HW IF PWR interface update)

## Dependencies

  NA
  
</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on **STM32 32-bit Arm Cortex MCUs**,
visit: [http://www.st.com/STM32](http://www.st.com/STM32)

This release note uses up to date web standards and, for this reason, should not
be opened with Internet Explorer but preferably with popular browsers such as
Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.
</footer>
