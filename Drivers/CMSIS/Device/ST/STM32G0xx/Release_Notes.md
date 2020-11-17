---
pagetitle: Release Notes for STM32G0xx CMSIS
lang: en
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

::: {.card .fluid}
::: {.sectione .dark}
<center>
# <small>Release Notes for</small> <mark>STM32G0xx CMSIS</mark>
Copyright &copy; 2018 STMicroelectronics\
    
[![ST logo](_htmresc/st_logo.png)](https://www.st.com){.logo}
</center>
:::
:::

# License

This software component is licensed by ST under BSD 3-Clause
license, the "License"; You may not use this component except in
compliance with the License. You may obtain a copy of the
License at:

[https://opensource.org/licenses/BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)

# Purpose

This driver provides the CMSIS device files for the stm32g0xx products. This covers 

- STM32G081/71/70xx devices
- STM32G041/31/30xx devices

This driver is composed of the descriptions of the registers under "Include" directory.

Various template files are provided to easily build an application. They can be adapted to fit applications requirements.

- Templates/system_stm32g0xx.c contains the initialization code referred as SystemInit.
- Startup files are provided as example for IAR&copy;, KEIL&copy; and SW4STM32&copy;.
- Linker files are provided as example for IAR&copy;, KEIL&copy; and SW4STM32&copy;.

:::

::: {.col-sm-12 .col-lg-8}
# Update History
::: {.collapse}
<input type="checkbox" id="collapse-section4" checked aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">V1.3.0 / 25-June-2019 </label>
<div>

## Main Changes

### Maintenance release

Maintenance release for STM32G0xx CMSIS supporting stm32g030xx, stm32g031xx, stm32g041xx, stm32g070xx, stm32g071xx, stm32g081xx devices.

  Headline
  --------
  Remove all DMAMUX register/bit definition related to internal debug features

  : Fixed bugs list
\

## Contents

CMSIS devices files for stm32g030xx, stm32g031xx, stm32g041xx devices.

CMSIS devices files for stm32g070xx, stm32g071xx, stm32g081xx devices.

## Known Limitations

  Headline
  -----------------------------------------------------------
  [MISRAC2012-Rule-8.9_b] Global variable APBPrescTable is only referenced from a single function

  : Requirements not met or planned in a forthcoming release

\

## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7.2

## Supported Devices and boards

- STM32G041xx, STM32G031xx and STM32G030xx devices
- STM32G081xx, STM32G071xx and STM32G070xx devices

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">V1.2.0 / 05-April-2019 </label>
<div>

## Main Changes

### Maintenance release and Product Update

First release for STM32G0xx CMSIS introducing **stm32g030xx, stm32g031xx, stm32g041xx** devices.

  Headline
  ----------------------------------------------------------
  New CMSIS Drivers files to support STM32G041xx, STM32G031xx, STM32G030xx

  : Additional features
\

  Headline
  --------
  Correct wrong definition of IS_TIM_TISEL_INSTANCE for STM32G0x0 devices
  Mention bit ADC_CCR_LFMEN as useless in STM32G0, kept for legacy purpose

  : Fixed bugs list
\

## Contents

CMSIS devices files for stm32g030xx, stm32g031xx, stm32g041xx devices.

CMSIS devices files for stm32g070xx, stm32g071xx, stm32g081xx devices.

## Known Limitations

  Headline
  -----------------------------------------------------------
  [MISRAC2012-Rule-8.9_b] Global variable APBPrescTable is only referenced from a single function
  Use latest CMSIS V5.4.0

  : Requirements not met or planned in a forthcoming release

\

## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7.2

## Supported Devices and boards

- STM32G041xx, STM32G031xx and STM32G030xx devices
- STM32G081xx, STM32G071xx and STM32G070xx devices

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">V1.1.0 / 06-February-2019 </label>
<div>

## Main Changes

### Maintenance release

Maintenance release for **STM32G0xx** devices (stm32g070xx, stm32g071xx, stm32g081xx devices)


 Headline
 --------
 [Keil] Startup file for Keil generated project contain errors
 [Licenses update] remove html tags in Startup files
 MISRAC2012 implementation
 Correct undefined reference to `LPUART1_IRQHandler' for some G0 variants

  : Fixed bugs list

## Contents

CMSIS devices files for stm32g070xx, stm32g071xx, stm32g081xx devices.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true"> V1.0.0 / 26-October-2018 </label>
<div>			

## Main Changes

### First release

First official release for **STM32G0xx** devices

## Contents

- CMSIS devices files for STM32G070xx, STM32G071xx and STM32G081xx

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on STM32G0xx,visit: [[www.st.com/stm32g0](http://www.st.com/stm32g0)]

This release note uses up to date web standards and, for this reason, should not be opened with Internet Explorer
but preferably with popular browsers such as Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.
</footer>
