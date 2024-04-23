[#ftl]

  /* Check if instance is valid */
  int32_t ret = BSP_ERROR_NONE;
  if ((Instance >= USBPD_PWR_INSTANCES_NBR) || (NULL == pVoltage))
  {
    ret = BSP_ERROR_WRONG_PARAM;
    *pVoltage = 0;
  }
  else
  {
    uint32_t value;
	uint32_t vadc;
	uint32_t voltage;
    value = LL_ADC_REG_ReadConversionData12(TCPP01_PORT0_VSENSE_ADC_INSTANCE);
	vadc = (value * VDDA_APPLI) / ADC_FULL_SCALE;
    voltage = vadc * (USBPD_PWR_VSENSE_RA + USBPD_PWR_VSENSE_RB ) / USBPD_PWR_VSENSE_RB ;
    *pVoltage = voltage;
  }
  return ret;