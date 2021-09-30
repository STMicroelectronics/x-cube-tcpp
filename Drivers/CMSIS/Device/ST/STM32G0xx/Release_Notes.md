---
pagetitle: Release Notes for STM32G0xx CMSIS
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}


<center>
# Release Notes for <mark>STM32G0xx CMSIS</mark>
Copyright &copy; 2018 STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>

# Purpose

This driver provides the CMSIS device files for the stm32g0xx products. This covers 

- STM32G081/71/70xx devices
- STM32G041/31/30xx devices
- **STM32G0C1/B1/B0xx devices**
- **STM32G061/51/50xx devices**

This driver is composed of the descriptions of the registers under "Include" directory.

Various template files are provided to easily build an application. They can be adapted to fit applications requirements.

- Templates/system_stm32g0xx.c contains the initialization code referred as SystemInit.
- Startup files are provided as example for IAR&copy;, KEIL&copy; and STM32CubeIDE&copy;.
- Linker files are provided as example for IAR&copy;, KEIL&copy; and STM32CubeIDE&copy;.

:::

::: {.col-sm-12 .col-lg-8}
# Update History
::: {.collapse}
<input type="checkbox" id="collapse-section6" checked aria-hidden="true">
<label for="collapse-section6" area-hidden="true">V1.4.1 / 17-June-2021 </label>
<div>

## Main Changes

- Update to remove wrong bits defined for DMAMUX Req ID.
- Protect Vector table modification following SRAM or FLASH preprocessor directive by a generic preprocessor directive : USER_VECT_TAB_ADDRESS.
- Add new atomic register access macros in stm32g0xx.h file.
- Add LSI maximum startup time datasheet value: LSI_STARTUP_TIME.
- Update Licensing information and format
  - Add LICENSE.txt file
  - Remove License message from Release_Notes.html
  - Update header files with new license format

## Contents

- CMSIS devices files for stm32g0B0xx, stm32g0B1xx, stm32g0C1xx devices.
- CMSIS devices files for stm32g050xx, stm32g051xx, stm32g061xx devices.
- CMSIS devices files for stm32g030xx, stm32g031xx, stm32g041xx devices.
- CMSIS devices files for stm32g070xx, stm32g071xx, stm32g081xx devices.

## Known Limitations

- None

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section5"  aria-hidden="true">
<label for="collapse-section5" area-hidden="true">V1.4.0 / 29-October-2020 </label>
<div>

## Main Changes

### Maintenance release and Product Update

Official release for STM32G0xx CMSIS introducing **stm32g0b0xx, stm32g0b1xx, stm32g0c1xx devices** and 
**stm32g050xx, stm32g051xx, stm32g061xx devices.**

Maintenance release for STM32G0xx CMSIS supporting stm32g030xx, stm32g031xx, stm32g041xx, stm32g070xx, stm32g071xx, stm32g081xx devices.

**Additional features**

  Headline
  --------
  New CMSIS Drivers files to support STM32G0C1xx, STM32G0B1xx, STM32G0B0xx
  New CMSIS Drivers files to support STM32G061xx, STM32G051xx, STM32G050xx
  Add CMSIS files for STM32CubeIDE and remove CMSIS files for SW4STM32

**Fixed bugs list**

  Headline
  --------
  [G0 64K][Flash] page number field is too big on 64k
  Fix wrong I2C instance in macro IS_I2C_WAKEUP_FROMSTOP_INSTANCE()
  Update to call SystemInit first in startup/Reset_Handler, so GCC code is similar to IAR/Keil
  Improve the startup code on GCC
  correction for swapped for BORR and BORF bit fields
  G0 GCC startup file should use cortex M0+
  Move FLASH_SIZE define from hal flash. h to cmsis device file
  Remove IS_TIM_SYNCHRO_INSTANCE macro from CMSIS
  Constants RCC_CFGR_ shall be removed from CMSIS
  Rename SYSCFG_ITLINE2_SR_RTC_WAKEUP into SYSCFG_ITLINE2_SR_RTC to be aligned with RM
  Correction of DMAMUX_CxCR_DMAREQ_ID that should be 0x7F instead of 0xFF  
  [Codespell] minor typo correction
  [FOSS-Audit] Change CMSIS BSD headers to Apache license
  [MISRA] Add U suffix for bit configuration of the Cortex-M0+ Processor
  [Value Line] Remove EXTI_EMR1_EM17 & EXTI_EMR1_EM18 bits (COMP1/COMP2 N/A on VL)
  [Value Line] TIM2 is not supported on G0 value line, so remove it from IS_TIM_CCX_INSTANCE() and IS_TIM_OCCS_INSTANCE() macros

## Contents

-**CMSIS devices files for stm32g0B0xx, stm32g0B1xx, stm32g0C1xx devices.**

-**CMSIS devices files for stm32g050xx, stm32g051xx, stm32g061xx devices.**

-CMSIS devices files for stm32g030xx, stm32g031xx, stm32g041xx devices.

-CMSIS devices files for stm32g070xx, stm32g071xx, stm32g081xx devices.

## Known Limitations

None

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">V1.3.0 / 25-June-2019 </label>
<div>

## Main Changes

### Maintenance release

Maintenance release for STM32G0xx CMSIS supporting stm32g030xx, stm32g031xx, stm32g041xx, stm32g070xx, stm32g071xx, stm32g081xx devices.

**Fixed bugs list**

  Headline
  --------
  Remove all DMAMUX register/bit definition related to internal debug features

## Contents

CMSIS devices files for stm32g030xx, stm32g031xx, stm32g041xx devices.

CMSIS devices files for stm32g070xx, stm32g071xx, stm32g081xx devices.

## Known Limitations

**Requirements not met or planned in a forthcoming release**

  Headline
  -----------------------------------------------------------
  [MISRAC2012-Rule-8.9_b] Global variable APBPrescTable is only referenced from a single function

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

**Additional features**

  Headline
  ----------------------------------------------------------
  New CMSIS Drivers files to support STM32G041xx, STM32G031xx, STM32G030xx

**Fixed bugs list**

  Headline
  --------
  Correct wrong definition of IS_TIM_TISEL_INSTANCE for STM32G0x0 devices
  Mention bit ADC_CCR_LFMEN as useless in STM32G0, kept for legacy purpose

## Contents

CMSIS devices files for stm32g030xx, stm32g031xx, stm32g041xx devices.

CMSIS devices files for stm32g070xx, stm32g071xx, stm32g081xx devices.

## Known Limitations

**Requirements not met or planned in a forthcoming release**

  Headline
  -----------------------------------------------------------
  [MISRAC2012-Rule-8.9_b] Global variable APBPrescTable is only referenced from a single function
  Use latest CMSIS V5.4.0

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

**Fixed bugs list**

 Headline
 --------
 [Keil] Startup file for Keil generated project contain errors
 [Licenses update] remove html tags in Startup files
 MISRAC2012 implementation
 Correct undefined reference to `LPUART1_IRQHandler' for some G0 variants

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
