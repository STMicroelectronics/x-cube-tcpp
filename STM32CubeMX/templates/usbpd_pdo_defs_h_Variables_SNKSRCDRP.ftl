[#ftl]

#ifndef _GUI_INTERFACE
#ifndef __USBPD_PWR_IF_C
extern uint8_t USBPD_NbPDO[4];
extern uint32_t PORT0_PDO_ListSRC[USBPD_MAX_NB_PDO];
extern uint32_t PORT0_PDO_ListSNK[USBPD_MAX_NB_PDO];
#else /* __USBPD_PWR_IF_C */
uint8_t USBPD_NbPDO[4] = {(PORT0_NB_SINKPDO),
                          (PORT0_NB_SOURCEPDO)};
#endif /* __USBPD_PWR_IF_C */
#endif /* _GUI_INTERFACE */


