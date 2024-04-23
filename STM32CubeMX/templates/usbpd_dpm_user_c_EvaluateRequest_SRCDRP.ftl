[#ftl]
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
         || ((rdo.FixedVariableRDO.MaxOperatingCurrent10mAunits > pdo.SRCFixedPDO.MaxCurrentIn10mAunits)&&(rdo.FixedVariableRDO.CapabilityMismatch==0)))
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