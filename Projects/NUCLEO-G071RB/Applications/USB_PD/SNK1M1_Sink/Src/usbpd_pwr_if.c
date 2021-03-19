/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_pwr_if.c
  * @author  MCD Application Team
  * @brief   This file contains power interface control functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#define __USBPD_PWR_IF_C

/* Includes ------------------------------------------------------------------*/
#include "usbpd_pwr_if.h"
#include "usbpd_hw_if.h"
#include "usbpd_dpm_core.h"
#include "usbpd_dpm_conf.h"
#include "usbpd_pdo_defs.h"
#include "usbpd_core.h"
#if defined(_TRACE)
#include "usbpd_trace.h"
#endif /* _TRACE */
#include "string.h"
#include "gui_api.h"
/* USER CODE BEGIN Include */
#if defined(_RTOS)
#if (osCMSIS >= 0x20000U)
#include "task.h"
#endif /* osCMSIS >= 0x20000U */
#endif /* _RTOS */
/* USER CODE END Include */

/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_TypeDef
  * @{
  */
/* USER CODE BEGIN Private_Typedef */

/* USER CODE END Private_Typedef */
/**
  * @}
  */

/* Private define ------------------------------------------------------------*/
/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Defines
  * @{
  */
/* USER CODE BEGIN Private_Define */

/* USER CODE END Private_Define */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Macros
  * @{
  */
#if defined(_TRACE)
#define PWR_IF_DEBUG_TRACE(_PORT_, __MESSAGE__)  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (_PORT_), 0u, (uint8_t*)(__MESSAGE__), sizeof(__MESSAGE__) - 1u)
#else
#define PWR_IF_DEBUG_TRACE(_PORT_, __MESSAGE__)
#endif /* _TRACE */
/* USER CODE BEGIN Private_Macro */
#define _PWR_UPDATE_VOLTAGE_MIN(_PDO_VOLT_, _SNK_VOLT_) \
            if ((_PDO_VOLT_) < (_SNK_VOLT_)) \
            { \
              /* Update min voltage */ \
              (_SNK_VOLT_) = (_PDO_VOLT_); \
            }
#define _PWR_UPDATE_VOLTAGE_MAX(_PDO_VOLT_, _SNK_VOLT_) \
            if ((_PDO_VOLT_) > (_SNK_VOLT_)) \
            { \
              /* Update min voltage */ \
              (_SNK_VOLT_) = (_PDO_VOLT_); \
            }
#define _PWR_UPDATE_CURRENT_MAX(_PDO_CURRENT_, _SNK_CURRENT_) \
            if ((_PDO_CURRENT_) > (_SNK_CURRENT_)) \
            { \
              /* Update min current */ \
              (_SNK_CURRENT_) = (_PDO_CURRENT_); \
            }
#define _PWR_UPDATE_POWER_MAX(_PDO_POWER_, _SNK_POWER_) \
            if ((_PDO_POWER_) > (_SNK_POWER_)) \
            { \
              /* Update min POWER */ \
              (_SNK_POWER_) = (_PDO_POWER_); \
            }
#if defined(_GUI_INTERFACE)
#define _PWR_CHECK_VOLTAGE_MIN(_PDO_VOLT_, _SNK_VOLT_) \
            /* Update min voltage */ \
            (_SNK_VOLT_) = (_PDO_VOLT_);
#define _PWR_CHECK_VOLTAGE_MAX(_PDO_VOLT_, _SNK_VOLT_) \
            /* Update min voltage */ \
            (_SNK_VOLT_) = (_PDO_VOLT_);
#define _PWR_CHECK_CURRENT_MAX(_PDO_CURRENT_, _SNK_CURRENT_) \
            /* Update min current */ \
            (_SNK_CURRENT_) = (_PDO_CURRENT_);
#define _PWR_CHECK_POWER_MAX(_PDO_POWER_, _SNK_POWER_) \
            /* Update min POWER */ \
            (_SNK_POWER_) = (_PDO_POWER_);
#else
#define _PWR_CHECK_VOLTAGE_MIN(_PDO_VOLT_, _SNK_VOLT_) \
          if ((_PDO_VOLT_) != (_SNK_VOLT_)) \
          { \
            /* Disalignment between PDO and DPM_SNKRequestedPower structure */ \
            _status = USBPD_ERROR; \
          }
#define _PWR_CHECK_VOLTAGE_MAX(_PDO_VOLT_, _SNK_VOLT_) \
          if ((_PDO_VOLT_) != (_SNK_VOLT_)) \
          { \
            /* Disalignment between PDO and DPM_SNKRequestedPower structure */ \
            _status = USBPD_ERROR; \
          }
#define _PWR_CHECK_CURRENT_MAX(_PDO_CURRENT_, _SNK_CURRENT_) \
          if ((_PDO_CURRENT_) != (_SNK_CURRENT_)) \
          { \
            /* Disalignment between PDO and DPM_SNKRequestedPower structure */ \
            _status = USBPD_ERROR; \
          }
#define _PWR_CHECK_POWER_MAX(_PDO_POWER_, _SNK_POWER_) \
          if ((_PDO_POWER_) != (_SNK_POWER_)) \
          { \
            /* Disalignment between PDO and DPM_SNKRequestedPower structure */ \
            _status = USBPD_ERROR; \
          }
#endif /* _GUI_INTERFACE */

/* USER CODE END Private_Macro */
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Variables
  * @{
  */
/* USER CODE BEGIN Private_Variables */
/**
  * @brief  USBPD Port PDO Storage array declaration
  */

/**** PDO ****/
USBPD_PWR_Port_PDO_Storage_TypeDef PWR_Port_PDO_Storage[USBPD_PORT_COUNT];

/* USER CODE END Private_Variables */
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Functions
  * @{
  */
/* USER CODE BEGIN USBPD_USER_PRIVATE_FUNCTIONS_Prototypes */

USBPD_StatusTypeDef PWR_IF_CheckUpdateSNKPower(uint8_t PortNum);
void     _PWR_CheckPDOContent(uint8_t PortNum);
/* USER CODE END USBPD_USER_PRIVATE_FUNCTIONS_Prototypes */
/**
  * @}
  */

/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Exported_Functions
  * @{
  */

/**
  * @brief  Initialize structures and variables related to power board profiles
  *         used by Sink and Source, for all available ports.
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_PWR_IF_Init(void)
{
/* USER CODE BEGIN USBPD_PWR_IF_Init */
  USBPD_StatusTypeDef _status = USBPD_OK;

  /* Set links to PDO values and number for Port 0 (defined in PDO arrays in H file).
   */

  PWR_Port_PDO_Storage[USBPD_PORT_0].SinkPDO.ListOfPDO = (uint32_t *)PORT0_PDO_ListSNK;
  PWR_Port_PDO_Storage[USBPD_PORT_0].SinkPDO.NumberOfPDO = &USBPD_NbPDO[0];
  _status |= PWR_IF_CheckUpdateSNKPower(USBPD_PORT_0);


  /* Add consistency check on PDO definition 
     Could help to detect unexpected flash settings content */
  _PWR_CheckPDOContent(USBPD_PORT_0);

  return _status;
/* USER CODE END USBPD_PWR_IF_Init */
}

/**
  * @brief  Checks if the power on a specified port is ready
  * @param  PortNum Port number
  * @param  Vsafe   Vsafe status based on @ref USBPD_VSAFE_StatusTypeDef
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_PWR_IF_SupplyReady(uint8_t PortNum, USBPD_VSAFE_StatusTypeDef Vsafe)
{
/* USER CODE BEGIN USBPD_PWR_IF_SupplyReady */
  USBPD_StatusTypeDef status;
  uint32_t _voltage;

  BSP_USBPD_PWR_VBUSGetVoltage(PortNum, &_voltage);
  if (USBPD_VSAFE_0V == Vsafe)
  {
    /* Vsafe0V */
    status = ((_voltage < USBPD_PWR_LOW_VBUS_THRESHOLD)? USBPD_OK: USBPD_ERROR);
  }
  else
  {
    /* Vsafe5V */
    status = ((_voltage > USBPD_PWR_HIGH_VBUS_THRESHOLD)? USBPD_OK: USBPD_ERROR);
  }

  return status;
/* USER CODE END USBPD_PWR_IF_SupplyReady */
}

/**
  * @brief  Reads the voltage and the current on a specified port
  * @param  PortNum Port number
  * @param  pVoltage: The Voltage in mV
  * @param  pCurrent: The Current in mA
  * @retval USBPD_ERROR or USBPD_OK
  */
USBPD_StatusTypeDef USBPD_PWR_IF_ReadVA(uint8_t PortNum, uint16_t *pVoltage, uint16_t *pCurrent)
{
/* USER CODE BEGIN USBPD_PWR_IF_ReadVA */
  return USBPD_ERROR;
/* USER CODE END USBPD_PWR_IF_ReadVA */
}

/**
  * @brief  Enables the VConn on the port.
  * @param  PortNum Port number
  * @param  CC      Specifies the CCx to be selected based on @ref CCxPin_TypeDef structure
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_PWR_IF_Enable_VConn(uint8_t PortNum, CCxPin_TypeDef CC)
{
/* USER CODE BEGIN USBPD_PWR_IF_Enable_VConn */
  return USBPD_ERROR;
/* USER CODE END USBPD_PWR_IF_Enable_VConn */
}

/**
  * @brief  Disable the VConn on the port.
  * @param  PortNum Port number
  * @param  CC      Specifies the CCx to be selected based on @ref CCxPin_TypeDef structure
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_PWR_IF_Disable_VConn(uint8_t PortNum, CCxPin_TypeDef CC)
{
/* USER CODE BEGIN USBPD_PWR_IF_Disable_VConn */
  return USBPD_ERROR;
/* USER CODE END USBPD_PWR_IF_Disable_VConn */
}

/**
  * @brief  Allow PDO data reading from PWR_IF storage.
  * @param  PortNum Port number
  * @param  DataId Type of data to be read from PWR_IF
  *         This parameter can be one of the following values:
  *           @arg @ref USBPD_CORE_DATATYPE_SRC_PDO Source PDO reading requested
  *           @arg @ref USBPD_CORE_DATATYPE_SNK_PDO Sink PDO reading requested
  * @param  Ptr Pointer on address where PDO values should be written (u8 pointer)
  * @param  Size Pointer on nb of u32 written by PWR_IF (nb of PDOs)
  * @retval None
  */
void USBPD_PWR_IF_GetPortPDOs(uint8_t PortNum, USBPD_CORE_DataInfoType_TypeDef DataId, uint8_t *Ptr, uint32_t *Size)
{
    {
      *Size = USBPD_NbPDO[0];
      memcpy(Ptr,PORT0_PDO_ListSNK, sizeof(uint32_t) * USBPD_NbPDO[0]);
    }
/* USER CODE BEGIN USBPD_PWR_IF_GetPortPDOs */
  uint32_t   nbpdo, index, nb_valid_pdo = 0;
  uint32_t   *ptpdoarray = NULL;
  USBPD_PDO_TypeDef pdo_first;
  USBPD_PDO_TypeDef pdo;

  /* Check if valid port */
  if (USBPD_PORT_IsValid(PortNum))
  {
    /* According to type of PDO to be read, set pointer on values and nb of elements */
    switch (DataId)
    {
    case USBPD_CORE_DATATYPE_SNK_PDO:
      nbpdo       = *PWR_Port_PDO_Storage[PortNum].SinkPDO.NumberOfPDO;
      ptpdoarray  = PWR_Port_PDO_Storage[PortNum].SinkPDO.ListOfPDO;
      /* Save the 1st PDO */
      pdo_first.d32 = *ptpdoarray;
      /* Reset FRS bit if current revision is PD2.0*/
      if (USBPD_SPECIFICATION_REV2 == DPM_Params[PortNum].PE_SpecRevision)
      {
        pdo_first.SNKFixedPDO.FastRoleSwapRequiredCurrent = USBPD_PDO_SNK_FIXED_FRS_NOT_SUPPORTED;
      }
      break;
    default:
      nbpdo = 0;
      break;
    }

    /* Copy PDO data in output buffer */
    for (index = 0; index < nbpdo; index++)
    {
      pdo.d32 = *ptpdoarray;
      /* Copy only PDO (and not APDO in case of current revision is PD2.0) */
      if ((USBPD_SPECIFICATION_REV2 == DPM_Params[PortNum].PE_SpecRevision)
       && (pdo.GenericPDO.PowerObject == USBPD_CORE_PDO_TYPE_APDO))
      {
      }
      else
      {
        /* Copy 1st PDO as potentially FRS or UNCHUNKED bits have been reset */
        if (0 == index)
        {
          (void)memcpy(Ptr, (uint8_t*)&pdo_first.d32, 4u);
        }
        else
        {
          (void)memcpy((Ptr + (nb_valid_pdo * 4u)), (uint8_t*)ptpdoarray, 4u);
        }
        nb_valid_pdo++;
      }
      ptpdoarray++;
    }
    /* Set nb of read PDO (nb of u32 elements); */
    *Size = nb_valid_pdo;
  }
/* USER CODE END USBPD_PWR_IF_GetPortPDOs */
}

/**
  * @brief  Find out SRC PDO pointed out by a position provided in a Request DO (from Sink).
  * @param  PortNum Port number
  * @param  RdoPosition RDO Position in list of provided PDO
  * @param  Pdo Pointer on PDO value pointed out by RDO position (u32 pointer)
  * @retval Status of search
  *         USBPD_OK : Src PDO found for requested DO position (output Pdo parameter is set)
  *         USBPD_FAIL : Position is not compliant with current Src PDO for this port (no corresponding PDO value)
  */
USBPD_StatusTypeDef USBPD_PWR_IF_SearchRequestedPDO(uint8_t PortNum, uint32_t RdoPosition, uint32_t *Pdo)
{
/* USER CODE BEGIN USBPD_PWR_IF_SearchRequestedPDO */
  return USBPD_FAIL;
/* USER CODE END USBPD_PWR_IF_SearchRequestedPDO */
}

/**
  * @brief  Function called in case of critical issue is detected to switch in safety mode.
  * @param  ErrorType Type of error detected by monitoring (based on @ref USBPD_PWR_IF_ERROR)
  * @retval None
  */
void USBPD_PWR_IF_AlarmType(USBPD_PWR_IF_ERROR ErrorType)
{
/* USER CODE BEGIN USBPD_PWR_IF_AlarmType */

/* USER CODE END USBPD_PWR_IF_AlarmType */
}

/**
  * @brief  the function is called in case of critical issue is detected to switch in safety mode.
  * @retval None
  */
void USBPD_PWR_IF_Alarm()
{
/* USER CODE BEGIN USBPD_PWR_IF_Alarm */

/* USER CODE END USBPD_PWR_IF_Alarm */
}

/**
  * @brief Function is called to get VBUS power status.
  * @param PortNum Port number
  * @param PowerTypeStatus  Power type status based on @ref USBPD_VBUSPOWER_STATUS
  * @retval UBBPD_TRUE or USBPD_FALSE
  */
uint8_t USBPD_PWR_IF_GetVBUSStatus(uint8_t PortNum, USBPD_VBUSPOWER_STATUS PowerTypeStatus)
{
/* USER CODE BEGIN USBPD_PWR_IF_GetVBUSStatus */
  uint8_t _status = USBPD_FALSE;
  uint32_t _vbus = HW_IF_PWR_GetVoltage(PortNum);

  switch(PowerTypeStatus)
  {
  case USBPD_PWR_BELOWVSAFE0V :
    if (_vbus < USBPD_PWR_LOW_VBUS_THRESHOLD) _status = USBPD_TRUE;
    break;
  case USBPD_PWR_VSAFE5V :
    if (_vbus >= USBPD_PWR_HIGH_VBUS_THRESHOLD) _status = USBPD_TRUE;
    break;
  case USBPD_PWR_SNKDETACH:
    if (_vbus < USBPD_PWR_HIGH_VBUS_THRESHOLD) _status = USBPD_TRUE;
    break;
  default :
    break;
  }
  return _status;
/* USER CODE END USBPD_PWR_IF_GetVBUSStatus */
}

/**
  * @brief Function is called to set the VBUS threshold when a request has been accepted.
  * @param PortNum Port number
  * @retval None
  */
void USBPD_PWR_IF_UpdateVbusThreshold(uint8_t PortNum)
{
/* USER CODE BEGIN USBPD_PWR_IF_UpdateVbusThreshold */
/* USER CODE END USBPD_PWR_IF_UpdateVbusThreshold */
}

/**
  * @brief Function is called to reset the VBUS threshold when there is a power reset.
  * @param PortNum Port number
  * @retval None
  */
void USBPD_PWR_IF_ResetVbusThreshold(uint8_t PortNum)
{
/* USER CODE BEGIN USBPD_PWR_IF_ResetVbusThreshold */
/* USER CODE END USBPD_PWR_IF_ResetVbusThreshold */
}

/* USER CODE BEGIN USBPD_USER_EXPORTED_FUNCTIONS_Definition */

/* USER CODE END USBPD_USER_EXPORTED_FUNCTIONS_Definition */
/**
  * @}
  */

/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Functions
  * @{
  */
/* USER CODE BEGIN USBPD_USER_PRIVATE_FUNCTIONS_Definition */
/**
  * @brief  Function to check validity between SNK PDO and power user settings
  * @param  PortNum Port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef PWR_IF_CheckUpdateSNKPower(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_OK;
  USBPD_PDO_TypeDef pdo;
  uint32_t _max_power = 0;
  uint16_t _voltage = 0, _current = 0, _power = 0;
  uint16_t _min_voltage = 0xFFFF, _max_voltage = 0, _max_current = 0;

  for (uint32_t _index = 0; _index < *PWR_Port_PDO_Storage[PortNum].SinkPDO.NumberOfPDO; _index++)
  {
    pdo.d32 = PWR_Port_PDO_Storage[PortNum].SinkPDO.ListOfPDO[_index];
    switch (pdo.GenericPDO.PowerObject)
    {
      case USBPD_CORE_PDO_TYPE_FIXED:    /*!< Fixed Supply PDO                             */
        _voltage = PWR_DECODE_50MV(pdo.SNKFixedPDO.VoltageIn50mVunits);
        _PWR_UPDATE_VOLTAGE_MIN(_voltage, _min_voltage);
        _PWR_UPDATE_VOLTAGE_MAX(_voltage, _max_voltage);
        _current = PWR_DECODE_10MA(pdo.SNKFixedPDO.OperationalCurrentIn10mAunits);
        _PWR_UPDATE_CURRENT_MAX(_current, _max_current);
        break;
      case USBPD_CORE_PDO_TYPE_BATTERY:  /*!< Battery Supply PDO                           */
        _voltage = PWR_DECODE_50MV(pdo.SNKBatteryPDO.MinVoltageIn50mVunits);
        _PWR_UPDATE_VOLTAGE_MIN(_voltage, _min_voltage);
        _voltage = PWR_DECODE_50MV(pdo.SNKBatteryPDO.MaxVoltageIn50mVunits);
        _PWR_UPDATE_VOLTAGE_MAX(_voltage, _max_voltage);
        _power = PWR_DECODE_MW(pdo.SNKBatteryPDO.OperationalPowerIn250mWunits);
        _PWR_UPDATE_POWER_MAX(_power, _max_power);
        break;
      case USBPD_CORE_PDO_TYPE_VARIABLE: /*!< Variable Supply (non-battery) PDO            */
        _voltage = PWR_DECODE_50MV(pdo.SNKVariablePDO.MinVoltageIn50mVunits);
        _PWR_UPDATE_VOLTAGE_MIN(_voltage, _min_voltage);
        _voltage = PWR_DECODE_50MV(pdo.SNKVariablePDO.MaxVoltageIn50mVunits);
        _PWR_UPDATE_VOLTAGE_MAX(_voltage, _max_voltage);
        _current = PWR_DECODE_10MA(pdo.SNKVariablePDO.OperationalCurrentIn10mAunits);
        _PWR_UPDATE_CURRENT_MAX(_current, _max_current);
        break;
      case USBPD_CORE_PDO_TYPE_APDO:     /*!< Augmented Power Data Object (APDO)           */
        _voltage = PWR_DECODE_100MV(pdo.SRCSNKAPDO.MinVoltageIn100mV);
        _PWR_UPDATE_VOLTAGE_MIN(_voltage, _min_voltage);
        _voltage = PWR_DECODE_100MV(pdo.SRCSNKAPDO.MaxVoltageIn100mV);
        _PWR_UPDATE_VOLTAGE_MAX(_voltage, _max_voltage);
        _current = PWR_DECODE_50MA(pdo.SRCSNKAPDO.MaxCurrentIn50mAunits);
        _PWR_UPDATE_CURRENT_MAX(_current, _max_current);
        break;
      default:
        break;
    }
  }

  _PWR_CHECK_VOLTAGE_MIN(_min_voltage, DPM_USER_Settings[PortNum].DPM_SNKRequestedPower.MinOperatingVoltageInmVunits);
  _PWR_CHECK_VOLTAGE_MAX(_max_voltage, DPM_USER_Settings[PortNum].DPM_SNKRequestedPower.MaxOperatingVoltageInmVunits);
  _PWR_CHECK_CURRENT_MAX(_max_current, DPM_USER_Settings[PortNum].DPM_SNKRequestedPower.MaxOperatingCurrentInmAunits);
  _max_power = (_max_voltage * _max_current) / 1000;
  _PWR_CHECK_POWER_MAX(_max_power, DPM_USER_Settings[PortNum].DPM_SNKRequestedPower.MaxOperatingPowerInmWunits);

  return _status;
}

