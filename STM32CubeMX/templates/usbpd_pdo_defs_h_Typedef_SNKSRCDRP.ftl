[#ftl]

 /**
   * @brief  USBPD Port PDO Structure definition
   */
 typedef struct
 {
   uint32_t *ListOfPDO;                          /*!< Pointer on Power Data Objects list, defining port capabilities */                                                
   uint8_t  *NumberOfPDO;                        /*!< Number of Power Data Objects defined in ListOfPDO
                                                 This parameter must be set at max to @ref USBPD_MAX_NB_PDO value */
 } USBPD_PortPDO_TypeDef;


 /**
   * @brief  USBPD Port PDO Storage Structure definition
   */


typedef struct
{
  USBPD_PortPDO_TypeDef    SourcePDO;            /*!< SRC Power Data Objects */
  USBPD_PortPDO_TypeDef    SinkPDO;              /*!< SNK Power Data Objects */

} USBPD_PWR_Port_PDO_Storage_TypeDef;