[#ftl]
[#assign MCU_FAMILY = FamilyName?lower_case]

/* Exported functions --------------------------------------------------------*/
void MX_TCPP_Init(void);
void MX_TCPP_Process(void);

void TCPP0203_PORT0_FLG_EXTI_IRQHANDLER(void);

[#if MCU_FAMILY == "stm32g0"||MCU_FAMILY == "stm32g4"]

[#else]
void HAL_IncTick(void);
[/#if]

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

