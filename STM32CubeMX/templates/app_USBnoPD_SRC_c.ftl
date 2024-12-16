[#ftl]
/* Includes ------------------------------------------------------------------*/
#include "app_tcpp.h"
#include "custom_board_usbpd_pwr.h"
#include "USBnoPD_Tim.h"

/* Private variables ---------------------------------------------------------*/

USBnoPD_StatesTypeDef USBnoPD_State =                                  USBnoPD_State_DETACHED;
uint16_t USBnoPD_adc_buffer[USBNOPD_ADC_USED_CHANNELS] =               {0};
uint16_t USBnoPD_adc_buffer_filtered[USBNOPD_ADC_USED_CHANNELS];
uint16_t USBnoPD_adc_converted_buffer[USBNOPD_ADC_USED_CHANNELS] =     {0};
uint16_t USBnoPD_debounce_counter =                                    0;
uint8_t USBnoPD_activeCC =                                             USBnoPD_CC1; /* Default */

/* Maximum digital value of the ADC output (12 Bits resolution)
   To convert ADC measurement to an absolute voltage value:
   VCHANNELx = ADCx_DATA x (VDD/ADC_FULL_SCALE)
  */
#define ADC_FULL_SCALE       (0x0FFFU)

/* Private function prototypes -----------------------------------------------*/
static uint32_t USBnoPD_TCPP0203_ConvertADCDataToVoltage(uint32_t ADCData, uint32_t Ra, uint32_t Rb);
static int32_t USBnoPD_TCPP0203_ConvertADCDataToCurrent(uint32_t ADCData, uint32_t Ga, uint32_t Rs);
static void USBnoPD_ProcessADC(void);
static void USBnoPD_IncrementDebounceCount(void);
static void USBnoPD_StateMachineRun(void);

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

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(TCPP0203_PORT0_FLG_EXTI_IRQN, 0, 0);
  HAL_NVIC_EnableIRQ(TCPP0203_PORT0_FLG_EXTI_IRQN);

  HAL_GPIO_WritePin(TCPP0203_PORT0_ENABLE_GPIO_PORT, TCPP0203_PORT0_ENABLE_GPIO_PIN, GPIO_PIN_SET);
  
  BSP_USBPD_PWR_Init(USBPD_PWR_TYPE_C_PORT_1);
  BSP_USBPD_PWR_SetPowerMode(USBPD_PWR_TYPE_C_PORT_1, USBPD_PWR_MODE_NORMAL);

  ADC_Start();
  USBnoPD_State = USBnoPD_State_DETACHED;
}

void MX_TCPP_Process(void)
{
  USBnoPD_StateMachineRun();
}

/**
  * @brief  Main state machine
  * @param  none
  * @retval none
  */
