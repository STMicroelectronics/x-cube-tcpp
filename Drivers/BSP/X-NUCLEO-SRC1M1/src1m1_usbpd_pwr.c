/**
  ******************************************************************************
  * @file    src1m1_usbpd_pwr.c
  * @author  MCD Application Team
  * @brief   This file provides a set of firmware functions to manage USB-PD Power
  *          available on STM32G4XX_NUCLEO board(MB1367) or STM32G0XX_NUCLEO board(MB1360) from STMicroelectronics :
  *            - VBUS control
  *            - VBUS voltage/current measurement
  *            - VCONN control
  *            - VBUS presence detection
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "src1m1_usbpd_pwr.h"
#include "src1m1_bus.h"

#if defined(_TRACE)
#include "usbpd_core.h"
#include "usbpd_trace.h"
#include "string.h"
#ifndef _STDIO
#include "stdio.h"
#endif /* _STDIO */
#endif /* _TRACE */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup SRC1M1
  * @{
  */

/** @addtogroup SRC1M1_USBPD_PWR
  * @{
  */

/** @defgroup SRC1M1_USBPD_PWR_Exported_Variables Exported Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup SRC1M1_USBPD_PWR_Private_Typedef Private Typedef
  * @{
  */
/**
  * @brief  Port HW configuration description
  */
typedef struct
{
  uint8_t                    Type;              /*!< HW component associated to Port          */
  uint8_t                    Address;           /*!< I2C address (if any) associated to Port  */
} USBPD_PWR_PortConfig_t;

/**
  * @brief  Port BSP Power status
  */
typedef struct
{
  uint8_t                           IsInitialized;     /*!< Port Context Initialization status       */
  uint8_t                           IsItEnabled;       /*!< Port IT enabled status                   */
  USBPD_PWR_PowerRoleTypeDef        PwrRole;           /*!< Port current power role (SRC or SNK)     */
  USBPD_PWR_PowerModeTypeDef        PwrSavingMode;     /*!< Port current power saving mode           */
  uint32_t                          LastFaultTick;     /*!< Current Tick on last detected fault      */
  USBPD_PWR_VBUSDetectCallbackFunc *VBUSDetectCallback;/*!< Port callback for VBUS detection event   */
} USBPD_PWR_PortStatus_t;

/**
  * @}
  */

/** @defgroup SRC1M1_USBPD_PWR_Private_Constants Private Constants
  * @{
  */

/* Maximum digital value of the ADC output (12 Bits resolution)
   To convert ADC measurement to an absolute voltage value:
   VCHANNELx = ADCx_DATA x (VDD/ADC_FULL_SCALE)
  */
#define ADC_FULL_SCALE       (0x0FFFU)

/* Delay between ADC end of calibration and ADC enable.                     */
/* Delay estimation in CPU cycles: Case of ADC enable done                  */
/* immediately after ADC calibration, ADC clock setting slow                */
/* (LL_ADC_CLOCK_ASYNC_DIV32). Use a higher delay if ratio                  */
/* (CPU clock / ADC clock) is above 32.                                     */
#if !defined(USBPD_CONFIG_MX)
#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES  (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32U)
#endif /* !USBPD_CONFIG_MX */

#define VDDA_APPLI            (3300U)

#define ABS(__VAL__) (((int32_t)(__VAL__)) < 0 ? - ((int32_t)(__VAL__)) : ((int32_t)(__VAL__)))

/* Minimum time between 2 detected faults allowing to consider that
   fault could be due to a false detection (could be recovered).
   => if 2 faults occurred in less than that duration, no recovery will be executed */
#define USBPD_PWR_FAULT_MIN_TIME_RECOVERY             (1000U)             /* 1s */

/**
  * @}
  */

/** @defgroup SRC1M1_USBPD_PWR_Private_Macros Private Macros
  * @{
  */
#ifdef _TRACE
#define BSP_USBPD_PWR_TRACE(_PORT_,_MSG_) USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (uint8_t)_PORT_, 0U , (uint8_t *)_MSG_,\
                                                          sizeof(_MSG_) - 1U);
#else
#define BSP_USBPD_PWR_TRACE(_PORT_,_MSG_)
#endif /* _TRACE */

/**
  * @}
  */

/** @defgroup SRC1M1_USBPD_PWR_Private_Functions Private Functions
  * @{
  */
#if !defined(USBPD_CONFIG_MX)
static void     PWR_TCPP0203_Configure_ADC(void);
static void     PWR_TCPP0203_Activate_ADC(void);
static void     PWR_TCPP0203_GPIOConfigInit(uint32_t PortNum);
static void     PWR_TCPP0203_ITConfigInit(uint32_t PortNum);
#endif /* !USBPD_CONFIG_MX */
static int32_t  PWR_TCPP0203_BUSConfigInit(uint32_t PortNum, uint16_t Address);
static int32_t  PWR_TCPP0203_ConfigDeInit(uint32_t PortNum);
static void     PWR_TCPP0203_EventCallback(uint32_t PortNum);

static uint32_t PWR_TCPP0203_ConvertADCDataToVoltage(uint32_t ADCData, uint32_t Ra, uint32_t Rb);
static int32_t  PWR_TCPP0203_ConvertADCDataToCurrent(uint32_t ADCData, uint32_t Ga, uint32_t Rs);

/**
  * @}
  */

/** @defgroup SRC1M1_USBPD_PWR_Private_Variables Private Variables
  * @{
  */
uint16_t usbpd_pwr_adcx_buff[VISENSE_ADC_BUFFER_SIZE]; /* Global ADC buffer to be filled by DMA */

static USBPD_PWR_PortConfig_t USBPD_PWR_Port_Configs[USBPD_PWR_INSTANCES_NBR] =
{
  {
    USBPD_PWR_HW_CONFIG_TYPE_TCPP02,             /* Port 0 : TCPP Type                               */
    TCPP0203_I2C_ADDRESS_X68,                    /* TCPP02 on shield (Address 0x34)                  */
  },
};

static USBPD_PWR_PortStatus_t USBPD_PWR_Port_Status[USBPD_PWR_INSTANCES_NBR] =
{
  {0U, 0U, POWER_ROLE_SOURCE, USBPD_PWR_MODE_OFF, 0U, NULL},
};

TCPP0203_Object_t      USBPD_PWR_PortCompObj[USBPD_PWR_INSTANCES_NBR] = { 0 };
TCPP0203_Drv_t        *USBPD_PWR_PortCompDrv[USBPD_PWR_INSTANCES_NBR] = { NULL };

/**
  * @}
  */

/** @addtogroup SRC1M1_USBPD_PWR_Exported_Functions
  * @{
  */

/**
  * @brief  Global initialization of PWR resource used by USB-PD
  * @param  PortNum Type-C port identifier
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_Init(uint32_t PortNum)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_Init --");

    if (USBPD_PWR_Port_Status[PortNum].IsInitialized == 0U)
    {
      /* Initialisation according to HW configuration type of port */
      switch (USBPD_PWR_Port_Configs[PortNum].Type)
      {
        case USBPD_PWR_HW_CONFIG_TYPE_TCPP02:
          /* Set default Role to SRC */
          USBPD_PWR_Port_Status[PortNum].PwrRole = POWER_ROLE_SOURCE;

          /* Set default Power Mode to Hibernate */
          USBPD_PWR_Port_Status[PortNum].PwrSavingMode = USBPD_PWR_MODE_HIBERNATE;

          /* Reset port callback for VBUS detection event */
          USBPD_PWR_Port_Status[PortNum].VBUSDetectCallback = NULL;

          /* Reset last detected fault Tick */
          USBPD_PWR_Port_Status[PortNum].LastFaultTick = 0;
#if !defined(USBPD_CONFIG_MX)
          /* Initialize required GPIOs */
          PWR_TCPP0203_GPIOConfigInit(PortNum);

          /* Initialize EXTI for FLGn signal */
          PWR_TCPP0203_ITConfigInit(PortNum);

          /* Enable component */
          TCPP0203_PORT0_ENABLE_GPIO_SET();
#endif /* !USBPD_CONFIG_MX */
          /* Initialize required BUS for communication */
          ret = PWR_TCPP0203_BUSConfigInit(PortNum, USBPD_PWR_Port_Configs[PortNum].Address);
          break;

        case USBPD_PWR_HW_CONFIG_TYPE_DEFAULT:
        case USBPD_PWR_HW_CONFIG_TYPE_TCPP01:
        case USBPD_PWR_HW_CONFIG_TYPE_TCPP03:
        default:
          ret = BSP_ERROR_WRONG_PARAM;
          break;
      }

      if (ret == BSP_ERROR_NONE)
      {
        /* Set Initialisation flag */
        USBPD_PWR_Port_Status[PortNum].IsInitialized = 1U;
      }
    }
  }
  return ret;
}

