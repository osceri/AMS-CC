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
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "iwdg.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SEGGER_SYSVIEW.h"

#include "programme_tasks.h"
#include "programme_queues.h"
#include "programme_data.h"
#include "programme_functions.h"

#include "queue.h"
#include "rtos_LTC.h"
#include "COM.h"
#include "PID.h"
#include "FAN.h"
#include "IMD.h"
#include "SM.h"
#include "SIM.h"
#include "CSE.h"
#include "queue.h"
#include "stdlib.h"

#include "Accumulator.h"

#define ARM_MATH_CM4
#include "arm_math.h"
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

const osThreadAttr_t SM_task_attributes = { .name = "SM_task", .stack_size =
		1024 * 4, .priority = (osPriority_t) osPriorityLow1, };
const osThreadAttr_t SIM_task_attributes = { .name = "SIM_task", .stack_size =
		2048 * 4, .priority = (osPriority_t) osPriorityBelowNormal, };
const osThreadAttr_t COOL_task_attributes = { .name = "COOL_task", .stack_size =
		160 * 4, .priority = (osPriority_t) osPriorityNormal6, };
const osThreadAttr_t CSE_task_attributes = { .name = "CSE_task", .stack_size =
		1024 * 4, .priority = (osPriority_t) osPriorityNormal5, };
const osThreadAttr_t CAN_tx_task_attributes =
		{ .name = "CAN_tx_task", .stack_size = 512 * 4, .priority =
				(osPriority_t) osPriorityBelowNormal2, };
const osThreadAttr_t CAN_rx_task_attributes =
		{ .name = "CAN_rx_task", .stack_size = 512 * 4, .priority =
				(osPriority_t) osPriorityBelowNormal1, };
const osThreadAttr_t COM_task_attributes = { .name = "COM_task", .stack_size =
		2048 * 4, .priority = (osPriority_t) osPriorityLow4, };
const osThreadAttr_t ADC_task_attributes = { .name = "ADC_task", .stack_size =
		160 * 4, .priority = (osPriority_t) osPriorityNormal3, };
const osThreadAttr_t GPIO_task_attributes = { .name = "GPIO_task", .stack_size =
		160 * 4, .priority = (osPriority_t) osPriorityNormal2, };
const osThreadAttr_t IMD_task_attributes = { .name = "IMD_task", .stack_size =
		256 * 4, .priority = (osPriority_t) osPriorityLow5, };
const osThreadAttr_t event_handler_task_attributes = { .name =
		"event_handler_task", .stack_size = 128 * 4, .priority =
		(osPriority_t) osPriorityHigh, };

const osThreadAttr_t IWDG_task_attributes = { .name = "IWDG_task", .stack_size =
		160 * 4, .priority = (osPriority_t) osPriorityLow, };
const task_info SM_task_info = { .periodicity = 0.8, .offset = 0 * 0.000,
		.execution_time = 0.010, };
const task_info SIM_task_info = { .periodicity = 0.8, .offset = 0 * 0.230,
		.execution_time = 0.1, };
const task_info COOL_task_info = { .periodicity = 0.8, .offset = 0 * 0.220,
		.execution_time = 0.001, };
const task_info CSE_task_info = { .periodicity = 0.8, .offset = 0 * 0.170,
		.execution_time = 0.035, };
const task_info CAN_tx_task_info = { .periodicity = 0.8, .offset = 0 * 0.140,
		.execution_time = 0.020, };
const task_info CAN_rx_task_info = { .periodicity = 0.8, .offset = 0 * 0.110,
		.execution_time = 0.020, };
const task_info COM_task_info = { .periodicity = 0.8, .offset = 0 * 0.060,
		.execution_time = 0.040, };
const task_info ADC_task_info = { .periodicity = 0.8, .offset = 0 * 0.050,
		.execution_time = 0.001, };
const task_info GPIO_task_info = { .periodicity = 0.8, .offset = 0 * 0.040,
		.execution_time = 0.001, };
const task_info IMD_task_info = { .periodicity = 0.8, .offset = 0 * 0.60,
		.execution_time = 0.001, };
const task_info event_handler_task_info = { .periodicity = 0.8, .offset = 0
		* 0.020, .execution_time = 0.001, };
const task_info IWDG_task_info = { .periodicity = 0.8, .offset = 0 * 0.010,
		.execution_time = 0.001, };

const queue_info state_queue_info = { .element_count = 1, .element_size =
		sizeof(state_t), };
const queue_info charge_state_queue_info = { .element_count = 1, .element_size =
		sizeof(charge_state_t), };
const queue_info GPIO_queue_info = { .element_count = 1, .element_size =
		sizeof(GPIO_t), };
const queue_info IMD_queue_info = { .element_count = 1, .element_size =
		sizeof(IMD_t), };
const queue_info can_rx_queue_info = { .element_count = 8, .element_size =
		sizeof(can_queue_element_t), };
const queue_info can1_tx_queue_info = { .element_count = 72, .element_size =
		sizeof(can_queue_element_t), };
const queue_info can2_tx_queue_info = { .element_count = 72, .element_size =
		sizeof(can_queue_element_t), };
