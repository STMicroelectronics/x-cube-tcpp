[#ftl]
[#assign MCU_FAMILY = FamilyName?lower_case]

/* Includes ------------------------------------------------------------------*/
#include "usbpd_ADC_SNK.h"
#include "usbpd_GPIO_SNK.h"
#include "STMicroelectronics.X-CUBE-TCPP_conf.h"

#define ${FamilyName?upper_case}XX
#define ADC_FULL_SCALE       (0x0FFFU)

/* Exported functions --------------------------------------------------------*/
void MX_TCPP_Init(void);
void MX_TCPP_Process(void);

[#if MCU_FAMILY == "stm32g0"||MCU_FAMILY == "stm32g4"||MCU_FAMILY == "stm32h7rs"]

[#else]
void HAL_IncTick(void);
[/#if]

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
[#if MCU_FAMILY == "stm32h7rs"]
/**
  * This project uses custom linker settings to place the HEAP in a non-cacheable section of RAM. This is due to
  * the dynamic allocation of DMA buffers in the HEAP that must not be cached for DMA to access the data directly.
  * The following address corresponds to the location of the HEAP.
  */
#define DMABUFFER_AREA          0x24071000
#define DMABUFFER_AREA_SIZE     MPU_REGION_SIZE_4KB
[/#if]
/* USER CODE END EC */