/**
  * @brief  Global de-initialization of PWR resource used by USB-PD
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_Deinit(uint32_t PortNum)
{
  int32_t ret;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_DeInit --");

    /* Deinitialize Power config for port */
    switch (USBPD_PWR_Port_Configs[PortNum].Type)
    {
      case USBPD_PWR_HW_CONFIG_TYPE_TCPP02:
        /* Perform any treatments required for de-initialization for GPIO, BUS communication, Interrupts */
        ret = PWR_TCPP0203_ConfigDeInit(PortNum);
        break;

      case USBPD_PWR_HW_CONFIG_TYPE_DEFAULT:
      case USBPD_PWR_HW_CONFIG_TYPE_TCPP01:
      case USBPD_PWR_HW_CONFIG_TYPE_TCPP03:
      default:
        ret = BSP_ERROR_WRONG_PARAM;
        break;
    }

    /* Reset Initialisation flag */
    USBPD_PWR_Port_Status[PortNum].IsInitialized = 0U;
  }

  return ret;
}

/**
  * @brief  Assign Power role for current Port (Source or Sink)
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  Role Type-C port role
  *         This parameter can take one of the following values:
  *         @arg @ref POWER_ROLE_SOURCE
  *         @arg @ref POWER_ROLE_SINK
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_SetRole(uint32_t PortNum, USBPD_PWR_PowerRoleTypeDef Role)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    switch (Role)
    {
      case POWER_ROLE_SOURCE:
        /* Port Role is now SRC */
        BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_SetRole : SRC --");
        /* Set port power Role */
        USBPD_PWR_Port_Status[PortNum].PwrRole = Role;

        /* Ensure that TCPP0203 mode is not set to Hibernate */
        if (USBPD_PWR_Port_Status[PortNum].PwrSavingMode == USBPD_PWR_MODE_HIBERNATE)
        {
          if (USBPD_PWR_PortCompDrv[PortNum]->SetPowerMode(&USBPD_PWR_PortCompObj[PortNum],
                                                           TCPP0203_POWER_MODE_LOWPOWER) != TCPP0203_OK)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            USBPD_PWR_Port_Status[PortNum].PwrSavingMode = USBPD_PWR_MODE_LOWPOWER;
          }
        }
        break;

      case POWER_ROLE_SINK:
      default:
        ret = BSP_ERROR_WRONG_PARAM;
        break;
    }
  }

  return ret;
}


/**
  * @brief  Set operating mode of Port regarding power saving constraints
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  PwrMode Type-C port power saving mode
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_MODE_OFF
  *         @arg @ref USBPD_PWR_MODE_HIBERNATE
  *         @arg @ref USBPD_PWR_MODE_LOWPOWER
  *         @arg @ref USBPD_PWR_MODE_NORMAL
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_SetPowerMode(uint32_t PortNum, USBPD_PWR_PowerModeTypeDef PwrMode)
{
  uint8_t flg_reg;
  int32_t ret = BSP_ERROR_NONE;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_SetPowerMode --");

    /* Evaluate requested Power config for port */
    switch (PwrMode)
    {
      case USBPD_PWR_MODE_OFF:
        /* Port should enter OFF mode : Reset TCPP0203 Reg0 */
        if (USBPD_PWR_PortCompDrv[PortNum]->Reset(&USBPD_PWR_PortCompObj[PortNum]) != TCPP0203_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        break;

      case USBPD_PWR_MODE_HIBERNATE:
        /* Port Mode is Hibernate : (Default state at startup)
             - Gate Driver Provider OFF
             - waiting for VBUS_OK in FLGn (interrupt)
        */
        BSP_USBPD_PWR_TRACE(PortNum, "-- Hibernate --");
        if (USBPD_PWR_PortCompDrv[PortNum]->SetPowerMode(&USBPD_PWR_PortCompObj[PortNum],
                                                         TCPP0203_POWER_MODE_HIBERNATE) != TCPP0203_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }

        /* Check if VBUS is present (in case of SNK VBUS powered device) */
        /* Read Flags register (FLGn) */
        if (USBPD_PWR_PortCompDrv[PortNum]->ReadFlagRegister(&USBPD_PWR_PortCompObj[PortNum], &flg_reg) == TCPP0203_OK)
        {
          /* If FLGn has been set to 0 in LOW POWER or HIBERNATE mode,
             it is expected that a VBUS_OK condition is detected.
             In this case, if any, vBus Detection callback could be called */
          if ((flg_reg & TCPP0203_FLAG_VBUS_OK_SET) == TCPP0203_FLAG_VBUS_OK_SET)
          {
            if (USBPD_PWR_Port_Status[PortNum].VBUSDetectCallback != NULL)
            {
              /* In case of TCPP03 in Hibernate Mode (SNK config),
                 Mode is set to Low power to enable TCPP02 behavior on CC lines */
              /* Switch to Low Power mode */
              BSP_USBPD_PWR_TRACE(PortNum, "-- Low Power --");
              if (USBPD_PWR_PortCompDrv[PortNum]->SetPowerMode(&USBPD_PWR_PortCompObj[PortNum],
                                                               TCPP0203_POWER_MODE_LOWPOWER) != TCPP0203_OK)
              {
                ret = BSP_ERROR_COMPONENT_FAILURE;
              }

              USBPD_PWR_Port_Status[PortNum].VBUSDetectCallback(PortNum, VBUS_CONNECTED);
            }
          }
          else
          {
            /* VBUS indication not present in FLGn flags : nothing more to be done */
          }
        }
        break;

      case USBPD_PWR_MODE_LOWPOWER:
        /* Port Mode is Low Power :
             - Gate Driver Consumer or Provider controlled by I2C
             - waiting for VBUS_OK in FLGn (interrupt)
        */
        BSP_USBPD_PWR_TRACE(PortNum, "-- Low Power --");
        if (USBPD_PWR_PortCompDrv[PortNum]->SetPowerMode(&USBPD_PWR_PortCompObj[PortNum],
                                                         TCPP0203_POWER_MODE_LOWPOWER) != TCPP0203_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        break;

      case USBPD_PWR_MODE_NORMAL:
        /* Port Mode is Normal : No power saving measure (Explicit contract in place) */
        BSP_USBPD_PWR_TRACE(PortNum, "-- Normal --");
        if (USBPD_PWR_PortCompDrv[PortNum]->SetPowerMode(&USBPD_PWR_PortCompObj[PortNum],
                                                         TCPP0203_POWER_MODE_NORMAL) != TCPP0203_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        break;

      default:
        ret = BSP_ERROR_WRONG_PARAM;
        break;
    }

    /* Set port low power operating mode */
    USBPD_PWR_Port_Status[PortNum].PwrSavingMode = PwrMode;
  }

  return ret;
}

/**
  * @brief  Get operating mode of Port regarding power saving constraints
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  PwrMode Pointer on current Type-C port power saving mode value
  *         Following values are available :
  *         @arg @ref USBPD_PWR_MODE_OFF
  *         @arg @ref USBPD_PWR_MODE_HIBERNATE
  *         @arg @ref USBPD_PWR_MODE_LOWPOWER
  *         @arg @ref USBPD_PWR_MODE_NORMAL
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_GetPowerMode(uint32_t PortNum, USBPD_PWR_PowerModeTypeDef *PwrMode)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_GetPowerMode --");

    /* Get port low power operating mode */
    *PwrMode = USBPD_PWR_Port_Status[PortNum].PwrSavingMode;
  }

  return ret;
}