const queue_info ams_temperatures_queue_info = { .element_count = 1,
		.element_size = sizeof(ams_temperatures_t), };
const queue_info dbu_status_1_queue_info = { .element_count = 1, .element_size =
		sizeof(dbu_status_1_t), };
const queue_info ivt_msg_result_i_queue_info = { .element_count = 1,
		.element_size = sizeof(ivt_msg_result_i_t), };
const queue_info ivt_msg_result_u1_queue_info = { .element_count = 1,
		.element_size = sizeof(ivt_msg_result_u1_t), };
const queue_info ivt_msg_result_u3_queue_info = { .element_count = 1,
		.element_size = sizeof(ivt_msg_result_u3_t), };

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int main() {
	/* INSTRUMENTATE */
	SEGGER_SYSVIEW_Conf();

	/* RESET PERIPHERALS, ETC. */
	HAL_Init();

	/* CONFIGURE SYSTEM CLOCK */
	SystemClock_Config();

	/* INITIALZE PERIPHERALS */
	MX_GPIO_Init();
	MX_SPI2_Init();
	MX_CAN1_Init();
	MX_CAN2_Init();
	MX_TIM1_Init();
	MX_DMA_Init();
	MX_TIM3_Init();
	MX_TIM2_Init();
	MX_ADC1_Init();

	//initialize_can(&hcan1, &hcan2);
	initialize_LTC(&hspi2);
	/* Initialize kernel */
	osKernelInitialize();

	/* INITIALIZE QUEUES */

	state_queue = xQueueCreate(state_queue_info.element_count,
			state_queue_info.element_size);

	charge_state_queue = xQueueCreate(charge_state_queue_info.element_count,
			charge_state_queue_info.element_size);

	GPIO_queue = xQueueCreate(GPIO_queue_info.element_count,
			GPIO_queue_info.element_size);

	IMD_queue = xQueueCreate(IMD_queue_info.element_count,
			IMD_queue_info.element_size);

	ams_temperatures_queue = xQueueCreate(
			ams_temperatures_queue_info.element_count,
			ams_temperatures_queue_info.element_size);

	dbu_status_1_queue = xQueueCreate(dbu_status_1_queue_info.element_count,
			dbu_status_1_queue_info.element_size);

	ivt_msg_result_i_queue = xQueueCreate(
			ivt_msg_result_i_queue_info.element_count,
			ivt_msg_result_i_queue_info.element_size);

	ivt_msg_result_u1_queue = xQueueCreate(
			ivt_msg_result_u1_queue_info.element_count,
			ivt_msg_result_u1_queue_info.element_size);

	ivt_msg_result_u3_queue = xQueueCreate(
			ivt_msg_result_u3_queue_info.element_count,
			ivt_msg_result_u3_queue_info.element_size);

	can_rx_queue = xQueueCreate(can_rx_queue_info.element_count,
			can_rx_queue_info.element_size);

	can1_tx_queue = xQueueCreate(can1_tx_queue_info.element_count,
			can1_tx_queue_info.element_size);

	can2_tx_queue = xQueueCreate(can2_tx_queue_info.element_count,
			can2_tx_queue_info.element_size);

	/* INITIALIZE TASKS */

	first_tick = 0.25 * TICK2HZ + osKernelGetTickCount(); // Wait for segger, etc

	SM_task_handle = osThreadNew(start_SM_task, NULL, &SM_task_attributes);

	SIM_task_handle = osThreadNew(start_SIM_task, NULL, &SIM_task_attributes);

	CSE_task_handle = osThreadNew(start_CSE_task, NULL, &CSE_task_attributes);

	IMD_task_handle = osThreadNew(start_IMD_task, NULL, &IMD_task_attributes);

	GPIO_task_handle = osThreadNew(start_GPIO_task, NULL,
			&GPIO_task_attributes);

	ADC_task_handle = osThreadNew(start_ADC_task, NULL, &ADC_task_attributes);

	COOL_task_handle = osThreadNew(start_COOL_task, NULL,
			&COOL_task_attributes);

	CAN_rx_task_handle = osThreadNew(start_CAN_rx_task, NULL,
			&CAN_rx_task_attributes);

	CAN_tx_task_handle = osThreadNew(start_CAN_tx_task, NULL,
			&CAN_tx_task_attributes);

	COM_task_handle = osThreadNew(start_COM_task, NULL, &COM_task_attributes);

	IWDG_task_handle = osThreadNew(start_IWDG_task, NULL,
			&IWDG_task_attributes);

	event_handler_task_handle = osThreadNew(start_event_handler_task, NULL,
			&event_handler_task_attributes);

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
	SEGGER_SYSVIEW_Start();

	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * SM_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += TICK2HZ * SM_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */
		state_machine_step();

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_IWDG_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * IWDG_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += TICK2HZ * IWDG_task_info.offset;
	osDelayUntil(next_tick);

	//initialize_IWDG(&hiwdg, IWDG_task_info.periodicity);

	for (;;) {
		/* Enter periodic behaviour */
		HAL_IWDG_Refresh(&hiwdg);

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_event_handler_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * event_handler_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += TICK2HZ * event_handler_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_IMD_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * IMD_task_info.periodicity;

	/* Make task-specific structures */
	IMD_t IMD;
	initialize_IMD(&htim2);

	/* Wait until offset */
	next_tick += TICK2HZ * IMD_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */

		if (xQueueReceive(IMD_queue, &IMD, 0)) {
			uint8_t frequency_range = (uint8_t) (IMD.frequency / 10);
		}

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_GPIO_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * GPIO_task_info.periodicity;

	/* Make task-specific structures */
	GPIO_t GPIO;

	/* Wait until offset */
	next_tick += TICK2HZ * GPIO_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */
		GPIO.AMS_error_latched = HAL_GPIO_ReadPin(AMS_error_latched_GPIO_Port,
		AMS_error_latched_Pin);
		GPIO.IMD_error_latched = HAL_GPIO_ReadPin(IMD_error_latched_GPIO_Port,
		IMD_error_latched_Pin);
		GPIO.SC_probe = HAL_GPIO_ReadPin(SC_probe_GPIO_Port, SC_probe_Pin);
		GPIO.IMD_ok = HAL_GPIO_ReadPin(IMD_ok_GPIO_Port, IMD_ok_Pin);
		GPIO.AIR_plus_closed = HAL_GPIO_ReadPin(AIR_plus_closed_GPIO_Port,
		AIR_plus_closed_Pin);
		GPIO.AIR_minus_closed = HAL_GPIO_ReadPin(AIR_minus_closed_GPIO_Port,
		AIR_minus_closed_Pin);
		GPIO.precharge_closed = HAL_GPIO_ReadPin(precharge_closed_GPIO_Port,
		precharge_closed_Pin);

		xQueueOverwrite(GPIO_queue, &GPIO);

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_ADC_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * ADC_task_info.periodicity;

	/* Make task-specific structures */
	ADC_initialize(&htim3);
	uint16_t adc_readings[4 * 16];
	ams_temperatures_t ams_temperatures;

	/* Wait until offset */
	next_tick += TICK2HZ * ADC_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */
		ADC_step((uint32_t*) &adc_readings, sizeof((uint32_t*) adc_readings));
		interpret_ADC_buffer(&ams_temperatures, adc_readings, 16);

		xQueueOverwrite(ams_temperatures_queue, &ams_temperatures);

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_COM_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * COM_task_info.periodicity;

	/* Make task-specific structures */
	const float voltage_time_constraint = 0.5;
	const float temperature_time_constraint = 1.0;
	const float current_time_constraint = 0.5;

	uint16_t voltage_sample_constraint = (voltage_time_constraint
			/ COM_task_info.periodicity);
	if (voltage_sample_constraint < 1) {
		voltage_sample_constraint = 1;
	}

	uint16_t temperature_sample_constraint = (temperature_time_constraint
			/ COM_task_info.periodicity);
	if (temperature_sample_constraint < 1) {
		temperature_sample_constraint = 1;
	}

	uint16_t current_sample_constraint = (current_time_constraint
			/ COM_task_info.periodicity);
	if (current_sample_constraint < 1) {
		current_sample_constraint = 1;
	}

	double *voltage;
	double *temperature;
	double *current;

	/* Wait until offset */
	next_tick += TICK2HZ * COM_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */

		//LTC_acquire_data(1);
		voltage = Accumulator_Y.Voltages;
		temperature = Accumulator_Y.Temperatures;
		current = &Accumulator_Y.DisplayCurrent;

		if (!COM_voltages_ok_d(voltage, 1, voltage_sample_constraint)) {
			//Error_Handler();
		}

		if (!COM_temperatures_ok_d(temperature, 1,
				temperature_sample_constraint)) {
			//Error_Handler();
		}

		if (!COM_current_ok_d(current, 1, current_sample_constraint)) {
			//Error_Handler();
		}

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_CAN_rx_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * CAN_rx_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += TICK2HZ * CAN_rx_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_CAN_tx_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * CAN_tx_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += TICK2HZ * CAN_tx_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_CSE_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * CSE_task_info.periodicity;

	/* Make task-specific structures */
	CSE_initialize();

	/* Wait until offset */
	next_tick += TICK2HZ * CSE_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */
		CSE_step();

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_COOL_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * COOL_task_info.periodicity;

	/* Make task-specific structures */
	PID_t PID;
	float temperature = 20;

	PID_initialize(&PID, // *ptr
			20,	// Reference temperature (sought)
			1, 	// Proportionate gain
			0,	// Proportionate gain
			0,	// Proportionate gain
			20,	// Lower PID output bound
			100	// Upper PID output bound
			);
	FAN_initialize(&htim1);

	/* Wait until offset */
	next_tick += TICK2HZ * COOL_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */

		PID_progress(&PID, temperature);
		FAN_duty_cycle(&htim1, PID.output);

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_SIM_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * SIM_task_info.periodicity;

	/* Make task-specific structures */
	initialize_SIM();
	uint16_t K = 0;

	/* Wait until offset */
	next_tick += TICK2HZ * SIM_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */
		SIM_step();

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

