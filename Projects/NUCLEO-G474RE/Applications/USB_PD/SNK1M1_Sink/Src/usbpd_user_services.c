/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_user_services.c
  * @author  MCD Application Team
  * @brief   USBPD user services code
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
/* USER CODE END Header */

#define USBPD_USER_SERVICES_C

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbpd_core.h"
#include "usbpd_user_services.h"
#include "usbpd_dpm_conf.h"
#include "usbpd_pwr_if.h"
#if defined(_TRACE)
#include "usbpd_trace.h"
#include "string.h"
#include "stdio.h"
#endif /* _TRACE */
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN Private_Typedef */
/* USER CODE END Private_Typedef */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Private_Define */
#define USED_PDO_SEL_METHOD    PDO_SEL_METHOD_MAX_PWR /*!< Method used to find the "best" PDO                        */

#define DPM_NO_SRC_PDO_FOUND   0xFFU                /*!< No match found between Received SRC PDO and SNK capabilities*/
/* USER CODE END Private_Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Private_Macro */
/* USER CODE END Private_Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Private_Variables */
extern USBPD_ParamsTypeDef              DPM_Params[USBPD_PORT_COUNT];
extern USBPD_SettingsTypeDef            DPM_Settings[USBPD_PORT_COUNT];
extern USBPD_USER_SettingsTypeDef       DPM_USER_Settings[USBPD_PORT_COUNT];
USBPD_HandleTypeDef                     DPM_Ports[USBPD_PORT_COUNT];
/* USER CODE END Private_Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN USBPD_USER_PRIVATE_FUNCTIONS_Prototypes */
#if defined (_SNK)
static uint32_t USER_SERV_SNK_EvaluateMatchWithSRCPDO(uint8_t PortNum,
                                                      uint32_t SrcPDO,
                                                      uint32_t* PtrRequestedVoltage,
                                                      uint32_t* PtrRequestedPower);

static uint32_t USER_SERV_FindVoltageIndex(           uint32_t PortNum,
                                                      USBPD_DPM_SNKPowerRequestDetailsTypeDef* PtrRequestPowerDetails,
                                                      uint8_t Method);

static void     USER_SERV_SNK_BuildRDOfromSelectedPDO(uint8_t PortNum,
                                                      uint8_t IndexSrcPDO,
                                                      USBPD_DPM_SNKPowerRequestDetailsTypeDef* PtrRequestPowerDetails,
                                                      USBPD_SNKRDO_TypeDef* Rdo,
                                                      USBPD_CORE_PDO_Type_TypeDef *PtrPowerObject);
#endif /* _SNK */
/* USER CODE END USBPD_USER_PRIVATE_FUNCTIONS_Prototypes */

/* Exported functions ------- ------------------------------------------------*/
/* USER CODE BEGIN USBPD_USER_EXPORTED_FUNCTIONS */
#if defined(_DRP) || defined(_SRC)
/**
  * @brief  Evaluate received Request Message from Sink port
  * @param  PortNum Port number
  * @param  PtrPowerObject  Pointer on the power data object
  * @retval USBPD status : USBPD_ACCEPT, USBPD_REJECT, USBPD_WAIT, USBPD_GOTOMIN
  */
USBPD_StatusTypeDef USBPD_USER_SERV_EvaluateReqMess(uint8_t PortNum, USBPD_CORE_PDO_Type_TypeDef *PtrPowerObject)
{
  USBPD_StatusTypeDef _retr = USBPD_REJECT;
  USBPD_PDO_TypeDef pdo;
  USBPD_SNKRDO_TypeDef rdo;

  /* read the request value received */
  rdo.d32 = DPM_Ports[PortNum].DPM_RcvRequestDOMsg;

  /* Search PDO in Port Source PDO list, that corresponds to Position provided in Request RDO */
  if (USBPD_PWR_IF_SearchRequestedPDO(PortNum,  rdo.GenericRDO.ObjectPosition, &pdo.d32) == USBPD_OK)
  {

    /* Evaluate the request */
    if(pdo.GenericPDO.PowerObject == USBPD_CORE_PDO_TYPE_FIXED)
    {
      if((rdo.FixedVariableRDO.OperatingCurrentIn10mAunits > pdo.SRCFixedPDO.MaxCurrentIn10mAunits)
         || (rdo.FixedVariableRDO.MaxOperatingCurrent10mAunits > pdo.SRCFixedPDO.MaxCurrentIn10mAunits))
      {
        /* Sink requests too much maximum operating current */
        /* USBPD_DPM_EvaluateRequest: Sink requests too much maximum operating current */
        _retr =  USBPD_REJECT;
      }
      else
      {
        /* Save the power object */
        *PtrPowerObject = pdo.GenericPDO.PowerObject;
        /* Set RDO position and requested voltage in DPM port structure */
        DPM_Ports[PortNum].DPM_RequestedVoltage = pdo.SRCFixedPDO.VoltageIn50mVunits * 50;
        DPM_Ports[PortNum].DPM_RDOPositionPrevious = DPM_Ports[PortNum].DPM_RDOPosition;
        DPM_Ports[PortNum].DPM_RDOPosition = rdo.GenericRDO.ObjectPosition;
         _retr = USBPD_ACCEPT;
      }
    }
  }

  return _retr;
}
#endif /* _DRP || _SRC */