/**
  * @brief  Initialize the hardware resources used by the Type-C power delivery (PD)
  *         controller.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSInit(uint32_t PortNum)
{
  int32_t ret;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VBUSInit --");

    /* Initialize required HW for VBUS management */
    switch (USBPD_PWR_Port_Configs[PortNum].Type)
    {
      case USBPD_PWR_HW_CONFIG_TYPE_TCPP02:
        /* Switch to Normal mode */
        ret = BSP_USBPD_PWR_SetPowerMode(PortNum, USBPD_PWR_MODE_NORMAL);
#if !defined(USBPD_CONFIG_MX)
        PWR_TCPP0203_Configure_ADC();
        PWR_TCPP0203_Activate_ADC();

        /*  Start Conversion */
        LL_ADC_REG_StartConversion(VISENSE_ADC_INSTANCE);
#endif /* !USBPD_CONFIG_MX */
        break;

      case USBPD_PWR_HW_CONFIG_TYPE_DEFAULT:
      case USBPD_PWR_HW_CONFIG_TYPE_TCPP01:
      case USBPD_PWR_HW_CONFIG_TYPE_TCPP03:
      default:
        ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
        break;
    }
  }
  return ret;
}

/**
  * @brief  Release the hardware resources used by the Type-C power delivery (PD)
  *         controller.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSDeInit(uint32_t PortNum)
{
  int32_t ret;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VBUSDeInit --");

    /* De-Initialize required HW for VBUS management */
    switch (USBPD_PWR_Port_Configs[PortNum].Type)
    {
      case USBPD_PWR_HW_CONFIG_TYPE_TCPP02:
        /* Switch to Low Power mode */
        ret = BSP_USBPD_PWR_SetPowerMode(PortNum, USBPD_PWR_MODE_LOWPOWER);
        break;

      case USBPD_PWR_HW_CONFIG_TYPE_DEFAULT:
      case USBPD_PWR_HW_CONFIG_TYPE_TCPP01:
      case USBPD_PWR_HW_CONFIG_TYPE_TCPP03:
      default:
        ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
        break;
    }
  }
  return ret;
}

/**
  * @brief  Enable power supply over VBUS.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSOn(uint32_t PortNum)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VBUSOn --");
    /* Port Role is now SRC : Close Gate Driver Provider */
    if (USBPD_PWR_Port_Status[PortNum].PwrRole == POWER_ROLE_SOURCE)
    {
      if (USBPD_PWR_PortCompDrv[PortNum]->SetGateDriverProvider(&USBPD_PWR_PortCompObj[PortNum],
                                                                TCPP0203_GD_PROVIDER_SWITCH_CLOSED) != TCPP0203_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      BSP_USBPD_PWR_TRACE(PortNum, "-- GDP/GDC setting : SRC --");
    }
    else
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Disable power supply over VBUS.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSOff(uint32_t PortNum)
{
  int32_t ret = BSP_ERROR_NONE;
#if (BSP_USBPD_PWR_DONT_WAIT_VBUSOFF_DISCHARGE == 0)
  uint32_t vbus;
#endif /* BSP_USBPD_PWR_DONT_WAIT_VBUSOFF_DISCHARGE == 0 */

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Open Gate driver provider */
    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VBUSOff --");
    if (USBPD_PWR_PortCompDrv[PortNum]->SetGateDriverProvider(&USBPD_PWR_PortCompObj[PortNum],
                                                              TCPP0203_GD_PROVIDER_SWITCH_OPEN) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
#if (BSP_USBPD_PWR_DONT_WAIT_VBUSOFF_DISCHARGE == 0)
    else
    {
      /* Set Discharge On */
      if (USBPD_PWR_PortCompDrv[PortNum]->SetVBusDischarge(&USBPD_PWR_PortCompObj[PortNum],
                                                           TCPP0203_VBUS_DISCHARGE_ON) != TCPP0203_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        /* Wait VBUS level becomes lower than USBPD_PWR_LOW_VBUS_THRESHOLD */
        uint8_t counter = 0;

        (void)BSP_USBPD_PWR_VBUSGetVoltage(PortNum, &vbus);

        while ((vbus >= (USBPD_PWR_LOW_VBUS_THRESHOLD - 200U)) && (counter < 20U))
        {
          counter++;
          HAL_Delay(20);
          (void)BSP_USBPD_PWR_VBUSGetVoltage(PortNum, &vbus);
        }

        /* Set Discharge Off */
        if (USBPD_PWR_PortCompDrv[PortNum]->SetVBusDischarge(&USBPD_PWR_PortCompObj[PortNum],
                                                             TCPP0203_VBUS_DISCHARGE_OFF) != TCPP0203_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }

        HAL_Delay(30);
      }
    }
#endif /* BSP_USBPD_PWR_DONT_WAIT_VBUSOFF_DISCHARGE == 0 */
  }
  return ret;
}

/**
  * @brief  Get actual VBUS status.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  pState VBUS status (1: On, 0: Off)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSIsOn(uint32_t PortNum, uint8_t *pState)
{
  int32_t ret = BSP_ERROR_NONE;
  uint8_t val = 0;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (USBPD_PWR_PortCompDrv[PortNum]->GetGateDriverProviderAck(&USBPD_PWR_PortCompObj[PortNum],
                                                                 &val) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      if (val == TCPP0203_GD_PROVIDER_SWITCH_ACK_OPEN)
      {
        *pState = 0;
      }
      else
      {
        *pState = 1;
      }
    }
  }
  return ret;
}

/**
  * @brief  Set a fixed PDO and manage the power control.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  VbusTargetInmv the vbus Target (in mV)
  * @param  OperatingCurrent the Operating Current (in mA)
  * @param  MaxOperatingCurrent the Max Operating Current (in mA)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSSetVoltage_Fixed(uint32_t PortNum,
                                           uint32_t VbusTargetInmv,
                                           uint32_t OperatingCurrent,
                                           uint32_t MaxOperatingCurrent)
{
  int32_t ret = BSP_ERROR_NONE;
  UNUSED(MaxOperatingCurrent);
  UNUSED(OperatingCurrent);
  UNUSED(VbusTargetInmv);

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  return ret;
}

/**
  * @brief  Set a variable PDO and manage the power control.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  VbusTargetMinInmv the vbus Target min (in mV)
  * @param  VbusTargetMaxInmv the vbus Target max (in mV)
  * @param  OperatingCurrent the Operating Current (in mA)
  * @param  MaxOperatingCurrent the Max Operating Current (in mA)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSSetVoltage_Variable(uint32_t PortNum,
                                              uint32_t VbusTargetMinInmv,
                                              uint32_t VbusTargetMaxInmv,
                                              uint32_t OperatingCurrent,
                                              uint32_t MaxOperatingCurrent)
{
  int32_t ret;
  UNUSED(MaxOperatingCurrent);
  UNUSED(OperatingCurrent);
  UNUSED(VbusTargetMaxInmv);
  UNUSED(VbusTargetMinInmv);

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  return ret;
}

/**
  * @brief  Set a Battery PDO and manage the power control.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  VbusTargetMin the vbus Target min (in mV)
  * @param  VbusTargetMax the vbus Target max (in mV)
  * @param  OperatingPower the Operating Power (in mW)
  * @param  MaxOperatingPower the Max Operating Power (in mW)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSSetVoltage_Battery(uint32_t PortNum,
                                             uint32_t VbusTargetMin,
                                             uint32_t VbusTargetMax,
                                             uint32_t OperatingPower,
                                             uint32_t MaxOperatingPower)
{
  int32_t ret;
  UNUSED(OperatingPower);
  UNUSED(VbusTargetMax);
  UNUSED(VbusTargetMin);
  UNUSED(MaxOperatingPower);

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Set the power, the precision must be at 5% */
    /* Set the current limitation */
    /* not implemented */

    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  return ret;
}

/**
  * @brief  Set a APDO and manage the power control.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  VbusTargetInmv the vbus Target (in mV)
  * @param  OperatingCurrent the Operating current (in mA)
  * @param  Delta Delta between with previous APDO (in mV), 0 means APDO start
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSSetVoltage_APDO(uint32_t PortNum,
                                          uint32_t VbusTargetInmv,
                                          uint32_t OperatingCurrent,
                                          int32_t Delta)
{
  int32_t ret;
  UNUSED(Delta);
  UNUSED(OperatingCurrent);
  UNUSED(VbusTargetInmv);

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  return ret;
}

/**
  * @brief  Set the VBUS disconnection voltage threshold.
  * @note   Callback function invoked when VBUS falls below programmed threshold.
  * @note   By default VBUS disconnection threshold is set to 3.3V
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  VoltageThreshold VBUS disconnection voltage threshold (in mV)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_SetVBUSDisconnectionThreshold(uint32_t PortNum,
                                                    uint32_t VoltageThreshold)
{
  UNUSED(VoltageThreshold);
  int32_t ret;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  return ret;
}

/**
  * @brief  Register USB Type-C Current callback function.
  * @note   Callback function invoked when VBUS rises above 4V (VBUS present) or
  *         when VBUS falls below programmed threshold (VBUS absent).
  * @note   Callback function is un-registered when callback function pointer
  *         argument is NULL.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  pfnVBUSDetectCallback callback function pointer
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_RegisterVBUSDetectCallback(uint32_t PortNum,
                                                 USBPD_PWR_VBUSDetectCallbackFunc *pfnVBUSDetectCallback)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if instance is valid */
  if ((PortNum >= USBPD_PWR_INSTANCES_NBR) || (NULL == pfnVBUSDetectCallback))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Set port callback for VBUS detection event */
    USBPD_PWR_Port_Status[PortNum].VBUSDetectCallback = pfnVBUSDetectCallback;
  }
  return ret;
}

