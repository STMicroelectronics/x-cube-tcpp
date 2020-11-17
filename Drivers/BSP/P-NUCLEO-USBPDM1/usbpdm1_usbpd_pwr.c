/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpdm1_usbpd_pwr.c
  * @author  MCD Application Team
  * @brief   This file provides a set of firmware functions to manage
  *          USB-PD Power
  *          available on TCPP01 board from STMicroelectronics.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbpdm1_usbpd_pwr.h"

/* USER CODE BEGIN include */
/* USER CODE END include */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup USBPDM1
  * @{
  */

/** @addtogroup USBPDM1_USBPD_PWR
  * @{
  */

/** @defgroup USBPDM1_USBPD_PWR_Private_Typedef Private Typedef
  * @{
  */
/* USER CODE BEGIN POWER_Private_Typedef */
/* USER CODE END POWER_Private_Typedef */
/**
  * @}
  */

/** @defgroup USBPDM1_USBPD_PWR_Private_Constants Private Constants
* @{
*/
/* USER CODE BEGIN POWER_Private_Constants */
/* Definitions of ADC hardware constraints delays */
/* Note: Only ADC IP HW delays are defined in ADC LL driver driver,           */
/*       not timeout values:                                                  */
/*       Timeout values for ADC operations are dependent to device clock      */
/*       configuration (system clock versus ADC clock),                       */
/*       and therefore must be defined in user application.                   */
/*       Refer to @ref ADC_LL_EC_HW_DELAYS for description of ADC timeout     */
/*       values definition.                                                   */
/* Timeout values for ADC operations. */
/* (calibration, enable settling time, disable settling time, ...)          */
/* Values defined to be higher than worst cases: low clock frequency,       */
/* maximum prescalers.                                                      */
/* Unit: ms                                                                 */
#define ADC_CALIBRATION_TIMEOUT_MS       (   1U)
#define ADC_ENABLE_TIMEOUT_MS            (   1U)
#define ADC_DISABLE_TIMEOUT_MS           (   1U)

/* Delay between ADC end of calibration and ADC enable.                     */
/* Delay estimation in CPU cycles: Case of ADC enable done                  */
/* immediately after ADC calibration, ADC clock setting slow                */
/* (LL_ADC_CLOCK_ASYNC_DIV32). Use a higher delay if ratio                  */
/* (CPU clock / ADC clock) is above 32.                                     */

#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES  (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32u)

#define VDDA_APPLI                          3300u

    
#if defined(USE_STM32G4XX_NUCLEO) || defined(USE_STM32G0XX_NUCLEO)
#define VSENSE_ADC_INSTANCE               ADC1
#else
#error "missing value definition for your your board"
#endif

#if defined(USE_STM32G4XX_NUCLEO)    
#define VSENSE_ADC_SET_CHANNEL() {                                                                                                \
                                       LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_15);                  \
                                       LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_15, LL_ADC_SAMPLINGTIME_247CYCLES_5);   \
                                       LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_15, LL_ADC_SINGLE_ENDED);                 \
                                  }

#define VSENSE_ADC_SET_CLOCK()          {                                                                              \
                                           LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};                        \
                                           ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV4;                 \
                                           ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;                  \
                                           LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);   \
                                        }

#define VSENSE_ADC_SET_SAMPLING_TIME()  {                                                                                           \
                                           LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_15, LL_ADC_SAMPLINGTIME_247CYCLES_5);  \
                                         }
#elif defined(USE_STM32G0XX_NUCLEO)
#define VSENSE_ADC_SET_CHANNEL()       {                                                                                            \
                                         LL_ADC_REG_SetSequencerChannels(VSENSE_ADC_INSTANCE, LL_ADC_CHANNEL_9);                  \
                                       }

                                     
#define VSENSE_ADC_SET_CLOCK()         {                                                                              \
                                         LL_ADC_SetClock(VSENSE_ADC_INSTANCE, LL_ADC_CLOCK_SYNC_PCLK_DIV4);          \
                                       }

