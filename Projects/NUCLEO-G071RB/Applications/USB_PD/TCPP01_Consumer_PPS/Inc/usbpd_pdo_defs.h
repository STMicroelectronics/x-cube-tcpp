/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_pdo_defs.h
  * @author  MCD Application Team
  * @brief   Header file for definition of PDO/APDO values for 1 port/SNK configuration
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef __USBPD_PDO_DEF_H_
#define __USBPD_PDO_DEF_H_

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#if defined(_GUI_INTERFACE)
#include "usbpd_gui_memmap.h"
#endif /* _GUI_INTERFACE */
/* USER CODE END Includes */

/* Define   ------------------------------------------------------------------*/
#define PORT0_NB_SOURCEPDO         0U   /* Number of Source PDOs (applicable for port 0)   */
#define PORT0_NB_SINKPDO           4U   /* Number of Sink PDOs (applicable for port 0)     */
#define PORT1_NB_SOURCEPDO         0U   /* Number of Source PDOs (applicable for port 1)   */
#define PORT1_NB_SINKPDO           0U   /* Number of Sink PDOs (applicable for port 1)     */

/* USER CODE BEGIN Define */
#define PORT0_NB_SOURCEAPDO        0U   /* Number of Source APDOs (applicable for port 0)  */
#define PORT0_NB_SINKAPDO          1U   /* Number of Sink APDOs (applicable for port 0)    */
#define PORT1_NB_SOURCEAPDO        0U   /* Number of Source APDOs (applicable for port 1)  */
#define PORT1_NB_SINKAPDO          0U   /* Number of Sink APDOs (applicable for port 1)    */
/* USER CODE END Define */

/* Exported typedef ----------------------------------------------------------*/

/* USER CODE BEGIN typedef */

/**
  * @brief  USBPD Port PDO Structure definition
  *
  */
typedef struct
{
  uint32_t *ListOfPDO;                          /*!< Pointer on Power Data Objects list, defining
                                                     port capabilities */
  uint8_t  *NumberOfPDO;                         /*!< Number of Power Data Objects defined in ListOfPDO
                                                     This parameter must be set at max to @ref USBPD_MAX_NB_PDO value */
} USBPD_PortPDO_TypeDef;

/**
  * @brief  USBPD Port PDO Storage Structure definition
  */
typedef struct
{
  USBPD_PortPDO_TypeDef    SinkPDO;          /*!< SNK Power Data Objects */
}USBPD_PWR_Port_PDO_Storage_TypeDef;

/* USER CODE END typedef */

/* Exported define -----------------------------------------------------------*/

/* USER CODE BEGIN Exported_Define */
#define USBPD_PDP_SNK_IN_WATTS          100 /* SINK PD Power in Watts */

/* Define board operating power and max power */
/* Request 5V */
#define USBPD_BOARD_REQUESTED_VOLTAGE_MV       5000
#define USBPD_BOARD_MIN_VOLTAGE_MV             3300
#define USBPD_BOARD_MAX_VOLTAGE_MV             20000

/* Max current */
/* 5000mA in sink mode */
#define USBPD_CORE_PDO_SNK_FIXED_MAX_CURRENT   5000

/* Definitions for APDO values */
#define USBPD_PDO_APDO_MAX_CURRENT 5   /* Max Current in A */
#define USBPD_PDO_APDO_MIN_VOLTAGE 3.3   /* Min voltage in V */
#define USBPD_PDO_APDO_MAX_VOLTAGE 5.9  /* Min voltage in V */
/* USER CODE END Exported_Define */

/* Exported constants --------------------------------------------------------*/

/* USER CODE BEGIN constants */

/* USER CODE END constants */

/* Exported macro ------------------------------------------------------------*/

/* USER CODE BEGIN macro */

/* USER CODE END macro */

/* Exported variables --------------------------------------------------------*/

/* USER CODE BEGIN variables */

/* USER CODE END variables */

#ifndef __USBPD_PWR_IF_C
extern uint8_t USBPD_NbPDO[4];

extern uint32_t PORT0_PDO_ListSNK[USBPD_MAX_NB_PDO];
#else

