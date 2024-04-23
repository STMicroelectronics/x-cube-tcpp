[#ftl]
  USBPD_PDO_TypeDef        _pdo;
  USBPD_SNKRDO_TypeDef     _rdo;
  _rdo.d32 = DPM_Ports[PortNum].DPM_RcvRequestDOMsg;
  _pdo.d32 = PORT0_PDO_ListSRC[0];
  return (BSP_ERROR_NONE == BSP_USBPD_PWR_VBUSSetVoltage_Fixed(PortNum,
                                               _pdo.SRCFixedPDO.VoltageIn50mVunits * 50,
                                               (_rdo.FixedVariableRDO.OperatingCurrentIn10mAunits * 10),
                                               (_rdo.FixedVariableRDO.MaxOperatingCurrent10mAunits * 10)
                                               )? USBPD_OK : USBPD_ERROR);
											   