#define VSENSE_ADC_SET_SAMPLING_TIME()  {                                                                                                                   \
                                            LL_ADC_SetChannelSamplingTime(VSENSE_ADC_INSTANCE, LL_ADC_CHANNEL_9, LL_ADC_SAMPLINGTIME_COMMON_1);           \
                                            LL_ADC_SetChannelSamplingTime(VSENSE_ADC_INSTANCE, LL_ADC_CHANNEL_VREFINT, LL_ADC_SAMPLINGTIME_COMMON_2);       \
                                        }
#else
#error "missing value definition for your your board"
#endif

/* USER CODE END POWER_Private_Constants */
/**
  * @}
  */

/** @defgroup USBPDM1_USBPD_PWR_Private_Macros Private Macros
  * @{
  */
/* USER CODE BEGIN POWER_Private_Macros */

/* USER CODE END POWER_Private_Macros */
/**
  * @}
  */

/** @defgroup USBPDM1_USBPD_PWR_Private_Variables Private Variables
  * @{
  */
/* USER CODE BEGIN POWER_Private_Variables */

/* USER CODE END POWER_Private_Variables */
/**
  * @}
  */

/** @defgroup USBPDM1_USBPD_PWR_Private_Functions Private Functions
  * @{
  */
/* USER CODE BEGIN POWER_Private_Prototypes */
static void     PWR_Configure_ADC(void);
static void     PWR_Activate_ADC(void);
static void     PWR_VCCInit(uint32_t Instance);
static void     PWR_VCCDeInit(uint32_t Instance);
static void     PWR_DB_OUTInit(uint32_t Instance);
/* USER CODE END POWER_Private_Prototypes */
/**
  * @}
  */

/** @addtogroup USBPDM1_USBPD_PWR_Exported_Functions
  * @{
  */

/**
  * @brief  Global initialization of PWR resource used by USB-PD
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_Init(uint32_t Instance)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_Init */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    PWR_VCCInit(Instance);
    PWR_DB_OUTInit(Instance);
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_Init */
}

/**
  * @brief  Global de-initialization of PWR resource used by USB-PD
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_Deinit(uint32_t Instance)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_Deinit */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    PWR_VCCDeInit(Instance);
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_Deinit */
}

/**
  * @brief  Initialize the hardware resources used by the Type-C power delivery (PD)
  *         controller.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSInit(uint32_t Instance)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSInit */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* For Sink cases */
    PWR_Configure_ADC();
    PWR_Activate_ADC();
    /*  MeasureVrefAnalog(); */
    LL_ADC_REG_StartConversion(VSENSE_ADC_INSTANCE);
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VBUSInit */
}

/**
  * @brief  Release the hardware resources used by the Type-C power delivery (PD)
  *         controller.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSDeInit(uint32_t Instance)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSDeInit */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (LL_ADC_REG_IsConversionOngoing(VSENSE_ADC_INSTANCE))
    {
      LL_ADC_REG_StopConversion(VSENSE_ADC_INSTANCE);
    }

    /* Disable ADC */
    if (LL_ADC_IsEnabled(VSENSE_ADC_INSTANCE))
    {
      LL_ADC_Disable(VSENSE_ADC_INSTANCE);
    }
    /* Reset ADC configuration */
#if defined(USE_STM32G0XX_NUCLEO)
    LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_ADC);
    LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_ADC);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC);
#elif defined(USE_STM32G4XX_NUCLEO)
    LL_AHB2_GRP1_ForceReset(LL_AHB2_GRP1_PERIPH_ADC12);
    LL_AHB2_GRP1_ReleaseReset(LL_AHB2_GRP1_PERIPH_ADC12);
    LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_ADC12);
#else
#error "missing value definition for your your board"
#endif
    
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VBUSDeInit */
}

