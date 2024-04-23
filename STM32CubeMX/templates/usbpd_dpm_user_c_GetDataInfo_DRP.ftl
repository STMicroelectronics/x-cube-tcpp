[#ftl]
  /* Case Port Source PDO Data information : */
  /* Case Port SINK PDO Data information : */
  case USBPD_CORE_DATATYPE_SRC_PDO :
  case USBPD_CORE_DATATYPE_SNK_PDO :
      USBPD_PWR_IF_GetPortPDOs(PortNum, DataId, Ptr, Size);
     *Size *= 4;
      break;
  case USBPD_CORE_DATATYPE_REQ_VOLTAGE :
    *Size = 4;
    (void)memcpy((uint8_t*)Ptr, (uint8_t *)&DPM_Ports[PortNum].DPM_RequestedVoltage, *Size);
    break;
  case USBPD_CORE_REVISION:
    {
      *Size = sizeof(USBPD_RevisionDO_TypeDef);
      USBPD_RevisionDO_TypeDef rev =
      {
        /* Hardcoded values, user should use a global USBPD_RevisionDO_TypeDef variable */
        .b.Revision_major = USBPD_REV_MAJOR,         /*!< Major revision */
        .b.Revision_minor = USBPD_REV_MINOR,         /*!< Minor revision */
        .b.Version_major  = USBPD_VERSION_MAJOR,     /*!< Major version  */
        .b.Version_minor  = USBPD_VERSION_MINOR      /*!< Minor version  */
      };

      memcpy((uint8_t *)Ptr, &rev, *Size);
      break;
    }
  case USBPD_CORE_PPS_STATUS :
    {
      /* Get current drawn by sink */
      USBPD_PPSSDB_TypeDef pps_status = {0};
      *Size = 4;
      (void)memcpy((uint8_t*)Ptr, (uint8_t *)&pps_status.d32, *Size);
    }
    break;
#if defined (USBPD_CORE_SNK_EXTENDED_CAPA)
  case USBPD_CORE_SNK_EXTENDED_CAPA :
    {
      *Size = sizeof(USBPD_SKEDB_TypeDef);
      memcpy((uint8_t*)Ptr, (uint8_t *)&DPM_USER_Settings[PortNum].DPM_SNKExtendedCapa, *Size);
    }
    break;
#endif /* USBPD_CORE_SNK_EXTENDED_CAPA */
  default:
    break;