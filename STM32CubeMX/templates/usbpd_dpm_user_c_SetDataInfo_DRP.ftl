[#ftl]
    /* Case Received Request PDO Data information : */
    case USBPD_CORE_DATATYPE_RDO_POSITION :
      if (Size == 4)
      {
        uint8_t* temp;
        temp = (uint8_t*)&DPM_Ports[PortNum].DPM_RDOPosition;
        (void)memcpy(temp, Ptr, Size);
        DPM_Ports[PortNum].DPM_RDOPositionPrevious = *Ptr;
        temp = (uint8_t*)&DPM_Ports[PortNum].DPM_RDOPositionPrevious;
        (void)memcpy(temp, Ptr, Size);
      }
      break;

    /* Case Received Sink PDO values Data information :*/
    case USBPD_CORE_DATATYPE_RCV_SNK_PDO :
      if (Size <= (USBPD_MAX_NB_PDO * 4))
      {
        uint8_t* rdo;
        DPM_Ports[PortNum].DPM_NumberOfRcvSNKPDO = (Size / 4);
        /* Copy PDO data in DPM Handle field */
        for (uint32_t index = 0; index < (Size / 4); index++)
        {
          rdo = (uint8_t*)&DPM_Ports[PortNum].DPM_ListOfRcvSNKPDO[index];
          (void)memcpy(rdo, (Ptr + (index * 4u)), (4u * sizeof(uint8_t)));
        }
      }
      break;

    case USBPD_CORE_DATATYPE_RCV_REQ_PDO :  /*!< Storage of Received Sink Request PDO value                */
      if (Size == 4)
      {
        memcpy((uint8_t *)&DPM_Ports[PortNum].DPM_RcvRequestDOMsg,  Ptr, 4);
      }
      break;

      /* Case Received Source PDO values Data information : */
    case USBPD_CORE_DATATYPE_RCV_SRC_PDO :
      if (Size <= (USBPD_MAX_NB_PDO * 4))
      {
        uint8_t* rdo;
        DPM_Ports[PortNum].DPM_NumberOfRcvSRCPDO = (Size / 4);
        /* Copy PDO data in DPM Handle field */
        for (uint32_t index = 0; index < (Size / 4); index++)
        {
          rdo = (uint8_t*)&DPM_Ports[PortNum].DPM_ListOfRcvSRCPDO[index];
          (void)memcpy(rdo, (Ptr + (index * 4u)), (4u * sizeof(uint8_t)));
        }
      }
      break;

    case USBPD_CORE_PPS_STATUS :
      {
        uint8_t*  ext_capa;
        ext_capa = (uint8_t*)&DPM_Ports[PortNum].DPM_RcvPPSStatus;
        memcpy(ext_capa, Ptr, Size);
      }

    case USBPD_CORE_REVISION:
    {
      /* Does nothing: User have to implement a global revision variable */
      USBPD_RevisionDO_TypeDef rev = {0};
      memcpy((uint8_t *)&rev, Ptr, sizeof(USBPD_RevisionDO_TypeDef));
      break;
    }
#if defined(USBPDCORE_SNK_CAPA_EXT)
    case USBPD_CORE_SNK_EXTENDED_CAPA :
      {
        uint8_t*  _snk_ext_capa;
        _snk_ext_capa = (uint8_t*)&DPM_Ports[PortNum].DPM_RcvSNKExtendedCapa;
        memcpy(_snk_ext_capa, Ptr, Size);
      }
#endif /* USBPDCORE_SNK_CAPA_EXT */

    default:
    break;