/**
  * @brief  Get actual voltage level measured on the VBUS line.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  pVoltage Pointer on measured voltage level (in mV)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSGetVoltage(uint32_t Instance, uint32_t *pVoltage)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSGetVoltage */

  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if ((Instance >= USBPD_PWR_INSTANCES_NBR) || (NULL == pVoltage))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    uint32_t val;
    val = __LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA_APPLI, LL_ADC_REG_ReadConversionData12(ADC1), LL_ADC_RESOLUTION_12B); /* mV */
    /* X-NUCLEO-USBPDM board is used */
    /* Value is multiplied by 5.97 (Divider R6/R7 (40.2K/200K) for VSENSE) */
    val *= 597;
    val /= 100;   
    *pVoltage = val; 
  }

  return ret;
  /* USER CODE END BSP_USBPD_PWR_VBUSGetVoltage */
}

/**
  * @brief  Get actual current level measured on the VBUS line.
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  pCurrent Pointer on measured current level (in mA)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSGetCurrent(uint32_t Instance, int32_t *pCurrent)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSGetCurrent */
  /* Check if instance is valid       */
  int32_t ret;

  if ((Instance >= USBPD_PWR_INSTANCES_NBR) || (NULL == pCurrent))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    *pCurrent = 0;
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VBUSGetCurrent */
}

/**
  * @brief  Set the VBUS disconnection voltage threshold.
  * @note   Callback funtion registered through BSP_USBPD_PWR_RegisterVBUSDetectCallback
  *         function call is invoked when VBUS falls below programmed threshold.
  * @note   By default VBUS disconnection threshold is set to 3.3V
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  VoltageThreshold VBUS disconnection voltage threshold (in mV)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_SetVBUSDisconnectionThreshold(uint32_t Instance,
                                                    uint32_t VoltageThreshold)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_SetVBUSDisconnectionThreshold */
  /* Check if instance is valid       */
  int32_t ret;
  UNUSED(VoltageThreshold);

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_SetVBUSDisconnectionThreshold */
}

/**
  * @brief  Set state of VCC (specific functions)
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  State VCC state
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VCCSetState(uint32_t Instance, uint32_t State)
{
  /* USER CODE BEGIN BSP_USBPD_PWR_VCCSetState */
  /* Check if instance is valid       */
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (0 == State)
    {
      LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_10);
    }
    else
    {
      LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_10);
    }
  }
  return ret;
  /* USER CODE END BSP_USBPD_PWR_VCCSetState */
}

/**
  * @}
  */

/** @addtogroup USBPDM1_USBPD_PWR_Private_Functions
  * @{
  */

/* USER CODE BEGIN POWER_Private_Functions */

/**
  * @brief  Configure ADC (ADC instance: VSENSE_ADC_INSTANCE) and GPIO used by ADC channels.
  * @note   In case re-use of this function outside of this example:
  *         This function includes checks of ADC hardware constraints before
  *         executing some configuration functions.
  *         - In this example, all these checks are not necessary but are
  *           implemented anyway to show the best practice usages
  *           corresponding to reference manual procedure.
  *           (On some STM32 series, setting of ADC features are not
  *           conditioned to ADC state. However, in order to be compliant with
  *           other STM32 series and to show the best practice usages,
  *           ADC state is checked anyway with same constraints).
  *           Software can be optimized by removing some of these checks,
  *           if they are not relevant considering previous settings and actions
  *           in user application.
  *         - If ADC is not in the appropriate state to modify some parameters,
  *           the setting of these parameters is bypassed without error
  *           reporting:
  *           it can be the expected behavior in case of recall of this
  *           function to update only a few parameters (which update fullfills
  *           the ADC state).
  *           Otherwise, it is up to the user to set the appropriate error
  *           reporting in user application.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
static void PWR_Configure_ADC(void)
{
  /*## Configuration of GPIO used by ADC channels ############################*/
  uint32_t wait_loop_index;

  /* Enable GPIO Clock */
