/**
  ******************************************************************************
  * @file    custom_board_usbpd_pwr.c
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
#include "custom_board_usbpd_pwr.h"
#include "app_tcpp.h"

#if  defined(_TRACE)
#include "usbpd_core.h"
#include "usbpd_trace.h"
#include "string.h"
#ifndef _STDIO
#include "stdio.h"
#endif /* _STDIO */
#endif /* _TRACE */

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup CUSTOM_BOARD
  * @{
  */

/** @addtogroup CUSTOM_BOARD_USBPD_PWR
  * @{
  */

/** @defgroup CUSTOM_BOARD_USBPD_PWR_Exported_Variables Exported Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup CUSTOM_BOARD_USBPD_PWR_Private_Typedef Private Typedef
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

/* USER CODE BEGIN Typedef */

/* USER CODE END Typedef */

/**
  * @}
  */

/** @defgroup CUSTOM_BOARD_USBPD_PWR_Private_Constants Private Constants
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
#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES  (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32U)
#define ABS(__VAL__) (((int32_t)(__VAL__)) < 0 ? - ((int32_t)(__VAL__)) : ((int32_t)(__VAL__)))

/* Minimum time between 2 detected faults allowing to consider that
   fault could be due to a false detection (could be recovered).
   => if 2 faults occurred in less than that duration, no recovery will be executed */
#define USBPD_PWR_FAULT_MIN_TIME_RECOVERY             (1000U)             /* 1s */

/* USER CODE BEGIN constants */

/* USER CODE END constants */

/**
  * @}
  */

/** @defgroup CUSTOM_BOARD_USBPD_PWR_Private_Macros Private Macros
  * @{
  */
#ifdef _TRACE
#define BSP_USBPD_PWR_TRACE(_PORT_,_MSG_) USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (uint8_t)_PORT_, 0U , (uint8_t *)_MSG_,\
                                                          sizeof(_MSG_) - 1U);
#else
#define BSP_USBPD_PWR_TRACE(_PORT_,_MSG_)
#endif /* _TRACE */

/* USER CODE BEGIN macro */

/* USER CODE END macro */

/**
  * @}
  */

/** @defgroup CUSTOM_BOARD_USBPD_PWR_Private_Functions Private Functions
  * @{
  */
static void     PWR_TCPP0203_Activate_ADC(void);
static int32_t  PWR_TCPP0203_BUSConfigInit(uint32_t PortNum, uint16_t Address);
static int32_t  PWR_TCPP0203_ConfigDeInit(uint32_t PortNum);
static void     PWR_TCPP0203_EventCallback(uint32_t PortNum);

static uint32_t PWR_TCPP0203_ConvertADCDataToVoltage(uint32_t ADCData, uint32_t Ra, uint32_t Rb);
#if !defined(ADC_VBUS_ONLY)
static int32_t  PWR_TCPP0203_ConvertADCDataToCurrent(uint32_t ADCData, uint32_t Ga, uint32_t Rs);
#endif

/* USER CODE BEGIN functions */

/* USER CODE END functions */
/**
  * @}
  */

/** @defgroup CUSTOM_BOARD_USBPD_PWR_Private_Variables Private Variables
  * @{
  */
uint16_t usbpd_pwr_adcx_buff[VISENSE_ADC_BUFFER_SIZE]; /* Global ADC buffer to be filled by DMA */

static USBPD_PWR_PortConfig_t USBPD_PWR_Port_Configs[USBPD_PWR_INSTANCES_NBR] =
{
  {
    USBPD_PWR_HW_CONFIG_TYPE_TCPP03,             /* Port 0 : TCPP Type - default, will be read  */
    TCPP0203_I2C_ADDRESS_X68,                    /* TCPP0x on shield (Address 0x34)             */
  },
};

static USBPD_PWR_PortStatus_t USBPD_PWR_Port_Status[USBPD_PWR_INSTANCES_NBR] =
{
  {0U, 0U, POWER_ROLE_SINK, USBPD_PWR_MODE_OFF, 0U, NULL},
};

TCPP0203_Object_t      USBPD_PWR_PortCompObj[USBPD_PWR_INSTANCES_NBR] = { 0 };
TCPP0203_Drv_t        *USBPD_PWR_PortCompDrv[USBPD_PWR_INSTANCES_NBR] = { NULL };

/* USER CODE BEGIN variables */

/* USER CODE END variables */

/**
  * @}
  */