#if defined(_SNK)
/**
  * @brief  Store the received source PDO
  * @param  PortNum Port number
  * @param  Ptr     Pointer on the data
  * @param  Size    Nb of bytes to be updated in DPM
  * @retval None
  */
void USBPD_USER_SERV_StoreSRCPDO(uint8_t PortNum, uint8_t *Ptr, uint32_t Size)
{
  /*!< Storage of Received Source PDO values        */
  if (Size <= (USBPD_MAX_NB_PDO * 4))
  {
    uint8_t* rdo;
    DPM_Ports[PortNum].DPM_NumberOfRcvSRCPDO = (Size / 4);
    /* Copy PDO data in DPM Handle field */
    for (uint32_t index = 0; index < (Size / 4); index++)
    {
      rdo = (uint8_t*)&DPM_Ports[PortNum].DPM_ListOfRcvSRCPDO[index];
      (void)memcpy(rdo, (Ptr + (index * 4u)), (4u * sizeof(uint8_t)));
    }
  }
}

/**
  * @brief  Evaluate received Capabilities Message from Source port and prepare the request message
  * @param  PortNum         Port number
  * @param  PtrRequestData  Pointer on selected request data object
  * @param  PtrPowerObjectType  Pointer on the power data object
  * @retval None
  */
void USBPD_USER_SERV_EvaluateCapa(uint8_t PortNum,
                                  uint32_t *PtrRequestData,
                                  USBPD_CORE_PDO_Type_TypeDef *PtrPowerObjectType)
{
  USBPD_PDO_TypeDef  fixed_pdo;
  USBPD_SNKRDO_TypeDef rdo;
  USBPD_HandleTypeDef *pdhandle = &DPM_Ports[PortNum];
  USBPD_USER_SettingsTypeDef *puser = (USBPD_USER_SettingsTypeDef *)&DPM_USER_Settings[PortNum];
  USBPD_DPM_SNKPowerRequestDetailsTypeDef snkpowerrequestdetails;
  uint32_t pdoindex, size;
  uint32_t snkpdolist[USBPD_MAX_NB_PDO];
  USBPD_PDO_TypeDef snk_fixed_pdo;

  snkpowerrequestdetails.RequestedVoltageInmVunits = 0;
  snkpowerrequestdetails.OperatingCurrentInmAunits = 0;

  /* Find the Pdo index for the requested voltage, depending on the wanted method */
  pdoindex = USER_SERV_FindVoltageIndex(PortNum, &snkpowerrequestdetails, USED_PDO_SEL_METHOD);

  /* Initialize RDO */
  rdo.d32 = 0;

  /* If no valid SNK PDO or if no SRC PDO match found
      (index>=nb of valid received SRC PDOs or function returned DPM_NO_SRC_PDO_FOUND */
  if (pdoindex >= pdhandle->DPM_NumberOfRcvSRCPDO)
  {
#if defined(_TRACE)
    uint8_t mess[]="PE_EvaluateCapability: could not find desired voltage";
    USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PortNum, 0, (uint8_t *)mess, sizeof(mess));
#endif /* _TRACE */
    fixed_pdo.d32 = pdhandle->DPM_ListOfRcvSRCPDO[0];
    /* Read SNK PDO list for retrieving useful data to fill in RDO */
    USBPD_PWR_IF_GetPortPDOs(PortNum, USBPD_CORE_DATATYPE_SNK_PDO, (uint8_t*)&snkpdolist[0], &size);
    /* Store value of 1st SNK PDO (Fixed) in local variable */
    snk_fixed_pdo.d32 = snkpdolist[0];
    rdo.FixedVariableRDO.ObjectPosition = 1;
    rdo.FixedVariableRDO.OperatingCurrentIn10mAunits  =  fixed_pdo.SRCFixedPDO.MaxCurrentIn10mAunits;
    rdo.FixedVariableRDO.MaxOperatingCurrent10mAunits =  fixed_pdo.SRCFixedPDO.MaxCurrentIn10mAunits;
    rdo.FixedVariableRDO.CapabilityMismatch = 1;
    rdo.FixedVariableRDO.USBCommunicationsCapable = snk_fixed_pdo.SNKFixedPDO.USBCommunicationsCapable;
    DPM_Ports[PortNum].DPM_RequestedCurrent = puser->DPM_SNKRequestedPower.MaxOperatingCurrentInmAunits;

    pdhandle->DPM_RequestDOMsg = rdo.d32;
    *PtrPowerObjectType = USBPD_CORE_PDO_TYPE_FIXED;
    *PtrRequestData = rdo.d32;
    pdhandle->DPM_RequestedVoltage = 5000;
    return;
  }

  USER_SERV_SNK_BuildRDOfromSelectedPDO(PortNum, pdoindex, &snkpowerrequestdetails,&rdo, PtrPowerObjectType);

  *PtrRequestData = pdhandle->DPM_RequestDOMsg;
}
#endif /* _SNK */
/* USER CODE END USBPD_USER_EXPORTED_FUNCTIONS */