#if defined(USE_STM32G4XX_NUCLEO)
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
#elif defined(USE_STM32G0XX_NUCLEO)
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
#else
#error "missing value definition for your your board"
#endif

  /* Configure GPIO in analog mode to be used as ADC input */
#if defined(USE_STM32G4XX_NUCLEO)  
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
#elif defined(USE_STM32G0XX_NUCLEO)  
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG);
#else
#error "missing value definition for your your board"
#endif

  /*## Configuration of ADC ##################################################*/

  /*## Configuration of ADC hierarchical scope: common to several ADC ########*/

  /* Enable ADC clock (core clock) */
#if defined(USE_STM32G4XX_NUCLEO)
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC12);
#elif defined(USE_STM32G0XX_NUCLEO)  
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC);
#else
#error "missing value definition for your your board"
#endif
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       All ADC instances of the ADC common group must be disabled.        */
  /* Note: In this example, all these checks are not necessary but are        */
  /*       implemented anyway to show the best practice usages                */
  /*       corresponding to reference manual procedure.                       */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
#if defined(USE_STM32G4XX_NUCLEO)
  if (__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE(ADC12_COMMON) == 0)
#else
  if (__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0)
#endif /*STM32G474xx */
  {
    /* Note: Call of the functions below are commented because they are       */
    /*       useless in this example:                                         */
    /*       setting corresponding to default configuration from reset state. */

    /* Set ADC clock (conversion clock) common to several ADC instances */
    /* Note: On this STM32 serie, ADC common clock asynchonous prescaler      */
    /*       is applied to each ADC instance if ADC instance clock is         */
    /*       set to clock source asynchronous                                 */
    /*       (refer to function "LL_ADC_SetClock()" below).                   */

    /* Set ADC measurement path to internal channels */
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(VSENSE_ADC_INSTANCE), LL_ADC_PATH_INTERNAL_VREFINT);

    /*## Configuration of ADC sampling time ####################*/
#if defined(USE_STM32G0XX_NUCLEO)
    LL_ADC_SetSamplingTimeCommonChannels(VSENSE_ADC_INSTANCE, LL_ADC_SAMPLINGTIME_COMMON_1, LL_ADC_SAMPLINGTIME_160CYCLES_5);
    LL_ADC_SetSamplingTimeCommonChannels(VSENSE_ADC_INSTANCE, LL_ADC_SAMPLINGTIME_COMMON_2, LL_ADC_SAMPLINGTIME_160CYCLES_5);
