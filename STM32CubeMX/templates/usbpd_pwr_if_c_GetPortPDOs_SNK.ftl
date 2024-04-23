[#ftl]
  if (DataId == USBPD_CORE_DATATYPE_SRC_PDO)
  {
#if defined (_GUI_INTERFACE)
    *Size = USBPD_NbPDO[1];
    memcpy(Ptr,PORT0_PDO_ListSRC, sizeof(uint32_t) * USBPD_NbPDO[1]);
#else
    *Size = PORT0_NB_SOURCEPDO;
    memcpy(Ptr,PORT0_PDO_ListSRC, sizeof(uint32_t) * PORT0_NB_SOURCEPDO);
#endif /* _GUI_INTERFACE */
  }
  else
  {
#if defined (_GUI_INTERFACE)
    *Size = USBPD_NbPDO[0];
    memcpy(Ptr,PORT0_PDO_ListSNK, sizeof(uint32_t) * USBPD_NbPDO[0]);
#else
    *Size = PORT0_NB_SINKPDO;
    memcpy(Ptr,PORT0_PDO_ListSNK, sizeof(uint32_t) * PORT0_NB_SINKPDO);
#endif /* _GUI_INTERFACE */
  }
