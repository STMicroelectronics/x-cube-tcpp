[#ftl]
[#assign MCU_FAMILY = FamilyName?lower_case]

#define ${FamilyName?upper_case}XX

/* Includes ------------------------------------------------------------------*/
#include "usbpd_ADC.h"
#include "usbpd_GPIO.h"
#include "STMicroelectronics.X-CUBE-TCPP_conf.h"

/* Exported functions --------------------------------------------------------*/
void MX_TCPP_Init(void);
void MX_TCPP_Process(void);

[#if MCU_FAMILY == "stm32g0"||MCU_FAMILY == "stm32g4"]

[#else]
void HAL_IncTick(void);
[/#if]

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

