/**
 ******************************************************************************
 * @file    STMicroelectronics.X-CUBE-TCPP.1.0.0
 * @brief   App Source application C file
 ******************************************************************************
 * @attention
 *
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "app_tcpp.h"

#if defined (_RTOS)
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#endif

#include "usbpd.h"
#include "src1m1_conf.h"

#ifdef _TRACE
#include "tracer_emb.h"
#endif /*_TRACE*/

#ifdef _GUI_INTERFACE
#include "gui_api.h"
#endif /*_GUI_INTERFACE*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

void MX_TCPP_Init(void)
{

}

void MX_TCPP_Process(void)
{

}

/**
  * @brief  This function handles the external line interrupt request.
  *         (Associated to FLGn line in case of TCPP0203 management)
  * @retval None
  */
void TCPP0203_PORT0_FLG_EXTI_IRQHANDLER(void)
{
  BSP_USBPD_PWR_EventCallback(USBPD_PWR_TYPE_C_PORT_1);
  TCPP0203_PORT0_FLG_EXTI_CLEAR_FLAG();
}