/**
  * @brief  Get actual voltage level measured on the VBUS line.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  pVoltage Pointer on measured voltage level (in mV)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSGetVoltage(uint32_t PortNum, uint32_t *pVoltage)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if instance is valid */
  if ((PortNum >= USBPD_PWR_INSTANCES_NBR) || (NULL == pVoltage))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    uint32_t voltage;
    static __IO uint16_t adc_value;
    adc_value = (uint16_t) usbpd_pwr_adcx_buff[ADCBUF_VSENSE];
    voltage = PWR_TCPP0203_ConvertADCDataToVoltage(adc_value, SRC1M1_VSENSE_RA, SRC1M1_VSENSE_RB);

    *pVoltage = voltage;
  }
  return ret;
}

/**
  * @brief  Get actual current level measured on the VBUS line.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  pCurrent Pointer on measured current level (in mA)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSGetCurrent(uint32_t PortNum, int32_t *pCurrent)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if instance is valid */
  if ((PortNum >= USBPD_PWR_INSTANCES_NBR) || (NULL == pCurrent))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    int32_t current;
    uint16_t adc_value;
    adc_value = (uint16_t) usbpd_pwr_adcx_buff[ADCBUF_ISENSE];
    current = PWR_TCPP0203_ConvertADCDataToCurrent(adc_value, SRC1M1_ISENSE_GA, SRC1M1_ISENSE_RS);

    *pCurrent = current;
  }
  return ret;
}