static void USBnoPD_StateMachineRun(void)
{
  switch(USBnoPD_State)
  {
    case USBnoPD_State_DETACHED:      /* IDLE, nothing connected         */
      /* Transition to next state */
      if (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_VPROV] > USBNOPD_VPROV_VOLTAGE_MIN)
      {
        /* Connection detected on CC1 */
        if ((USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC1] > USBNOPD_CC_VOLTAGE_MINRD) &&
            (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC1] < USBNOPD_CC_VOLTAGE_MAXRD) &&
            (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC2] < USBNOPD_CC_VOLTAGE_MAXRA ||
             USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC2] > USBNOPD_CC_VOLTAGE_MINOPEN))
        {
          /* Reset debouncing counter */
          USBnoPD_debounce_counter = 0u;

          /* Start the debouncing timer */
          TIM_Base_Start_IT();

          /* Active CC is now CC1 */
          USBnoPD_activeCC = USBnoPD_CC1;

          /* Go to attaching state */
          USBnoPD_State = USBnoPD_State_ATTACHING;
        }
        /* Connection detected on CC2 */
        else if ((USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC2] > USBNOPD_CC_VOLTAGE_MINRD) &&
                 (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC2] < USBNOPD_CC_VOLTAGE_MAXRD) &&
                 (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC1] < USBNOPD_CC_VOLTAGE_MAXRA ||
                  USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC1] > USBNOPD_CC_VOLTAGE_MINOPEN))
        {
          /* Reset debouncing counter */
          USBnoPD_debounce_counter = 0u;

          /* Start the debouncing timer */ 
          TIM_Base_Start_IT();

          /* Active CC is now CC2 */
          USBnoPD_activeCC = USBnoPD_CC2;

          /* Go to attaching state */
          USBnoPD_State = USBnoPD_State_ATTACHING;
        }
      }
      break;

    case USBnoPD_State_ATTACHING:     /* Attachment ongoing - debouncing */
      /* If a glitch is detected, reset debounce counter and go to detached state*/
      if (((USBnoPD_activeCC == USBnoPD_CC1) &&
           ((USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC1] > USBNOPD_CC_VOLTAGE_MAXRD) ||
            (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC1] < USBNOPD_CC_VOLTAGE_MINRD))) ||
          ((USBnoPD_activeCC == USBnoPD_CC2) &&
           ((USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC2] > USBNOPD_CC_VOLTAGE_MAXRD) ||
            (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC2] < USBNOPD_CC_VOLTAGE_MINRD))))
      {
        /* Reset debouncing counter */
        USBnoPD_debounce_counter = 0u;

        /* Stop the debouncing timer */
        TIM_Base_Stop_IT();

        /* Go to attaching state */
        USBnoPD_State = USBnoPD_State_DETACHED;
      }
      /* If debouncing is over */
      else if (USBnoPD_debounce_counter >= USBNOPD_DEBOUNCE_ATTACH_TICKS)
      {
        /* Reset debouncing counter */
        USBnoPD_debounce_counter = 0u;

        /* Stop the debouncing timer */
        TIM_Base_Stop_IT();

        /* Turn ON Vbus */
        BSP_USBPD_PWR_VBUSDischargeOff(USBPD_PWR_TYPE_C_PORT_1);
        BSP_USBPD_PWR_VBUSOn(USBPD_PWR_TYPE_C_PORT_1);

        /* Go to attached state */
        USBnoPD_State = USBnoPD_State_ATTACHED;
      }
      break;

    case USBnoPD_State_ATTACHED:      /* Attached                        */
      /* If we detect a fault on Vbus or Vprov and
         CC voltage still correspond to an attached state */
      if (((USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_VBUSC]  > USBNOPD_VBUS_VOLTAGE_MAX) ||
           (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_VPROV] < USBNOPD_VPROV_VOLTAGE_MIN))&&
          (((USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC1] > USBNOPD_CC_VOLTAGE_MINRD) &&
            (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC1] < USBNOPD_CC_VOLTAGE_MAXRD))||
           ((USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC2] > USBNOPD_CC_VOLTAGE_MINRD) &&
            (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC2] < USBNOPD_CC_VOLTAGE_MAXRD))))
      {
        /* Cut VBUS */
        BSP_USBPD_PWR_VBUSOff(USBPD_PWR_TYPE_C_PORT_1);
        BSP_USBPD_PWR_VBUSDischargeOn(USBPD_PWR_TYPE_C_PORT_1);

        /* Go to fault state */
        USBnoPD_State = USBnoPD_State_FAULT;
      }
      /* If a detachment is detected */
      else if (((USBnoPD_activeCC == USBnoPD_CC1) &&
                (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC1] > USBNOPD_CC_VOLTAGE_MAXRD)) ||
               ((USBnoPD_activeCC == USBnoPD_CC2) &&
                (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC2] > USBNOPD_CC_VOLTAGE_MAXRD)))
      {
        /* Reset debouncing counter */
        USBnoPD_debounce_counter = 0u;

        /* Start the debouncing timer */
        TIM_Base_Start_IT();

        /* Go to detaching state */
        USBnoPD_State = USBnoPD_State_DETACHING;
      }
      break;

    case USBnoPD_State_DETACHING:     /* Detachment ongoing - debouncing */
      /* Detach abort */
      if (((USBnoPD_activeCC == USBnoPD_CC1) &&
           (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC1] < USBNOPD_CC_VOLTAGE_MAXRD)) ||
          ((USBnoPD_activeCC == USBnoPD_CC2) &&
           (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC2] < USBNOPD_CC_VOLTAGE_MAXRD)))
      {
        /* Reset debouncing counter */
        USBnoPD_debounce_counter = 0u;

        /* Stop the debouncing timer */
        TIM_Base_Stop_IT();

        /* Go back to attached state */
        USBnoPD_State = USBnoPD_State_ATTACHED;
      }
      /* If debouncing is over */
      else if (USBnoPD_debounce_counter >= USBNOPD_DEBOUNCE_DETACH_TICKS)
      {
        /* Reset debouncing counter */
        USBnoPD_debounce_counter = 0u;

        /* Stop the debouncing timer */
        TIM_Base_Stop_IT();

        /* Turn OFF Vbus */
        BSP_USBPD_PWR_VBUSOff(USBPD_PWR_TYPE_C_PORT_1);
        BSP_USBPD_PWR_VBUSDischargeOn(USBPD_PWR_TYPE_C_PORT_1);

        /* Go to discharging state */
        USBnoPD_State = USBnoPD_State_DISCHARGING;
      }
      break;

    case USBnoPD_State_DISCHARGING:   /* Vbus discharge after detach ongoing */
      /* If Vbus is considered measured as 0v */
      if (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_VBUSC]  < USBNOPD_VSAFE_VOLTAGE_MAX)
      {
        /* Reset debouncing counter */
        USBnoPD_debounce_counter = 0u;

        /* Stop the debouncing timer */
        TIM_Base_Stop_IT();

        /* Stop Vbus discharge */
        BSP_USBPD_PWR_VBUSDischargeOff(USBPD_PWR_TYPE_C_PORT_1);

        /* Go to detached state */
        USBnoPD_State = USBnoPD_State_DETACHED;
      }
      break;

    case USBnoPD_State_FAULT:         /* Hardware fault                  */
      /* In case of a fault, do nothing until a detach is detected and Vbus is at 0v */
      if ((((USBnoPD_activeCC == USBnoPD_CC1) &&
            (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC1] > USBNOPD_CC_VOLTAGE_MINOPEN)) ||
           ((USBnoPD_activeCC == USBnoPD_CC2) &&
            (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC2] > USBNOPD_CC_VOLTAGE_MINOPEN))) &&
          (USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_VBUSC]  < USBNOPD_VSAFE_VOLTAGE_MAX))
      {
        /* Stop Vbus discharge */
        BSP_USBPD_PWR_VBUSDischargeOff(USBPD_PWR_TYPE_C_PORT_1);

        /* Go to detached state */
        USBnoPD_State = USBnoPD_State_DETACHED;
      }
      break;

  default:                            /* Should not happen               */
    /* Cut VBUS */
    BSP_USBPD_PWR_VBUSOff(USBPD_PWR_TYPE_C_PORT_1);
    BSP_USBPD_PWR_VBUSDischargeOn(USBPD_PWR_TYPE_C_PORT_1);

    /* Go to fault state */
    USBnoPD_State = USBnoPD_State_FAULT;
    break;
  }
  /* 1ms delay between each state machine iterations */
  HAL_Delay(1);
}