/* USER CODE BEGIN USBPD_USER_PRIVATE_FUNCTIONS */

#if defined(_SNK)
/**
  * @brief  Examinate a given SRC PDO to check if matching with SNK capabilities.
  * @param  PortNum             Port number
  * @param  SrcPDO              Selected SRC PDO (32 bits)
  * @param  PtrRequestedVoltage Pointer on Voltage value that could be reached if SRC
                                PDO is requested (only valid if USBPD_TRUE is returned) in mV
  * @param  PtrRequestedPower   Pointer on Power value that could be reached if SRC PDO is requested
                                (only valid if USBPD_TRUE is returned) in mW
  * @retval USBPD_FALSE of USBPD_TRUE (USBPD_TRUE returned in SRC PDO is considered matching with SNK profile)
  */
static uint32_t USER_SERV_SNK_EvaluateMatchWithSRCPDO(uint8_t PortNum,
                                                      uint32_t SrcPDO,
                                                      uint32_t* PtrRequestedVoltage,
                                                      uint32_t* PtrRequestedPower)
{
  USBPD_PDO_TypeDef  srcpdo, snkpdo;
  uint32_t match = USBPD_FALSE;
  uint32_t nbsnkpdo;
  uint32_t snkpdo_array[USBPD_MAX_NB_PDO];
  uint16_t i, srcvoltage50mv, srcmaxcurrent10ma;
  uint16_t snkvoltage50mv, snkopcurrent10ma;
  uint32_t maxrequestedpower, currentrequestedpower;
  uint32_t maxrequestedvoltage, currentrequestedvoltage;

  /* Retrieve SNK PDO list from PWR_IF storage : PDO values + nb of u32 written by PWR_IF (nb of PDOs) */
  USBPD_PWR_IF_GetPortPDOs(PortNum, USBPD_CORE_DATATYPE_SNK_PDO, (uint8_t*)snkpdo_array, &nbsnkpdo);

  if (0 == nbsnkpdo)
  {
    return(USBPD_FALSE);
  }

  /* Set default output values */
  maxrequestedpower    = 0;
  maxrequestedvoltage  = 0;

  /* Check SRC PDO value according to its type */
  srcpdo.d32 = SrcPDO;
  switch(srcpdo.GenericPDO.PowerObject)
  {
    /* SRC Fixed Supply PDO */
    case USBPD_CORE_PDO_TYPE_FIXED:
      srcvoltage50mv = srcpdo.SRCFixedPDO.VoltageIn50mVunits;
      srcmaxcurrent10ma = srcpdo.SRCFixedPDO.MaxCurrentIn10mAunits;

      /* Loop through SNK PDO list */
      for (i=0; i<nbsnkpdo; i++)
      {
        currentrequestedpower = 0;
        currentrequestedvoltage = 0;

        /* Retrieve SNK PDO value according to its type */
        snkpdo.d32 = snkpdo_array[i];
        switch(snkpdo.GenericPDO.PowerObject)
        {
          /* SNK Fixed Supply PDO */
          case USBPD_CORE_PDO_TYPE_FIXED:
          {
            snkvoltage50mv = snkpdo.SNKFixedPDO.VoltageIn50mVunits;
            snkopcurrent10ma = snkpdo.SNKFixedPDO.OperationalCurrentIn10mAunits;

            /* Match if :
                 SNK Voltage = SRC Voltage
                 &&
                 SNK Op Current <= SRC Max Current

               Requested Voltage : SNK Voltage
               Requested Op Current : SNK Op Current
               Requested Max Current : SNK Op Current
            */
            if (  (snkvoltage50mv == srcvoltage50mv)
                &&(snkopcurrent10ma <= srcmaxcurrent10ma))
            {
              currentrequestedpower = (snkvoltage50mv * snkopcurrent10ma) / 2; /* to get value in mW */
              currentrequestedvoltage = snkvoltage50mv;
            }
            break;
          }
            /* SNK Augmented Power Data Object (APDO) */
          case USBPD_CORE_PDO_TYPE_APDO:
            break;

          default:
            break;
        }

        if (currentrequestedpower > maxrequestedpower)
        {
          match = USBPD_TRUE;
          maxrequestedpower   = currentrequestedpower;
          maxrequestedvoltage = currentrequestedvoltage;
        }
      }
      break;

    /* Augmented Power Data Object (APDO) */
    case USBPD_CORE_PDO_TYPE_APDO:
      {
        uint16_t srcmaxvoltage100mv, srcmaxcurrent50ma;
        srcmaxvoltage100mv = srcpdo.SRCSNKAPDO.MaxVoltageIn100mV;
        srcmaxcurrent50ma = srcpdo.SRCSNKAPDO.MaxCurrentIn50mAunits;

        /* Loop through SNK PDO list */
        for (i=0; i<nbsnkpdo; i++)
        {
          currentrequestedpower = 0;
          currentrequestedvoltage = 0;

          /* Retrieve SNK PDO value according to its type */
          snkpdo.d32 = snkpdo_array[i];
          switch(snkpdo.GenericPDO.PowerObject)
          {
            case USBPD_CORE_PDO_TYPE_FIXED:
              /* No match */
              break;
            /* SNK Augmented Power Data Object (APDO) */
            case USBPD_CORE_PDO_TYPE_APDO:
              {
                uint16_t snkmaxvoltage100mv, snkminvoltage100mv, snkmaxcurrent50ma;

                snkminvoltage100mv = snkpdo.SRCSNKAPDO.MinVoltageIn100mV;
                snkmaxvoltage100mv = snkpdo.SRCSNKAPDO.MaxVoltageIn100mV;
                snkmaxcurrent50ma = snkpdo.SRCSNKAPDO.MaxCurrentIn50mAunits;

                /* Match if voltage match with the APDO voltage range */
                if ((PWR_DECODE_100MV(snkminvoltage100mv) <= (*PtrRequestedVoltage))
                 && ((*PtrRequestedVoltage) <= PWR_DECODE_100MV(snkmaxvoltage100mv))
                 && (snkmaxcurrent50ma <= srcmaxcurrent50ma))
                {
                  if (0 != *PtrRequestedPower)
                  {
                    /* to get value in mW */
                    currentrequestedpower = (*PtrRequestedVoltage * PWR_DECODE_50MA(snkmaxcurrent50ma)) / 1000;
                    currentrequestedvoltage = (*PtrRequestedVoltage / 50);
                  }
                  else
                  {
                    /* to get value in mW */
                    *PtrRequestedVoltage = MIN(PWR_DECODE_100MV(srcmaxvoltage100mv),                                 \
                                           PWR_DECODE_100MV(snkmaxvoltage100mv));
                    currentrequestedpower = (*PtrRequestedVoltage * PWR_DECODE_50MA(snkmaxcurrent50ma)) / 1000;
                    currentrequestedvoltage = (*PtrRequestedVoltage / 50);
                  }
                }
              }
              break;

            default:
              break;
          }

          if (currentrequestedpower > maxrequestedpower)
          {
            match = USBPD_TRUE;
            maxrequestedpower   = currentrequestedpower;
            maxrequestedvoltage = currentrequestedvoltage;
          }
        }
      }
      break;

    default:
      return(USBPD_FALSE);
  }

  if (maxrequestedpower > 0)
  {
    *PtrRequestedPower   = maxrequestedpower;
    *PtrRequestedVoltage = maxrequestedvoltage * 50; /* value in mV */
  }
  return(match);
}