/**
  * @brief  Activate discharge on VBUS.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSDischargeOn(uint32_t PortNum)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VBUSDischargeOn --");

    if (USBPD_PWR_PortCompDrv[PortNum]->SetGateDriverProvider(&USBPD_PWR_PortCompObj[PortNum],
                                                              TCPP0203_GD_PROVIDER_SWITCH_OPEN) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      if (USBPD_PWR_PortCompDrv[PortNum]->SetVBusDischarge(&USBPD_PWR_PortCompObj[PortNum],
                                                           TCPP0203_VBUS_DISCHARGE_ON) != TCPP0203_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
  }
  return ret;
}

/**
  * @brief  Deactivate discharge on VBUS.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VBUSDischargeOff(uint32_t PortNum)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VBUSDischargeOff --");

    if (USBPD_PWR_PortCompDrv[PortNum]->SetVBusDischarge(&USBPD_PWR_PortCompObj[PortNum],
                                                         TCPP0203_VBUS_DISCHARGE_OFF) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Initialize VCONN sourcing.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VCONNInit(uint32_t PortNum,
                                uint32_t CCPinId)
{
  UNUSED(CCPinId);
  int32_t ret = BSP_ERROR_NONE;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* No specific treatment required for initialising VCONN management from TCPP03 */
    /* Initialise VCONN management from TCPP03 :
        Ensure VCONN switch to Open/Open
        Ensure VCONN Discharge OFF
    */

    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VCONNInit --");

    if (USBPD_PWR_PortCompDrv[PortNum]->SetVConnSwitch(&USBPD_PWR_PortCompObj[PortNum],
                                                       TCPP0203_VCONN_SWITCH_OPEN) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if (USBPD_PWR_PortCompDrv[PortNum]->SetVConnDischarge(&USBPD_PWR_PortCompObj[PortNum],
                                                          TCPP0203_VCONN_DISCHARGE_OFF) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  De-initialize VCONN sourcing.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VCONNDeInit(uint32_t PortNum,
                                  uint32_t CCPinId)
{
  UNUSED(CCPinId);
  int32_t ret = BSP_ERROR_NONE;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* De-initialise VCONN management from TCPP03 :
        Force VCONN switch to Open/Open
        Force VCONN Discharge OFF
    */

    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VCONNDeInit --");

    if (USBPD_PWR_PortCompDrv[PortNum]->SetVConnSwitch(&USBPD_PWR_PortCompObj[PortNum],
                                                       TCPP0203_VCONN_SWITCH_OPEN) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      if (USBPD_PWR_PortCompDrv[PortNum]->SetVConnDischarge(&USBPD_PWR_PortCompObj[PortNum],
                                                            TCPP0203_VCONN_DISCHARGE_OFF) != TCPP0203_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
  }
  return ret;
}

/**
  * @brief  Enable VCONN sourcing.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VCONNOn(uint32_t PortNum,
                              uint32_t CCPinId)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if parameters are valid */
  if ((PortNum >= USBPD_PWR_INSTANCES_NBR)
      || ((CCPinId != USBPD_PWR_TYPE_C_CC1) && (CCPinId != USBPD_PWR_TYPE_C_CC2))
     )
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Set TCPP0203 VCONN Switch according to CC pin used for VCONN */
    if (CCPinId == USBPD_PWR_TYPE_C_CC1)
    {
      if (USBPD_PWR_PortCompDrv[PortNum]->SetVConnSwitch(&USBPD_PWR_PortCompObj[PortNum],
                                                         TCPP0203_VCONN_SWITCH_CC1) != TCPP0203_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    else
    {
      if (USBPD_PWR_PortCompDrv[PortNum]->SetVConnSwitch(&USBPD_PWR_PortCompObj[PortNum],
                                                         TCPP0203_VCONN_SWITCH_CC2) != TCPP0203_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
  }
  return ret;
}

/**
  * @brief  Disable VCONN sourcing.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  CCPinId CC pin identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VCONNOff(uint32_t PortNum,
                               uint32_t CCPinId)
{
  UNUSED(CCPinId);
  int32_t ret = BSP_ERROR_NONE;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Switch off VCONN : restore VCONN switch to Open/open position */
    if (USBPD_PWR_PortCompDrv[PortNum]->SetVConnSwitch(&USBPD_PWR_PortCompObj[PortNum],
                                                       TCPP0203_VCONN_SWITCH_OPEN) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Discharge VCONN path */
      if (USBPD_PWR_PortCompDrv[PortNum]->SetVConnDischarge(&USBPD_PWR_PortCompObj[PortNum],
                                                            TCPP0203_VCONN_DISCHARGE_ON) != TCPP0203_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        /* Wait DISCHARGE is completed then disable VCONN Discharge */
        HAL_Delay(10U);
        if (USBPD_PWR_PortCompDrv[PortNum]->SetVConnDischarge(&USBPD_PWR_PortCompObj[PortNum],
                                                              TCPP0203_VCONN_DISCHARGE_OFF) != TCPP0203_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
      }
    }
  }
  return ret;
}

/**
  * @brief  Get actual VCONN status.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @param  pState VCONN status (1: On, 0: Off)
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VCONNIsOn(uint32_t PortNum,
                                uint32_t CCPinId, uint8_t *pState)
{
  int32_t ret = BSP_ERROR_NONE;
  uint8_t vconnSwitch;

  *pState = 0U;

  /* Check if parameters are valid */
  if ((PortNum >= USBPD_PWR_INSTANCES_NBR) || (NULL == pState)
      || ((CCPinId != USBPD_PWR_TYPE_C_CC1) && (CCPinId != USBPD_PWR_TYPE_C_CC2))
     )
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Get TCPP0203 VCONN Switch position */
    if (USBPD_PWR_PortCompDrv[PortNum]->GetVConnSwitchAck(&USBPD_PWR_PortCompObj[PortNum],
                                                          &vconnSwitch) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      if (((CCPinId == USBPD_PWR_TYPE_C_CC1) && (vconnSwitch == TCPP0203_VCONN_SWITCH_CC1))
          || ((CCPinId == USBPD_PWR_TYPE_C_CC2) && (vconnSwitch == TCPP0203_VCONN_SWITCH_CC2))
         )
      {
        *pState = 1U;
      }
    }
  }
  return ret;
}

/**
  * @brief  Activate discharge on VCONN.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VCONNDischargeOn(uint32_t PortNum)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VCONNDischargeOn --");

    if (USBPD_PWR_PortCompDrv[PortNum]->SetVConnSwitch(&USBPD_PWR_PortCompObj[PortNum],
                                                       TCPP0203_VCONN_SWITCH_OPEN) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      if (USBPD_PWR_PortCompDrv[PortNum]->SetVConnDischarge(&USBPD_PWR_PortCompObj[PortNum],
                                                            TCPP0203_VCONN_DISCHARGE_ON) != TCPP0203_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
  }
  return ret;
}

/**
  * @brief  Deactivate discharge on VCONN.
  * @param  PortNum Type-C port identifier
  *         This parameter can take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval BSP status
  */
int32_t BSP_USBPD_PWR_VCONNDischargeOff(uint32_t PortNum)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VCONNDischargeOff --");

    if (USBPD_PWR_PortCompDrv[PortNum]->SetVConnDischarge(&USBPD_PWR_PortCompObj[PortNum],
                                                          TCPP0203_VCONN_DISCHARGE_OFF) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  USBPD PWR callback used to notify a asynchronous PWR event.
  *         (This callback caould be called fromp an IT routine, associated to
  *         any PWR related event detection mechanism)
  * @param  PortNum Type-C port identifier
  * @retval None
  */
void BSP_USBPD_PWR_EventCallback(uint32_t PortNum)
{
  BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_EventCallback --");

  /* Handle USBPD PWR event */
  switch (USBPD_PWR_Port_Configs[PortNum].Type)
  {
    case USBPD_PWR_HW_CONFIG_TYPE_TCPP02:
      /* In case of TCPP0203, this callback could be called when an event is detected
         on FLGn line */
      PWR_TCPP0203_EventCallback(PortNum);
      break;

    case USBPD_PWR_HW_CONFIG_TYPE_DEFAULT:
    case USBPD_PWR_HW_CONFIG_TYPE_TCPP01:
    case USBPD_PWR_HW_CONFIG_TYPE_TCPP03:
    default:
      break;
  }
}

/**
  * @}
  */

/** @addtogroup SRC1M1_USBPD_PWR_Private_Functions
  * @{
  */

#if !defined(USBPD_CONFIG_MX)
/**
  * @brief  Configure TCPP0203 used GPIO.
  * @note   GPIO used for TCPP0203 operation includes VBUS measurement, ENABLE pin driving
  * @param  PortNum   Port number
  * @retval None
  */
static void PWR_TCPP0203_GPIOConfigInit(uint32_t PortNum)
{
  UNUSED(PortNum);

  /* Configure ENABLE GPIO */
  TCPP0203_PORT0_ENABLE_GPIO_CLK_ENABLE();
  /* Configure IO in output push-pull mode to drive ENABLE */
  LL_GPIO_SetPinMode(TCPP0203_PORT0_ENABLE_GPIO_PORT, TCPP0203_PORT0_ENABLE_GPIO_PIN, TCPP0203_PORT0_ENABLE_GPIO_MODE);
  LL_GPIO_SetPinOutputType(TCPP0203_PORT0_ENABLE_GPIO_PORT, TCPP0203_PORT0_ENABLE_GPIO_PIN,
                           TCPP0203_PORT0_ENABLE_GPIO_OUTPUT);
  LL_GPIO_SetPinSpeed(TCPP0203_PORT0_ENABLE_GPIO_PORT, TCPP0203_PORT0_ENABLE_GPIO_PIN, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinPull(TCPP0203_PORT0_ENABLE_GPIO_PORT, TCPP0203_PORT0_ENABLE_GPIO_PIN, TCPP0203_PORT0_ENABLE_GPIO_PUPD);

  /* Set ENABLE to default state */
  TCPP0203_PORT0_ENABLE_GPIO_DEFVALUE();

  /* Configure IANA GPIO */
  TCPP0203_PORT0_IANA_GPIO_CLK_ENABLE();
  /* Configure GPIO in Analog mode */
  LL_GPIO_SetPinMode(TCPP0203_PORT0_IANA_GPIO_PORT, TCPP0203_PORT0_IANA_GPIO_PIN, TCPP0203_PORT0_IANA_GPIO_MODE);

  /* Configure VBUS Connector sensing GPIO */
  TCPP0203_PORT0_VBUSC_GPIO_CLK_ENABLE();
  /* Configure GPIO in Analog mode */
  LL_GPIO_SetPinMode(TCPP0203_PORT0_VBUSC_GPIO_PORT, TCPP0203_PORT0_VBUSC_GPIO_PIN, TCPP0203_PORT0_VBUSC_GPIO_MODE);

  /* Configure VBUS Provider sensing GPIO */
  TCPP0203_PORT0_VBUSPROV_GPIO_CLK_ENABLE();
  /* Configure GPIO in Analog mode */
  LL_GPIO_SetPinMode(TCPP0203_PORT0_VBUSPROV_GPIO_PORT, TCPP0203_PORT0_VBUSPROV_GPIO_PIN,
                     TCPP0203_PORT0_VBUSPROV_GPIO_MODE);
}

/**
  * @brief  Configure ADC (ADC instance for Vsense and Isense: VISENSE_ADC_INSTANCE) and GPIO used by ADC channels.
  * @param  None
  * @retval None
  */
static void PWR_TCPP0203_Configure_ADC(void)
{
#if (USE_TIMEOUT == 1)
  uint32_t Timeout ; /* Variable used for Timeout management */
#endif /* USE_TIMEOUT */

  /* Init with LL driver */
#if defined(USE_STM32G4XX_NUCLEO)
  LL_RCC_SetADCClockSource(LL_RCC_ADC12_CLKSOURCE_SYSCLK);
#elif defined(USE_STM32G0XX_NUCLEO)
  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_SYSCLK);
#endif /* USE_STM32G4XX_NUCLEO */


  /* DMA controller clock enable */
#if defined(USE_STM32G4XX_NUCLEO)
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMAMUX1);
#endif /* USE_STM32G4XX_NUCLEO */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  LL_ADC_InitTypeDef ADC_InitStruct = {0U};
  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0U};
#if defined(USE_STM32G4XX_NUCLEO)
  LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0U};
#endif /* USE_STM32G4XX_NUCLEO */

  /* Peripheral clock enable */
#if defined(USE_STM32G4XX_NUCLEO)
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC12);
#elif defined(USE_STM32G0XX_NUCLEO)
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC);
#else
#error "missing value definition for your your board"
#endif /* USE_STM32G4XX_NUCLEO */

  /* VISENSE_ADC_INSTANCE DMA Init */
  LL_DMA_SetDataTransferDirection(VISENSE_DMA_INSTANCE, VISENSE_DMA_CHANNEL, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
  LL_DMA_SetChannelPriorityLevel(VISENSE_DMA_INSTANCE, VISENSE_DMA_CHANNEL, LL_DMA_PRIORITY_LOW);
  LL_DMA_SetMode(VISENSE_DMA_INSTANCE, VISENSE_DMA_CHANNEL, LL_DMA_MODE_CIRCULAR);
  LL_DMA_SetPeriphIncMode(VISENSE_DMA_INSTANCE, VISENSE_DMA_CHANNEL, LL_DMA_PERIPH_NOINCREMENT);
  LL_DMA_SetMemoryIncMode(VISENSE_DMA_INSTANCE, VISENSE_DMA_CHANNEL, LL_DMA_MEMORY_INCREMENT);
  LL_DMA_SetPeriphSize(VISENSE_DMA_INSTANCE, VISENSE_DMA_CHANNEL, LL_DMA_PDATAALIGN_HALFWORD);
  LL_DMA_SetMemorySize(VISENSE_DMA_INSTANCE, VISENSE_DMA_CHANNEL, LL_DMA_MDATAALIGN_HALFWORD);

#if defined(USE_STM32G4XX_NUCLEO)
  LL_DMA_SetPeriphRequest(VISENSE_DMA_INSTANCE, VISENSE_DMA_CHANNEL, VISENSE_DMA_REQ);
#elif defined(USE_STM32G0XX_NUCLEO)
  LL_DMAMUX_SetRequestID(DMAMUX1, VISENSE_DMAMUX_CHANNEL, VISENSE_DMA_REQ);
#endif /* USE_STM32G4XX_NUCLEO */

  /* Set DMA transfer addresses of source and destination */
  LL_DMA_ConfigAddresses(VISENSE_DMA_INSTANCE,
                         VISENSE_DMA_CHANNEL,
                         LL_ADC_DMA_GetRegAddr(VISENSE_ADC_INSTANCE, LL_ADC_DMA_REG_REGULAR_DATA),
                         (uint32_t)usbpd_pwr_adcx_buff,
                         LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  /* Set DMA transfer size */
  LL_DMA_SetDataLength(VISENSE_DMA_INSTANCE, VISENSE_DMA_CHANNEL, VISENSE_ADC_BUFFER_SIZE);

  /* Activation of DMA ---------------------------------------------------------*/
  /* Enable the DMA transfer */
  LL_DMA_EnableChannel(VISENSE_DMA_INSTANCE, VISENSE_DMA_CHANNEL);

  /** Common config
  */
#if defined(USE_STM32G0XX_NUCLEO)
  LL_ADC_REG_SetSequencerConfigurable(VISENSE_ADC_INSTANCE, LL_ADC_REG_SEQ_FIXED);
  ADC_InitStruct.Clock = LL_ADC_CLOCK_SYNC_PCLK_DIV4;
#endif /* USE_STM32G0XX_NUCLEO */
  ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
  ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
  LL_ADC_Init(VISENSE_ADC_INSTANCE, &ADC_InitStruct);

  ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
  ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_ENABLE_2RANKS;
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
  ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
  ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
  LL_ADC_REG_Init(VISENSE_ADC_INSTANCE, &ADC_REG_InitStruct);
#if defined(USE_STM32G4XX_NUCLEO)
  LL_ADC_SetGainCompensation(VISENSE_ADC_INSTANCE, 0U);
#elif defined(USE_STM32G0XX_NUCLEO)
  LL_ADC_REG_SetSequencerScanDirection(VISENSE_ADC_INSTANCE, LL_ADC_REG_SEQ_SCAN_DIR_FORWARD);
  LL_ADC_SetTriggerFrequencyMode(VISENSE_ADC_INSTANCE, LL_ADC_CLOCK_FREQ_MODE_HIGH);
  LL_ADC_REG_SetSequencerChAdd(VISENSE_ADC_INSTANCE, VSENSE_ADC_CHANNEL | ISENSE_ADC_CHANNEL);
#endif /* USE_STM32G4XX_NUCLEO */
  LL_ADC_SetOverSamplingScope(VISENSE_ADC_INSTANCE, LL_ADC_OVS_DISABLE);
#if defined(USE_STM32G4XX_NUCLEO)
  ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV4;
  ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
  LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(VISENSE_ADC_INSTANCE), &ADC_CommonInitStruct);
#elif defined(USE_STM32G0XX_NUCLEO)
  /* Poll for ADC channel configuration ready */
#if (USE_TIMEOUT == 1)
  Timeout = ADC_CHANNEL_CONF_RDY_TIMEOUT_MS;
#endif /* USE_TIMEOUT */
  while (LL_ADC_IsActiveFlag_CCRDY(VISENSE_ADC_INSTANCE) == 0U)
  {
#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag())
    {
      if (Timeout-- == 0U)
      {
        Error_Handler();
      }
    }
#endif /* USE_TIMEOUT */
  }
  /* Clear flag ADC channel configuration ready */
  LL_ADC_ClearFlag_CCRDY(VISENSE_ADC_INSTANCE);
  LL_ADC_SetSamplingTimeCommonChannels(VISENSE_ADC_INSTANCE, LL_ADC_SAMPLINGTIME_COMMON_1,
                                       LL_ADC_SAMPLINGTIME_160CYCLES_5);
  LL_ADC_DisableIT_EOC(VISENSE_ADC_INSTANCE);
  LL_ADC_DisableIT_EOS(VISENSE_ADC_INSTANCE);
#endif /* USE_STM32G4XX_NUCLEO */

#if defined(USE_STM32G4XX_NUCLEO)
  /* Disable ADC deep power down (enabled by default after reset state) */
  LL_ADC_DisableDeepPowerDown(VISENSE_ADC_INSTANCE);
#endif /* USE_STM32G4XX_NUCLEO */
  /* Enable ADC internal voltage regulator */
  LL_ADC_EnableInternalRegulator(VISENSE_ADC_INSTANCE);

  /* Delay for ADC internal voltage regulator stabilization. */
  /* Compute number of CPU cycles to wait for, from delay in us.            */
  /* Note: Variable divided by 2 to compensate partially                    */
  /*       CPU processing cycles (depends on compilation optimization).     */
  /* Note: If system core clock frequency is below 200kHz, wait time        */
  /*       is only a few CPU processing cycles.                             */
  uint32_t wait_loop_index;
  wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000u * 2U))) / 10U);
  while (wait_loop_index != 0U)
  {
    wait_loop_index--;
  }

  /** Configure Regular Channels
  */
