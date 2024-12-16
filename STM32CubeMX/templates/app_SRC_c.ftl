[#ftl]
[#assign MCU_FAMILY = FamilyName?lower_case]

/* Includes ------------------------------------------------------------------*/
#include "app_tcpp.h"

#if defined (_RTOS)
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#endif

#include "usbpd.h"

#ifdef _TRACE
#include "tracer_emb.h"
#endif /*_TRACE*/

#ifdef _GUI_INTERFACE
#include "gui_api.h"
#endif /*_GUI_INTERFACE*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
[#if MCU_FAMILY == "stm32h7rs"]
void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as Normal Non Cacheable for SRAM1 */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = DMABUFFER_AREA;
  MPU_InitStruct.Size = DMABUFFER_AREA_SIZE;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER3;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}
[/#if]

void MX_TCPP_Init(void)
{
[#if MCU_FAMILY == "stm32h7rs"]
  /* USER CODE BEGIN MX_TCPP_Init */
  // Tracer for USBPD needs Dcache to be disabled to avoid corrupted transfer
  SCB_DisableDCache();
  
  MPU_Config();
  /* USER CODE END MX_TCPP_Init */
[/#if]
}

void MX_TCPP_Process(void)
{

}

[#if MCU_FAMILY == "stm32g0"||MCU_FAMILY == "stm32g4"||MCU_FAMILY == "stm32h7rs"]

[#else]
/**
  * @brief This function is called to increment a global variable "uwTick"
  *        used as application time base.
  * @note In the default implementation, this variable is incremented each 1ms
  *       in Systick ISR.
  * @note This function overwrites the weak function.
  * @retval None
  */
void HAL_IncTick(void)
{
  uwTick += (uint32_t)uwTickFreq;
  USBPD_DPM_TimerCounter();
#if defined(_GUI_INTERFACE)
  GUI_TimerCounter();
#endif /* _GUI_INTERFACE */
}
[/#if]