/**
  * @brief  Calculate the VBUS voltage level corresponding to ADC raw converted data.
  * @note   Voltage level is measured though a voltage divider
  *  Example :
  *  Voltage -- Ra ----.-   ^
  *                    |    |
  *                    Rb   | vadc = VBUS * Rb /(Ra + Rb)
  *                    |    |
  *                   GND   |
  * vadc = raw_data * (ADC_FULL_SCALE / VDD)
  * @param  ADCData  ADC raw converted data (resolution 12 bits)
  * @param  Ra       value of Ra resistance
  * @param  Rb       value of Rb resistance
  * @retval analog voltage (unit: mV)
  */
static uint32_t USBnoPD_TCPP0203_ConvertADCDataToVoltage(uint32_t ADCData, uint32_t Ra, uint32_t Rb)
{
  uint32_t voltage;
  uint32_t vadc;

  /* Convert ADC RAW data to voltage */
  vadc = (ADCData * VDD_VALUE) / ADC_FULL_SCALE;

  /* If no Ra or Rb are defined, return vadc directly */
  if ((Ra == 0u) && (Rb == 0u))
  {
    voltage = vadc;
  }
  else
  {
    /* Avoid dividing by zero */
    if (Rb == 0u)
    {
      voltage = 0u;
    }
    else
    {
      /* Apply voltage divider */
      voltage = vadc * (Ra + Rb) / Rb;
    }
  }
  return voltage;
}