#if defined(USE_STM32G4XX_NUCLEO)
  LL_ADC_REG_SetSequencerRanks(VISENSE_ADC_INSTANCE, VSENSE_ADC_RANK, VSENSE_ADC_CHANNEL);
  LL_ADC_SetChannelSamplingTime(VISENSE_ADC_INSTANCE, VSENSE_ADC_CHANNEL, LL_ADC_SAMPLINGTIME_247CYCLES_5);
  LL_ADC_SetChannelSingleDiff(VISENSE_ADC_INSTANCE, VSENSE_ADC_CHANNEL, LL_ADC_SINGLE_ENDED);

  LL_ADC_REG_SetSequencerRanks(VISENSE_ADC_INSTANCE, ISENSE_ADC_RANK, ISENSE_ADC_CHANNEL);
  LL_ADC_SetChannelSamplingTime(VISENSE_ADC_INSTANCE, ISENSE_ADC_CHANNEL, LL_ADC_SAMPLINGTIME_247CYCLES_5);
  LL_ADC_SetChannelSingleDiff(VISENSE_ADC_INSTANCE, ISENSE_ADC_CHANNEL, LL_ADC_SINGLE_ENDED);
#elif defined(USE_STM32G0XX_NUCLEO)
  LL_ADC_SetChannelSamplingTime(VISENSE_ADC_INSTANCE, VSENSE_ADC_CHANNEL, LL_ADC_SAMPLINGTIME_COMMON_1);
#endif /* USE_STM32G4XX_NUCLEO */
}

/**
  * @brief  Perform ADC activation procedure to make it ready to convert
  *         (ADC instance: VISENSE_ADC_INSTANCE).
  * @note   Operations:
  *         - ADC instance
  *           - Run ADC self calibration
  *           - Enable ADC
  *         - ADC group regular
  *           none: ADC conversion start-stop to be performed
  *                 after this function
  *         - ADC group injected
  *           Feature not available          (feature not available on this STM32 series)
  * @retval None
  */
static void PWR_TCPP0203_Activate_ADC(void)
{
  __IO uint32_t wait_loop_index = 0U;
  __IO uint32_t backup_setting_adc_dma_transfer;
#if (USE_TIMEOUT == 1)
  uint32_t Timeout = 0U; /* Variable used for timeout management */
#endif /* USE_TIMEOUT */

  /* Operation on ADC hierarchical scope: ADC instance -----------------------*/

  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 series, setting of these features is conditioned to  */
  /*       ADC state:                                                         */
  /*       ADC must be disabled.                                              */
  /* Note: In this example, all these checks are not necessary but are        */
  /*       implemented anyway to show the best practice usages                */
  /*       corresponding to reference manual procedure.                       */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
  if (0U == LL_ADC_IsEnabled(VISENSE_ADC_INSTANCE))
  {
#if defined(STM32G474xx)
    LL_ADC_DisableDeepPowerDown(VISENSE_ADC_INSTANCE);
#endif /* STM32G474xx */
    /* Enable ADC internal voltage regulator */
    LL_ADC_EnableInternalRegulator(VISENSE_ADC_INSTANCE);

    /* Delay for ADC internal voltage regulator stabilization.                */
    /* Compute number of CPU cycles to wait for, from delay in us.            */
    /* Note: Variable divided by 2 to compensate partially                    */
    /*       CPU processing cycles (depends on compilation optimization).     */
    /* Note: If system core clock frequency is below 200kHz, wait time        */
    /*       is only a few CPU processing cycles.                             */
    wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000u * 2u))) / 10u);
    while (0U != wait_loop_index)
    {
      wait_loop_index--;
    }

    /* Disable ADC DMA transfer request during calibration */
    /* Note: Specificity of this STM32 series: Calibration factor is           */
    /*       available in data register and also transferred by DMA.           */
    /*       To not insert ADC calibration factor among ADC conversion data   */
    /*       in DMA destination address, DMA transfer must be disabled during */
    /*       calibration.                                                     */
    backup_setting_adc_dma_transfer = LL_ADC_REG_GetDMATransfer(VISENSE_ADC_INSTANCE);
    LL_ADC_REG_SetDMATransfer(VISENSE_ADC_INSTANCE, LL_ADC_REG_DMA_TRANSFER_NONE);