/**
  * @brief  Find PDO index depending on a method: max/min power, voltage, or current.
  * @param  PortNum Port number
  * @param  PtrRequestPowerDetails  Sink requested power details structure pointer
  * @param  Method  Sink requested power details structure pointer
  * @retval Index of PDO within source capabilities message (DPM_NO_SRC_PDO_FOUND indicating not found)
  */
static uint32_t USER_SERV_FindVoltageIndex(uint32_t PortNum,
                                           USBPD_DPM_SNKPowerRequestDetailsTypeDef* PtrRequestPowerDetails,
                                           uint8_t Method)
{
  uint32_t *ptpdoarray;
  USBPD_PDO_TypeDef  pdo;
  uint32_t voltage, reqvoltage, nbpdo, allowablepower, selpower, allowablecurrent, selcurrent;
  uint32_t curr_index = DPM_NO_SRC_PDO_FOUND, temp_index;
  USBPD_USER_SettingsTypeDef *puser = (USBPD_USER_SettingsTypeDef *)&DPM_USER_Settings[PortNum];

  allowablepower = 0;
  selpower       = 0;
  reqvoltage     = 0;
  voltage        = 0;
  selcurrent     = 0;

  /* Search PDO index among Source PDO of Port */
  nbpdo = DPM_Ports[PortNum].DPM_NumberOfRcvSRCPDO;
  ptpdoarray = DPM_Ports[PortNum].DPM_ListOfRcvSRCPDO;

  /* search the best PDO in the list of source PDOs */
  for(temp_index = 0; temp_index < nbpdo; temp_index++)
  {
    pdo.d32 = ptpdoarray[temp_index];

    /* For APDO */
    if(USBPD_CORE_PDO_TYPE_APDO == pdo.GenericPDO.PowerObject)
    {
      curr_index = temp_index;
      voltage = reqvoltage = MIN(5900, pdo.SRCSNKAPDO.MaxVoltageIn100mV*100);
      selpower = reqvoltage * pdo.SRCSNKAPDO.MaxCurrentIn50mAunits*50;

      PtrRequestPowerDetails->MaxOperatingCurrentInmAunits = pdo.SRCSNKAPDO.MaxCurrentIn50mAunits*50;
      PtrRequestPowerDetails->OperatingCurrentInmAunits    = pdo.SRCSNKAPDO.MaxCurrentIn50mAunits*50;
      PtrRequestPowerDetails->MaxOperatingPowerInmWunits   = 0;
      PtrRequestPowerDetails->OperatingPowerInmWunits      = selpower;
      PtrRequestPowerDetails->RequestedVoltageInmVunits    = reqvoltage;

      return curr_index;
    }

    /* For PDO */
    else
    {
    /* check if the received source PDO is matching any of the SNK PDO */
    allowablepower = 0;
    if (USBPD_TRUE == USER_SERV_SNK_EvaluateMatchWithSRCPDO(PortNum, pdo.d32, &voltage, &allowablepower))
    {
      allowablecurrent = (allowablepower / voltage) * 1000;

      /* choose the best PDO depending on the user preferences */
      switch(Method)
      {
        case PDO_SEL_METHOD_MAX_PWR:
          if (allowablepower >= selpower)
          {
            /* Consider the current PDO the best one until now */
            curr_index = temp_index;
            selpower   = allowablepower;
            reqvoltage = voltage;
            selcurrent = allowablecurrent;
          }
          break;
        case PDO_SEL_METHOD_MIN_PWR:
          if ((allowablepower < selpower) || (selpower == 0))
          {
            /* Consider the current PDO the best one until now */
            curr_index = temp_index;
            selpower   = allowablepower;
            reqvoltage = voltage;
            selcurrent = allowablecurrent;
          }
          break;
        case PDO_SEL_METHOD_MAX_VOLT:
          if (voltage >= reqvoltage)
          {
            /* Consider the current PDO the best one until now */
            curr_index = temp_index;
            selpower   = allowablepower;
            reqvoltage = voltage;
            selcurrent = allowablecurrent;
          }
          break;
        case PDO_SEL_METHOD_MIN_VOLT:
          if ((voltage < reqvoltage) || (reqvoltage == 0))
          {
            /* Consider the current PDO the best one until now */
            curr_index = temp_index;
            selpower   = allowablepower;
            reqvoltage = voltage;
            selcurrent = allowablecurrent;
          }
          break;
        case PDO_SEL_METHOD_MAX_CUR:
          if (allowablecurrent >= selcurrent)
          {
            /* Consider the current PDO the best one until now */
            curr_index = temp_index;
            selpower   = allowablepower;
            reqvoltage = voltage;
            selcurrent = allowablecurrent;
          }
          break;
        case PDO_SEL_METHOD_MIN_CUR:
          if ((allowablecurrent < selcurrent) || (selcurrent == 0))
          {
            /* Consider the current PDO the best one until now */
            curr_index = temp_index;
            selpower   = allowablepower;
            reqvoltage = voltage;
            selcurrent = allowablecurrent;
          }
          break;

        default:
          /* Default behavior: last PDO is selected */
          curr_index = temp_index;
          selpower   = allowablepower;
          reqvoltage = voltage;
          selcurrent = allowablecurrent;
      }

  }
    }
  }

  /* If a suitable PDO was found */
  if (curr_index != DPM_NO_SRC_PDO_FOUND)
  {
    /* Fill the request power details */
    PtrRequestPowerDetails->MaxOperatingCurrentInmAunits = puser->DPM_SNKRequestedPower.MaxOperatingCurrentInmAunits;
    PtrRequestPowerDetails->OperatingCurrentInmAunits    = (1000 * selpower)/voltage;
    PtrRequestPowerDetails->MaxOperatingPowerInmWunits   = puser->DPM_SNKRequestedPower.MaxOperatingPowerInmWunits;
    PtrRequestPowerDetails->OperatingPowerInmWunits      = selpower;
    PtrRequestPowerDetails->RequestedVoltageInmVunits    = reqvoltage;
  }

  return curr_index;
}

