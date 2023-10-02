/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define IN   72000000
#define PSC  71
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

Lcd_HandleTypeDef lcd;
Lcd_PortType ports[] = {DB5_GPIO_Port, DB6_GPIO_Port, DB7_GPIO_Port};
Lcd_PinType pins[] = {DB4_Pin, DB5_Pin, DB6_Pin, DB7_Pin};
char rpm_lcd[5], rpm_perc_lcd[5], adc_val_lcd[5], adc_perc_lcd[5]; // 32 bits = 4*8 , 5 = 4 + 1 (end of string char)

uint32_t clk ; //timer clock
uint8_t mode = 0; // 1 represents active, 0 represents NOT active
uint32_t D1=0, D2=0; // active duration limits
uint32_t clk_per_rotation = 0 ; //duration of one shaft rotation
uint32_t N = 0; //number of interrupts (ARR overflow)
uint32_t f_captured = 0; //frequency by which the shaft rotates
uint32_t one_min_count = 0;
uint8_t one_min_flag = 0;// for checking the speed of shaft each minute
uint16_t adc_val = 0; //adc detected value
uint16_t adc_perc = 0;
uint32_t rpm = 0; //for shaft
uint32_t rpm_max = 0;
uint32_t rpm_perc =0 ;

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
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  // Initializations:
  HAL_TIM_Base_Start(&htim1);
  HAL_TIM_IC_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_ADC_Start(&hadc1);
  clk = IN/(PSC+1);


  //1 ms counter
  HAL_TIM_Base_Start(&htim3);

  //first minute
  TIM2 -> CCR1 = 99; //maximum value for PWM
  HAL__delay(1000);
  Lcd_cursor(&lcd, 1, 1);
  sprintf(rpm_lcd, "%ld" , rpm);
  lcd = Lcd_create(ports, pins, RS_GPIO_Port, RS_Pin, EN_GPIO_Port, EN_Pin, LCD_4_BIT_MODE );
  Lcd_string(&lcd, rpm_lcd);
  rpm_max = rpm;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	 Lcd_clear(&lcd);

	 adc_val = HAL_ADC_GetValue(&hadc1);
	 rpm_perc = (uint32_t)(((float)rpm /(float)rpm_max)*100);

	 Lcd_cursor(&lcd, 1, 1);
	 sprintf(rpm_lcd, "%ld", rpm);
	 lcd = Lcd_create(ports, pins,RS_GPIO_Port, RS_Pin, EN_GPIO_Port, EN_Pin, LCD_4_BIT_MODE );
	 Lcd_string(&lcd, rpm_lcd);

	// if(one_min_flag == 0){

	 //}
	 if(one_min_flag == 1){
		 Lcd_cursor(&lcd, 1, 10);
		 sprintf(rpm_perc_lcd, "%ld", rpm_perc);
		 Lcd_string(&lcd, rpm_perc_lcd);

		 Lcd_cursor(&lcd, 2, 1);
		 sprintf(adc_val_lcd, "%d", adc_val);
		 Lcd_string(&lcd, adc_val_lcd);

		 Lcd_cursor(&lcd, 2, 10);
		 sprintf(adc_perc_lcd, "%d", adc_perc);
		 Lcd_string(&lcd, adc_perc_lcd);
	 }
	 HAL_Delay(50);


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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(mode == 0){
		D1 = TIM1 -> CCR1;
		mode = 1;
		N = 0;
	}
	if(mode == 1){
		D2 = TIM1 ->CCR1;
		clk_per_rotation = (D2 + N*(TIM1->ARR)) - D1;
		f_captured = (uint32_t)(clk/clk_per_rotation);
		rpm = f_captured*60;
		mode = 0;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim1){
		N = N + 1;
	}
	if(htim == &htim3){
		if(one_min_flag == 1){
			if(adc_perc > rpm_perc){
				TIM2 -> CCR1 = (TIM2 ->CCR1) + 1;
			}
			if(adc_perc < rpm_perc){
				TIM2 -> CCR1 = (TIM2 ->CCR1) - 1;
			}
		}
		one_min_count++;
		if(one_min_count == 60000){
			one_min_count = 1;
		}
	}
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