uint8_t USBPD_NbPDO[4] = {(PORT0_NB_SINKPDO + PORT0_NB_SINKAPDO), 
                          ((PORT0_NB_SOURCEPDO + PORT0_NB_SOURCEAPDO)),
                          ((PORT1_NB_SINKPDO + PORT1_NB_SINKAPDO)),
                          ((PORT1_NB_SOURCEPDO + PORT1_NB_SOURCEAPDO))};


/* Definition of Sink PDO for Port 0 */
uint32_t PORT0_PDO_ListSNK[USBPD_MAX_NB_PDO] =
{
  /* PDO 1 */
  ( ((PWR_A_10MA(USBPD_CORE_PDO_SNK_FIXED_MAX_CURRENT/1000.0)) << USBPD_PDO_SNK_FIXED_OP_CURRENT_Pos)    |
    ((PWR_V_50MV(USBPD_BOARD_REQUESTED_VOLTAGE_MV/1000.0)) << USBPD_PDO_SNK_FIXED_VOLTAGE_Pos) |
     USBPD_PDO_SNK_FIXED_FRS_NOT_SUPPORTED                                                     |
     USBPD_PDO_SNK_FIXED_EXT_POWER_AVAILABLE                                                   |
     USBPD_PDO_SRC_FIXED_USBSUSPEND_NOT_SUPPORTED                                              |
     USBPD_PDO_SNK_FIXED_HIGHERCAPAB_NOT_SUPPORTED                                             |
     USBPD_PDO_SNK_FIXED_DRP_NOT_SUPPORTED                                                     |
     USBPD_PDO_TYPE_FIXED
  ),
  /* PDO 2 */
  ( ((PWR_A_10MA(USBPD_CORE_PDO_SNK_FIXED_MAX_CURRENT/1000.0)) << USBPD_PDO_SNK_FIXED_OP_CURRENT_Pos)    |
    ((PWR_V_50MV(9000/1000.0)) << USBPD_PDO_SNK_FIXED_VOLTAGE_Pos) |
     USBPD_PDO_TYPE_FIXED
  ),
  /* PDO 3 */
  ( ((PWR_A_10MA(USBPD_CORE_PDO_SNK_FIXED_MAX_CURRENT/1000.0)) << USBPD_PDO_SNK_FIXED_OP_CURRENT_Pos)    |
    ((PWR_V_50MV(15000/1000.0)) << USBPD_PDO_SNK_FIXED_VOLTAGE_Pos) |
     USBPD_PDO_TYPE_FIXED
  ),
  /* PDO 4 */
  ( ((PWR_A_10MA(USBPD_CORE_PDO_SNK_FIXED_MAX_CURRENT/1000.0)) << USBPD_PDO_SNK_FIXED_OP_CURRENT_Pos)    |
    ((PWR_V_50MV(USBPD_BOARD_MAX_VOLTAGE_MV/1000.0)) << USBPD_PDO_SNK_FIXED_VOLTAGE_Pos) |
     USBPD_PDO_TYPE_FIXED
  ),
  /* PDO 5 : SRC APDO */ 
  ( (((PWR_A_50MA(USBPD_PDO_APDO_MAX_CURRENT)) << USBPD_PDO_SNK_APDO_MAX_CURRENT_Pos) & (USBPD_PDO_SNK_APDO_MAX_CURRENT_Msk))  |
    (((PWR_V_100MV(USBPD_PDO_APDO_MIN_VOLTAGE)) << USBPD_PDO_SNK_APDO_MIN_VOLTAGE_Pos) & (USBPD_PDO_SNK_APDO_MIN_VOLTAGE_Msk)) |
    (((PWR_V_100MV(USBPD_PDO_APDO_MAX_VOLTAGE)) << USBPD_PDO_SNK_APDO_MAX_VOLTAGE_Pos) & (USBPD_PDO_SNK_APDO_MAX_VOLTAGE_Msk)) |
     USBPD_PDO_TYPE_APDO
  ),
  /* PDO 6 */ (0x00000000U),
  /* PDO 7 */ (0x00000000U)
};


#endif /* __USBPD_PWR_IF_C */

/* Exported functions --------------------------------------------------------*/

/* USER CODE BEGIN functions */

/* USER CODE END functions */

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_PDO_DEF_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/