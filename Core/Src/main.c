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
#include "cmsis_os.h"
#include "can.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "programme_structure.h"

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
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int main() {

	//SEGGER_SYSVIEW_Conf();

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals  - in the correct order*/
	MX_GPIO_Init();
	MX_SPI2_Init();
	MX_CAN1_Init();
	MX_CAN2_Init();
	MX_TIM1_Init();
	MX_DMA_Init();
	MX_TIM3_Init();
	MX_TIM2_Init();
	MX_ADC1_Init();

	/* Initialize peripherals */
	//initialize_can(&hcan1, &hcan2);
	//initialize_LTC(&hspi2);

	/* Initialize kernel */
	osKernelInitialize();

	/* Start threads */

	SM_task_handle = osThreadNew(start_SM_task, NULL, &SM_task_attributes);

	IWDG_task_handle = osThreadNew(start_IWDG_task, NULL,
			&IWDG_task_attributes);

	event_handler_task_handle = osThreadNew(start_event_handler_task, NULL,
			&event_handler_task_attributes);

	IMD_task_handle = osThreadNew(start_IMD_task, NULL, &IMD_task_attributes);

	GPIO_task_handle = osThreadNew(start_GPIO_task, NULL,
			&GPIO_task_attributes);

	ADC_task_handle = osThreadNew(start_ADC_task, NULL, &ADC_task_attributes);

	COM_task_handle = osThreadNew(start_COM_task, NULL, &COM_task_attributes);

	CAN_rx_task_handle = osThreadNew(start_CAN_rx_task, NULL,
			&CAN_rx_task_attributes);

	CAN_tx_task_handle = osThreadNew(start_CAN_tx_task, NULL,
			&CAN_tx_task_attributes);

	COOL_task_handle = osThreadNew(start_COOL_task, NULL,
			&COOL_task_attributes);

	SIM_task_handle = osThreadNew(start_SIM_task, NULL, &SIM_task_attributes);

	/* Launch RTOS ! */
	osKernelStart();

	for (;;)
		;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI
			| RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 84;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* ENTER TASKS */

void start_SM_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = osKernelGetTickCount();
	uint32_t tick_increment = configTICK_RATE_HZ * SM_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += configTICK_RATE_HZ * SM_task_info.offset;
	osDelayUntil(next_tick);

	/* Enter periodic behaviour */
	for (;;) {

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_IWDG_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = osKernelGetTickCount();
	uint32_t tick_increment = configTICK_RATE_HZ * IWDG_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += configTICK_RATE_HZ * IWDG_task_info.offset;
	osDelayUntil(next_tick);

	/* Enter periodic behaviour */
	for (;;) {

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_event_handler_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = osKernelGetTickCount();
	uint32_t tick_increment = configTICK_RATE_HZ * event_handler_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += configTICK_RATE_HZ * event_handler_task_info.offset;
	osDelayUntil(next_tick);

	/* Enter periodic behaviour */
	for (;;) {

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_IMD_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = osKernelGetTickCount();
	uint32_t tick_increment = configTICK_RATE_HZ * IMD_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += configTICK_RATE_HZ * IMD_task_info.offset;
	osDelayUntil(next_tick);

	/* Enter periodic behaviour */
	for (;;) {

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_GPIO_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = osKernelGetTickCount();
	uint32_t tick_increment = configTICK_RATE_HZ * GPIO_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += configTICK_RATE_HZ * GPIO_task_info.offset;
	osDelayUntil(next_tick);

	/* Enter periodic behaviour */
	for (;;) {

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_ADC_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = osKernelGetTickCount();
	uint32_t tick_increment = configTICK_RATE_HZ * ADC_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += configTICK_RATE_HZ * ADC_task_info.offset;
	osDelayUntil(next_tick);

	/* Enter periodic behaviour */
	for (;;) {

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_COM_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = osKernelGetTickCount();
	uint32_t tick_increment = configTICK_RATE_HZ * COM_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += configTICK_RATE_HZ * COM_task_info.offset;
	osDelayUntil(next_tick);

	/* Enter periodic behaviour */
	for (;;) {

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_CAN_rx_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = osKernelGetTickCount();
	uint32_t tick_increment = configTICK_RATE_HZ * CAN_rx_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += configTICK_RATE_HZ * CAN_rx_task_info.offset;
	osDelayUntil(next_tick);

	/* Enter periodic behaviour */
	for (;;) {

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_CAN_tx_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = osKernelGetTickCount();
	uint32_t tick_increment = configTICK_RATE_HZ * CAN_tx_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += configTICK_RATE_HZ * CAN_tx_task_info.offset;
	osDelayUntil(next_tick);

	/* Enter periodic behaviour */
	for (;;) {

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_CSE_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = osKernelGetTickCount();
	uint32_t tick_increment = configTICK_RATE_HZ * CSE_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += configTICK_RATE_HZ * CSE_task_info.offset;
	osDelayUntil(next_tick);

	/* Enter periodic behaviour */
	for (;;) {

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_COOL_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = osKernelGetTickCount();
	uint32_t tick_increment = configTICK_RATE_HZ * COOL_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += configTICK_RATE_HZ * COOL_task_info.offset;
	osDelayUntil(next_tick);

	/* Enter periodic behaviour */
	for (;;) {

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_SIM_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = osKernelGetTickCount();
	uint32_t tick_increment = configTICK_RATE_HZ * SIM_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += configTICK_RATE_HZ * SIM_task_info.offset;
	osDelayUntil(next_tick);

	/* Enter periodic behaviour */
	for (;;) {

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM5 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM5) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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

