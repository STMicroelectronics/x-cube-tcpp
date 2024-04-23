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

[#if MCU_FAMILY == "stm32g0"||MCU_FAMILY == "stm32g4"]

[#else]
void HAL_IncTick(void);
[/#if]

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

