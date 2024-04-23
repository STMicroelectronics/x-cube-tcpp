[#ftl]

#if !defined(USBPD_REV_MAJOR)
#define USBPD_REV_MAJOR      (3U)    /* USBPD Specification revision major */
#define USBPD_REV_MINOR      (1U)    /* USBPD Specification revision minor */
#define USBPD_VERSION_MAJOR  (1U)    /* USBPD Specification version major  */
#define USBPD_VERSION_MINOR  (7U)    /* USBPD Specification version minor  */
#endif /* !USBPD_REV_MAJOR */

/** @brief  USBPD PDO Selection method enum definition
  *
  */
typedef enum
{
  PDO_SEL_METHOD_MAX_PWR,
  PDO_SEL_METHOD_MIN_PWR,
  PDO_SEL_METHOD_MAX_VOLT,
  PDO_SEL_METHOD_MIN_VOLT,
  PDO_SEL_METHOD_MAX_CUR,
  PDO_SEL_METHOD_MIN_CUR
} USBPD_DPM_PDO_SelectionMethodTypeDef;

typedef enum {
  DPM_USER_EVENT_TIMER,         /* TIMER EVENT */
  DPM_USER_EVENT_GUI,           /* GUI EVENT */
  DPM_USER_EVENT_NONE,          /* NO EVENT */
} DPM_USER_EVENT;

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

  int16_t                       DPM_MeasuredCurrent;                     /*!< Value of measured current                                            */
  volatile uint32_t             DPM_ErrorCode;                           /*!< USB PD Error code                                                    */
  volatile uint8_t              DPM_IsConnected;                         /*!< USB PD connection state                                              */
  uint16_t                      DPM_Reserved:14;                         /*!< Reserved bytes                                                       */
#if defined(_GUI_INTERFACE)
  volatile uint16_t             DPM_TimerMeasReport;                     /*!< Timer used to send measurement report                                */
#endif /* _GUI_INTERFACE */
} USBPD_HandleTypeDef;