#endif

    /*## Configuration of ADC hierarchical scope: multimode ####################*/

    /* Note: Feature not available on this STM32 serie */
  }

  /*## Configuration of ADC hierarchical scope: ADC instance #################*/

  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       ADC must be disabled.                                              */
  if (0u == LL_ADC_IsEnabled(VSENSE_ADC_INSTANCE))
  {
    /* Note: Call of the functions below are commented because they are       */
    /*       useless in this example:                                         */
    /*       setting corresponding to default configuration from reset state. */

    /* Set ADC clock (conversion clock) */
    VSENSE_ADC_SET_CLOCK();

    /* Set ADC channels sampling time */
    /* Note: On this STM32 serie, sampling time is common to all channels     */
    /*       of the entire ADC instance.                                      */
    /*       Therefore, sampling time is configured here under ADC instance   */
    /*       scope (not under channel scope as on some other STM32 devices    */
    /*       on which sampling time is channel wise).                         */
    /* Note: Considering interruption occurring after each ADC conversion     */
    /*       when ADC conversion is out of the analog watchdog window         */
    /*       selected (IT from ADC analog watchdog),                          */
    /*       select sampling time and ADC clock with sufficient               */
    /*       duration to not create an overhead situation in IRQHandler.      */

    /* Delay for internal voltage reference stabilization time.               */
    /* Compute number of CPU cycles to wait for, from delay in us.            */
    /* Note: Variable divided by 2 to compensate partially                    */
    /*       CPU processing cycles (depends on compilation optimization).     */
    /* Note: If system core clock frequency is below 200kHz, wait time        */
    /*       is only a few CPU processing cycles.                             */
    wait_loop_index = ((LL_ADC_DELAY_VREFINT_STAB_US * (SystemCoreClock / (100000u * 2u))) / 10u);
    while (0u != wait_loop_index)
    {
      wait_loop_index--;
    }

    /* Set ADC channels sampling time */
    /* Note: On this STM32 serie, sampling time is common to groups           */
    /*       of severals channels within ADC instance.                        */
    /*       Therefore, groups of sampling sampling times are configured      */
    /*       here under ADC instance scope.                                   */
    /*       Then, sampling time of channels are configured below             */
    /*       among group of sampling times available, at channel scope.       */
    VSENSE_ADC_SET_SAMPLING_TIME();
  }

  /*## Configuration of ADC hierarchical scope: ADC group regular ############*/

  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       ADC must be disabled or enabled without conversion on going        */
  /*       on group regular.                                                  */
  uint32_t is_conversion = LL_ADC_REG_IsConversionOngoing(VSENSE_ADC_INSTANCE);
  uint32_t is_enabled = LL_ADC_IsEnabled(VSENSE_ADC_INSTANCE);
  if ((0u == is_conversion) || (0u == is_enabled))
  {
    /* Set ADC group regular trigger source */
    LL_ADC_REG_SetTriggerSource(VSENSE_ADC_INSTANCE, LL_ADC_REG_TRIG_SOFTWARE);

    /* Set ADC group regular continuous mode */
    LL_ADC_REG_SetContinuousMode(VSENSE_ADC_INSTANCE, LL_ADC_REG_CONV_CONTINUOUS);

    /* Set ADC group regular overrun behavior */
    LL_ADC_REG_SetOverrun(VSENSE_ADC_INSTANCE, LL_ADC_REG_OVR_DATA_OVERWRITTEN);

    /* Set ADC group regular sequencer */
    /* Note: On this STM32 serie, ADC group regular sequencer has             */
    /*       two settings:                                                    */
    /*       - Sequencer configured to fully configurable:                    */
    /*         sequencer length and each rank                                 */
    /*         affectation to a channel are configurable.                     */
    /*         Channels selection is limited to channels 0 to 14.             */
    /*       - Sequencer configured to not fully configurable:                */
    /*         sequencer length and each rank affectation to a channel        */
    /*         are fixed by channel HW number.                                */
    /*         Channels selection is not limited (channels 0 to 18).          */
    /*       Refer to description of function                                 */
    /*       "LL_ADC_REG_SetSequencerConfigurable()".                         */
  
#if !defined(USE_STM32G4XX_NUCLEO)
    /* Clear flag ADC channel configuration ready */
    LL_ADC_ClearFlag_CCRDY(VSENSE_ADC_INSTANCE);
#endif /* STM32G474xx */
    
    /* Set ADC group regular sequence: channel on rank corresponding to       */
    /* channel number.                                                        */
    VSENSE_ADC_SET_CHANNEL();
  }

  /*## Configuration of ADC hierarchical scope: ADC group injected ###########*/

  /* Note: Feature not available on this STM32 serie */

  /*## Configuration of ADC hierarchical scope: channels #####################*/

  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       ADC must be disabled or enabled without conversion on going        */
  /*       on either groups regular or injected.                              */
  uint32_t reg_is_ongoing = LL_ADC_REG_IsConversionOngoing(VSENSE_ADC_INSTANCE);
  uint32_t adc_is_enabled = LL_ADC_IsEnabled(VSENSE_ADC_INSTANCE);
  if ((0u == adc_is_enabled) || (0u == reg_is_ongoing))
  {
    /* Set ADC channels sampling time */
    VSENSE_ADC_SET_SAMPLING_TIME();
  }

  /*## Configuration of ADC transversal scope: analog watchdog ###############*/

  /* Note: On this STM32 serie, there is only 1 analog watchdog available.    */

  /* Set ADC analog watchdog: channels to be monitored */
  LL_ADC_SetAnalogWDMonitChannels(VSENSE_ADC_INSTANCE, LL_ADC_AWD1, LL_ADC_AWD_ALL_CHANNELS_REG);

  /* Set ADC analog watchdog: thresholds */
  LL_ADC_ConfigAnalogWDThresholds(VSENSE_ADC_INSTANCE, LL_ADC_AWD1, 700, 600);

  /*## Configuration of ADC transversal scope: oversampling ##################*/

  /* Set ADC oversampling scope */
  LL_ADC_SetOverSamplingScope(VSENSE_ADC_INSTANCE, LL_ADC_OVS_GRP_REGULAR_CONTINUED);

  /* Set ADC oversampling parameters */
  LL_ADC_ConfigOverSamplingRatioShift(VSENSE_ADC_INSTANCE, LL_ADC_OVS_RATIO_16, LL_ADC_OVS_SHIFT_RIGHT_4);

  /*## Configuration of ADC interruptions ####################################*/
  /* Enable ADC analog watchdog 1 interruption */
}

