/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32l4xx_nucleo.h"
#include "usbpd_usb_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

typedef enum {
  CC_STATUS_NONE,
  CC_STATUS_DEFAULT,
  CC_STATUS_1P5A,
  CC_STATUS_3P0A,
} CC_STATUS_ENUM;

#define ADC_CONVERTED_DATA_BUFFER_SIZE  (3U)

#define VDD_APPLI                       (3300U)
#define ADC_MAX_VALUE_12B               (4095U)

#define ADC_INDEX_VBUS                  (0U)
#define ADC_INDEX_CC1                   (1U)
#define ADC_INDEX_CC2                   (2U)

/* X-NUCLEO-USBPDM board is used */
    /* Value is multiplied by 5.97 (Divider R6/R7 (40.2K/200K) for VSENSE) */
#define VBUS_LOW_THRESHOLD            (800U) /* 800mV */
#define ADC_VOLTAGE_DIVIDER_RATIO     (597U)
#define ADC_VBUS_VALUE_LOW_THRESHOLD  ((((VBUS_LOW_THRESHOLD * 100U) / ADC_VOLTAGE_DIVIDER_RATIO) *  ADC_MAX_VALUE_12B) / VDD_APPLI)

/* VRd-USB: ~0.2V < VCCx < ~0.66V  */
/* VRd-1.5: ~0.66V < VCCx < ~1.23V */
/* VRd-3.0: ~1.23V < VCCx          */
#define VRD_USB_MIN                     200U    /* ~0.2V*/                         
#define VRD_USB_MAX                     660U    /* 0.66V*/                         
#define VRD_1P5A_MAX                    1230U    /* 1.23V*/    

#define ADC_VALUE_VRD_USB_MIN  ((VRD_USB_MIN *  ADC_MAX_VALUE_12B) / VDD_APPLI)
#define ADC_VALUE_VRD_USB_MAX  ((VRD_USB_MAX *  ADC_MAX_VALUE_12B) / VDD_APPLI)
#define ADC_VALUE_VRD_VRD_1P5A_MAX ((VRD_1P5A_MAX *  ADC_MAX_VALUE_12B) / VDD_APPLI)

/* Define the number of samples to be read */
#define NB_TOTAL_SAMPLES        3U /* 1 sample ready every 1ms (done in systick handler)*/

/* Time to wait for starting a new set of blinks */
#define TIME_BETWEEN_2_SET_OF_BLINKS    2000U   /* 2s */

/* Time of blinking*/
#define TIME_BLINK                      500U   /* 500ms */

/* Number of blinks depending of the RP resistor value */
#define NB_BLINK_DEFAULT                2U   /* 1 blink (need 2 LED toggles */
#define NB_BLINK_1P5A                   4U   /* 2 blink (need 4 LED toggles */
#define NB_BLINK_3P0A                   6U   /* 3 blink (need 6 LED toggles */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

osThreadId defaultTaskHandle;
/* USER CODE BEGIN PV */
uint16_t aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]; /*!< ADC group regular conversion data */

/* Variable to save the previous CC status detected */
CC_STATUS_ENUM  PreviousCCStatus = CC_STATUS_NONE;

/* To avoid error in CC status reading (due to PD messages sent by port partner), 
   we will take several samples before confirmation of the CC status */
uint8_t NbCCStatus_Read = 0;

/* CC status correctly detected */
uint8_t CCDetected = 0;

/* Queue used for LED tasks */
osMessageQId LEDQueue;

/**/
/* 
  Variable to save the total number of blinks used when a NO PD capable is detected:
  - 1 blink: NO PD with RP set to default (500mA)
  - 2 blinks: NO PD with RP set to 1.5A
  - 3 blinks: NO PD with RP set to 3.5A
*/
uint8_t TotalBlinking = 0;
/* Variable to save the ongoing blinks */
uint8_t NbBlinking = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
void StartDefaultTask(void const * argument);

