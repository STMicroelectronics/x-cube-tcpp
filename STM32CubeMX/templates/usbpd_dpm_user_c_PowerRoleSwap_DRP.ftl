[#ftl]
    switch (Status)
    {
    case USBPD_PRS_STATUS_VBUS_OFF:
      if (CurrentRole == USBPD_PORTPOWERROLE_SRC)
      {
#if defined(_VCONN_SUPPORT)
        /* This variable is used to avoid VCONN disable during HR procedure */
        uint32_t _vconnstate = DPM_Params[PortNum].VconnStatus;
        DPM_Params[PortNum].VconnStatus = USBPD_FALSE;
#endif /* _VCONN_SUPPORT */
        /* In case of power role swap keep VCONN On */
        DPM_TurnOffPower(PortNum, CurrentRole);
#if defined(_VCONN_SUPPORT)
        /* restore vconn status */
        DPM_Params[PortNum].VconnStatus = _vconnstate;
#endif /* _VCONN_SUPPORT */
      }
      break;
    case USBPD_PRS_STATUS_SRC_RP2RD:
      DPM_AssertRd(PortNum);
      USBPD_DPM_WaitForTime(5);
      break;
    case USBPD_PRS_STATUS_SNK_RD2RP:
      DPM_AssertRp(PortNum);
      break;
    case USBPD_PRS_STATUS_VBUS_ON:
      DPM_TurnOnPower(PortNum, CurrentRole);
      break;
    default:
      break;
    }