[#ftl]

#ifdef _GUI_INTERFACE
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
#endif /*_GUI_INTERFACE*/

  switch(State)
  {
  case USBPD_CAD_EVENT_ATTACHED:
  case USBPD_CAD_EVENT_ATTEMC:
    {
    if (DPM_Params[PortNum].PE_PowerRole == USBPD_PORTPOWERROLE_SRC)
    {
      if (USBPD_OK != USBPD_PWR_IF_VBUSEnable(PortNum))
      {
        /* Should not occur */
        NVIC_SystemReset();
      }
    }
    break;
    }
  case USBPD_CAD_EVENT_DETACHED :
  case USBPD_CAD_EVENT_EMC :
  default :

    if (DPM_Params[PortNum].PE_PowerRole == USBPD_PORTPOWERROLE_SRC)
    {
      if (USBPD_OK != USBPD_PWR_IF_VBUSDisable(PortNum))
      {
        /* Should not occur */
        while(1);
      }
    }
    break;
  }
  