/* USER CODE BEGIN PFP */
void LED_Task(void const * argument);
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
  BSP_LED_Init(LED4);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  osMessageQDef(queueLED, 1, uint16_t);
  LEDQueue = osMessageCreate(osMessageQ(queueLED), NULL);
  osThreadDef(LED, LED_Task, osPriorityNormal, 0, 128);
  osThreadCreate(osThread(LED), NULL);
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 3;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_ADC;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_MSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 3;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */
  
  /* WD  not used for VBUS 
  AnalogWDGConfig.HighThreshold = ((840 * 4095) / 3300);
  AnalogWDGConfig.LowThreshold =  ((133 * 4095) / 3300);
  */
  HAL_ADCEx_Calibration_Start(&hadc1, sConfig.SingleDiff);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)aADCxConvertedData, hadc1.Init.NbrOfConversion);
  /* USER CODE END ADC1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(VCC_OUT_GPIO_Port, VCC_OUT_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : VCC_OUT_Pin */
  GPIO_InitStruct.Pin = VCC_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(VCC_OUT_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void Check_TypeC_Connection(void)
{
    /* Check if VBUS is present */
    if (aADCxConvertedData[ADC_INDEX_VBUS] > ADC_VBUS_VALUE_LOW_THRESHOLD)
    {
      USBPD_USBIF_DeviceStart(0);
      CC_STATUS_ENUM cc_status;
      /* Check value of the Rp Resistor on CC1 line */
      if (aADCxConvertedData[ADC_INDEX_CC1] > ADC_VALUE_VRD_USB_MIN)
      {
        /* CC1 line is connected*/
        if (aADCxConvertedData[ADC_INDEX_CC1] < ADC_VALUE_VRD_USB_MAX)
        {
          /* VRd-USB default is presented */
          cc_status = CC_STATUS_DEFAULT;
        }
        else if (aADCxConvertedData[ADC_INDEX_CC1] < ADC_VALUE_VRD_VRD_1P5A_MAX)
        {
          /* VRd-1.5A is presented */
          cc_status = CC_STATUS_1P5A;
        }
        else
        {
          /* VRd-3.0A is presented */
          cc_status = CC_STATUS_3P0A;
        }
      }
      else
      {
        /* CC2 line is connected*/
        if (aADCxConvertedData[ADC_INDEX_CC2] < ADC_VALUE_VRD_USB_MAX)
        {
          /* VRd-USB default is presented */
          cc_status = CC_STATUS_DEFAULT;
        }
        else if (aADCxConvertedData[ADC_INDEX_CC2] < ADC_VALUE_VRD_VRD_1P5A_MAX)
        {
          /* VRd-1.5A is presented */
          cc_status = CC_STATUS_1P5A;
        }
        else
        {
          /* VRd-3.0A is presented */
          cc_status = CC_STATUS_3P0A;
        }
      }
      
      /* Check the validity of the CC status 
         Accept only after NB_TOTAL_SAMPLES samples */
      if (NbCCStatus_Read < NB_TOTAL_SAMPLES)
      {
        /* If previous is the same as the current one */
        if ((PreviousCCStatus == CC_STATUS_NONE) || (cc_status == PreviousCCStatus))
        {
          PreviousCCStatus = cc_status;
          NbCCStatus_Read++;
        }
        else
        {
          /* Read NB_TOTAL_SAMPLES more samples */
          NbCCStatus_Read = 0;
          PreviousCCStatus = CC_STATUS_NONE;
        }
      }
      else
      {
        /* Sample is valid, we can send a msg to LED task to blink accordingly */
        if (CCDetected == 0)
        {
          switch (cc_status)
          {
          case CC_STATUS_DEFAULT:
            (void)osMessagePut(LEDQueue, DPM_USER_EVENT_NOPD_DEFAULT, 0);
            break;
          case CC_STATUS_1P5A:
            (void)osMessagePut(LEDQueue, DPM_USER_EVENT_NOPD_1P5A, 0);
            break;
          case CC_STATUS_3P0A:
            (void)osMessagePut(LEDQueue, DPM_USER_EVENT_NOPD_3P0A, 0);
            break;
          case CC_STATUS_NONE:
          default:
            break;
          }
          CCDetected = 1;
        }
      }
    }
    else
    {
      PreviousCCStatus = CC_STATUS_NONE;
      CCDetected = 0;
      (void)osMessagePut(LEDQueue, DPM_USER_EVENT_DETACH, 0);
      USBPD_USBIF_DeviceStop(0);
    }
}

void LED_Task(void const * argument)
{
  uint32_t _timing = osWaitForever;
  /* Infinite loop */
  for(;;)
  {
    osEvent event = osMessageGet(LEDQueue, _timing);
    
    /* Check the CC attach detected on Type-C */
    switch ((DPM_USER_EVENT)event.value.v)
    {
    /* PD power supply is detected, only switch on the LED */
    case DPM_USER_EVENT_EXPLICIT_CONTRACT:
      BSP_LED_On(LED4);
      _timing = osWaitForever;
      break;
    /*NO PD power supply detected, blink linked to presented RP resistor */
    case DPM_USER_EVENT_NOPD_DEFAULT:
      TotalBlinking = NB_BLINK_DEFAULT;    
      _timing = 0;
      break;
    case DPM_USER_EVENT_NOPD_1P5A:
      TotalBlinking = NB_BLINK_1P5A;    
      _timing = 0;
      break;
    case DPM_USER_EVENT_NOPD_3P0A:
      TotalBlinking = NB_BLINK_3P0A;    
      _timing = 0;
      break;
    case DPM_USER_EVENT_TIMER:
      if (NbBlinking < TotalBlinking)
      {
        BSP_LED_Toggle(LED4);
        _timing = TIME_BLINK;
        NbBlinking++;
      }
      else
      {
        /* Wait for time before next set of blinks */
        _timing = TIME_BETWEEN_2_SET_OF_BLINKS;
        NbBlinking = 0;
      }
      break;
    case DPM_USER_EVENT_DETACH:
    default:
      BSP_LED_Off(LED4);
      _timing = osWaitForever;
      break;
    }
  }
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