/** @addtogroup CUSTOM_BOARD_USBPD_PWR_Exported_Functions
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

  /* USER CODE BEGIN BSP_USBPD_PWR_Init */
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
        case USBPD_PWR_HW_CONFIG_TYPE_TCPP03:
          /* Set default Role to SNK */
          USBPD_PWR_Port_Status[PortNum].PwrRole = POWER_ROLE_SINK;

          /* Set default Power Mode to Hibernate */
          USBPD_PWR_Port_Status[PortNum].PwrSavingMode = USBPD_PWR_MODE_HIBERNATE;

          /* Reset port callback for VBUS detection event */
          USBPD_PWR_Port_Status[PortNum].VBUSDetectCallback = NULL;

          /* Reset last detected fault Tick */
          USBPD_PWR_Port_Status[PortNum].LastFaultTick = 0;

          /* Enable component */
          TCPP0203_PORT0_ENABLE_GPIO_SET();

          /* Initialize required BUS for communication */
          ret = PWR_TCPP0203_BUSConfigInit(PortNum, USBPD_PWR_Port_Configs[PortNum].Address);
          break;

        case USBPD_PWR_HW_CONFIG_TYPE_DEFAULT:
        case USBPD_PWR_HW_CONFIG_TYPE_TCPP01:
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
  /* USER CODE END BSP_USBPD_PWR_Init */
	
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

  /* USER CODE BEGIN BSP_USBPD_PWR_Deinit */
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
      case USBPD_PWR_HW_CONFIG_TYPE_TCPP03:
        /* Perform any treatments required for de-initialization for GPIO, BUS communication, Interrupts */
        ret = PWR_TCPP0203_ConfigDeInit(PortNum);
        break;

      case USBPD_PWR_HW_CONFIG_TYPE_DEFAULT:
      case USBPD_PWR_HW_CONFIG_TYPE_TCPP01:
      default:
        ret = BSP_ERROR_WRONG_PARAM;
        break;
    }

    /* Reset Initialisation flag */
    USBPD_PWR_Port_Status[PortNum].IsInitialized = 0U;
  }
  /* USER CODE END BSP_USBPD_PWR_Deinit */
  
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
  
  /* USER CODE BEGIN BSP_USBPD_PWR_SetRole */
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
        /* Port Role is now SNK */
        BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_SetRole : SNK --");
        /* Set port power Role */
        USBPD_PWR_Port_Status[PortNum].PwrRole = Role;
        break;

      default:
        ret = BSP_ERROR_WRONG_PARAM;
        break;
    }
  }
  /* USER CODE END BSP_USBPD_PWR_SetRole */
  
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
  int32_t ret = BSP_ERROR_NONE;
  
  /* USER CODE BEGIN BSP_USBPD_PWR_SetPowerMode */
  uint8_t flg_reg;

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
        /* Port Mode is Hibernate : (Default state at startup) suitable for SNK role
             - Dead Batteries Enabled,
             - Gate Driver Consumer controlled by I2C, Gate Driver Provider OFF
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
                 Mode is set to Low power to enable TCPP03 behavior on CC lines */
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
        /* Port Mode is Low Power : suitable while toggling SNK/SRC is case of DRP
             - Dead Batteries Disabled,
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
  /* USER CODE END BSP_USBPD_PWR_SetPowerMode */
  
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
  
  /* USER CODE BEGIN BSP_USBPD_PWR_GetPowerMode */
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
  /* USER CODE END BSP_USBPD_PWR_GetPowerMode */ 
  
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

  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSInit */
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
      case USBPD_PWR_HW_CONFIG_TYPE_TCPP03:
        /* Switch to Normal mode */
        ret = BSP_USBPD_PWR_SetPowerMode(PortNum, USBPD_PWR_MODE_NORMAL);

        PWR_TCPP0203_Activate_ADC();

        /*  Start Conversion */
        LL_ADC_REG_StartConversion(VISENSE_ADC_INSTANCE);
        break;

      case USBPD_PWR_HW_CONFIG_TYPE_DEFAULT:
      case USBPD_PWR_HW_CONFIG_TYPE_TCPP01:
      default:
        ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
        break;
    }
  }
  /* USER CODE END BSP_USBPD_PWR_VBUSInit */
  
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
  
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSDeInit */
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
      /* Only for TCPP03 */
      if (USBPD_PWR_HW_CONFIG_TYPE_TCPP03 == USBPD_PWR_Port_Configs[PortNum].Type)
      {
        /* Open Gate Driver Consumer, only for TCPP03 */
        if (USBPD_PWR_PortCompDrv[PortNum]->SetGateDriverConsumer(&USBPD_PWR_PortCompObj[PortNum],
                                                                  TCPP0203_GD_CONSUMER_SWITCH_OPEN) != TCPP0203_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
          return ret;
        }
      }

      /* Common for TCPP02 / TCPP03 */
      if ((USBPD_PWR_HW_CONFIG_TYPE_TCPP02 == USBPD_PWR_Port_Configs[PortNum].Type) ||
          (USBPD_PWR_HW_CONFIG_TYPE_TCPP03 == USBPD_PWR_Port_Configs[PortNum].Type))
      {
        HAL_Delay(2);

        if (USBPD_PWR_PortCompDrv[PortNum]->SetGateDriverProvider(&USBPD_PWR_PortCompObj[PortNum],
                                                                  TCPP0203_GD_PROVIDER_SWITCH_CLOSED) != TCPP0203_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        BSP_USBPD_PWR_TRACE(PortNum, "-- GDP/GDC setting : SRC --");
      }
      else
      {
        ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
      }
    }
    else
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  /* USER CODE END BSP_USBPD_PWR_VBUSDeInit */
  
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
  
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSOff */
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
  /* USER CODE END BSP_USBPD_PWR_VBUSOff */
  
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
  
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSIsOn */
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
  /* USER CODE END BSP_USBPD_PWR_VBUSIsOn */
  
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
  int32_t ret;

  /* USER CODE BEGIN BSP_USBPD_PWR_SetVBUSDisconnectionThreshold */
  UNUSED(VoltageThreshold);

  /* Check if instance is valid */
  if (PortNum >= USBPD_PWR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  /* USER CODE END BSP_USBPD_PWR_SetVBUSDisconnectionThreshold */
  
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

  /* USER CODE BEGIN BSP_USBPD_PWR_RegisterVBUSDetectCallback */
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
  /* USER CODE END BSP_USBPD_PWR_RegisterVBUSDetectCallback */

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

  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSGetVoltage */
  /* Check if instance is valid */
  if ((PortNum >= USBPD_PWR_INSTANCES_NBR) || (NULL == pVoltage))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    uint32_t voltage;
    static __IO uint16_t adc_value;
#if defined (ADC_VBUS_ONLY)
	adc_value = LL_ADC_REG_ReadConversionData12(VISENSE_ADC_INSTANCE); 
#else
    adc_value = (uint16_t) usbpd_pwr_adcx_buff[ADCBUF_VSENSE];
#endif
    voltage = PWR_TCPP0203_ConvertADCDataToVoltage(adc_value, USBPD_PWR_VSENSE_RA, USBPD_PWR_VSENSE_RB);

    *pVoltage = voltage;
  }
  /* USER CODE END BSP_USBPD_PWR_VBUSGetVoltage */
  
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

  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSGetCurrent */
  /* Check if instance is valid */
  if ((PortNum >= USBPD_PWR_INSTANCES_NBR) || (NULL == pCurrent))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    int32_t current;