/**
  * @brief  Add consistency check on PDO definition (Could help to detect unexpected flash settings content)
  * @param  PortNum Port number
  * @retval None
  */
void _PWR_CheckPDOContent(uint8_t PortNum)
{
  uint32_t firstpdo;
  uint8_t error_in_content = 0U;

  /* Add consistency check on PDO definition for Ports 0 and 1 */
  
#if defined(_SRC)||defined(_DRP)
  /* SRC PDO
     - Nb of SRC PDO should be < USBPD_MAX_NB_PDO
     - if SRC PDO are defined, 1st SRC PDO should at least have following characteristics :
         Type Fixed (bits 31-30)
         bits 23-22 shall be reserved
         Voltage in 50mv units = 5V (bits19-10 should be equal to 100)
  */
  if (*PWR_Port_PDO_Storage[PortNum].SourcePDO.NumberOfPDO > USBPD_MAX_NB_PDO)
  {
    error_in_content = 1U;
  }
  else
  {
    if (*PWR_Port_PDO_Storage[PortNum].SourcePDO.NumberOfPDO >= 1U)
    {
      firstpdo = (PWR_Port_PDO_Storage[PortNum].SourcePDO.ListOfPDO[0U])
                 & (USBPD_PDO_TYPE_Msk | USBPD_PDO_SRC_FIXED_VOLTAGE_Msk | (0x3U << 22U));
      if (firstpdo != (USBPD_PDO_TYPE_FIXED | (100U << USBPD_PDO_SRC_FIXED_VOLTAGE_Pos)))
      {
        error_in_content = 1U;
      }
    }
  }
#endif /* _SRC || _DRP */

#if defined(_SNK)||defined(_DRP)
  /* SNK PDO
     - Nb of SNK PDO should be < USBPD_MAX_NB_PDO
     - if SNK PDO are defined, 1st SNK PDO should at least have following characteristics :
         Type Fixed (bits 31-30)
         bits 22-20 shall be reserved
         Voltage in 50mv units = 5V (bits19-10 should be equal to 100)
  */
  if (*PWR_Port_PDO_Storage[PortNum].SinkPDO.NumberOfPDO > USBPD_MAX_NB_PDO)
  {
    error_in_content = 1U;
  }
  else
  {
    if (*PWR_Port_PDO_Storage[PortNum].SinkPDO.NumberOfPDO >= 1U)
    {
      firstpdo = (PWR_Port_PDO_Storage[PortNum].SinkPDO.ListOfPDO[0U])
                 & (USBPD_PDO_TYPE_Msk | USBPD_PDO_SNK_FIXED_VOLTAGE_Msk | (0x3U << 20U));
      if (firstpdo != (USBPD_PDO_TYPE_FIXED | (100U << USBPD_PDO_SNK_FIXED_VOLTAGE_Pos)))
      {
        error_in_content = 1U;
      }
    }
  }
#endif /* _SNK || _DRP */

  if (error_in_content == 1U)
  {
#if defined(_RTOS)
#if (osCMSIS >= 0x20000U)
    /* Stop RTOS scheduling */
    vTaskSuspendAll();
#endif /* osCMSIS >= 0x20000U */
#endif /* _RTOS */
    /* disable all interrupt to lock the system */
    __disable_irq();
    while(1);
  }
}
/* USER CODE END USBPD_USER_PRIVATE_FUNCTIONS_Definition */
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