/**
  * @brief  Build RDO to be used in Request message according to selected PDO from received SRC Capabilities
  * @param  PortNum           Port number
  * @param  IndexSrcPDO       Index on the selected SRC PDO (value between 0 to 6)
  * @param  PtrRequestPowerDetails  Sink requested power details structure pointer
  * @param  Rdo               Pointer on the RDO
  * @param  PtrPowerObject    Pointer on the selected power object
  * @retval None
  */
static void USER_SERV_SNK_BuildRDOfromSelectedPDO(uint8_t PortNum,
                                                  uint8_t IndexSrcPDO,
                                                  USBPD_DPM_SNKPowerRequestDetailsTypeDef* PtrRequestPowerDetails,
                                                  USBPD_SNKRDO_TypeDef* Rdo,
                                                  USBPD_CORE_PDO_Type_TypeDef *PtrPowerObject)
{
  uint32_t mv = 0,ma = 0, size;
  USBPD_PDO_TypeDef  pdo;
  USBPD_SNKRDO_TypeDef rdo;
  USBPD_HandleTypeDef *pdhandle = &DPM_Ports[PortNum];
  USBPD_USER_SettingsTypeDef *puser = (USBPD_USER_SettingsTypeDef *)&DPM_USER_Settings[PortNum];
  uint32_t snkpdolist[USBPD_MAX_NB_PDO];
  USBPD_PDO_TypeDef snk_fixed_pdo;

  /* Initialize RDO */
  rdo.d32 = 0;

  /* Read SNK PDO list for retrieving useful data to fill in RDO */
  USBPD_PWR_IF_GetPortPDOs(PortNum, USBPD_CORE_DATATYPE_SNK_PDO, (uint8_t*)&snkpdolist[0], &size);

  /* Store value of 1st SNK PDO (Fixed) in local variable */
  snk_fixed_pdo.d32 = snkpdolist[0];

  /* Set common fields in RDO */
  pdo.d32 = pdhandle->DPM_ListOfRcvSRCPDO[0];
  rdo.GenericRDO.USBCommunicationsCapable     = snk_fixed_pdo.SNKFixedPDO.USBCommunicationsCapable;
  if (USBPD_SPECIFICATION_REV2 < DPM_Params[PortNum].PE_SpecRevision)
  {
    rdo.FixedVariableRDO.UnchunkedExtendedMessage = DPM_Settings[PortNum].PE_PD3_Support.d.PE_UnchunkSupport;
    DPM_Params[PortNum].PE_UnchunkSupport   = USBPD_FALSE;
    /* Set unchuncked bit if supported by port partner;*/
    if (USBPD_TRUE == pdo.SRCFixedPDO.UnchunkedExtendedMessage)
    {
      DPM_Params[PortNum].PE_UnchunkSupport   = USBPD_TRUE;
    }
  }

  /* If no valid SNK PDO or if no SRC PDO match found (index>=nb of valid received SRC PDOs */
  if ((size < 1) || (IndexSrcPDO >= pdhandle->DPM_NumberOfRcvSRCPDO))
  {
    /* USBPD_DPM_EvaluateCapabilities: Mismatch, could not find desired pdo index */
#ifdef _TRACE
    uint8_t mess[]="DPM_SNK_BuildRDOfromSelectedPDO: Pb in SRC PDO selection";
    USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PortNum, 0, (uint8_t*)mess, sizeof(mess));
#endif /* _TRACE */
    rdo.FixedVariableRDO.ObjectPosition = 1;
    rdo.FixedVariableRDO.OperatingCurrentIn10mAunits  = pdo.SRCFixedPDO.MaxCurrentIn10mAunits;
    rdo.FixedVariableRDO.MaxOperatingCurrent10mAunits = pdo.SRCFixedPDO.MaxCurrentIn10mAunits;
    rdo.FixedVariableRDO.CapabilityMismatch           = 1;
    rdo.FixedVariableRDO.USBCommunicationsCapable     = snk_fixed_pdo.SNKFixedPDO.USBCommunicationsCapable;
    DPM_Ports[PortNum].DPM_RequestedCurrent           = puser->DPM_SNKRequestedPower.MaxOperatingCurrentInmAunits;
    /* USBPD_DPM_EvaluateCapabilities: Mismatch, could not find desired pdo index */

    pdhandle->DPM_RequestDOMsg = rdo.d32;
    return;
  }

  /* Set the Object position */
  rdo.GenericRDO.ObjectPosition               = IndexSrcPDO + 1;
  rdo.GenericRDO.NoUSBSuspend                 = 0;

  /* Extract power information from Power Data Object */
  pdo.d32 = pdhandle->DPM_ListOfRcvSRCPDO[IndexSrcPDO];

  *PtrPowerObject = pdo.GenericPDO.PowerObject;

  /* Retrieve request details from SRC PDO selection */
  mv = PtrRequestPowerDetails->RequestedVoltageInmVunits;
  ma = PtrRequestPowerDetails->OperatingCurrentInmAunits;

  switch(pdo.GenericPDO.PowerObject)
  {
  case USBPD_CORE_PDO_TYPE_FIXED:
    {
      DPM_Ports[PortNum].DPM_RequestedCurrent           = ma;
      rdo.FixedVariableRDO.OperatingCurrentIn10mAunits  = ma / 10;
      rdo.FixedVariableRDO.MaxOperatingCurrent10mAunits = ma / 10;
    }
    break;

  case USBPD_CORE_PDO_TYPE_APDO:
    {
      DPM_Ports[PortNum].DPM_RequestedCurrent    = ma;
      rdo.ProgRDO.ObjectPosition                 = IndexSrcPDO + 1;
      rdo.ProgRDO.OperatingCurrentIn50mAunits    = ma / 50;
      rdo.ProgRDO.OutputVoltageIn20mV            = mv / 20;
    }
    break;
  default:
    break;
  }

  pdhandle->DPM_RequestDOMsg = rdo.d32;
  pdhandle->DPM_RDOPosition  = rdo.GenericRDO.ObjectPosition;

  Rdo->d32 = pdhandle->DPM_RequestDOMsg;
  /* Get the requested voltage */
  pdhandle->DPM_RequestedVoltage = mv;
}
#endif /* _SNK */
/* USER CODE END USBPD_USER_PRIVATE_FUNCTIONS */


