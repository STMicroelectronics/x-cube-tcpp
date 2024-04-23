[#ftl]
[#assign MCU_FAMILY = FamilyName?lower_case]

#include "${FamilyName?lower_case}xx_hal.h"

/* Exported functions --------------------------------------------------------*/
void MX_TCPP_Init(void);
void MX_TCPP_Process(void);

[#if MCU_FAMILY == "stm32g0"||MCU_FAMILY == "stm32g4"]

[#else]
void HAL_IncTick(void);
[/#if]

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/



