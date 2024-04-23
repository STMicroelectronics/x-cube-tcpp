[#ftl]
  switch (Status)
  {
  case USBPD_HR_STATUS_WAIT_VBUS_VSAFE0V:
    if (USBPD_PORTPOWERROLE_SRC == CurrentRole)
    {
      /* Reset the power supply */
      DPM_TurnOffPower(PortNum, USBPD_PORTPOWERROLE_SRC);
    }
    break;
  case USBPD_HR_STATUS_WAIT_VBUS_VSAFE5V:
    if (CurrentRole == USBPD_PORTPOWERROLE_SRC)
    {
      /* Power on the power supply */
      DPM_TurnOnPower(PortNum, CurrentRole);
    }
    break;
  default:
    break;
  }
  