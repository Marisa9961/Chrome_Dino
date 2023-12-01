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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Key.h"
#include "Game.h"
#include "OLED.h"
#include "Led.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

  Show_GameBegin();
  while(1){
    if(Get_Start())
      break;
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  OLED_Clear();
  HAL_TIM_Base_Start_IT(&htim4);
  while (1)
  {
    Game_Proc();
    Key_Proc();
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
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim4)
	{
		if(++OLED_Slow == 40) OLED_Slow = 0;
		if(++Key_Slow == 10) Key_Slow = 0;
		
		//处理小恐龙的奔跑跳跃
		Dino_Count++;
		if(Dino_Count == 50)
		{
			Dino_Flag ^= 1;
			
			if(Dino_Jump_Key == 1)
			{
				if(Dino_Jump_Flag_Flag == 0 && Jump_FinishFlag == 0)
				{
					Dino_Jump_Flag ++;
					if(Dino_Jump_Flag == 8)
						Dino_Jump_Flag_Flag = 1;
				}
				else if(Dino_Jump_Flag_Flag == 1)
				{
					Dino_Jump_Flag --;
					if(Dino_Jump_Flag == 0)
					{
						Dino_Jump_Flag_Flag = 0;
						Jump_FinishFlag = 1;
					}
				}
			}
			
			switch(Dino_Jump_Flag)
			{
				case 0:Height = 0; break;
				case 1:Height = 6; break;
				case 2:Height = 10;break;
				case 3:Height = 15;break;
				case 4:Height = 18;break;
				case 5:Height = 21;break;
				case 6:Height = 23;break;
				case 7:Height = 25;break;
				case 8:Height = 25;break;
			}
			
			Dino_Count = 0;
		}
		
		//随机生成仙人�?
		Cactus_Count++;
		if(Cactus_Count >= Cactus_CreatTime)
		{
			Cactus_CreatTime = rand() % 3;
			Cactus_CreatTime += 1;
			Cactus_CreatTime *= Cactus_CreatTime_Multiplier;
			
			Cactus_CreatNumber = rand() % 3;
			switch(Cactus_CreatNumber)
			{
				case 0:
					Cactus_Flag1 = 0;
				break;
				case 1:
					Cactus_Flag2 = 0;
				break;
				case 2:
					Cactus_Flag3 = 0;
				break;
			}
			Cactus_Count= 0;
		}
		
		//加�??
		Grade_Count++;
		if(Grade_Count == 200)
		{
			Grade ++;
			if(Grade == 50)
				Speed ++;
			if(Grade == 100)
			{
				Speed ++;
				Cactus_CreatTime_Multiplier = 500;
			}
			if(Grade == 150)
			{
				Speed ++;
				Cactus_CreatTime_Multiplier = 800;
			}
			if(Grade == 200)
				Speed ++;
			Grade_Count = 0;
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
