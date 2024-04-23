[#ftl]

#if !defined(USBPD_REV_MAJOR)
#define USBPD_REV_MAJOR      (3U)    /* USBPD Specification revision major */
#define USBPD_REV_MINOR      (1U)    /* USBPD Specification revision minor */
#define USBPD_VERSION_MAJOR  (1U)    /* USBPD Specification version major  */
#define USBPD_VERSION_MINOR  (7U)    /* USBPD Specification version minor  */
#endif /* !USBPD_REV_MAJOR */

#if defined (_DRP)
typedef enum {
  DPM_USER_EVENT_TIMER,         /* TIMER EVENT */
  DPM_USER_EVENT_GUI,           /* GUI EVENT */
  DPM_USER_EVENT_NONE,          /* NO EVENT */
} DPM_USER_EVENT;
#endif /* _DRP */

/**
  * @brief  USBPD DPM handle Structure definition
  * @{
  */
typedef struct
{
  uint32_t                      DPM_ListOfRcvSNKPDO[USBPD_MAX_NB_PDO];   /*!< The list of received Sink Power Data Objects from Port partner (when Port partner is a Sink or a DRP port). */
  uint32_t                      DPM_NumberOfRcvSNKPDO;                   /*!< The number of received Sink Power Data Objects from port Partner (when Port partner is a Sink or a DRP port). */
  uint32_t                      DPM_ListOfRcvSRCPDO[USBPD_MAX_NB_PDO];   /*!< The list of received Source Power Data Objects from Port partner     */
  uint32_t                      DPM_NumberOfRcvSRCPDO;                   /*!< The number of received Source Power Data Objects from port Partner  (when Port partner is a Source or a DRP port). */
  uint32_t                      DPM_RcvRequestDOMsg;                     /*!< Received request Power Data Object message from the port Partner     */
  uint32_t                      DPM_RequestDOMsgPrevious;                /*!< Previous Request Power Data Object message to be sent                */


  USBPD_PPSSDB_TypeDef          DPM_RcvPPSStatus;                        /*!< PPS Status received by port partner                                  */
  USBPD_SKEDB_TypeDef           DPM_RcvSNKExtendedCapa;                  /*!< SNK Extended Capability received by port partner                     */

  uint32_t                      DPM_RequestDOMsg;                        /*!< Request Power Data Object message to be sent                         */
  uint32_t                      DPM_RDOPosition;                         /*!< RDO Position of requested DO in Source list of capabilities          */
  uint32_t                      DPM_RDOPositionPrevious;                 /*!< RDO Position of previous requested DO in Source list of capabilities */
  uint32_t                      DPM_RequestedVoltage;                    /*!< Value of requested voltage                                           */
  uint32_t                      DPM_RequestedCurrent;                    /*!< Value of requested current                                           */
#if defined (_DRP)
  int16_t                       DPM_MeasuredCurrent;                     /*!< Value of measured current                                            */
  volatile uint32_t             DPM_ErrorCode;                           /*!< USB PD Error code                                                    */
  volatile uint8_t              DPM_IsConnected;                         /*!< USB PD connection state                                              */
  uint16_t                      DPM_Reserved:14;                         /*!< Reserved bytes                                                       */
#if defined(_GUI_INTERFACE)
  volatile uint16_t             DPM_TimerMeasReport;                     /*!< Timer used to send measurement report                                */
#endif /* _GUI_INTERFACE */
#endif /* _DRP */
} USBPD_HandleTypeDef;

#ifdef _GUI_INTERFACE
typedef void     (*GUI_NOTIFICATION_POST)(uint8_t PortNum, uint16_t EventVal);
typedef uint32_t (*GUI_NOTIFICATION_FORMAT_SEND)(uint32_t PortNum, uint32_t TypeNotification, uint32_t Value);
typedef void     (*GUI_SAVE_INFO)(uint8_t PortNum, uint8_t DataId, uint8_t *Ptr, uint32_t Size);
#endif /*_GUI_INTERFACE*/