[#ftl]

  /* Check if instance is valid */
  int32_t ret = BSP_ERROR_NONE;
  if ((Instance >= USBPD_PWR_INSTANCES_NBR) || (NULL == pCurrent))
  {
    ret = BSP_ERROR_WRONG_PARAM;
    *pCurrent = 0;
  }
  else
  {
    *pCurrent = 0;
  }
  return ret;