/**
  * @brief  Perform ADC activation procedure to make it ready to convert
  *         (ADC instance: VSENSE_ADC_INSTANCE).
  * @note   Operations:
  *         - ADC instance
  *           - Run ADC self calibration
  *           - Enable ADC
  *         - ADC group regular
  *           none: ADC conversion start-stop to be performed
  *                 after this function
  *         - ADC group injected
  *           Feature not available                                  (feature not available on this STM32 serie)
  * @retval None
  */
static void PWR_Activate_ADC(void)
{
  __IO uint32_t wait_loop_index = 0;
  __IO uint32_t backup_setting_adc_dma_transfer;
#if (USE_TIMEOUT == 1)
  uint32_t Timeout = 0; /* Variable used for timeout management */
#endif /* USE_TIMEOUT */

  /*## Operation on ADC hierarchical scope: ADC instance #####################*/

  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       ADC must be disabled.                                              */
  /* Note: In this example, all these checks are not necessary but are        */
  /*       implemented anyway to show the best practice usages                */
  /*       corresponding to reference manual procedure.                       */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
  if (0u == LL_ADC_IsEnabled(VSENSE_ADC_INSTANCE))
  {
#if defined(STM32G474xx)
    LL_ADC_DisableDeepPowerDown(VSENSE_ADC_INSTANCE);
#endif
    /* Enable ADC internal voltage regulator */
    LL_ADC_EnableInternalRegulator(VSENSE_ADC_INSTANCE);

    /* Delay for ADC internal voltage regulator stabilization.                */
    /* Compute number of CPU cycles to wait for, from delay in us.            */
    /* Note: Variable divided by 2 to compensate partially                    */
    /*       CPU processing cycles (depends on compilation optimization).     */
    /* Note: If system core clock frequency is below 200kHz, wait time        */
    /*       is only a few CPU processing cycles.                             */
    wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000u * 2u))) / 10u);
    while (0u != wait_loop_index)
    {
      wait_loop_index--;
    }

    /* Disable ADC DMA transfer request during calibration */
    /* Note: Specificity of this STM32 serie: Calibration factor is           */
    /*       available in data register and also transfered by DMA.           */
    /*       To not insert ADC calibration factor among ADC conversion data   */
    /*       in DMA destination address, DMA transfer must be disabled during */
    /*       calibration.                                                     */
    backup_setting_adc_dma_transfer = LL_ADC_REG_GetDMATransfer(VSENSE_ADC_INSTANCE);
    LL_ADC_REG_SetDMATransfer(VSENSE_ADC_INSTANCE, LL_ADC_REG_DMA_TRANSFER_NONE);

#if !defined (STM32G474xx)    
    /* Run ADC self calibration */
    LL_ADC_StartCalibration(VSENSE_ADC_INSTANCE);
