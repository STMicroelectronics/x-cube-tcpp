[#ftl]
static  void     DPM_SNK_BuildRDOfromSelectedPDO(uint8_t PortNum, uint8_t IndexSrcPDO, USBPD_DPM_SNKPowerRequestDetails_TypeDef* PtrRequestPowerDetails,
                                             USBPD_SNKRDO_TypeDef* Rdo, USBPD_CORE_PDO_Type_TypeDef *PtrPowerObject);
static uint32_t  DPM_FindVoltageIndex(uint32_t PortNum, USBPD_DPM_SNKPowerRequestDetails_TypeDef* PtrRequestPowerDetails, uint8_t Method);

static void                DPM_AssertRp(uint8_t PortNum);
static void                DPM_AssertRd(uint8_t PortNum);

static USBPD_StatusTypeDef DPM_TurnOnPower(uint8_t PortNum, USBPD_PortPowerRole_TypeDef Role);
static USBPD_StatusTypeDef DPM_TurnOffPower(uint8_t PortNum, USBPD_PortPowerRole_TypeDef Role);