#if !defined (STM32G474xx)
    /* Run ADC self calibration */
    LL_ADC_StartCalibration(VISENSE_ADC_INSTANCE);
#endif /* STM32G474xx */

    /* Poll for ADC effectively calibrated */
#if (USE_TIMEOUT == 1)
    Timeout = ADC_CALIBRATION_TIMEOUT_MS;
#endif /* USE_TIMEOUT */

    while (0U != LL_ADC_IsCalibrationOnGoing(VISENSE_ADC_INSTANCE))
    {
#if (USE_TIMEOUT == 1)
      /* Check Systick counter flag to decrement the time-out value */
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        if (0U == Timeout--)
        {
          /* Time-out occurred. Set LED to blinking mode */
          LED_Blinking(LED_BLINK_ERROR);
        }
      }
#endif /* USE_TIMEOUT */
    }

    /* Restore ADC DMA transfer request after calibration */
    LL_ADC_REG_SetDMATransfer(VISENSE_ADC_INSTANCE, backup_setting_adc_dma_transfer);

    /* Delay between ADC end of calibration and ADC enable.                   */
    /* Note: Variable divided by 2 to compensate partially                    */
    /*       CPU processing cycles (depends on compilation optimization).     */
    wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1U);
    while (0U != wait_loop_index)
    {
      wait_loop_index--;
    }

    /* Enable ADC */
    LL_ADC_Enable(VISENSE_ADC_INSTANCE);

    /* Poll for ADC ready to convert */
#if (USE_TIMEOUT == 1)
    Timeout = ADC_ENABLE_TIMEOUT_MS;
#endif /* USE_TIMEOUT */

    while (0U == LL_ADC_IsActiveFlag_ADRDY(VISENSE_ADC_INSTANCE))
    {
#if (USE_TIMEOUT == 1)
      /* Check Systick counter flag to decrement the time-out value */
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        if (0U == Timeout--)
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

  /* Operation on ADC hierarchical scope: ADC group regular ------------------*/
  /* Note: No operation on ADC group regular performed here.                  */
  /*       ADC group regular conversions to be performed after this function  */
  /*       using function:                                                    */
  /*       "LL_ADC_REG_StartConversion();"                                    */

  /* Operation on ADC hierarchical scope: ADC group injected -----------------*/
  /* Note: Feature not available on this STM32 series */
}


/**
  * @brief  Configure TCPP0203 low level interrupt.
  * @note   Corresponds to EXTI mapped onto FLGn pin of TCPP0203
  * @param  PortNum   Port number
  * @retval None
  */
static void PWR_TCPP0203_ITConfigInit(uint32_t PortNum)
{
  if (USBPD_PWR_Port_Status[PortNum].IsItEnabled == 0U)
  {
    USBPD_PWR_Port_Status[PortNum].IsItEnabled = 1U;

    /* Enable the GPIO EXTI clock */
    TCPP0203_PORT0_FLG_GPIO_CLK_ENABLE();
    /* Configure IO */
    LL_GPIO_SetPinMode(TCPP0203_PORT0_FLG_GPIO_PORT, TCPP0203_PORT0_FLG_GPIO_PIN, TCPP0203_PORT0_FLG_GPIO_MODE);
    LL_GPIO_SetPinPull(TCPP0203_PORT0_FLG_GPIO_PORT, TCPP0203_PORT0_FLG_GPIO_PIN, TCPP0203_PORT0_FLG_GPIO_PUPD);
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* Connect External Line to the GPIO*/
    TCPP0203_PORT0_FLG_SET_EXTI();

    /* Enable a falling trigger EXTI line x Interrupt */
    TCPP0203_PORT0_FLG_EXTI_ENABLE();
    TCPP0203_PORT0_FLG_TRIG_ENABLE();

    /* Configure NVIC for EXTIx_IRQn */
    NVIC_SetPriority(TCPP0203_PORT0_FLG_EXTI_IRQN, TCPP0203_PORT0_FLG_IT_PRIORITY);
    NVIC_EnableIRQ(TCPP0203_PORT0_FLG_EXTI_IRQN);
  }
}
#endif /* !USBPD_CONFIG_MX */

/**
  * @brief  I2C BUS registration for TCPP0203 communication
  * @param  PortNum   Port number
  * @param  Address   I2C Address
  * @retval BSP status
  */
