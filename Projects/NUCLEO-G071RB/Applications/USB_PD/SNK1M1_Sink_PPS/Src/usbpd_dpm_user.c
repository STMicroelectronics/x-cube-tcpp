/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_dpm_user.c
  * @author  MCD Application Team
  * @brief   USBPD DPM user code
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

#define USBPD_DPM_USER_C
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbpd_core.h"
#include "usbpd_dpm_user.h"
#include "usbpd_pdo_defs.h"
#include "usbpd_dpm_core.h"
#include "usbpd_dpm_conf.h"
#include "usbpd_vdm_user.h"
#include "usbpd_pwr_if.h"
#include "usbpd_pwr_user.h"
#include "cmsis_os.h"
#if defined(_TRACE)
#include "usbpd_trace.h"
#include "string.h"
#include "stdio.h"
#endif /* _TRACE */
/* USER CODE BEGIN Includes */
#include "usbpd_user_services.h"
#include "usbpd_os_port.h"
/* USER CODE END Includes */

/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @addtogroup STM32_USBPD_APPLICATION_DPM_USER
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN Private_Typedef */

/* USER CODE END Private_Typedef */

/* Private define ------------------------------------------------------------*/
/** @defgroup USBPD_USER_PRIVATE_DEFINES USBPD USER Private Defines
  * @{
  */
#define DPM_GUI_NOTIF_ISCONNECTED       (1 << 5)
#define DPM_GUI_NOTIF_POWER_EVENT       (1 << 15)
#if (osCMSIS < 0x20000U)
void                USBPD_DPM_UserExecute(void const *argument);
#else
void                USBPD_DPM_UserExecute(void *argument);
#endif /* osCMSIS < 0x20000U */
/* USER CODE BEGIN Private_Define */
#define DPM_BOX_MESSAGES_MAX      30u

#define DPM_TIMER_ENABLE_MSK      ((uint16_t)0x8000U)       /*!< Enable Timer Mask                                                        */
#define DPM_TIMER_READ_MSK        ((uint16_t)0x7FFFU)       /*!< Read Timer Mask                                                          */

#if defined(_RTOS) || defined(USBPD_THREADX)
#if defined(_RTOS)
#if (osCMSIS < 0x20000U)
#define DPM_USER_STACK_SIZE_ADDON_FOR_OS              1
#else
#define DPM_USER_STACK_SIZE_ADDON_FOR_OS              3
#endif /* osCMSIS < 0x20000U */
#define OS_DPM_PRIORITY                               osPriorityLow
#elif defined(USBPD_THREADX)
#define DPM_USER_STACK_SIZE_ADDON_FOR_OS              3
#define OS_DPM_PRIORITY                               10

#endif /* _RTOS */
#define OS_DPM_STACK_SIZE                             (300U * DPM_USER_STACK_SIZE_ADDON_FOR_OS)
#endif /* _RTOS || USBPD_THREADX */

#if _SRC_CAPA_EXT
/* Timer used to send a GET_SRC_CAPA_EXT*/
#define DPM_TIMER_GET_SRC_CAPA_EXT                    300u  /*!< 300ms */
#endif /* _SRC_CAPA_EXT */

/* USER CODE END Private_Define */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/** @defgroup USBPD_USER_PRIVATE_MACROS USBPD USER Private Macros
  * @{
  */
#if defined(_TRACE)
#define DPM_USER_DEBUG_TRACE_SIZE       50u
#define DPM_USER_DEBUG_TRACE(_PORT_, ...)  do {                                                                \
      char _str[DPM_USER_DEBUG_TRACE_SIZE];                                                                    \
      uint8_t _size = snprintf(_str, DPM_USER_DEBUG_TRACE_SIZE, __VA_ARGS__);                                  \
      if (_size < DPM_USER_DEBUG_TRACE_SIZE)                                                                   \
        USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (uint8_t)(_PORT_), 0, (uint8_t*)_str, strlen(_str));                \
      else                                                                                                     \
        USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (uint8_t)(_PORT_), 0, (uint8_t*)_str, DPM_USER_DEBUG_TRACE_SIZE);   \
  } while(0)

#define DPM_USER_ERROR_TRACE(_PORT_, _STATUS_, ...)  do {                                                      \
    if (USBPD_OK != _STATUS_) {                                                                                \
        char _str[DPM_USER_DEBUG_TRACE_SIZE];                                                                  \
        uint8_t _size = snprintf(_str, DPM_USER_DEBUG_TRACE_SIZE, __VA_ARGS__);                                \
        if (_size < DPM_USER_DEBUG_TRACE_SIZE)                                                                 \
          USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (uint8_t)(_PORT_), 0, (uint8_t*)_str, strlen(_str));              \
        else                                                                                                   \
          USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (uint8_t)(_PORT_), 0, (uint8_t*)_str, DPM_USER_DEBUG_TRACE_SIZE); \
    }                                                                                                          \
  } while(0)
#else
#define DPM_USER_DEBUG_TRACE(_PORT_, ...)
#define DPM_USER_ERROR_TRACE(_PORT_, _STATUS_, ...)
#endif /* _TRACE */
/* USER CODE BEGIN Private_Macro */

