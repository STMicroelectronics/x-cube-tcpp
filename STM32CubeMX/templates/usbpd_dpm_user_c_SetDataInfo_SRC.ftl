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

    case USBPD_CORE_REVISION:
      {
        /* Does nothing: User have to implement a global revision variable */
        USBPD_RevisionDO_TypeDef rev = {0};
        memcpy((uint8_t *)&rev, Ptr, sizeof(USBPD_RevisionDO_TypeDef));
        break;
      }

    default:
    break;