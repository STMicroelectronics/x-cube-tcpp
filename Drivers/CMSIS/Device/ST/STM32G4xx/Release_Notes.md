---
pagetitle: Release Notes for STM32G4xx CMSIS
lang: en
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

::: {.card .fluid}
::: {.sectione .dark}
<center>
# <small>Release Notes for</small> STM32G4xx CMSIS
Copyright &copy; 2019 STMicroelectronics\

[![ST logo](_htmresc/st_logo.png)](https://www.st.com){.logo}
</center>
:::
:::

# Purpose

This driver provides the CMSIS device for the stm32g4xx products. This covers following series :

   - STM32G431/41xx

   - STM32G471xx

   - STM32G473/83xx

   - STM32G474/84xx

This driver is composed of the descriptions of the registers under "Include" directory.

Various template file are provided to easily build an application. They can be adapted to fit applications requirements.

- Templates/system_stm32g4xx.c contains the initialization code referred as SystemInit.
- Startup files are provided as example for IAR&copy;, KEIL&copy; and SW4STM32&copy;.
- Linker files are provided as example for IAR&copy;, KEIL&copy; and SW4STM32&copy;.

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section1_2_2" checked aria-hidden="true">
<label for="collapse-section1_2_2" aria-hidden="true">V1.2.2 / 10-November-2021</label>
<div>

## Main Changes

### Maintenance release

  - General updates to fix known defects and enhancements implementation.

## Contents

  : Additional features
\

  Headline
  --------
  - General updates to fix known defects and enhancements implementation.
  - All source files: update disclaimer to add reference to the new license agreement.
  - Add LSI maximum startup time datasheet value: LSI_STARTUP_TIME.
  - Add reset enable bits definitions for slave timers other than timer A.
  - Fix Doxygen grouping issues.
  - Add new atomic register access macros in stm32g4xx.h file.
  - Remove extra TIM5 references in the stm32g471xx.h CMSIS file and add support to the TIM20.


  : Fixed bugs list
\


## Known Limitations


## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.50.4 + ST-Link
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.31 + ST-Link
- CubeIDE toolchain V1.6.0

## Supported Devices and boards

- STM32G431xx, STM32G441xx devices
- STM32G471xx devices
- STM32G473xx, STM32G483xx devices
- STM32G474xx, STM32G484xx devices
- STM32G491xx, STM32G4A1xx devices

Note: in the section above, main changes are highlighted in **bold** since previous release.


</div>
:::
::: {.collapse}
<input type="checkbox" id="collapse-section1_2_1" aria-hidden="true">
<label for="collapse-section1_2_1" aria-hidden="true">V1.2.1 / 11-January-2021</label>
<div>

## Main Changes

### Maintenance release

  - General updates to fix known defects and enhancements implementation

## Contents

  : Additional features
\

  Headline
  --------
  - General updates to fix known defects and enhancements implementation.
  - Protect Vector table modification following SRAM or FLASH preprocessor directive by a generic preprocessor directive: USER_VECT_TAB_ADDRESS. 

  : Fixed bugs list
\


## Known Limitations


## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain **V8.50.4** + ST-Link
- RealView Microcontroller Development Kit (MDK-ARM) toolchain **V5.31** + ST-Link
- CubeIDE toolchain **V1.6.0**

## Supported Devices and boards

- STM32G431xx, STM32G441xx devices
- STM32G471xx devices
- STM32G473xx, STM32G483xx devices
- STM32G474xx, STM32G484xx devices
- STM32G491xx, STM32G4A1xx devices

Note: in the section above, main changes are highlighted in **bold** since previous release.


</div>
:::
::: {.collapse}
<input type="checkbox" id="collapse-section1_2_0" aria-hidden="true">
<label for="collapse-section1_2_0" aria-hidden="true">V1.2.0 / 26-June-2020</label>
<div>

## Main Changes

### Maintenance release

  - Add support for STM32G491xx and STM32G4A1 devices
  - General updates to fix known defects and enhancements implementation

## Contents

  : Additional features
\

  Headline
  --------
  - Add startup files for STM32G491xx/4A1xx devices
  - Rename **"TIM7_DAC_IRQHandler"** to **"TIM7_IRQHandler"** in MDK-ARM startup file for STM32G431xx/441xx/bk1cb devices
  - Remove HRTIM_BMTRGR useless constant definitions
  - Remove ADC_CFGR2_LFTRIG useless constant definitions

  : Fixed bugs list
\


## Known Limitations


## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain **V8.40.1**
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.27.1
- STM32CubeIDE toolchain V1.3.0

## Supported Devices and boards

- STM32G431xx, STM32G441xx devices
- STM32G471xx devices
- STM32G473xx, STM32G483xx devices
- STM32G474xx, STM32G484xx devices
- **STM32G491xx, STM32G4A1xx devices**

Note: in the section above, main changes are highlighted in **bold** since previous release.


</div>
:::
::: {.collapse}
<input type="checkbox" id="collapse-section1_1_1" aria-hidden="true">
<label for="collapse-section1_1_1" aria-hidden="true">V1.1.1 / 14-February-2020</label>
<div>

## Main Changes

### Maintenance release

General updates to fix known defects and enhancements implementation

## Contents

  Headline
  ----------------------------------------------------------
  - General updates to fix known defects and enhancements implementation

  : Additional features
\

  Headline
  --------
  Update STM32G473/483 startup files to support FDCAN2/3 intances
  Remove IS_TIM_SYNCHRO_INSTANCE macro from device header files

  : Fixed bugs list
\


## Known Limitations


## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.32.3
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.27.1
- STM32CubeIDE toolchain V1.3.0

## Supported Devices and boards

- STM32G431xx, STM32G441xx devices
- STM32G471xx devices
- STM32G473xx, STM32G483xx devices
- STM32G474xx, STM32G484xx devices

Note: in the section above, main changes are highlighted in **bold** since previous release.


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1_1_0" aria-hidden="true">
<label for="collapse-section1_1_0" aria-hidden="true">V1.1.0 / 28-June-2019</label>
<div>

## Main Changes

### Maintenance release

Maintenance release of CMSIS Devices drivers supporting STM32G431xx, STM32G441xx, STM32G471xx, STM32G473xx, **STM32G483xx**, STM32G474xx and STM32G484xx devices

## Contents

  Headline
  ----------------------------------------------------------
  [STM32G483xx} New CMSIS driver files to support STM32G483xx
  [STM32G471] **Enable ADC3 for STM32G471 product**
  [COMP] **Suppress of Deglitcher mode** - not supported on STM32G4 series

  : Additional features
\

  Headline
  --------
  [COMP] Suppress of Deglitcher mode not supported on STM32G4 series
  [HRTIM] Wrong definition of HRTIM1_TIMx constants
  [MDK-ARM] fix issue in startup file : invalid config wizard annotations

  : Fixed bugs list
\



## Known Limitations


## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7.2

## Supported Devices and boards

- STM32G431xx, STM32G441xx devices
- STM32G471xx devices
- STM32G473xx, **STM32G483xx** devices
- STM32G474xx, STM32G484xx devices

Note: in the section above, main changes are highlighted in **bold** since previous release.


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1_0_0" aria-hidden="true">
<label for="collapse-section1_0_0" aria-hidden="true">V1.0.0 / 12-April-2019</label>
<div>

## Main Changes

### First release

First official release for STM32G4xx devices

## Contents

CMSIS devices files for STM32G431xx, STM32G441xx, STM32G471xx, STM32G473xx, STM32G474xx and STM32G484xx.

## Known Limitations

   Headline
   -----------------------------------------------------------
   Support of STM32G483xx device in CMSIS


## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.20.2
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.25
- System Workbench STM32 (SW4STM32) toolchain V2.7.2

## Supported Devices and boards

- STM32G431xx, STM32G441xx devices
- STM32G471xx devices
- STM32G473xx devices
- STM32G474xx, STM32G484xx devices

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on STM32G4xx, visit: [[www.st.com/stm32g4](http://www.st.com/stm32g4)]

</footer>
