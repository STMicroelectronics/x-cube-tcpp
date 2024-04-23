[#ftl]
/**
  * @brief  Turn Off power supply.
  * @param  PortNum The current port number
  * @param  Role    Port power role
  * @retval USBPD_OK, USBPD_ERROR
  */
static USBPD_StatusTypeDef DPM_TurnOffPower(uint8_t PortNum, USBPD_PortPowerRole_TypeDef Role)
{
  USBPD_StatusTypeDef status;
  status = USBPD_PWR_IF_VBUSDisable(PortNum);
  return status;
}

/**
  * @brief  Turn On power supply.
  * @param  PortNum The current port number
  * @param  Role    Port power role
  * @retval USBPD_ACCEPT, USBPD_WAIT, USBPD_REJECT
  */
static USBPD_StatusTypeDef DPM_TurnOnPower(uint8_t PortNum, USBPD_PortPowerRole_TypeDef Role)
{
  USBPD_StatusTypeDef status;
  /* Enable the output */
  status = USBPD_PWR_IF_VBUSEnable(PortNum);
  return status;
}
