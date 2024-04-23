[#ftl]
  USBPD_SNKRDO_TypeDef rdo;              /* get the requested RDO */
  USBPD_PDO_TypeDef    SelectedPDO;
  rdo.d32 = DPM_Ports[PortNum].DPM_RequestDOMsg;
  SelectedPDO.d32 = DPM_Ports[PortNum].DPM_ListOfRcvSRCPDO[rdo.GenericRDO.ObjectPosition - 1];
  switch (SelectedPDO.GenericPDO.PowerObject)
  {
    case USBPD_CORE_PDO_TYPE_FIXED :
    {
      switch (SelectedPDO.SRCFixedPDO.VoltageIn50mVunits * 50)
      {
        case 5000 :
          vbus_transition = USBPD_PWR_VBUS_THRESHOLD_5V;
          break;
        case 9000 :
          vbus_transition = USBPD_PWR_VBUS_THRESHOLD_9V;
          break;
        case 15000 :
          vbus_transition = USBPD_PWR_VBUS_THRESHOLD_15V;
          break;
        case 20000 :
          vbus_transition = USBPD_PWR_VBUS_THRESHOLD_20V;
          break;
      }
      break;
    }
#if defined(USBPD_REV30_SUPPORT) && defined(USBPDCORE_PPS)
    case USBPD_CORE_PDO_TYPE_APDO :
    {
      vbus_transition = USBPD_PWR_VBUS_THRESHOLD_APDO;
      break;
    }
#endif /*_USBPD_REV30_SUPPORT && PPS*/
    case USBPD_CORE_PDO_TYPE_BATTERY :
    case USBPD_CORE_PDO_TYPE_VARIABLE :
    {
      /* Not yet handled */
      break;
    }
  }