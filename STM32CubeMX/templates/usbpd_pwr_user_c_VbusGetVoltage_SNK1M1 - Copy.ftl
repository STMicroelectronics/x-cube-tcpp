[#ftl]

  /* Check if instance is valid */
  if ((Instance >= USBPD_PWR_INSTANCES_NBR) || (NULL == pVoltage))
  {
    *pVoltage = 0;
  }
  else
  {
    uint32_t value;
	uint32_t vadc;
	uint32_t voltage;
    value = LL_ADC_REG_ReadConversionData12(ADC1);
	vadc = (value * VDDA_APPLI) / ADC_FULL_SCALE;
    voltage = vadc * (SNK1M1_VSENSE_RA + SNK1M1_VSENSE_RB ) / SNK1M1_VSENSE_RB ;
    *pVoltage = voltage;
  }