#if defined (ADC_VBUS_ONLY)
	current=0;
#else
    uint16_t adc_value;
    adc_value = (uint16_t) usbpd_pwr_adcx_buff[ADCBUF_ISENSE];
    current = PWR_TCPP0203_ConvertADCDataToCurrent(adc_value, USBPD_PWR_ISENSE_GA, USBPD_PWR_ISENSE_RS);
#endif

    *pCurrent = current;
  }
  /* USER CODE END BSP_USBPD_PWR_VBUSGetCurrent */
  
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
  
  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSDischargeOn */
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
  /* USER CODE END BSP_USBPD_PWR_VBUSDischargeOn */
  
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

  /* USER CODE BEGIN BSP_USBPD_PWR_VBUSDischargeOff */
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
  /* USER CODE END BSP_USBPD_PWR_VBUSDischargeOff */
  
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
  /* USER CODE BEGIN BSP_USBPD_PWR_EventCallback */
  BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_EventCallback --");

  /* Handle USBPD PWR event */
  switch (USBPD_PWR_Port_Configs[PortNum].Type)
  {
    case USBPD_PWR_HW_CONFIG_TYPE_TCPP02:
    case USBPD_PWR_HW_CONFIG_TYPE_TCPP03:
      /* In case of TCPP0203, this callback could be called when an event is detected
         on FLGn line */
      PWR_TCPP0203_EventCallback(PortNum);
      break;

    case USBPD_PWR_HW_CONFIG_TYPE_DEFAULT:
    case USBPD_PWR_HW_CONFIG_TYPE_TCPP01:
    default:
      break;
  }
  /* USER CODE END BSP_USBPD_PWR_EventCallback */
}

/**
  * @}
  */

/** @addtogroup DRP1M1_USBPD_PWR_Private_Functions
  * @{
  */

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
  /* Defined in ADC.c */
  ADC_Start();
}

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
  TCPP0203IOCtx.Init        = TCPP0203_I2C_Init;
  TCPP0203IOCtx.DeInit      = TCPP0203_I2C_DeInit;
  TCPP0203IOCtx.ReadReg     = TCPP0203_I2C_ReadReg;
  TCPP0203IOCtx.WriteReg    = TCPP0203_I2C_WriteReg;
  TCPP0203IOCtx.GetTick     = TCPP0203_GetTick;

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
    /* Reset TCPP0203 Reg0 register */
    else if (USBPD_PWR_PortCompDrv[PortNum]->Reset(&USBPD_PWR_PortCompObj[PortNum]) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Check the component type and update config accordingly */
      USBPD_PWR_Port_Configs[PortNum].Type = tcpp_type;

      /* Update Port Config */
      if (TCPP0203_DEVICE_TYPE_02 == tcpp_type)
      {
        USBPD_PWR_Port_Configs[PortNum].Type = USBPD_PWR_HW_CONFIG_TYPE_TCPP02;
        BSP_USBPD_PWR_TRACE(PortNum, "-- TCPP02 detected");
      }
      else if (TCPP0203_DEVICE_TYPE_03 == tcpp_type)
      {
        USBPD_PWR_Port_Configs[PortNum].Type = USBPD_PWR_HW_CONFIG_TYPE_TCPP03;
        BSP_USBPD_PWR_TRACE(PortNum, "-- TCPP03 detected");
      }

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

#if !defined(ADC_VBUS_ONLY)
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
#endif

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