#endif /* STM32G474xx */
    
    /* Poll for ADC effectively calibrated */
#if (USE_TIMEOUT == 1)
    Timeout = ADC_CALIBRATION_TIMEOUT_MS;
#endif /* USE_TIMEOUT */

    while (0u != LL_ADC_IsCalibrationOnGoing(VSENSE_ADC_INSTANCE))
    {
#if (USE_TIMEOUT == 1)
      /* Check Systick counter flag to decrement the time-out value */
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        if (0u == Timeout--)
        {
          /* Time-out occurred. Set LED to blinking mode */
          LED_Blinking(LED_BLINK_ERROR);
        }
      }
#endif /* USE_TIMEOUT */
    }

    /* Restore ADC DMA transfer request after calibration */
    LL_ADC_REG_SetDMATransfer(VSENSE_ADC_INSTANCE, backup_setting_adc_dma_transfer);

    /* Delay between ADC end of calibration and ADC enable.                   */
    /* Note: Variable divided by 2 to compensate partially                    */
    /*       CPU processing cycles (depends on compilation optimization).     */
    wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1u);
    while (0u != wait_loop_index)
    {
      wait_loop_index--;
    }

    /* Enable ADC */
    LL_ADC_Enable(VSENSE_ADC_INSTANCE);

    /* Poll for ADC ready to convert */
#if (USE_TIMEOUT == 1)
    Timeout = ADC_ENABLE_TIMEOUT_MS;
#endif /* USE_TIMEOUT */

    while (0u == LL_ADC_IsActiveFlag_ADRDY(VSENSE_ADC_INSTANCE))
    {
#if (USE_TIMEOUT == 1)
      /* Check Systick counter flag to decrement the time-out value */
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        if (0u == Timeout--)
        {
          /* Time-out occurred. Set LED to blinking mode */
          LED_Blinking(LED_BLINK_ERROR);
        }
      }
#endif /* USE_TIMEOUT */
    }

    /* Note: ADC flag ADRDY is not cleared here to be able to check ADC       */
    /*       status afterwards.                                               */
    /*       This flag should be cleared at ADC Deactivation, before a new    */
    /*       ADC activation, using function "LL_ADC_ClearFlag_ADRDY()".       */
  }

  /*## Operation on ADC hierarchical scope: ADC group regular ################*/
  /* Note: No operation on ADC group regular performed here.                  */
  /*       ADC group regular conversions to be performed after this function  */
  /*       using function:                                                    */
  /*       "LL_ADC_REG_StartConversion();"                                    */

  /*## Operation on ADC hierarchical scope: ADC group injected ###############*/
  /* Note: Feature not available on this STM32 serie */
}

/**
  * @brief  Init VCC (specific functions)
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval None
  */
static void PWR_VCCInit(uint32_t Instance)
{
  /* Configure the VCC pin in output mode and set the pin at reset set */
#if defined(USE_STM32G4XX_NUCLEO)
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_10, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_10);
#elif defined(USE_STM32G0XX_NUCLEO)
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_10, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_10);  
#else
#error "missing value definition for your your board"
#endif  
}

/**
  * @brief  De-Init VCC (specific functions)
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval None
  */
static void PWR_VCCDeInit(uint32_t Instance)
{
#if defined(USE_STM32G4XX_NUCLEO)
  LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
#elif defined(USE_STM32G0XX_NUCLEO)
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);  
#else
#error "missing value definition for your your board"
#endif  
}


/**
  * @brief  Init DB_OUT (specific functions)
  * @param  Instance Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval None
  */
static void PWR_DB_OUTInit(uint32_t Instance)
{
  /* Configure the DB_OUT pin in output mode and set the pin at reset set */
#if defined(USE_STM32G4XX_NUCLEO)  
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1);
#elif defined(USE_STM32G0XX_NUCLEO)
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
#else
#error "missing value definition for your your board"
#endif  
}


/* USER CODE END POWER_Private_Functions */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
