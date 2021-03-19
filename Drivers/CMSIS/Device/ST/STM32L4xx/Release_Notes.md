---
pagetitle: Release Notes for STM32L4xx CMSIS
lang: en
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

::: {.card .fluid}
::: {.sectione .dark}
<center>
# <small>Release Notes for</small> <mark>STM32L4xx CMSIS</mark>
Copyright &copy; 2017 STMicroelectronics\
    
[![ST logo](../../../../../_htmresc/st_logo.png)](https://www.st.com){.logo}
</center>
:::
:::

# License

This software component is licensed by ST under Apache-2.0 license, the \"License\"; You may
not use this component except in compliance with the License. You may
obtain a copy of the License at:

[Apache License v2.0](https://opensource.org/licenses/Apache-2.0)

# Purpose

This driver provides the CMSIS device for the stm32l4xx products. This covers

- STM32L412xx/STM32L422x devices
- STM32L431xx/STM32L432xx/STM32L433xx/STM32L442xx/STM32L443xx devices
- STM32L451xx/STM32L452xx/STM32L462xx devices
- STM32L471xx/STM32L475xx/STM32L476xx/STM32L485xx/STM32L486xx devices
- STM32L496xx/STM32L4A6xx devices
- STM32L4P5xx/STM32L4Q5xx devices
- STM32L4R5xx/STM32L4R7xx/STM32L4R9xx/STM32L4S5xx/STM32L4S7xx and STM32L4S9xx devices

This driver is composed of the descriptions of the registers under “Include” directory.

Various template file are provided to easily build an application. They can be adapted to fit applications requirements.

- Templates/system_stm32l4xx.c contains the initialization code referred as SystemInit.
- Startup files are provided as example for IAR©, KEIL© and SW4STM32©.
- Linker files are provided as example for IAR©, KEIL© and SW4STM32©.

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section23" checked aria-hidden="true">
<label for="collapse-section23" aria-hidden="true">V1.7.0 / 26-June-2020</label>
<div>

## Main Changes

- stm32l4p5xx.h, stm32l4q5xx.h, stm32l4r5xx.h, stm32l4r7xx.h, stm32l4r9xx.h, stm32l4s5xx.h, stm32l4s7xx.h, stm32l4s9xx.h and stm32l4xxxxx.h
  - Update bit definitions in OCTOSPI registers
    - Remove WRAPSIZE field in DCR2 register as the wrap is not supported for L4+ devices
    - Update DLYBYP, CSHT, TOF and SIOO bit definitions

- stm32l431xx.h, stm32l432xx.h, stm32l433xx.h, stm32l442xx.h, stm32l443xx.h and stm32l4xxxxx.h
  - Align HSITRIM configuration with reference manual

- stm32l412xx.h, stm32l422xx.h, stm32l431xx.h, stm32l432xx.h, stm32l433xx.h, stm32l442xx.h, stm32l443xx.h, stm32l451xx.h,
  stm32l452xx.h, stm32l462xx.h, stm32l496xx.h, stm32l4a6xx.h, stm32l4p5xx.h, stm32l4q5xx.h, stm32l4r5xx.h, stm32l4r7xx.h,
  stm32l4r9xx.h, stm32l4s5xx.h, stm32l4s7xx.h and stm32l4s9xx.h
  - Add bit descriptions for CRS_CR_TRIM field in CRS_RC register

- All gcc startup files
  - Align startup files with IAR/Keil startup files by calling SystemInit() before data initialization


</div>
:::



::: {.collapse}
<input type="checkbox" id="collapse-section22" aria-hidden="true">
<label for="collapse-section22" aria-hidden="true">V1.6.1 / 07-February-2020</label>
<div>

## Main Changes

- Add bit definitions in OCTOSPI registers
  - stm32l4p5xx.h, stm32l4q5xx.h, stm32l4r5xx.h, stm32l4r7xx.h, stm32l4r9xx.h, stm32l4s5xx.h, stm32l4s7xx.h and stm32l4s9xx.h 
    - Add OCTOSPI DLYBYP field definition in DCR1 register
  - stm32L4p5xx.h and stm32L4q5xx.h
    - Add OCTOSPI MAXTRAN field definition in DCR3 register

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section21" aria-hidden="true">
<label for="collapse-section21" aria-hidden="true">V1.6.0 / 22-November-2019</label>
<div>

## Main Changes

- Add the support of **STM32L4P5xx/STM32L4Q5xx** devices
  - Add stm32l4p5xx.h and stm32l4q5xx.h device description files
  - Add startup files startup_stm32l4p5xx.s and stm32l4q5xx.s for EWARM, MDK-ARM and CubeIDE toolchains    
  - Add EWARM associated linker files for execution from internal RAM or internal FLASH  
  - stm32l4xx.h
    - Add the following device defines:
      - "#define STM32L4P5xx" for all STM32L4P5xx devices
      - "#define STM32L4Q5xx" for all STM32L4Q5xx devices

- All gcc startup files
  - Remove reference to Atollic IDE in comments

- stm32l412xx.h and stm32l422xx.h
  - Remove RTC_ICSR_ALRAWF and RTC_ICSR_ALRBWF bits definition (not applicable to these devices)
  - Correct RTC_WUTR_WUTOCLR_Msk definition comment

- gcc startup_stm32l412xx.s and startup_stm32l422xx.s files  
  - Add missing USB_IRQHander definition 

- stm32l4r5xx.h, stm32l4r7xx.h, stm32l4r9xx.h, stm32l4s5xx.h, stm32l4s7xx.h and stm32l4s9xx.h
  - Add RCC_DLYCFGR register definition
  - Swap I2C4_EV_IRQn and I2C4_ER_IRQn definitions to be aligned with startup files 

- stm32l496xx.h and stm32l4a6xx.h
  - Remove PEMPTY bit definition (not applicable to these devices)

- All MDK-ARM start-up files
  - Move back the start tag of the configuration wizard to make sure STMicroelectronics copyright section doesn't interfere with the configuration annotation parser

- All stm32l4XXxx.h device description files
  - Remove IS_TIM_SYNCHRO_INSTANCE macro

  
</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section20" aria-hidden="true">
<label for="collapse-section20" aria-hidden="true">V1.5.1 / 03-April-2019</label>
<div>

## Main Changes

- stm32l412xx.h, stm32l422xx.h, stm32l431xx.h, stm32l432xx.h, stm32l433xx.h, stm32l442xx.h, stm32l443xx.h, stm32l451xx.h, stm32l452xx.h, stm32l462xx.h and stm32l471xx.h
  - Correct comment related to PVD_PVM_IRQn definition
  
- stm32l412xx.h and stm32l422xx.h
  - Add missing definitions of QUADSPI DFM, FSEL and DHHC bits
  - Add missing definition of ADC_SMPR1_SMPPLUS bit
  - Correct PWR_CR3 bit name EN_ULP into ENULP
  
- stm32l412xx.h, stm32l422xx.h, stm32l431xx.h, stm32l432xx.h, stm32l433xx.h, stm32l442xx.h, stm32l443xx.h, stm32l451xx.h, stm32l452xx.h and stm32l462xx.h
  - Correct FLASH_PCROP1SR and FLASH_PCROP1ER registers definition
    - Correct size of PCROP fields : 16 bits for STM32L45xx/L46xx and 15 bits for STM32L43xx/L44xx
    - Correct size of page number field : 7 bits for STM32L43xx/L44xx and 6 bits STM32L41xx/L42xx
    - Correct size of WRP fields : 7 bits for STM32L43xx/L44xx
 
- stm32l432xx.h and stm32l442xx.h
  - Add PWR_CR2_PVME1 definition 
     
- stm32l4r5xx.h, stm32l4r7xx.h, stm32l4r9xx.h, stm32l4s5xx.h, stm32l4s7xx.h and stm32l4s9xx.h
  - Correct SDMMC_POWER bits definition
  - Correct comments related to SDMMC_CLKCR_WIDBUS definition
  - Correct EXTI_IMR1_IM mask definition
  
- stm32l4xx.h
  - Correct ErrorStatus typedef 
  
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section19" aria-hidden="true">
<label for="collapse-section19" aria-hidden="true">V1.5.0 / 27-July-2018</label>
<div>			

## Main Changes

-   [Add ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[stm32l412xx.h
    and
    []{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[stm32l422xx.h]{style="font-size: 10pt; font-family: Verdana;"}[
    device description
    files]{style="font-size: 10pt; font-family: Verdana;"}
-   [Add startup files
    ]{style="font-size: 10pt; font-family: Verdana;"}[startup\_]{style="font-size: 10pt; font-family: Verdana;"}[stm32l412xx.s]{style="font-size: 10pt; font-family: Verdana;"}[[
    ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[and[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[startup\_]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[stm32l422xx.s]{style="font-size: 10pt; font-family: Verdana;"}[
    for EWARM, MDK-ARM and SW4STM32
    toolchains]{style="font-size: 10pt; font-family: Verdana;"}
-   [Add EWARM associated linker
    files ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[for
    execution from internal RAM or internal
    FLASH ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}

- [stm32l4xx.h]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}

  - [Add the following device
defines:]{style="font-size: 10pt; font-family: Verdana;"}

    -   [\"\#define STM32L412xx\" for all STM32L412xx
    devices]{style="font-size: 10pt; font-family: Verdana;"}
    -   [\"\#define STM32L422xx\" for all STM32L422xx
    devices]{style="font-size: 10pt; font-family: Verdana;"}

- [stm32l4r5xx.h, stm32l4r7xx.h,
stm32l4r9xx.h, ]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4s5xx.h,
stm32l4s7xx.h and
stm32l4s9xx.h]{style="font-size: 10pt; font-family: Verdana;"}[
description files]{style="font-size: 10pt; font-family: Verdana;"}

  -   [Update SDMMC mask
    register]{style="font-size: 10pt; font-family: Verdana;"}
  -   [Correct ]{style="font-size: 10pt; font-family: Verdana;"}[SDMMC
    ]{style="font-size: 10pt; font-family: Verdana;"}[DPSMACT and
    CPSMACT bits
    definition]{style="font-size: 10pt; font-family: Verdana;"}

- [All ]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4XXxx.h
device description files
except ]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4rXxx.h
and ]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4sXxx.h]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}

  -   []{style="font-size: 10pt; font-family: Verdana;"}[Correct Misra
    C:2012 Rule10.3 warning in replacing 0xFFUL by 0xFFU when CRC IDR
    register is 8-bit long
    only]{style="font-size: 10pt; font-family: Verdana;"}

- [All
iar/linker/stm32]{style="font-size: 10pt; font-family: Verdana;"}[l4XXxx\_flash.icf
and
]{style="font-size: 10pt; font-family: Verdana;"}[iar/linker/stm32]{style="font-size: 10pt; font-family: Verdana;"}[l4XXxx\_sram.icf]{style="font-size: 10pt; font-family: Verdana;"}

  -   [Remove SRAM1 and SRAM2 (and
    SRAM3 ]{style="font-size: 10pt; font-family: Verdana;"}[for ]{style="font-size: 10pt; font-family: Verdana;"}[stm32]{style="font-size: 10pt; font-family: Verdana;"}[l4rXxx\_flash.icf]{style="font-size: 10pt; font-family: Verdana;"}[,
    ]{style="font-size: 10pt; font-family: Verdana;"}[stm32]{style="font-size: 10pt; font-family: Verdana;"}[l4sXxx\_flash.icf]{style="font-size: 10pt; font-family: Verdana;"}[,
    ]{style="font-size: 10pt; font-family: Verdana;"}[stm32]{style="font-size: 10pt; font-family: Verdana;"}[l4rXxx\_sram.icf]{style="font-size: 10pt; font-family: Verdana;"}[
    and
    ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[stm32]{style="font-size: 10pt; font-family: Verdana;"}[l4sXxx\_sram.icf]{style="font-size: 10pt; font-family: Verdana;"}[)
    start and end addresses from ICF editor
    section]{style="font-size: 10pt; font-family: Verdana;"}
  -   [Define RAM section as covering whole SRAM1 plus SRAM2 (and SRAM3
    when applicable]{style="font-size: 10pt; font-family: Verdana;"}[)
    when the SRAM regions are
    contiguous]{style="font-size: 10pt; font-family: Verdana;"}

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">V1.4.3 / 30-April-2018</label>
<div>			

## Main Changes

- []{style="font-size: 10pt; font-family: Verdana;"}[All ]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4XXxx.h
device description
files]{style="font-size: 10pt; font-family: Verdana;"}

  - [Fix DFSDM\_FLTICR\_CLRSCDF
  definition]{style="font-size: 10pt; font-family: Verdana;"}
  - [Cleanup USB Host & Device bit
  definitions]{style="font-size: 10pt; font-family: Verdana;"}
  - [\[MISRAC2012-Rule-10.6\] Use \'UL\' postfix for \_Msk definitions
  and memory/peripheral base addresses\
  ]{style="font-size: 10pt; font-family: Verdana;"}

- [stm32l4r5xx.h,
[]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4r7xx.h,[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4r9xx.h,[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4s5xx.h,[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4s7xx.h[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[and[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4s9xx.h devices description files]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}

   - []{style="font-size: 10pt; font-family: Verdana;"}[Fix
    SDMMC\_STA\_DPSMACT and
    ]{style="font-size: 10pt; font-family: Verdana;"}[SDMMC\_STA\_]{style="font-size: 10pt; font-family: Verdana;"}[CPSMACT
    definitions (V1.4.1 update was
    incorrect)]{style="font-size: 10pt; font-family: Verdana;"}

- [All ]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4XXxx.h
device description files
except ]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4r5xx.h, ]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4r7xx.h,[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4r9xx.h,[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4s5xx.h,[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4s7xx.h[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[and[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4s9xx.h]{style="font-size: 10pt; font-family: Verdana;"}

   - []{style="font-size: 10pt; font-family: Verdana;"}[Add missing
    USART\_CR3\_UCESM bit
    definition]{style="font-size: 10pt; font-family: Verdana;"}

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">V1.4.2 / 22-December-2017</label>
<div>	

## Main Changes

-   []{style="font-size: 10pt; font-family: Verdana;"}[SPI\_TypeDef
    cleanup of Reserved
    fields.]{style="font-size: 10pt; font-family: Verdana;"}

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section6" aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">V1.4.1 / 6-October-2017</label>
<div>	

## Main Changes

[]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4r5xx.h and stm32l4s5xx.h description files
]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}

-   []{style="font-size: 10pt; font-family: Verdana;"}[Remove LTDC
    peripheral definitions as LTDC not available on STM32L4R5xx/STM32L4S5xx devices
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[[]{style="font-weight: bold;"}]{style="font-size: 10pt; font-family: Verdana;"}[[]{style="font-weight: bold;"}]{style="font-size: 10pt; font-family: Verdana;"}[[]{style="font-weight: bold;"}]{style="font-size: 10pt; font-family: Verdana;"}[[]{style="font-weight: bold;"}]{style="font-size: 10pt; font-family: Verdana;"}[[]{style="font-weight: bold;"}]{style="font-size: 10pt; font-family: Verdana;"}[[]{style="font-weight: bold;"}]{style="font-size: 10pt; font-family: Verdana;"}[[]{style="font-weight: bold;"}
    ]{style="font-size: 10pt; font-family: Verdana;"}

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section7" aria-hidden="true">
<label for="collapse-section7" aria-hidden="true">V1.4.0 / 25-August-2017</label>
<div>	


## Main Changes

- [Add the support
of ]{style="font-size: 10pt; font-family: Verdana;"}[[STM32L4R5xx/]{style="font-weight: bold;"}]{style="font-size: 10pt; font-family: Verdana;"}[[STM32L4R7xx/]{style="font-weight: bold;"}]{style="font-size: 10pt; font-family: Verdana;"}[[STM32L4R9xx/]{style="font-weight: bold;"}]{style="font-size: 10pt; font-family: Verdana;"}[[STM32L4S5xx/]{style="font-weight: bold;"}]{style="font-size: 10pt; font-family: Verdana;"}[[STM32L4S7xx/STM32L4S9xx]{style="font-weight: bold;"}]{style="font-size: 10pt; font-family: Verdana;"}[[]{style="font-weight: bold;"}]{style="font-size: 10pt; font-family: Verdana;"}[[]{style="font-weight: bold;"}
devices]{style="font-size: 10pt; font-family: Verdana;"}

  - [Add ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4r5xx.h,
    []{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4r7xx.h,[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4r9xx.h,[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4s5xx.h,[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4s7xx.h[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[and[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4s9xx.h device description files]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}
  - [Add startup files startup\_stm32l4r5xx.s, startup\_stm32l4r7xx.s, startup\_stm32l4r9xx.s, startup\_stm32l4s5xx.s, startup\_stm32l4s7xx.s and stm32l4s9xx.s for EWARM, MDK-ARM and SW4STM32 toolchains
    ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[[
    ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[ ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[[
     ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[[
     ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[[
    ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[[ ]{.Apple-converted-space}]{style="font-size: 10pt; font-family: Verdana;"}
  - [Add EWARM associated linker files for execution from internal RAM or internal FLASH]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[
     ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[
     ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}

  - [stm32l4xx.h]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}
    - [Add the following device defines:]{style="font-size: 10pt; font-family: Verdana;"}
      - [\"\#define STM32L4R5xx\" for all STM32L4R5xx
      devices]{style="font-size: 10pt; font-family: Verdana;"}
      - [\"\#define STM32L4R7xx\" for all STM32L4R7xx
      devices]{style="font-size: 10pt; font-family: Verdana;"}
      - [\"\#define STM32L4R9xx\" for all STM32L4R9xx
      devices]{style="font-size: 10pt; font-family: Verdana;"}
      - [\"\#define STM32L4S5xx\" for all STM32L4S5xx
      devices]{style="font-size: 10pt; font-family: Verdana;"}
      - [\"\#define STM32L4S7xx\" for all STM32L4S7xx
      devices]{style="font-size: 10pt; font-family: Verdana;"}
      - [\"\#define STM32L4S9xx\" for all STM32L4S9xx
    devices]{style="font-size: 10pt; font-family: Verdana;"}

- [All adequate stm32l4XXxx.h device description files]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}

  - []{style="font-size: 10pt; font-family: Verdana;"}[PWR: Rename
    PWR\_CR3\_EIWF to
    PWR\_CR3\_EIWUL]{style="font-size: 10pt; font-family: Verdana;"}
  - [TIM: Add missing TIM8 check in debug macro IS\_TIM\_HALL\_SENSOR\_INTERFACE\_INSTANCE() devices]{style="font-size: 10pt; font-family: Verdana;"}[ 
    ]{style="font-size: 10pt; font-family: Verdana;"}
  - []{style="font-size: 10pt; font-family: Verdana;"}[USB: Fix
    USB\_EP0R\...USB\_EP7R
    definitions]{style="font-size: 10pt; font-family: Verdana;"}

- [system\_stm32l4xx.c]{style="font-size: 10pt; font-family: Verdana;"}

  - [Fix MISRA C 2004 rule 10.6 : a \'U\' suffix shall be applied to all
    constants of \'unsigned\'
    type]{style="font-size: 10pt; font-family: Verdana;"}

[]{style="font-size: 10pt; font-family: Verdana;"}

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section8" aria-hidden="true">
<label for="collapse-section8" aria-hidden="true">V1.3.2 / 16-June-2017</label>
<div>	

## Main Changes

- [stm32l451xx.h, stm32l452xx.h, stm32l462xx.h description
files]{style="font-size: 10pt; font-family: Verdana;"}

  - [Remove EXTI\_IMR2\_IM34 and EXTI\_EMR2\_IM34 bits (no SWPMI EXTI
    line)]{style="font-size: 10pt; font-family: Verdana;"}

- [stm32l432xx.h, stm32l442xx.h, stm32l452xx.h, stm32l462xx.h description
files]{style="font-size: 10pt; font-family: Verdana;"}

  - []{style="font-size: 10pt; font-family: Verdana;"}[Remove
    EXTI\_IMR2\_IM36 and EXTI\_EMR2\_IM36 bits from STM32L4x2x devices
    (no PVM2 EXTI line)]{style="font-size: 10pt; font-family: Verdana;"}

- [stm32l431xx.h, stm32l432xx.h, stm32l442xx.h, stm32l451xx.h,
stm32l452xx.h, stm32l462xx.h, stm32l471xx.h, stm32l475xx.h,
stm32l485xx.h ]{style="font-size: 10pt; font-family: Verdana;"}[description
files]{style="font-size: 10pt; font-family: Verdana;"}

  - [Remove EXTI\_IMR2\_IM39 and EXTI\_EMR2\_IM39 bits
    ]{style="font-size: 10pt; font-family: Verdana;"}[from
    ]{style="font-size: 10pt; font-family: Verdana;"}[STM32L4x1x,
    ]{style="font-size: 10pt; font-family: Verdana;"}[STM32L4x2x
    and ]{style="font-size: 10pt; font-family: Verdana;"}[STM32L4x5x]{style="font-size: 10pt; font-family: Verdana;"}[
    devices]{style="font-size: 10pt; font-family: Verdana;"}[
    (]{style="font-size: 10pt; font-family: Verdana;"}[no LCD EXTI
    line)]{style="font-size: 10pt; font-family: Verdana;"}

- [All ]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4XXxx.h
device description
files]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}

  - []{style="font-size: 10pt; font-family: Verdana;"}[Remove erroneous
    SRAM2\_BB\_BASE
    definition]{style="font-size: 10pt; font-family: Verdana;"}
  - [Add missing EXTI\_EMR2\_EM
    constants]{style="font-size: 10pt; font-family: Verdana;"}
  - [Add missing TIM8 check in debug macro
    IS\_TIM\_HALL\_SENSOR\_INTERFACE\_INSTANCE() devices where TIM8 is
    present\
    ]{style="font-size: 10pt; font-family: Verdana;"}

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section9" aria-hidden="true">
<label for="collapse-section9" aria-hidden="true">V1.3.1 / 21-April-2017</label>
<div>	

## Main Changes

- [stm32l496xx.h and stm32l4a6xx.h device description files]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}

  - []{style="font-size: 10pt; font-family: Verdana;"}[FIREWALL]{style="font-size: 10pt; font-family: Verdana;"}

    - [Fix FW\_VDSSA\_ADD\_Msk and FW\_VDSL\_LENG\_Msk
    definitions]{style="font-size: 10pt; font-family: Verdana;"}

  - [TIM16]{style="font-size: 10pt; font-family: Verdana;"}

    - [Fix TIM16\_OR1\_TI1\_RMP\_Msk
    definition]{style="font-size: 10pt; font-family: Verdana;"}

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section10" aria-hidden="true">
<label for="collapse-section10" aria-hidden="true">V1.3.0 / 17-February-2017</label>
<div>	

## Main Changes

- [Add the support
of []{style="font-weight: bold;"}]{style="font-size: 10pt; font-family: Verdana;"}[[STM32L496xx/STM32L4A6xx]{style="font-weight: bold;"}]{style="font-size: 10pt; font-family: Verdana;"}[[]{style="font-weight: bold;"}
devices]{style="font-size: 10pt; font-family: Verdana;"}

  - [Add ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[stm32l496xx.h and stm32l4a6xx.h device description files
     ]{style="font-size: 10pt; font-family: Verdana;"}[    
    ]{style="font-size: 10pt; font-family: Verdana;"}
  - [Add startup files startup\_stm32l496xx.s and startup\_stm32l4a6xx.s for EWARM, MDK-ARM and SW4STM32 toolchains
    ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}
  - [Add linker
    files ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[stm32l496xx\_flash.icf, stm32l496xx\_sram.icf, stm32l4a6xx\_flash.icf and stm32l4a6xx\_sram.icf used within EWARM workspaces]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}

- [stm32l4xx.h]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}

  - [Add the following device
defines:]{style="font-size: 10pt; font-family: Verdana;"}

    - [\"\#define STM32L496xx\" for all STM32L496xx
    devices]{style="font-size: 10pt; font-family: Verdana;"}

    - [\"\#define STM32L4A6xx\" for all STM32L4A6xx
    devices]{style="font-size: 10pt; font-family: Verdana;"}

- [All ]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4XXxx.h
device description
files]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}

  - []{style="font-size: 10pt; font-family: Verdana;"}[Fix
    RCC\_CRRCR\_HSI48CAL\_Pos and RCC\_CRRCR\_HSI48CAL
    values]{style="font-size: 10pt; font-family: Verdana;"}

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section11" aria-hidden="true">
<label for="collapse-section11" aria-hidden="true">V1.2.0 / 28-October-2016</label>
<div>


## Main Changes

- [Add the support
of [STM32L451xx/STM32L452xx/STM32L462xx]{style="font-weight: bold;"}
devices]{style="font-size: 10pt; font-family: Verdana;"}

  - [Add ]{style="font-size: 10pt; font-family: Verdana;"}[stm32l451xx.h, stm32l452xx.h and stm32l462xx.h device description files
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[ 
    ]{style="font-size: 10pt; font-family: Verdana;"}
  - [Add startup files startup\_stm32l451xx.s, startup\_stm32l452xx.s and startup\_stm32l462xx.s for EWARM, MDK-ARM and SW4STM32 toolchains
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[    
    ]{style="font-size: 10pt; font-family: Verdana;"}
  - [Add linker files stm32l451xx\_flash.icf, stm32l451xx\_sram.icf, stm32l452xx\_flash.icf, stm32l452xx\_sram.icf, stm32l462xx\_flash.icf and stm32l462xx\_sram.icf used within EWARM workspaces
     ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[ ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[     
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}

- [stm32l4xx.h]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}
  - [Add the following device
    defines:]{style="font-size: 10pt; font-family: Verdana;"}
  - [\"\#define STM32L451xx\" for all STM32L451xx
    devices]{style="font-size: 10pt; font-family: Verdana;"}
  - []{style="font-size: 10pt; font-family: Verdana;"}[\"\#define
    STM32L452xx\" for all STM32L452xx
    devices]{style="font-size: 10pt; font-family: Verdana;"}
  - [\"\#define STM32L462xx\" for all STM32L462xx
    devices]{style="font-size: 10pt; font-family: Verdana;"}

- [All ]{style="font-size: 10pt; font-family: Verdana;"}[stm32l4XXxx.h device description files except stm32l451xx.h, stm32l452xx.h and stm32l462xx.h]{style="font-size: 10pt; font-family: Verdana;"}[ 
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}

  - [Add DAC\_CHANNEL2\_SUPPORT definition as DAC channel 2 is present on these devices
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}

- [stm32l432xx.h and stm32l442xx.h device description files]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}

  - []{style="font-size: 10pt; font-family: Verdana;"}[Remove
    PWR\_CR2\_PVME1 and PWR\_SR2\_PVMO1 as VDDUSB is internally
    connected to VDD on these devices thus no PVM1
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}

- [stm32l475xx.h, stm32l476xx.h, stm32l485xx.h and stm32l486xx.h device description files]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}

  - [Remove USB OTG Core Id version definitions USB\_OTG\_CORE\_ID\_310A
    and USB\_OTG\_CORE\_ID\_320A as reserved for internal
    usage]{style="font-size: 10pt; font-family: Verdana;"}

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section12" aria-hidden="true">
<label for="collapse-section12" aria-hidden="true">V1.1.2 / 12-September-2016</label>
<div>

## Main Changes

- []{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[Fix
DAC\_SR\_BWST1 bit
definition]{style="font-size: 10pt; font-family: Verdana;"}

- [Fix SDMMC\_DCTRL\_DBLOCKSIZE\_2 and SDMMC\_DCTRL\_DBLOCKSIZE\_3 bits
definition]{style="font-size: 10pt; font-family: Verdana;"}

- [USB OTG FS]{style="font-size: 10pt; font-family: Verdana;"}

  - [Add GSNPSID, GHWCFG1 and GHWCFG2 register
    mapping]{style="font-size: 10pt; font-family: Verdana;"}
  - [Add USB OTG Core Id version definitions: USB\_OTG\_CORE\_ID\_310A
    and USB\_OTG\_CORE\_ID\_320A\
    ]{style="font-size: 10pt; font-family: Verdana;"}

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section13" aria-hidden="true">
<label for="collapse-section13" aria-hidden="true">V1.1.1 / 29-April-2016</label>
<div>

## Main Changes

- []{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[All
device register description files enriched with \_Pos and \_Msk defines
to be used with \_VAL2FLD(field, value) and \_FLD2VAL(field, value) from
CMSIS Core (previous defines are kept for
compatibility)]{style="font-size: 10pt; font-family: Verdana;"}

- [stm32l471xx.h, stm32l475xx.h, stm32l476xx.h, stm32l485xx.h and stm32l486xx.h device description files
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}

  - [Update DFSDM peripheral instance and register definitions (named
    DFSDM1 peripheral from now instead of
    DFSDM)]{style="font-size: 10pt; font-family: Verdana;"}
    - [Rename DFSDM[x]{style="font-style: italic;"}\_IRQn to
    DFSDM1\_FLT[x]{style="font-style: italic;"}\_IRQn for
    [x]{style="font-style: italic;"}=0,1,2 and 3
     (filters)]{style="font-size: 10pt; font-family: Verdana;"}
    - [Rename DFSDM channels and filters to refer to DFSDM1 peripheral
    instance with DFSDM1
    prefix]{style="font-size: 10pt; font-family: Verdana;"}
      - [DFSDM1\_Channel0, DFSDM1\_Channel1, \...
    DFSDM1\_Channel7]{style="font-size: 10pt; font-family: Verdana;"}
      - [DFSDM1\_Filter0,
    ]{style="font-size: 10pt; font-family: Verdana;"}[DFSDM1\_Filter1,
    \..., ]{style="font-size: 10pt; font-family: Verdana;"}[DFSDM1\_Filter3]{style="font-size: 10pt; font-family: Verdana;"}
    - [Rename registers in DFSDM\_Filter\_TypeDef to use FLT prefix for
    filter]{style="font-size: 10pt; font-family: Verdana;"}
      - [FLTCR1, FLTCR2, FLTISR, FLTICR, FLTJCHGR, FLTFCR, FLTJDATAR,
    FLTRDATAR,
    FLTAWHTR, ]{style="font-size: 10pt; font-family: Verdana;"}[FLTAWLTR,
    FLTAWSR, FLTAWCFR, FLTEXMAX, FLTEXMIN,
    FLTCNVTIMR]{style="font-size: 10pt; font-family: Verdana;"}
    - [Rename register in DFSDM\_Filter\_TypeDef to use CH prefix
    for channel]{style="font-size: 10pt; font-family: Verdana;"}
      - [CHAWSCDR\
    ]{style="font-size: 10pt; font-family: Verdana;"}
    - [Update RCC definitions for
    DFSDM1]{style="font-size: 10pt; font-family: Verdana;"}
      - [Rename RCC\_APB2RSTR\_DFSDMRST to
    RCC\_APB2RSTR\_DFSDM1RST]{style="font-size: 10pt; font-family: Verdana;"}
      - [Rename RCC\_APB2ENR\_DFSDMEN to
    RCC\_APB2ENR\_DFSDM1EN]{style="font-size: 10pt; font-family: Verdana;"}
      - [Rename RCC\_APB2SMENR\_DFSDMSMEN to
    RCC\_APB2SMENR\_DFSDM1SMEN]{style="font-size: 10pt; font-family: Verdana;"}
      - [Rename RCC\_CCIPR\_DFSDMSEL to
    RCC\_CCIPR\_DFSDM1SEL]{style="font-size: 10pt; font-family: Verdana;"}
    - [Update TIM definitions for
    DFSDM1]{style="font-size: 10pt; font-family: Verdana;"}
      - [Rename TIM1\_OR2\_BKDFBK0E to
        TIM1\_OR2\_BKDF1BK0E]{style="font-size: 10pt; font-family: Verdana;"}
      - [Rename TIM1\_OR3\_BK2DFBK1E to
        TIM1\_OR3\_BK2DF1BK1E]{style="font-size: 10pt; font-family: Verdana;"}
      - [Rename TIM8\_OR2\_BKDFBK2E to
        TIM8\_OR2\_BKDF1BK2E]{style="font-size: 10pt; font-family: Verdana;"}
      - [Rename TIM8\_OR3\_BK2DFBK3E to
        TIM8\_OR3\_BK2DF1BK3E]{style="font-size: 10pt; font-family: Verdana;"}
      - [Rename TIM15\_OR2\_BKDFBK0E to
        TIM15\_OR2\_BKDF1BK0E]{style="font-size: 10pt; font-family: Verdana;"}
      - [Rename TIM16\_OR2\_BKDFBK1E to
        TIM16\_OR2\_BKDF1BK1E]{style="font-size: 10pt; font-family: Verdana;"}
      - [Rename TIM17\_OR2\_BKDFBK2E to
        TIM17\_OR2\_BKDF1BK2E]{style="font-size: 10pt; font-family: Verdana;"}

  - [FMC]{style="font-size: 10pt; font-family: Verdana;"}

    - [Add FMC\_BWTRx\_BUSTURN register bit
    definition]{style="font-size: 10pt; font-family: Verdana;"}

- [startup\_stm32l471xx.s, startup\_stm32l475xx.s, startup\_stm32l476xx.s, startup\_stm32l485xx.s and startup\_stm32l486xx.s
]{style="font-size: 10pt; font-family: Verdana;"}[ ]{style="font-size: 10pt; font-family: Verdana;"}[ ]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}
  - [Rename DFSDM[x]{style="font-style: italic;"}\_IRQHandler function entry points to DFSDM1\_FLT[x]{style="font-style: italic;"}\_IRQHandler for [x]{style="font-style: italic;"}=0,1,2 and 3 (filters)
    ]{style="font-size: 10pt; font-family: Verdana;"}[    
     ]{style="font-size: 10pt; font-family: Verdana;"}

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section14" aria-hidden="true">
<label for="collapse-section14" aria-hidden="true">V1.1.0 / 26-February-2016</label>
<div>


## Main Changes

- [Add the support of
[STM32L431xx/STM32L432xx/STM32L433xx/STM32L442xx/STM32L443xx]{style="font-weight: bold;"}
devices]{style="font-size: 10pt; font-family: Verdana;"}

  - [Add ]{style="font-size: 10pt; font-family: Verdana;"}[stm32l431xx.h, stm32l432xx.h, stm32l433xx.h, stm32l442xx.h and stm32l443xx.h device description files
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[    
    ]{style="font-size: 10pt; font-family: Verdana;"}
  - [Add startup files startup\_stm32l431xx.s, startup\_stm32l432xx.s, startup\_stm32l433xx.s, startup\_stm32l442xx.s and startup\_stm32l443xx.s for EWARM, MDK-ARM and SW4STM32 toolchains
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[    
    ]{style="font-size: 10pt; font-family: Verdana;"}
  - [Add linker files stm32l431xx\_flash.icf, stm32l431xx\_sram.icf, stm32l432xx\_flash.icf, stm32l432xx\_sram.icf, stm32l433xx\_flash.icf, stm32l433xx\_sram.icf, stm32l442xx\_flash.icf, stm32l442xx\_sram.icf, stm32l443xx\_flash.icf and stm32l443xx\_sram.icf used within EWARM workspaces
     ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[     
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}[
    ]{style="font-size: 10pt; font-family: Verdana;"}

- [stm32l4xx.h]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}
  - [Add the following device
    defines:]{style="font-size: 10pt; font-family: Verdana;"}
    - [\"\#define STM32L431xx\" for all STM32L431xx
    devices]{style="font-size: 10pt; font-family: Verdana;"}
    - []{style="font-size: 10pt; font-family: Verdana;"}[\"\#define
    STM32L432xx\" for all STM32L432xx
    devices]{style="font-size: 10pt; font-family: Verdana;"}
    - [\"\#define STM32L433xx\" for all STM32L433xx
    devices]{style="font-size: 10pt; font-family: Verdana;"}
    - [\"\#define STM32L442xx\" for all STM32L442xx
    devices]{style="font-size: 10pt; font-family: Verdana;"}
    - [\"\#define STM32L443xx\" for all STM32L443xx
    devices]{style="font-size: 10pt; font-family: Verdana;"}

- [stm32l471xx.h, stm32l475xx.h, stm32l476xx.h, stm32l485xx.h and stm32l486xx.h device description files
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}
  - [DFSDM - alignment with registers & bits naming used in documentation]{style="font-size: 10pt; font-family: Verdana;"}
  - [Rename DFSDM\_AWSCDR\_WDATA to DFSDM\_CHWDATR\_WDATA]{style="font-size: 10pt; font-family: Verdana;"}
  - [Rename DFSDM\_AWSCDR\_INDAT0 to DFSDM\_CHDATINR\_INDAT0]{style="font-size: 10pt; font-family: Verdana;"}
  - [Rename DFSDM\_AWSCDR\_INDAT0 to DFSDM\_CHDATINR\_INDAT0]{style="font-size: 10pt; font-family: Verdana;"}

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section15" aria-hidden="true">
<label for="collapse-section15" aria-hidden="true">V1.0.3 / 29-January-2016</label>
<div>

## Main Changes

- [stm32l471xx.h, stm32l475xx.h, stm32l476xx.h, stm32l485xx.h and stm32l486xx.h device description files
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}

  - []{style="font-size: 10pt; font-family: Verdana;"}[Apply MISRA C 2004
rule 10.6 (\'U\' suffix
added)]{style="font-size: 10pt; font-family: Verdana;"}

  - [Add PACKAGE\_BASE, UID\_BASE and FLASHSIZE\_BASE base address
definitions\
]{style="font-size: 10pt; font-family: Verdana;"}

  - [ADC]{style="font-size: 10pt; font-family: Verdana;"}

    - [Update ADC\_CSR register bit definition]{style="font-size: 10pt; font-family: Verdana;"}

  - [LPUART]{style="font-size: 10pt; font-family: Verdana;"}
  - [Add IS\_LPUART\_INSTANCE() to check USART instance with low power capatibility]{style="font-size: 10pt; font-family: Verdana;"}

- [system\_stm32l4xx.h/.c]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}
  -   []{style="font-size: 10pt; font-family: Verdana;"}[Add declaration
    of APB Prescaler table values (const uint8\_t 
    APBPrescTable\[8\])]{style="font-size: 10pt; font-family: Verdana;"}

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section16" aria-hidden="true">
<label for="collapse-section16" aria-hidden="true">V1.0.2 / 25-November-2015</label>
<div>


## Main Changes

- [stm32l471xx.h, stm32l475xx.h, stm32l476xx.h, stm32l485xx.h and stm32l486xx.h device description files
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}

  - []{style="font-size: 10pt; font-family: Verdana;"}[GPIO]{style="font-size: 10pt; font-family: Verdana;"}
    - [Align GPIO register bit definitions with RM0351 (legacy definitions
    preserved for
    compatibility)]{style="font-size: 10pt; font-family: Verdana;"}
  - [FMC]{style="font-size: 10pt; font-family: Verdana;"}
    - [Remove FMC\_BCR1\_WFDIS bit definition (write fifo feature not
    available)]{style="font-size: 10pt; font-family: Verdana;"}
  - [PWR]{style="font-size: 10pt; font-family: Verdana;"}   
    -  [Stop 0 mode introduction]{style="font-size: 10pt; font-family: Verdana;"}
      - [PWR\_CR1\_LPMS\_STOP1MR renamed to PWR\_CR1\_LPMS\_STOP0]{style="font-size: 10pt; font-family: Verdana;"}
      - [PWR\_CR1\_LPMS\_STOP1LPR renamed to]{style="font-size: 10pt; font-family: Verdana;"}[PWR\_CR1\_LPMS\_STOP1]{style="font-size: 10pt; font-family: Verdana;"}
  - [RCC]{style="font-size: 10pt; font-family: Verdana;"}
    - [Align RCC register bit definitions with RM0351 (legacy
        definitions preserved for
        compatibility)]{style="font-size: 10pt; font-family: Verdana;"}
    []{style="font-size: 10pt; font-family: Verdana;"}
  - [TIM]{style="font-size: 10pt; font-family: Verdana;"}
    - [Add IS\_TIM\_HALL\_SENSOR\_INTERFACE\_INSTANCE() to check timer
    instance capability for hall sensor
    interface]{style="font-size: 10pt; font-family: Verdana;"}

- [system\_stm32l4xx.c]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}
  - []{style="font-size: 10pt; font-family: Verdana;"}[Fix PLLCFGR
    default value in
    SystemInit()]{style="font-size: 10pt; font-family: Verdana;"}

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section17" aria-hidden="true">
<label for="collapse-section17" aria-hidden="true">V1.0.1 / 16-September-2015</label>
<div>


## Main Changes

- [stm32l471xx.h, stm32l475xx.h, stm32l476xx.h, stm32l485xx.h and stm32l486xx.h device
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
]{style="font-size: 10pt; font-family: Verdana;"}[
description files]{style="font-size: 10pt; font-family: Verdana;"}

  - []{style="font-size: 10pt; font-family: Verdana;"}[DBGMCU]{style="font-size: 10pt; font-family: Verdana;"}
    - [Fix DBGMCU\_IDCODE\_DEV\_ID mask
    definition]{style="font-size: 10pt; font-family: Verdana;"}
  - [FLASH]{style="font-size: 10pt; font-family: Verdana;"}
    - [Add FLASH\_OPTR\_nRST\_SHDW bit
    definition]{style="font-size: 10pt; font-family: Verdana;"}
  - [I2C]{style="font-size: 10pt; font-family: Verdana;"}
    - [Fix naming I2C\_CR1\_DNF instead of
    I2C\_CR1\_DFN]{style="font-size: 10pt; font-family: Verdana;"}
  - [TIM]{style="font-size: 10pt; font-family: Verdana;"}
    - [Add TIM16\_OR2\_BKDFBK1E bit definition]{style="font-size: 10pt; font-family: Verdana;"}
    - [Add TIM17\_OR2\_BKDFBK2E bit definition]{style="font-size: 10pt; font-family: Verdana;"}

- [system\_stm32l4xx.c]{style="font-size: 10pt; font-family: Verdana;"}[]{style="font-size: 10pt; font-family: Verdana;"}
  - []{style="font-size: 10pt; font-family: Verdana;"}[SystemCoreClockUpdate()
    corrected for SystemCoreClock computation when PLL is
    enabled]{style="font-size: 10pt; font-family: Verdana;"}

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section18" aria-hidden="true">
<label for="collapse-section18" aria-hidden="true">V1.0.0 / 26-June-2015</label>
<div>

## Main Changes

-   [First official release
    for ]{style="font-size: 10pt; font-family: Verdana;"}[STM32L471xx, STM32L475xx, STM32L476xx, STM32L485xx and STM32L486xx devices
    ]{style="font-size: 10pt; font-family: Verdana; font-style: italic; font-weight: bold;"}[[   
    ]{style="font-style: italic; font-weight: bold;"}
    ]{style="font-size: 10pt; font-family: Verdana;"}

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on <mark>STM32 Microcontrollers</mark> ,
visit: [http://www.st.com/STM32](http://www.st.com/STM32)

This release note uses up to date web standards and, for this reason, should not be opened with Internet Explorer but preferably with popular browsers such as Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.

</footer>