/**
  * @brief  Calculate the VBUS current level corresponding to ADC raw converted data.
  * @note   ADC measurement provides measurement on IANA pin.
  * @param  ADCData  ADC raw converted data (resolution 12 bits)
  * @param  Ga       value of TCPP0X Iana gain in V/V
  * @param  Rs       value of shunt resistor in milliohm
  * @retval VBUS analog current (unit: mA)
  */
static int32_t USBnoPD_TCPP0203_ConvertADCDataToCurrent(uint32_t ADCData, uint32_t Ga, uint32_t Rs)
{
  int32_t current;
  uint32_t vadc;

  /* Avoid dividing by zero */
  if ((Ga == 0u) || (Rs == 0u))
  {
    current = 0u;
  }
  else
  {
    vadc = (ADCData * VDD_VALUE) / ADC_FULL_SCALE;
    current = (int32_t)((vadc * 1000u) / (Ga * Rs));
  }

  return current;
}

/**
  * @brief  Process the ADC values and update USBnoPD_adc_converted_buffer with measured values.
  * @param  none
  * @retval none
  */
void USBnoPD_ProcessADC(void)
{
  /* Perform ADC Filtering */
  for (uint8_t i = 0u; i < USBNOPD_ADC_USED_CHANNELS; i++)
  {
    USBnoPD_adc_buffer_filtered[i] = (USBnoPD_adc_buffer_filtered[i] + USBnoPD_adc_buffer[i]) >> 1u;
  }
  /* Update the voltage buffer by converting the filtered values */
  USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC1] =
    USBnoPD_TCPP0203_ConvertADCDataToVoltage(USBnoPD_adc_buffer_filtered[USBnoPD_ADC_Index_CC1],USBNOPD_SRC1M1_NORA,USBNOPD_SRC1M1_NORB);
  USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC2] =
    USBnoPD_TCPP0203_ConvertADCDataToVoltage(USBnoPD_adc_buffer_filtered[USBnoPD_ADC_Index_CC2],USBNOPD_SRC1M1_NORA,USBNOPD_SRC1M1_NORB);
  USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_VBUSC] =
    USBnoPD_TCPP0203_ConvertADCDataToVoltage(USBnoPD_adc_buffer_filtered[USBnoPD_ADC_Index_VBUSC],USBPD_PWR_VSENSE_RA,USBPD_PWR_VSENSE_RB);
  USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_ISENSE] =
    USBnoPD_TCPP0203_ConvertADCDataToCurrent(USBnoPD_adc_buffer_filtered[USBnoPD_ADC_Index_ISENSE],USBPD_PWR_ISENSE_GA,USBPD_PWR_ISENSE_RS);
  USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_VPROV] =
    USBnoPD_TCPP0203_ConvertADCDataToVoltage(USBnoPD_adc_buffer_filtered[USBnoPD_ADC_Index_VPROV],USBPD_PWR_VSENSE_RA,USBPD_PWR_VSENSE_RB);
}

/**
  * @brief  Increment debounce counter, used in timer IRQHandler
  * @param  none
  * @retval none
  */
void USBnoPD_IncrementDebounceCount(void)
{
  USBnoPD_debounce_counter++;
}

/**
  * @brief  Handle a fault raised by the TCPP (OCP), used by EXTI IRQHandler
  * @param  none
  * @retval none
  */
void USBnoPD_TCPPFaultHandling(void)
{
    /* Cut VBUS */
  BSP_USBPD_PWR_VBUSOff(USBPD_PWR_TYPE_C_PORT_1);
  BSP_USBPD_PWR_VBUSDischargeOn(USBPD_PWR_TYPE_C_PORT_1);

  /* Go to Fault state */
  USBnoPD_State = USBnoPD_State_FAULT;
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  //UNUSED(GPIO_Pin);
  //USBnoPD_TCPPFaultHandling();
}


/**
  * @brief This function handles TIM2 global interrupt.
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  USBnoPD_IncrementDebounceCount();
}


/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  USBnoPD_ProcessADC();
}
