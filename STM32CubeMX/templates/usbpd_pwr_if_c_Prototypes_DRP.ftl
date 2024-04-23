[#ftl]
USBPD_StatusTypeDef PWR_IF_CheckUpdateSNKPower(uint8_t PortNum);
/* Functions to initialize Source PDOs */
uint32_t _PWR_SRCFixedPDO(float  _C_, float _V_,
                          USBPD_CORE_PDO_PeakCurr_TypeDef _PK_,
                          USBPD_CORE_PDO_DRDataSupport_TypeDef DRDSupport,
                          USBPD_CORE_PDO_USBCommCapable_TypeDef UsbCommCapable,
                          USBPD_CORE_PDO_ExtPowered_TypeDef ExtPower,
                          USBPD_CORE_PDO_USBSuspendSupport_TypeDef UsbSuspendSupport,
                          USBPD_CORE_PDO_DRPowerSupport_TypeDef DRPSupport);
uint32_t _PWR_SRCVariablePDO(float _MAXV_, float _MINV_, float _C_);
uint32_t _PWR_SRCBatteryPDO(float _MAXV_,float _MINV_,float _PWR_);
/* Functions to initialize Sink PDOs */
uint32_t _PWR_SNKFixedPDO(float  _C_, float _V_,
                          USBPD_CORE_PDO_DRDataSupport_TypeDef DRDSupport,
                          USBPD_CORE_PDO_USBCommCapable_TypeDef UsbCommCapable,
                          USBPD_CORE_PDO_ExtPowered_TypeDef ExtPower,
                          USBPD_CORE_PDO_HigherCapability_TypeDef HigherCapab,
                          USBPD_CORE_PDO_DRPowerSupport_TypeDef DRPSupport);
uint32_t _PWR_SNKVariablePDO(float  _MAXV_,float _MINV_,float _C_);
uint32_t _PWR_SNKBatteryPDO(float _MAXV_,float _MINV_,float _PWR_);
uint32_t _PWR_ProgrammablePowerSupplyAPDO(float _MAXC_,float _MINV_,float _MAXV_);
void     _PWR_CheckPDOContent(uint8_t PortNum);