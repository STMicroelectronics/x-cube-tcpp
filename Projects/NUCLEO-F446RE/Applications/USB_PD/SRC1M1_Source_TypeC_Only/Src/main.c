/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "src1m1_usbpd_pwr.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
USBnoPD_StatesTypeDef USBnoPD_State = USBnoPD_State_DETACHED;
uint16_t USBnoPD_adc_buffer[USBNOPD_ADC_USED_CHANNELS] = {0};
uint16_t USBnoPD_adc_converted_buffer[USBNOPD_ADC_USED_CHANNELS] = {0};
uint16_t USBnoPD_debounce_counter = 0;
uint8_t USBnoPD_activeCC = USBnoPD_CC1; /* Default */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
static uint32_t USBnoPD_TCPP0203_ConvertADCDataToVoltage(uint32_t ADCData, uint32_t Ra, uint32_t Rb);
static int32_t  USBnoPD_TCPP0203_ConvertADCDataToCurrent(uint32_t ADCData, uint32_t Ga, uint32_t Rs);
static void USBnoPD_StateMachineRun(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(ENABLE_GPIO_Port, ENABLE_Pin, GPIO_PIN_SET);

  BSP_USBPD_PWR_Init(USBPD_PWR_TYPE_C_PORT_1);
  BSP_USBPD_PWR_SetPowerMode(USBPD_PWR_TYPE_C_PORT_1, USBPD_PWR_MODE_NORMAL);

  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&USBnoPD_adc_buffer, USBNOPD_ADC_USED_CHANNELS);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    /* Run the state machine */
    USBnoPD_StateMachineRun();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 5;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 2099;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 39;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ENABLE_GPIO_Port, ENABLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : GREEN_LED_Pin */
  GPIO_InitStruct.Pin = GREEN_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GREEN_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : FLGN_Pin */
  GPIO_InitStruct.Pin = FLGN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(FLGN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ENABLE_Pin */
  GPIO_InitStruct.Pin = ENABLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ENABLE_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* USER CODE BEGIN 4 */
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
  /* Local buffer for filtered RAW ADC values */
  static uint16_t USBnoPD_adc_buffer_filtered[USBNOPD_ADC_USED_CHANNELS];

  /* Perform ADC Filtering */
  for (uint8_t i = 0u; i < USBNOPD_ADC_USED_CHANNELS; i++)
  {
    USBnoPD_adc_buffer_filtered[i] = (USBnoPD_adc_buffer_filtered[i] + USBnoPD_adc_buffer[i]) >> 1u;
  }

  /* Update the voltage buffer by converting the filtered values */
  USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC1] =
    USBnoPD_TCPP0203_ConvertADCDataToVoltage(USBnoPD_adc_buffer_filtered[USBnoPD_ADC_Index_CC1],
                                                                         USBNOPD_SRC1M1_NORA,
                                                                         USBNOPD_SRC1M1_NORB);
  USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_CC2] =
    USBnoPD_TCPP0203_ConvertADCDataToVoltage(USBnoPD_adc_buffer_filtered[USBnoPD_ADC_Index_CC2],
                                                                         USBNOPD_SRC1M1_NORA,
                                                                         USBNOPD_SRC1M1_NORB);
  USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_VBUSC] =
    USBnoPD_TCPP0203_ConvertADCDataToVoltage(USBnoPD_adc_buffer_filtered[USBnoPD_ADC_Index_VBUSC],
                                                                         SRC1M1_VSENSE_RA,
                                                                         SRC1M1_VSENSE_RB);

  USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_ISENSE] =
    USBnoPD_TCPP0203_ConvertADCDataToCurrent(USBnoPD_adc_buffer_filtered[USBnoPD_ADC_Index_ISENSE],
                                                                         SRC1M1_ISENSE_GA,
                                                                         SRC1M1_ISENSE_RS);
  USBnoPD_adc_converted_buffer[USBnoPD_ADC_Index_VPROV] =
    USBnoPD_TCPP0203_ConvertADCDataToVoltage(USBnoPD_adc_buffer_filtered[USBnoPD_ADC_Index_VPROV],
                                                                         SRC1M1_VSENSE_RA,
                                                                         SRC1M1_VSENSE_RB);
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

  /* Turn off green LED to indicate source is OFF */
  HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);

  /* Go to Fault state */
  USBnoPD_State = USBnoPD_State_FAULT;
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
          HAL_TIM_Base_Start_IT(&htim2);

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
          HAL_TIM_Base_Start_IT(&htim2);

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
        HAL_TIM_Base_Stop_IT(&htim2);

        /* Go to attaching state */
        USBnoPD_State = USBnoPD_State_DETACHED;
      }
      /* If debouncing is over */
      else if (USBnoPD_debounce_counter >= USBNOPD_DEBOUNCE_ATTACH_TICKS)
      {
        /* Reset debouncing counter */
        USBnoPD_debounce_counter = 0u;

        /* Stop the debouncing timer */
        HAL_TIM_Base_Stop_IT(&htim2);

        /* Turn ON Vbus */
        BSP_USBPD_PWR_VBUSDischargeOff(USBPD_PWR_TYPE_C_PORT_1);
        BSP_USBPD_PWR_VBUSOn(USBPD_PWR_TYPE_C_PORT_1);

        /* Turn on green LED to indicate source is ON */
        HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);

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

        /* Turn off green LED to indicate source is OFF */
        HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);

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
        HAL_TIM_Base_Start_IT(&htim2);

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
        HAL_TIM_Base_Stop_IT(&htim2);

        /* Go back to attached state */
        USBnoPD_State = USBnoPD_State_ATTACHED;
      }
      /* If debouncing is over */
      else if (USBnoPD_debounce_counter >= USBNOPD_DEBOUNCE_DETACH_TICKS)
      {
        /* Reset debouncing counter */
        USBnoPD_debounce_counter = 0u;

        /* Stop the debouncing timer */
        HAL_TIM_Base_Stop_IT(&htim2);

        /* Turn OFF Vbus */
        BSP_USBPD_PWR_VBUSOff(USBPD_PWR_TYPE_C_PORT_1);
        BSP_USBPD_PWR_VBUSDischargeOn(USBPD_PWR_TYPE_C_PORT_1);

        /* Turn off green LED to indicate source is OFF */
        HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);

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
        HAL_TIM_Base_Stop_IT(&htim2);

        /* Stop Vbus discharge */
        BSP_USBPD_PWR_VBUSDischargeOff(USBPD_PWR_TYPE_C_PORT_1);

        /* Turn off green LED to indicate source is OFF */
        HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);

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

    /* Turn off green LED to indicate source is OFF */
    HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);

    /* Go to fault state */
    USBnoPD_State = USBnoPD_State_FAULT;
    break;
  }
  /* 1ms delay between each state machine iterations */
  HAL_Delay(1);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
