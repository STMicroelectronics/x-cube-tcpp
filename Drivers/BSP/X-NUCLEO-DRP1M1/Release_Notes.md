---
pagetitle: X-NUCLEO-DRP1DM1 BSP Driver
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}


<center>
# Release Notes for X-NUCLEO-DRP1DM1 BSP Driver
Copyright &copy; 2020 STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>

# License

This software component is licensed by ST under BSD 3-Clause license, the "License"; 
You may not use this component except in compliance with the License. You may obtain a copy of the License at:

[http://www.opensource.org/licenses/BSD-3-Clause](http://www.opensource.org/licenses/BSD-3-Clause)

# Purpose

The X-NUCLEO-DRP1DM1 is an expansion board for the NUCLEO-G071RB and NUCLEO-G474RE development boards or any STM32 Nucleo development board equipped with a USB Type-C™ peripheral.

[![IMAGE](_htmresc/x-nucleo-drp1m1.jpg)](https://www.st.com/en/ecosystems/x-nucleo-drp1m1.html)

It provides a straightforward means for evaluating USB Type-C™ Power Delivery in DRP mode based on TCPP03-M12.
The USB Type-C connector can supply the STM32 Nucleo development board thanks to a 3.3 V LDO.



:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section3" checked aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">V1.2.0 / 06-July-2021</label>
<div>

## Main Changes

### Maintenance release


## Contents

  Headline
  --------
  Implementation of OCP recovery procedure in case of recoverable errors (OCP detected at USB key insertion)
  Update calls to TCP0203 component API, using TCPP0203_Driver structure
  Update Port descriptor Pwr mode when forced to Low Power in SRC case

  : Fixed bugs list

## Known Limitations

  Outstanding bugs list : None

  Requirements not met or planned in a forthcoming release : None

## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.50.6
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.31
- STM32CubeIDE toolchain V1.7.0

## Supported Devices and boards

  The X-NUCLEO-DRP1M1 is an expansion board for the NUCLEO-G071RB and NUCLEO-G474RE.

## Backward compatibility

  No compatibility break with previous version

## Dependencies

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">V1.1.0 / 12-Mar-2021</label>
<div>

## Main Changes

### Maintenance release


## Contents

  Headline
  --------
  Solve some GCC warnings + MISRA corrections
  Add check of TCPP03 powermode value when configuring to SRC mode (should not be Hibernate)
  Implement BSP_USBPD_PWR_VBUSIsOn() body
  Move resistance values used for VBUS sensing in configuration part
  Power mode update moved from device to BSP
  Remove useless definitions + MCUAStyle corrections
  Management of VBUS powered Sink case : Automatic switch to Low power mode, when Hibernate is selected, and VBUS is detected
  Solve Ellysis failed TC : TD.PD.PC.E2 PS_RDY Sent Timely
  Implement Discharge time during VBUS off procedure
  Solve SNK problems on MQP (no detection when in Hibernate)

  : Fixed bugs list

## Known Limitations

  Outstanding bugs list : None

  Requirements not met or planned in a forthcoming release : None

## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.32.3
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.27.1
- STM32CubeIDE toolchain V1.5.0

## Supported Devices and boards

  The X-NUCLEO-DRP1M1 is an expansion board for the NUCLEO-G071RB and NUCLEO-G474RE.

## Backward compatibility

  No compatibility break with previous version

## Dependencies

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">V1.0.0 / 29-June-2020</label>
<div>

## Contents

  Headline
  --------
  Initial release of BSP driver for Nucleo G4 + TCPP03  (DRP configuration)

  : Fixed bugs list

## Known Limitations

  Outstanding bugs list : None

  Requirements not met or planned in a forthcoming release : None

## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.32.3
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.27.1
- STM32CubeIDE toolchain V1.5.0

## Supported Devices and boards

  The X-NUCLEO-DRP1M1 is an expansion board for the NUCLEO-G071RB and NUCLEO-G474RE.

## Backward compatibility

  No compatibility break with previous version

## Dependencies

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on STM32,visit: [[www.st.com/stm32](http://www.st.com)]

This release note uses up to date web standards and, for this reason, should not be opened with Internet Explorer
but preferably with popular browsers such as Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.
</footer>