/* USER CODE END Private_Macro */
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup USBPD_USER_PRIVATE_VARIABLES USBPD USER Private Variables
  * @{
  */
GUI_NOTIFICATION_POST         DPM_GUI_PostNotificationMessage   = NULL;
GUI_NOTIFICATION_FORMAT_SEND  DPM_GUI_FormatAndSendNotification = NULL;
GUI_SAVE_INFO                 DPM_GUI_SaveInfo                  = NULL;

/* USER CODE BEGIN Private_Variables */
extern USBPD_HandleTypeDef DPM_Ports[USBPD_PORT_COUNT];
static OS_QUEUE_ID DPMMsgBox;
static OS_TASK_ID DPMThread;
/* USER CODE END Private_Variables */
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup USBPD_USER_PRIVATE_FUNCTIONS USBPD USER Private Functions
  * @{
  */
/* USER CODE BEGIN USBPD_USER_PRIVATE_FUNCTIONS_Prototypes */
#if defined(_RTOS)
static uint32_t CheckDPMTimers(void);
#endif /* _RTOS */


/* USER CODE END USBPD_USER_PRIVATE_FUNCTIONS_Prototypes */
/**
  * @}
  */

/* Exported functions ------- ------------------------------------------------*/
/** @defgroup USBPD_USER_EXPORTED_FUNCTIONS USBPD USER Exported Functions
  * @{
  */
/* USER CODE BEGIN USBPD_USER_EXPORTED_FUNCTIONS */

/* USER CODE END USBPD_USER_EXPORTED_FUNCTIONS */

/** @defgroup USBPD_USER_EXPORTED_FUNCTIONS_GROUP1 USBPD USER Exported Functions called by DPM CORE
  * @{
  */
/* USER CODE BEGIN USBPD_USER_EXPORTED_FUNCTIONS_GROUP1 */

/* USER CODE END USBPD_USER_EXPORTED_FUNCTIONS_GROUP1 */

/**
  * @brief  Initialize DPM (port power role, PWR_IF, CAD and PE Init procedures)
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_UserInit(void)
{
/* USER CODE BEGIN USBPD_DPM_UserInit */

  /* PWR SET UP */
  if(USBPD_OK !=  USBPD_PWR_IF_Init())
  {
    return USBPD_ERROR;
  }

#if defined(_RTOS) || defined(USBPD_THREADX)
  OS_INIT();
  OS_CREATE_QUEUE(DPMMsgBox, "DPM", DPM_BOX_MESSAGES_MAX, sizeof(uint32_t));
  OS_DEFINE_TASK(DPM, USBPD_DPM_UserExecute, OS_DPM_PRIORITY, OS_DPM_STACK_SIZE, &DPMMsgBox);
  OS_CREATE_TASK(DPMThread, DPM, USBPD_DPM_UserExecute, OS_DPM_PRIORITY, OS_DPM_STACK_SIZE, &DPMMsgBox);
#endif /* _RTOS || USBPD_THREADX */
#if defined(USBPD_THREADX)
  /* In FREERTOS mode, this function is called by GUI_Init() and does not need a MemoryPtr */
  GUI_InitOS(MemoryPtr);
#endif /* USBPD_THREADX */

  return USBPD_OK;

#if defined(_RTOS) || defined(USBPD_THREADX)
error:
  return _retr;
#endif /* _RTOS || USBPD_THREADX */
/* USER CODE END USBPD_DPM_UserInit */
}

/**
  * @brief  Function to set the function ptr linked to GUI interface
  * @param  PtrFormatSend Pointer on function to format and send GUI notifications
  * @param  PtrPost       Pointer on function to send GUI notifications
  * @param  PtrSaveInfo   Pointer on function to save information from Port Partner
  * @retval None
  */
void USBPD_DPM_SetNotification_GUI(GUI_NOTIFICATION_FORMAT_SEND PtrFormatSend, GUI_NOTIFICATION_POST PtrPost, GUI_SAVE_INFO PtrSaveInfo)
{
  DPM_GUI_PostNotificationMessage   = PtrPost;
  DPM_GUI_FormatAndSendNotification = PtrFormatSend;
  DPM_GUI_SaveInfo                  = PtrSaveInfo;
}

/**
  * @brief  User delay implementation which is OS dependent
  * @param  Time time in ms
  * @retval None
  */
void USBPD_DPM_WaitForTime(uint32_t Time)
{
  osDelay(Time);
}

/**
  * @brief  User processing time, it is recommended to avoid blocking task for long time
  * @param  argument  DPM User event
  * @retval None
  */
#if (osCMSIS < 0x20000U)
void USBPD_DPM_UserExecute(void const *argument)
#else
void USBPD_DPM_UserExecute(void *argument)
#endif /* osCMSIS < 0x20000U */
{
/* USER CODE BEGIN USBPD_DPM_UserExecute */
#if defined(_RTOS)
  uint32_t _timing = osWaitForever;
  osMessageQId  queue = *(osMessageQId *)argument;

  do
  {
#if (osCMSIS < 0x20000U)
    osEvent event = osMessageGet(queue, _timing);
    switch (((DPM_USER_EVENT)event.value.v & 0xF))
#else
    uint32_t event;
    (void)osMessageQueueGet(queue, &event, NULL, _timing);
    switch (((DPM_USER_EVENT)event & 0xF))
#endif /* osCMSIS < 0x20000U */
    {
    case DPM_USER_EVENT_TIMER:
      {
#if defined(USBPD_REV30_SUPPORT)
#if _SRC_CAPA_EXT
        /* Manage the extended capa */
        DPM_ManageExtendedCapa();
#endif /* _SRC_CAPA_EXT */
#endif /* USBPD_REV30_SUPPORT */
        break;
      }

    default:
      break;
    }
    _timing = CheckDPMTimers();
  }
  while(1);
#elif defined(USBPD_THREADX)
#else
#if defined(_GUI_INTERFACE)
  GUI_Execute();
#endif /* _GUI_INTERFACE */
#endif /* _RTOS */
/* USER CODE END USBPD_DPM_UserExecute */
}

/**
  * @brief  UserCableDetection reporting events on a specified port from CAD layer.
  * @param  PortNum The handle of the port
  * @param  State CAD state
  * @retval None
  */
void USBPD_DPM_UserCableDetection(uint8_t PortNum, USBPD_CAD_EVENT State)
{
  switch(State)
  {
  case USBPD_CAD_EVENT_ATTEMC:
  case USBPD_CAD_EVENT_ATTACHED:
    /* Format and send a notification to GUI if enabled */
    if (NULL != DPM_GUI_FormatAndSendNotification)
    {
      DPM_GUI_FormatAndSendNotification(PortNum, DPM_GUI_NOTIF_ISCONNECTED, 0);
    }
    break;
  default :
    /* Format and send a notification to GUI if enabled */
    if (NULL != DPM_GUI_FormatAndSendNotification)
    {
      DPM_GUI_FormatAndSendNotification(PortNum, DPM_GUI_NOTIF_ISCONNECTED | DPM_GUI_NOTIF_POWER_EVENT, 0);
    }
  }
/* USER CODE BEGIN USBPD_DPM_UserCableDetection */
  switch(State)
  {
  case USBPD_CAD_EVENT_ATTEMC:
    DPM_Ports[PortNum].DPM_IsConnected = 1;
    break;

  case USBPD_CAD_EVENT_ATTACHED:
    /* Format and send a notification to GUI if enabled */
    DPM_Ports[PortNum].DPM_IsConnected = 1;
    break;

  case USBPD_CAD_EVENT_DETACHED :
  case USBPD_CAD_EVENT_EMC :
  default :
    /* reset all values received from port partner */
    memset(&DPM_Ports[PortNum], 0, sizeof(DPM_Ports[PortNum]));
    break;
  }
/* USER CODE END USBPD_DPM_UserCableDetection */
}

/**
  * @brief  function used to manage user timer.
  * @param  PortNum Port number
  * @retval None
  */
void USBPD_DPM_UserTimerCounter(uint8_t PortNum)
{
/* USER CODE BEGIN USBPD_DPM_UserTimerCounter */

/* USER CODE END USBPD_DPM_UserTimerCounter */
}

/**
  * @}
  */

/** @defgroup USBPD_USER_EXPORTED_FUNCTIONS_GROUP2 USBPD USER Exported Callbacks functions called by PE
  * @{
  */

/**
  * @brief  Callback function called by PE to inform DPM about PE event.
  * @param  PortNum The current port number
  * @param  EventVal @ref USBPD_NotifyEventValue_TypeDef
  * @retval None
  */
void USBPD_DPM_Notification(uint8_t PortNum, USBPD_NotifyEventValue_TypeDef EventVal)
{
  /* Forward PE notifications to GUI if enabled */
  if (NULL != DPM_GUI_PostNotificationMessage)
  {
    DPM_GUI_PostNotificationMessage(PortNum, EventVal);
  }
/* USER CODE BEGIN USBPD_DPM_Notification */
  /* Manage event notified by the stack? */
  switch (EventVal)
  {
//    case USBPD_NOTIFY_POWER_EXPLICIT_CONTRACT :
//      break;
    case USBPD_NOTIFY_REQUEST_ACCEPTED:
      /* Update DPM_RDOPosition only if current role is SNK */
      if (USBPD_PORTPOWERROLE_SNK == DPM_Params[PortNum].PE_PowerRole)
      {
        USBPD_SNKRDO_TypeDef rdo;
        rdo.d32                             = DPM_Ports[PortNum].DPM_RequestDOMsg;
        DPM_Ports[PortNum].DPM_RDOPosition  = rdo.GenericRDO.ObjectPosition;
        if (NULL != DPM_GUI_SaveInfo)
        {
          DPM_GUI_SaveInfo(PortNum, USBPD_CORE_DATATYPE_RDO_POSITION, (uint8_t*)&DPM_Ports[PortNum].DPM_RDOPosition, 4);
        }
      }
      break;
//    case USBPD_NOTIFY_REQUEST_REJECTED:
//    case USBPD_NOTIFY_REQUEST_WAIT:
//      break;
//    case USBPD_NOTIFY_POWER_SWAP_TO_SNK_DONE:
//      break;
//    case USBPD_NOTIFY_STATE_SNK_READY:
//      break;
//    case USBPD_NOTIFY_HARDRESET_RX:
//    case USBPD_NOTIFY_HARDRESET_TX:
//      break;
//    case USBPD_NOTIFY_STATE_SRC_DISABLED:
//      break;
//    case USBPD_NOTIFY_ALERT_RECEIVED :
//      break;
//    case USBPD_NOTIFY_CABLERESET_REQUESTED :
//      break;
//    case USBPD_NOTIFY_MSG_NOT_SUPPORTED :
//      break;
//    case USBPD_NOTIFY_PE_DISABLED :
//      break;
//    case USBPD_NOTIFY_USBSTACK_START:
//      break;
//    case USBPD_NOTIFY_USBSTACK_STOP:
//      break;
//    case USBPD_NOTIFY_DATAROLESWAP_DFP :
//      break;
//    case USBPD_NOTIFY_DATAROLESWAP_UFP :
//      break;
    default:
      DPM_USER_DEBUG_TRACE(PortNum, "ADVICE: USBPD_DPM_Notification:%d", EventVal);
      break;
  }
/* USER CODE END USBPD_DPM_Notification */
}

/**
  * @brief  Callback function called by PE layer when HardReset message received from PRL
  * @param  PortNum The current port number
  * @param  CurrentRole the current role
  * @param  Status status on hard reset event
  * @retval None
  */
void USBPD_DPM_HardReset(uint8_t PortNum, USBPD_PortPowerRole_TypeDef CurrentRole, USBPD_HR_Status_TypeDef Status)
{
/* USER CODE BEGIN USBPD_DPM_HardReset */
  DPM_USER_DEBUG_TRACE(PortNum, "ADVICE: update USBPD_DPM_HardReset");
/* USER CODE END USBPD_DPM_HardReset */
}

/**
  * @brief  DPM callback to allow PE to retrieve information from DPM/PWR_IF.
  * @param  PortNum Port number
  * @param  DataId  Type of data to be updated in DPM based on @ref USBPD_CORE_DataInfoType_TypeDef
  * @param  Ptr     Pointer on address where DPM data should be written (u8 pointer)
  * @param  Size    Pointer on nb of u8 written by DPM
  * @retval None
  */
void USBPD_DPM_GetDataInfo(uint8_t PortNum, USBPD_CORE_DataInfoType_TypeDef DataId, uint8_t *Ptr, uint32_t *Size)
{
/* USER CODE BEGIN USBPD_DPM_GetDataInfo */
  /* Check type of information targeted by request */
  switch (DataId)
  {
    case USBPD_CORE_DATATYPE_SNK_PDO:           /*!< Handling of port Sink PDO, requested by get sink capa*/
      USBPD_PWR_IF_GetPortPDOs(PortNum, DataId, Ptr, Size);
      *Size *= 4;
      break;
//  case USBPD_CORE_EXTENDED_CAPA:              /*!< Source Extended capability message content          */
    // break;
    case USBPD_CORE_DATATYPE_REQ_VOLTAGE:       /*!< Get voltage value requested for BIST tests, expect 5V*/
      *Size = 4;
      (void)memcpy((uint8_t*)Ptr, (uint8_t *)&DPM_Ports[PortNum].DPM_RequestedVoltage, *Size);
      break;
//  case USBPD_CORE_INFO_STATUS:                /*!< Information status message content                  */
    // break;
//  case USBPD_CORE_MANUFACTURER_INFO:          /*!< Retrieve of Manufacturer info message content       */
    // break;
//  case USBPD_CORE_BATTERY_STATUS:             /*!< Retrieve of Battery status message content          */
    // break;
//  case USBPD_CORE_BATTERY_CAPABILITY:         /*!< Retrieve of Battery capability message content      */
    // break;
    case USBPD_CORE_REVISION:
      {
        *Size = sizeof(USBPD_RevisionDO_TypeDef);
        USBPD_RevisionDO_TypeDef rev =
        {
          /* Hardcoded values as example, user may want to use a global USBPD_RevisionDO_TypeDef variable */
          .b.Revision_major = 3,                  /*!< Major revision */
          .b.Revision_minor = 1,                  /*!< Minor revision */
          .b.Version_major  = 1,                  /*!< Major version  */
          .b.Version_minor  = 7,                  /*!< Minor version  */
        };
        memcpy((uint8_t *)Ptr, &rev, *Size);
      }
      break;

    case USBPD_CORE_PPS_STATUS :
      {
        /* Get current drawn by sink */
        USBPD_PPSSDB_TypeDef pps_status = {0};

        *Size = 4;
        (void)memcpy((uint8_t*)Ptr, (uint8_t *)&pps_status.d32, *Size);
      }
      break;

    case USBPD_CORE_SNK_EXTENDED_CAPA :
      {
        *Size = sizeof(USBPD_SKEDB_TypeDef);
        memcpy((uint8_t*)Ptr, (uint8_t *)&DPM_USER_Settings[PortNum].DPM_SNKExtendedCapa, *Size);
      }
      break;

    default:
      DPM_USER_DEBUG_TRACE(PortNum, "ADVICE: update USBPD_DPM_GetDataInfo:%d", DataId);
      break;
  }
/* USER CODE END USBPD_DPM_GetDataInfo */
}

/**
  * @brief  DPM callback to allow PE to update information in DPM/PWR_IF.
  * @param  PortNum Port number
  * @param  DataId  Type of data to be updated in DPM based on @ref USBPD_CORE_DataInfoType_TypeDef
  * @param  Ptr     Pointer on the data
  * @param  Size    Nb of bytes to be updated in DPM
  * @retval None
  */
void USBPD_DPM_SetDataInfo(uint8_t PortNum, USBPD_CORE_DataInfoType_TypeDef DataId, uint8_t *Ptr, uint32_t Size)
{
/* USER CODE BEGIN USBPD_DPM_SetDataInfo */
  /* Check type of information targeted by request */
  switch (DataId)
  {
    case USBPD_CORE_DATATYPE_RDO_POSITION:      /*!< Reset the PDO position selected by the sink only */
      if (Size == 4)
      {
        uint8_t* temp;
        temp = (uint8_t*)&DPM_Ports[PortNum].DPM_RDOPosition;
        (void)memcpy(temp, Ptr, Size);
        DPM_Ports[PortNum].DPM_RDOPositionPrevious = *Ptr;
        temp = (uint8_t*)&DPM_Ports[PortNum].DPM_RDOPositionPrevious;
        (void)memcpy(temp, Ptr, Size);
      }

        break;
     case USBPD_CORE_DATATYPE_RCV_SRC_PDO:         /*!< Storage of Received Source PDO values        */
         USBPD_USER_SERV_StoreSRCPDO(PortNum, Ptr, Size);
         break;
    //  case USBPD_CORE_DATATYPE_RCV_SNK_PDO:       /*!< Storage of Received Sink PDO values          */
       // break;
    //  case USBPD_CORE_EXTENDED_CAPA:              /*!< Source Extended capability message content   */
       // break;
    //  case USBPD_CORE_PPS_STATUS:                 /*!< PPS Status message content                   */
       // break;
    //  case USBPD_CORE_INFO_STATUS:                /*!< Information status message content           */
       // break;
    //  case USBPD_CORE_ALERT:                      /*!< Storing of received Alert message content    */
       // break;
    //  case USBPD_CORE_GET_MANUFACTURER_INFO:      /*!< Storing of received Get Manufacturer info message content */
       // break;
    //  case USBPD_CORE_GET_BATTERY_STATUS:         /*!< Storing of received Get Battery status message content    */
       // break;
    //  case USBPD_CORE_GET_BATTERY_CAPABILITY:     /*!< Storing of received Get Battery capability message content*/
        // break;
    case USBPD_CORE_DATATYPE_RCV_REQ_PDO :
      if (Size == 4)
      {
        uint8_t* rdo;
        rdo = (uint8_t*)&DPM_Ports[PortNum].DPM_RcvRequestDOMsg;
        (void)memcpy(rdo, Ptr, Size);
      }
      break;

    case USBPD_CORE_PPS_STATUS :
      {
        uint8_t*  ext_capa;
        ext_capa = (uint8_t*)&DPM_Ports[PortNum].DPM_RcvPPSStatus;
        memcpy(ext_capa, Ptr, Size);
      }
      break;
    case USBPD_CORE_SNK_EXTENDED_CAPA :
      {
        uint8_t*  _snk_ext_capa;
        _snk_ext_capa = (uint8_t*)&DPM_Ports[PortNum].DPM_RcvSNKExtendedCapa;
        memcpy(_snk_ext_capa, Ptr, Size);
      }
      break;

    default:
      DPM_USER_DEBUG_TRACE(PortNum, "ADVICE: update USBPD_DPM_SetDataInfo:%d", DataId);
      break;
  }
/* USER CODE END USBPD_DPM_SetDataInfo */

  /* Forward info to GUI if enabled */
  if (NULL != DPM_GUI_SaveInfo)
  {
    DPM_GUI_SaveInfo(PortNum, DataId, Ptr, Size);
  }
}

/**
  * @brief  Evaluate received Capabilities Message from Source port and prepare the request message
  * @param  PortNum         Port number
  * @param  PtrRequestData  Pointer on selected request data object
  * @param  PtrPowerObjectType  Pointer on the power data object
  * @retval None
  */
void USBPD_DPM_SNK_EvaluateCapabilities(uint8_t PortNum, uint32_t *PtrRequestData, USBPD_CORE_PDO_Type_TypeDef *PtrPowerObjectType)
{
/* USER CODE BEGIN USBPD_DPM_SNK_EvaluateCapabilities */
  USBPD_USER_SERV_EvaluateCapa(PortNum, PtrRequestData, PtrPowerObjectType);
/* USER CODE END USBPD_DPM_SNK_EvaluateCapabilities */
}

/**
  * @brief  Callback to be used by PE to evaluate a Vconn swap
  * @param  PortNum Port number
  * @retval USBPD_ACCEPT, USBPD_REJECT, USBPD_WAIT
  */
USBPD_StatusTypeDef USBPD_DPM_EvaluateVconnSwap(uint8_t PortNum)
{
/* USER CODE BEGIN USBPD_DPM_EvaluateVconnSwap */
  USBPD_StatusTypeDef status = USBPD_REJECT;
  if (USBPD_TRUE == DPM_USER_Settings[PortNum].PE_VconnSwap)
  {
    status = USBPD_ACCEPT;
  }

  return status;
/* USER CODE END USBPD_DPM_EvaluateVconnSwap */
}

/**
  * @brief  Callback to be used by PE to manage VConn
  * @param  PortNum Port number
  * @param  State Enable or Disable VConn on CC lines
  * @retval USBPD_ACCEPT, USBPD_REJECT
  */
USBPD_StatusTypeDef USBPD_DPM_PE_VconnPwr(uint8_t PortNum, USBPD_FunctionalState State)
{
/* USER CODE BEGIN USBPD_DPM_PE_VconnPwr */
  return USBPD_ERROR;
/* USER CODE END USBPD_DPM_PE_VconnPwr */
}

/**
  * @brief  DPM callback to allow PE to forward extended message information.
  * @param  PortNum Port number
  * @param  MsgType Type of message to be handled in DPM
  *         This parameter can be one of the following values:
  *           @arg @ref USBPD_EXT_SECURITY_REQUEST Security Request extended message
  *           @arg @ref USBPD_EXT_SECURITY_RESPONSE Security Response extended message
  * @param  ptrData   Pointer on address Extended Message data could be read (u8 pointer)
  * @param  DataSize  Nb of u8 that compose Extended message
  * @retval None
  */
void USBPD_DPM_ExtendedMessageReceived(uint8_t PortNum, USBPD_ExtendedMsg_TypeDef MsgType, uint8_t *ptrData, uint16_t DataSize)
{
/* USER CODE BEGIN USBPD_DPM_ExtendedMessageReceived */

/* USER CODE END USBPD_DPM_ExtendedMessageReceived */
}

/**
  * @brief  DPM callback to allow PE to enter ERROR_RECOVERY state.
  * @param  PortNum Port number
  * @retval None
  */
void USBPD_DPM_EnterErrorRecovery(uint8_t PortNum)
{
/* USER CODE BEGIN USBPD_DPM_EnterErrorRecovery */
  /* Inform CAD to enter recovery mode */
  USBPD_CAD_EnterErrorRecovery(PortNum);
/* USER CODE END USBPD_DPM_EnterErrorRecovery */
}

/**
  * @brief  Callback used to ask application the reply status for a DataRoleSwap request
  * @note   if the callback is not set (ie NULL) the stack will automatically reject the request
  * @param  PortNum Port number
  * @retval Returned values are:
            @ref USBPD_ACCEPT if DRS can be accepted
            @ref USBPD_REJECT if DRS is not accepted in one data role (DFP or UFP) or in PD2.0 config
            @ref USBPD_NOTSUPPORTED if DRS is not supported at all by the application (in both data roles) - P3.0 only
  */
USBPD_StatusTypeDef USBPD_DPM_EvaluateDataRoleSwap(uint8_t PortNum)
{
/* USER CODE BEGIN USBPD_DPM_EvaluateDataRoleSwap */
  USBPD_StatusTypeDef status = USBPD_REJECT;
  /* Sent NOT_SUPPORTED if DRS is not supported at all by the application (in both data roles) - P3.0 only */
  if ((USBPD_FALSE == DPM_USER_Settings[PortNum].PE_DataSwap)
    || ((USBPD_FALSE == DPM_USER_Settings[PortNum].PE_DR_Swap_To_DFP)
    && (USBPD_FALSE == DPM_USER_Settings[PortNum].PE_DR_Swap_To_UFP)))
  {
    status = USBPD_NOTSUPPORTED;
  }
  else
  {
    /* ACCEPT DRS if at least supported by 1 data role */
    if (((USBPD_TRUE == DPM_USER_Settings[PortNum].PE_DR_Swap_To_DFP) && (USBPD_PORTDATAROLE_UFP == DPM_Params[PortNum].PE_DataRole))
       || ((USBPD_TRUE == DPM_USER_Settings[PortNum].PE_DR_Swap_To_UFP) && (USBPD_PORTDATAROLE_DFP == DPM_Params[PortNum].PE_DataRole)))
    {
      status = USBPD_ACCEPT;
    }
  }
  return status;
/* USER CODE END USBPD_DPM_EvaluateDataRoleSwap */
}

/**
  * @brief  Callback to be used by PE to check is VBUS is ready or present
  * @param  PortNum Port number
  * @param  Vsafe   Vsafe status based on @ref USBPD_VSAFE_StatusTypeDef
  * @retval USBPD_DISABLE or USBPD_ENABLE
  */
USBPD_FunctionalState USBPD_DPM_IsPowerReady(uint8_t PortNum, USBPD_VSAFE_StatusTypeDef Vsafe)
{
/* USER CODE BEGIN USBPD_DPM_IsPowerReady */
  return ((USBPD_OK == USBPD_PWR_IF_SupplyReady(PortNum, Vsafe)) ? USBPD_ENABLE : USBPD_DISABLE);
/* USER CODE END USBPD_DPM_IsPowerReady */
}

/**
  * @}
  */

/** @defgroup USBPD_USER_EXPORTED_FUNCTIONS_GROUP3 USBPD USER Functions PD messages requests
  * @{
  */

/**
  * @brief  Request the PE to send a hard reset
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestHardReset(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_HardReset(PortNum);
  DPM_USER_ERROR_TRACE(PortNum, _status, "HARD RESET not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a cable reset.
  * @note   Only a DFP Shall generate Cable Reset Signaling. A DFP Shall only generate Cable Reset Signaling within an Explicit Contract.
            The DFP has to be supplying VCONN prior to a Cable Reset
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestCableReset(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CableReset(PortNum);
  DPM_USER_ERROR_TRACE(PortNum, _status, "CABLE RESET not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a GOTOMIN message
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGotoMin(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_GOTOMIN, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GOTOMIN not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a PING message
  * @note   In USB-PD stack, only ping management for P3.0 is implemented.
  *         If PD2.0 is used, PING timer needs to be implemented on user side.
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestPing(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_PING, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "PING not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a request message.
  * @param  PortNum     The current port number
  * @param  IndexSrcPDO Index on the selected SRC PDO (value between 1 to 7)
  * @param  RequestedVoltage Requested voltage (in MV and use mainly for APDO)
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestMessageRequest(uint8_t PortNum, uint8_t IndexSrcPDO, uint16_t RequestedVoltage)
{
  USBPD_StatusTypeDef _status = USBPD_ERROR;
/* USER CODE BEGIN USBPD_DPM_RequestMessageRequest */
  uint32_t voltage, allowablepower;
  USBPD_SNKRDO_TypeDef rdo;
  USBPD_PDO_TypeDef  pdo;
  USBPD_CORE_PDO_Type_TypeDef pdo_object;
  USBPD_USER_SettingsTypeDef *puser = (USBPD_USER_SettingsTypeDef *)&DPM_USER_Settings[PortNum];
  USBPD_DPM_SNKPowerRequestDetailsTypeDef request_details;
  rdo.d32 = 0;

  /* selected SRC PDO */
  pdo.d32 = DPM_Ports[PortNum].DPM_ListOfRcvSRCPDO[(IndexSrcPDO - 1)];
  voltage = RequestedVoltage;
  allowablepower = (puser->DPM_SNKRequestedPower.MaxOperatingCurrentInmAunits * RequestedVoltage) / 1000U;

  if (USBPD_TRUE == USER_SERV_SNK_EvaluateMatchWithSRCPDO(PortNum, pdo.d32, &voltage, &allowablepower))
  {
    /* Check that voltage has been correctly selected */
    if (RequestedVoltage == voltage)
    {
      request_details.RequestedVoltageInmVunits    = RequestedVoltage;
      request_details.OperatingCurrentInmAunits    = (1000U * allowablepower)/RequestedVoltage;
      request_details.MaxOperatingCurrentInmAunits = puser->DPM_SNKRequestedPower.MaxOperatingCurrentInmAunits;
      request_details.MaxOperatingPowerInmWunits   = puser->DPM_SNKRequestedPower.MaxOperatingPowerInmWunits;
      request_details.OperatingPowerInmWunits      = puser->DPM_SNKRequestedPower.OperatingPowerInmWunits;

      USER_SERV_SNK_BuildRDOfromSelectedPDO(PortNum, (IndexSrcPDO - 1), &request_details, &rdo, &pdo_object);

      _status = USBPD_PE_Send_Request(PortNum, rdo.d32, pdo_object);
    }
  }
/* USER CODE END USBPD_DPM_RequestMessageRequest */
  DPM_USER_ERROR_TRACE(PortNum, _status, "REQUEST not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a GET_SRC_CAPA message
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetSourceCapability(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_GET_SRC_CAP, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_SRC_CAPA not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a GET_SNK_CAPA message
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetSinkCapability(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_GET_SNK_CAP, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_SINK_CAPA not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to perform a Data Role Swap.
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestDataRoleSwap(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_DR_SWAP, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "DRS not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to perform a Power Role Swap.
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestPowerRoleSwap(uint8_t PortNum)
{
  DPM_USER_ERROR_TRACE(PortNum, USBPD_ERROR, "PRS not accepted by the stack");
  return USBPD_ERROR;
}

/**
  * @brief  Request the PE to perform a VCONN Swap.
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestVconnSwap(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_VCONN_SWAP, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "VCS not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a soft reset
  * @param  PortNum The current port number
  * @param  SOPType SOP Type based on @ref USBPD_SOPType_TypeDef
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestSoftReset(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_SOFT_RESET, SOPType);
  DPM_USER_ERROR_TRACE(PortNum, _status, "SOFT_RESET not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a Source Capability message.
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestSourceCapability(uint8_t PortNum)
{
  /* PE will directly get the PDO saved in structure @ref PWR_Port_PDO_Storage */
  USBPD_StatusTypeDef _status = USBPD_PE_Request_DataMessage(PortNum, USBPD_DATAMSG_SRC_CAPABILITIES, NULL);
  DPM_USER_ERROR_TRACE(PortNum, _status, "SRC_CAPA not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a VDM discovery identity
  * @param  PortNum The current port number
  * @param  SOPType SOP Type
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestVDM_DiscoveryIdentify(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType)
{
  USBPD_StatusTypeDef _status = USBPD_ERROR;
/* USER CODE BEGIN USBPD_DPM_RequestVDM_DiscoveryIdentify */
  if (USBPD_SOPTYPE_SOP == SOPType)
  {
    _status = USBPD_PE_SVDM_RequestIdentity(PortNum, SOPType);
  }
/* USER CODE END USBPD_DPM_RequestVDM_DiscoveryIdentify */
  DPM_USER_ERROR_TRACE(PortNum, _status, "VDM Discovery Ident not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a VDM discovery SVID
  * @param  PortNum The current port number
  * @param  SOPType SOP Type
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestVDM_DiscoverySVID(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType)
{
  USBPD_StatusTypeDef _status = USBPD_PE_SVDM_RequestSVID(PortNum, SOPType);
  DPM_USER_ERROR_TRACE(PortNum, _status, "VDM discovery SVID not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to perform a VDM Discovery mode message on one SVID.
  * @param  PortNum The current port number
  * @param  SOPType SOP Type
  * @param  SVID    SVID used for discovery mode message
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestVDM_DiscoveryMode(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, uint16_t SVID)
{
  USBPD_StatusTypeDef _status = USBPD_PE_SVDM_RequestMode(PortNum, SOPType, SVID);
  DPM_USER_ERROR_TRACE(PortNum, _status, "VDM Discovery mode not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to perform a VDM mode enter.
  * @param  PortNum   The current port number
  * @param  SOPType   SOP Type
  * @param  SVID      SVID used for discovery mode message
  * @param  ModeIndex Index of the mode to be entered
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestVDM_EnterMode(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, uint16_t SVID, uint8_t ModeIndex)
{
  USBPD_StatusTypeDef _status = USBPD_PE_SVDM_RequestModeEnter(PortNum, SOPType, SVID, ModeIndex);
  DPM_USER_ERROR_TRACE(PortNum, _status, "VDM mode enter not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to perform a VDM mode exit.
  * @param  PortNum   The current port number
  * @param  SOPType   SOP Type
  * @param  SVID      SVID used for discovery mode message
  * @param  ModeIndex Index of the mode to be exit
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestVDM_ExitMode(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, uint16_t SVID, uint8_t ModeIndex)
{
  USBPD_StatusTypeDef _status = USBPD_PE_SVDM_RequestModeExit(PortNum, SOPType, SVID, ModeIndex);
  DPM_USER_ERROR_TRACE(PortNum, _status, "VDM mode exit not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a Display Port status
  * @param  PortNum   The current port number
  * @param  SOPType   SOP Type
  * @param  SVID      Used SVID
  * @param  pDPStatus Pointer on DP Status data (32 bit)
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestDisplayPortStatus(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, uint16_t SVID, uint32_t *pDPStatus)
{
  USBPD_StatusTypeDef _status;
/* USER CODE BEGIN USBPD_DPM_RequestDisplayPortStatus */
  /*USBPD_VDM_FillDPStatus(PortNum, (USBPD_DPStatus_TypeDef*)pDPStatus);*/
/* USER CODE END USBPD_DPM_RequestDisplayPortStatus */
  _status = USBPD_PE_SVDM_RequestSpecific(PortNum, SOPType, SVDM_SPECIFIC_1, SVID);
  DPM_USER_ERROR_TRACE(PortNum, _status, "Display Port status not accepted by the stack");
  return _status;
}
/**
  * @brief  Request the PE to send a Display Port Config
  * @param  PortNum   The current port number
  * @param  SOPType   SOP Type
  * @param  SVID      Used SVID
  * @param  pDPConfig Pointer on DP Config data (32 bit)
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestDisplayPortConfig(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, uint16_t SVID, uint32_t *pDPConfig)
{
  USBPD_StatusTypeDef _status;
/* USER CODE BEGIN USBPD_DPM_RequestDisplayPortConfig */
  /*USBPD_VDM_FillDPConfig(PortNum, (USBPD_DPConfig_TypeDef*)pDPConfig);*/
/* USER CODE END USBPD_DPM_RequestDisplayPortConfig */
  _status = USBPD_PE_SVDM_RequestSpecific(PortNum, SOPType, SVDM_SPECIFIC_2, SVID);
  DPM_USER_ERROR_TRACE(PortNum, _status, "Display Port Config not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to perform a VDM Attention.
  * @param  PortNum The current port number
  * @param  SOPType SOP Type
  * @param  SVID    Used SVID
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestAttention(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, uint16_t SVID)
{
  USBPD_StatusTypeDef _status = USBPD_PE_SVDM_RequestAttention(PortNum, SOPType, SVID);
  DPM_USER_ERROR_TRACE(PortNum, _status, "VDM ATTENTION not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send an ALERT to port partner
  * @param  PortNum The current port number
  * @param  Alert   Alert based on @ref USBPD_ADO_TypeDef
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestAlert(uint8_t PortNum, USBPD_ADO_TypeDef Alert)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_DataMessage(PortNum, USBPD_DATAMSG_ALERT, (uint32_t*)&Alert.d32);
  DPM_USER_ERROR_TRACE(PortNum, _status, "ALERT not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to get a source capability extended
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetSourceCapabilityExt(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_GET_SRC_CAPEXT, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_SRC_CAPA_EXT not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to get a sink capability extended
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetSinkCapabilityExt(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_GET_SNK_CAPEXT, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_SINK_CAPA_EXT not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to get a manufacturer info
  * @param  PortNum The current port number
  * @param  SOPType SOP Type
  * @param  pManuInfoData Pointer on manufacturer info based on @ref USBPD_GMIDB_TypeDef
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetManufacturerInfo(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType, uint8_t* pManuInfoData)
{
  USBPD_StatusTypeDef _status = USBPD_ERROR;
  if (USBPD_SOPTYPE_SOP == SOPType)
  {
    _status = USBPD_PE_SendExtendedMessage(PortNum, SOPType, USBPD_EXT_GET_MANUFACTURER_INFO, (uint8_t*)pManuInfoData, sizeof(USBPD_GMIDB_TypeDef));
  }
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_MANU_INFO not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to request a GET_PPS_STATUS
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetPPS_Status(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_GET_PPS_STATUS, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_PPS_STATUS not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to request a GET_STATUS
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetStatus(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_GET_STATUS, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_STATUS not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to perform a Fast Role Swap.
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestFastRoleSwap(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_FR_SWAP, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "FRS not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a GET_COUNTRY_CODES message
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetCountryCodes(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_GET_COUNTRY_CODES, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_COUNTRY_CODES not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a GET_COUNTRY_INFO message
  * @param  PortNum     The current port number
  * @param  CountryCode Country code (1st character and 2nd of the Alpha-2 Country)
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetCountryInfo(uint8_t PortNum, uint16_t CountryCode)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_DataMessage(PortNum, USBPD_DATAMSG_GET_COUNTRY_INFO, (uint32_t*)&CountryCode);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_COUNTRY_INFO not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a GET_BATTERY_CAPA
  * @param  PortNum         The current port number
  * @param  pBatteryCapRef  Pointer on the Battery Capability reference
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetBatteryCapability(uint8_t PortNum, uint8_t *pBatteryCapRef)
{
  USBPD_StatusTypeDef _status = USBPD_PE_SendExtendedMessage(PortNum, USBPD_SOPTYPE_SOP, USBPD_EXT_GET_BATTERY_CAP, (uint8_t*)pBatteryCapRef, 1);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_BATTERY_CAPA not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a GET_BATTERY_STATUS
  * @param  PortNum           The current port number
  * @param  pBatteryStatusRef Pointer on the Battery Status reference
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetBatteryStatus(uint8_t PortNum, uint8_t *pBatteryStatusRef)
{
  USBPD_StatusTypeDef _status = USBPD_PE_SendExtendedMessage(PortNum, USBPD_SOPTYPE_SOP, USBPD_EXT_GET_BATTERY_STATUS, (uint8_t*)pBatteryStatusRef, 1);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_BATTERY_STATUS not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a SECURITY_REQUEST
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestSecurityRequest(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_ERROR;
  DPM_USER_ERROR_TRACE(PortNum, _status, "SECURITY_REQUEST not accepted by the stack");
  return _status;
}

/**
  * @}
  */

/** @addtogroup USBPD_USER_PRIVATE_FUNCTIONS
  * @{
  */

/* USER CODE BEGIN USBPD_USER_PRIVATE_FUNCTIONS */


#if defined(_RTOS)
static uint32_t CheckDPMTimers(void)
{
  uint32_t _timing = osWaitForever;
  return _timing;
}
#endif /* _RTOS */
/* USER CODE END USBPD_USER_PRIVATE_FUNCTIONS */

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