static int32_t PWR_TCPP0203_BUSConfigInit(uint32_t PortNum, uint16_t Address)
{
  int32_t                  ret = BSP_ERROR_NONE;
  TCPP0203_IO_t            TCPP0203IOCtx;
  uint8_t                  tcpp_type;

  /* Configure the TCPP0203 I2C driver */
  TCPP0203IOCtx.Address     = Address;
  TCPP0203IOCtx.Init        = BSP_I2C_Init;
  TCPP0203IOCtx.DeInit      = BSP_I2C_DeInit;
  TCPP0203IOCtx.ReadReg     = BSP_I2C_ReadReg;
  TCPP0203IOCtx.WriteReg    = BSP_I2C_WriteReg;
  TCPP0203IOCtx.GetTick     = BSP_GetTick;

  /* Register the component on BUS IO */
  if (TCPP0203_RegisterBusIO(&USBPD_PWR_PortCompObj[PortNum], &TCPP0203IOCtx) != TCPP0203_OK)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  else
  {
    /* Initialisation step */
    USBPD_PWR_PortCompDrv[PortNum] = &TCPP0203_Driver;
    if (USBPD_PWR_PortCompDrv[PortNum]->Init(&USBPD_PWR_PortCompObj[PortNum]) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }

    /* Read the component type */
    if (USBPD_PWR_PortCompDrv[PortNum]->ReadTCPPType(&USBPD_PWR_PortCompObj[PortNum], &tcpp_type) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    /* Check the component type */
    else if ((tcpp_type != TCPP0203_DEVICE_TYPE_02)
             && (tcpp_type != TCPP0203_DEVICE_TYPE_03))
    {
      ret = BSP_ERROR_UNKNOWN_COMPONENT;
    }
    /* Check the component type is same as declared in config */
    else if (((USBPD_PWR_HW_CONFIG_TYPE_TCPP02 == USBPD_PWR_Port_Configs[PortNum].Type)
              && (tcpp_type != TCPP0203_DEVICE_TYPE_02))
             || ((USBPD_PWR_HW_CONFIG_TYPE_TCPP03 == USBPD_PWR_Port_Configs[PortNum].Type)
                 && (tcpp_type != TCPP0203_DEVICE_TYPE_03)))
    {
      ret = BSP_ERROR_UNKNOWN_COMPONENT;
    }
    /* Reset TCPP0203 Reg0 register */
    else if (USBPD_PWR_PortCompDrv[PortNum]->Reset(&USBPD_PWR_PortCompObj[PortNum]) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Registration completed */
    }
  }
  return ret;
}

/**
  * @brief  De-Initialize TCPP0203 HW resources associated to Type-C port
  * @param  PortNum   Port number
  * @retval BSP Status
  */
static int32_t PWR_TCPP0203_ConfigDeInit(uint32_t PortNum)
{
  UNUSED(PortNum);
  return (BSP_ERROR_NONE);
}

/**
  * @brief  Handle Event callback
  * @param  PortNum   Port number
  * @retval None
  */
static void PWR_TCPP0203_EventCallback(uint32_t PortNum)
{
  uint8_t flg_reg;

  /* In case of TCPP0203, this callback could be called when an event is detected
     on FLGn line */
  if (USBPD_PWR_Port_Status[PortNum].IsInitialized == 1U)
  {
    /* Read Flags register (FLGn) */
    if (USBPD_PWR_PortCompDrv[PortNum]->ReadFlagRegister(&USBPD_PWR_PortCompObj[PortNum], &flg_reg) == TCPP0203_OK)
    {
#ifdef _TRACE
      char _str[13];
      (void)sprintf(_str, "Reg2_0x%02x", flg_reg);
      BSP_USBPD_PWR_TRACE(PortNum, _str);
#endif /* _TRACE */

      /* If FLGn has been set to 0 in LOW POWER or HIBERNATE mode,
         it is expected that a VBUS_OK condition is detected.
         In this case, if any, vBus Detection callback could be called */
      switch (USBPD_PWR_Port_Status[PortNum].PwrSavingMode)
      {
        case USBPD_PWR_MODE_HIBERNATE:
        {
          if ((flg_reg & TCPP0203_FLAG_VBUS_OK_SET) == TCPP0203_FLAG_VBUS_OK_SET)
          {
            if (USBPD_PWR_Port_Status[PortNum].VBUSDetectCallback != NULL)
            {
              /* In case of TCPP03 in Hibernate Mode (SNK config),
                 Mode is set to Low power to enable TCPP03 behavior on CC lines */
              /* Switch to Low Power mode */
              (void)BSP_USBPD_PWR_SetPowerMode(PortNum, USBPD_PWR_MODE_LOWPOWER);

              USBPD_PWR_Port_Status[PortNum].VBUSDetectCallback(PortNum, VBUS_CONNECTED);
            }
          }
          else
          {
            /* In all other cases (NORMAL mode), FLGn signals fault detection events. */
            BSP_USBPD_PWR_TRACE(PortNum, "-- Wrong detection --");
          }
          break;
        }

        case USBPD_PWR_MODE_LOWPOWER:
        {
          if ((flg_reg & TCPP0203_FLAG_VBUS_OK_SET) == TCPP0203_FLAG_VBUS_OK_SET)
          {
            if (USBPD_PWR_Port_Status[PortNum].VBUSDetectCallback != NULL)
            {
              USBPD_PWR_Port_Status[PortNum].VBUSDetectCallback(PortNum, VBUS_CONNECTED);
            }
          }
          else
          {
            /* In all other cases (NORMAL mode), FLGn signals fault detection events. */
            BSP_USBPD_PWR_TRACE(PortNum, "-- Wrong detection --");
          }
          break;
        }

        case USBPD_PWR_MODE_NORMAL:
        {
          uint32_t tickfault = HAL_GetTick();
          uint8_t recoveryword;

          /* In all other cases (NORMAL mode), FLGn signals fault detection events. */
          /* Check type of error raised in FLGn */
          if ((flg_reg & TCPP0203_FLAG_OCP_VCONN_SET) == TCPP0203_FLAG_OCP_VCONN_SET)
          {
            BSP_USBPD_PWR_TRACE(PortNum, "-- OCP VCONN detected --");
          }
          if ((flg_reg & TCPP0203_FLAG_OCP_VBUS_SET) == TCPP0203_FLAG_OCP_VBUS_SET)
          {
            BSP_USBPD_PWR_TRACE(PortNum, "-- OCP VBUS detected --");

            /* Check if detected fault could be recovered */
            /* In case of OCP fault, allow to restart and check if a 2nd OCP fault occurs in a limited time */
            if ((tickfault > USBPD_PWR_Port_Status[PortNum].LastFaultTick)
                && ((tickfault - USBPD_PWR_Port_Status[PortNum].LastFaultTick) > USBPD_PWR_FAULT_MIN_TIME_RECOVERY))
            {
              /* Send Recovery word to TCPP0203 :
                 GDC and GDP open (TCPP0203_GD_PROVIDER_SWITCH_OPEN is 0) */
              BSP_USBPD_PWR_TRACE(PortNum, "-- Send Recovery Word --");
              recoveryword = TCPP0203_GD_CONSUMER_SWITCH_OPEN | TCPP0203_POWER_MODE_NORMAL;
              (void)TCPP0203_WriteCtrlRegister(&USBPD_PWR_PortCompObj[PortNum], &recoveryword);

              /* In case PWR Role is SRC, try to restore VBUS as soon as possible */
              if (USBPD_PWR_Port_Status[PortNum].PwrRole == POWER_ROLE_SOURCE)
              {
                (void)TCPP0203_SetPowerMode(&USBPD_PWR_PortCompObj[PortNum], TCPP0203_POWER_MODE_NORMAL);
                (void)TCPP0203_SetGateDriverProvider(&USBPD_PWR_PortCompObj[PortNum],
                                                     TCPP0203_GD_PROVIDER_SWITCH_CLOSED);
                BSP_USBPD_PWR_TRACE(PortNum, "-- GDP/GDC setting : SRC (Restored) --");
              }
              else
              {
                if (USBPD_PWR_Port_Status[PortNum].VBUSDetectCallback != NULL)
                {
                  /* Notify Error thanks to callback */
                  USBPD_PWR_Port_Status[PortNum].VBUSDetectCallback(PortNum, VBUS_NOT_CONNECTED);
                }
              }
            }
            USBPD_PWR_Port_Status[PortNum].LastFaultTick = tickfault;
          }
          if ((flg_reg & TCPP0203_FLAG_OVP_VBUS_SET) == TCPP0203_FLAG_OVP_VBUS_SET)
          {
            BSP_USBPD_PWR_TRACE(PortNum, "-- OVP VBUS detected --");
          }
          if ((flg_reg & TCPP0203_FLAG_OTP_SET) == TCPP0203_FLAG_OTP_SET)
          {
            BSP_USBPD_PWR_TRACE(PortNum, "-- Over Temp detected --");
          }
          if ((flg_reg & TCPP0203_FLAG_OVP_CC_SET) == TCPP0203_FLAG_OVP_CC_SET)
          {
            BSP_USBPD_PWR_TRACE(PortNum, "-- OVP CC detected --");
          }
          break;
        }

        default:
        {
          /* Unexpected port state */
          BSP_USBPD_PWR_TRACE(PortNum, "-- Unexpected State for FLGn event --");
          break;
        }
      }
    }
  }
}

/**
  * @brief  Calculate the VBUS voltage level corresponding to ADC raw converted data.
  * @note   Voltage level is measured though a voltage divider
  *  Example :
  *  Voltage -- Ra ----.-   ^
  *                    |    |
  *                    Rb   | vadc = VBUS * Rb /(Ra + Rb)
  *                    |    |
  *                   GND   |
  * vadc = raw_data * (ADC_FULL_SCALE / VDD)
  * @param  ADCData  ADC raw converted data (resolution 12 bits)
  * @param  Ra       value of Ra resistance
  * @param  Rb       value of Rb resistance
  * @retval analog voltage (unit: mV)
  */
static uint32_t PWR_TCPP0203_ConvertADCDataToVoltage(uint32_t ADCData, uint32_t Ra, uint32_t Rb)
{
  uint32_t voltage;
  uint32_t vadc;

  /* Avoid dividing by zero */
  if (Rb == 0u)
  {
    voltage = 0u;
  }
  else
  {
    vadc = (ADCData * VDD_VALUE) / ADC_FULL_SCALE;
    voltage = vadc * (Ra + Rb) / Rb;
  }

  return voltage;
}

/**
  * @brief  Calculate the VBUS current level corresponding to ADC raw converted data.
  * @note   ADC measurement provides measurement on IANA pin.
  * @param  ADCData  ADC raw converted data (resolution 12 bits)
  * @param  Ga       value of TCPP0X Iana gain in V/V
  * @param  Rs       value of shunt resistor in milliohm
  * @retval VBUS analog current (unit: mA)
  */
static int32_t PWR_TCPP0203_ConvertADCDataToCurrent(uint32_t ADCData, uint32_t Ga, uint32_t Rs)
{
  int32_t current;
  uint32_t vadc;

  /* Avoid dividing by zero */
  if ((Ga == 0u) || (Rs == 0u))
  {
    current = 0u;
  }
  else
  {
    vadc = (ADCData * VDD_VALUE) / ADC_FULL_SCALE;
    current = (int32_t)((vadc * 1000u) / (Ga * Rs));
  }

  return current;
}

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

