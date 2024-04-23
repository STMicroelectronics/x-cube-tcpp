[#ftl]

#if !defined(USBPD_DPM_USER_C)
extern USBPD_HandleTypeDef                  DPM_Ports[USBPD_PORT_COUNT];
#else
USBPD_HandleTypeDef                         DPM_Ports[USBPD_PORT_COUNT] =
{
  {
    .DPM_Reserved = 0,
#if defined(USBPDCORE_SNK_CAPA_EXT)
    .DPM_RcvSNKExtendedCapa = {0},                  /*!< SNK Extended Capability received by port partner                     */
#endif /* USBPDCORE_SNK_CAPA_EXT */
  }
};
#endif /* !USBPD_DPM_USER_C */
