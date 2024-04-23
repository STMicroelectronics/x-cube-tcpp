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
  
  uint32_t   nbpdo, index, nb_valid_pdo = 0;
  uint32_t   *ptpdoarray = NULL;
  USBPD_PDO_TypeDef pdo_first;
  USBPD_PDO_TypeDef pdo;

  /* Check if valid port */
  if (USBPD_PORT_IsValid(PortNum))
  {
    /* According to type of PDO to be read, set pointer on values and nb of elements */
    switch (DataId)
    {
      case USBPD_CORE_DATATYPE_SRC_PDO :
        nbpdo = *PWR_Port_PDO_Storage[PortNum].SourcePDO.NumberOfPDO;
        ptpdoarray = PWR_Port_PDO_Storage[PortNum].SourcePDO.ListOfPDO;
        /* Save the 1st PDO */
        pdo_first.d32 = *ptpdoarray;
        /* Reset unchunked bit if current revision is PD2.0*/
        if (USBPD_SPECIFICATION_REV2 == DPM_Params[PortNum].PE_SpecRevision)
        {
          pdo_first.SRCFixedPDO.UnchunkedExtendedMessage  = USBPD_PDO_SRC_FIXED_UNCHUNK_NOT_SUPPORTED;
        }
        break;

      case USBPD_CORE_DATATYPE_SNK_PDO:
        nbpdo       = *PWR_Port_PDO_Storage[PortNum].SinkPDO.NumberOfPDO;
        ptpdoarray  = PWR_Port_PDO_Storage[PortNum].SinkPDO.ListOfPDO;
        /* Save the 1st PDO */
        pdo_first.d32 = *ptpdoarray;
        /* Reset FRS bit if current revision is PD2.0*/
        if (USBPD_SPECIFICATION_REV2 == DPM_Params[PortNum].PE_SpecRevision)
        {
          pdo_first.SNKFixedPDO.FastRoleSwapRequiredCurrent = USBPD_PDO_SNK_FIXED_FRS_NOT_SUPPORTED;
        }
        break;

      default:
        nbpdo = 0;
        break;
    }
    /* Copy PDO data in output buffer */
    for (index = 0; index < nbpdo; index++)
    {
      pdo.d32 = *ptpdoarray;
      /* Copy only PDO (and not APDO in case of current revision is PD2.0) */
      if ((USBPD_SPECIFICATION_REV2 == DPM_Params[PortNum].PE_SpecRevision)
         && (pdo.GenericPDO.PowerObject == USBPD_CORE_PDO_TYPE_APDO))
      {
      }
      else
      {
        /* Copy 1st PDO as potentially FRS or UNCHUNKED bits have been reset */
        if (0 == index)
        {
          (void)memcpy(Ptr, (uint8_t*)&pdo_first.d32, 4u);
        }
        else
        {
          (void)memcpy((Ptr + (nb_valid_pdo * 4u)), (uint8_t*)ptpdoarray, 4u);
        }
        nb_valid_pdo++;
      }
      ptpdoarray++;
    }
    /* Set nb of read PDO (nb of u32 elements); */
    *Size = nb_valid_pdo;
  }