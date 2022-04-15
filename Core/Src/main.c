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
#include "CSE.h"
#include "queue.h"
#include "stdlib.h"
#include "SIM0.h"

#include "smile_data.h"
#include "smile_callbacks.h"
#include "smile.h"
#include "canlib_data.h"
#include "canlib_callbacks.h"
#include "canlib.h"

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

const task_info SM_task_info = { { .name = "SM_task", .stack_size = 1024 * 4,
		.priority = (osPriority_t) osPriorityLow2, }, .periodicity = 0.8,
		.offset = 0 * 0.000, .execution_time = 0.010, };

const task_info SIM_task_info = { { .name = "SIM_task", .stack_size = 3000 * 4,
		.priority = (osPriority_t) osPriorityLow, }, .periodicity = 0.8,
		.offset = 0 * 0.230, .execution_time = 0.1, };

const task_info COOL_task_info = { { .name = "COOL_task", .stack_size = 160 * 4,
		.priority = (osPriority_t) osPriorityNormal6, }, .periodicity = 0.8,
		.offset = 0 * 0.220, .execution_time = 0.001, };

const task_info CSE_task_info = { { .name = "CSE_task", .stack_size = 1024 * 4,
		.priority = (osPriority_t) osPriorityNormal5, }, .periodicity = 0.8,
		.offset = 0 * 0.170, .execution_time = 0.035, };

const task_info CAN_task_info = { { .name = "CAN_task", .stack_size = 512 * 4,
		.priority = (osPriority_t) osPriorityBelowNormal2, },
		.periodicity = 0.8, .offset = 0 * 0.140, .execution_time = 0.020, };

const task_info COM_task_info = { { .name = "COM_task", .stack_size = 2048 * 4,
		.priority = (osPriority_t) osPriorityLow4, }, .periodicity = 0.8,
		.offset = 0 * 0.060, .execution_time = 0.040, };

const task_info ADC_task_info = { { .name = "ADC_task", .stack_size = 160 * 4,
		.priority = (osPriority_t) osPriorityNormal3, }, .periodicity = 0.8,
		.offset = 0 * 0.050, .execution_time = 0.001, };

const task_info GPIO_task_info = { { .name = "GPIO_task", .stack_size = 160 * 4,
		.priority = (osPriority_t) osPriorityNormal2, }, .periodicity = 0.8,
		.offset = 0 * 0.040, .execution_time = 0.001, };

const task_info IMD_task_info = { { .name = "IMD_task", .stack_size = 256 * 4,
		.priority = (osPriority_t) osPriorityLow5, }, .periodicity = 0.8,
		.offset = 0 * 0.60, .execution_time = 0.001, };

const task_info event_handler_task_info = { { .name = "event_handler_task",
		.stack_size = 128 * 4, .priority = (osPriority_t) osPriorityHigh, },
		.periodicity = 0.8, .offset = 0 * 0.020, .execution_time = 0.001, };

const task_info IWDG_task_info = { { .name = "IWDG_task", .stack_size = 160 * 4,
		.priority = (osPriority_t) osPriorityLow1, }, .periodicity = 0.8,
		.offset = 0 * 0.010, .execution_time = 0.001, };

const queue_info GPIO_queue_info = { .element_count = 1, .element_size =
		sizeof(GPIO_t), };

const queue_info IMD_queue_info = { .element_count = 1, .element_size =
		sizeof(IMD_t), };

const queue_info temperatures_queue_info = { .element_count = 1, .element_size =
		sizeof(temperatures_t), };

const queue_info accumulator_current_queue_info = { .element_count = 1,
		.element_size = sizeof(double*), };

const queue_info accumulator_voltage_queue_info = { .element_count = 1,
		.element_size = sizeof(double*), };

const queue_info vehicle_voltage_queue_info = { .element_count = 1,
		.element_size = sizeof(double*), };

const queue_info cell_voltages_queue_info = { .element_count = 1,
		.element_size = sizeof(double*), };

const queue_info cell_temperatures_queue_info = { .element_count = 1,
		.element_size = sizeof(double*), };

const queue_info start_drive_queue_info = { .element_count = 1, .element_size =
		sizeof(uint8_t), };

const queue_info start_charge_queue_info = { .element_count = 1, .element_size =
		sizeof(uint8_t), };

const queue_info start_balance_queue_info = { .element_count = 1,
		.element_size = sizeof(uint8_t), };

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

	initialize_CAN(&hcan1, &hcan2);
	initialize_LTC(&hspi2);

	/* Initialize kernel */
	osKernelInitialize();

	/* INITIALIZE QUEUES */

	GPIO_queue = xQueueCreate(GPIO_queue_info.element_count,
			GPIO_queue_info.element_size);

	IMD_queue = xQueueCreate(IMD_queue_info.element_count,
			IMD_queue_info.element_size);

	temperatures_queue = xQueueCreate(temperatures_queue_info.element_count,
			temperatures_queue_info.element_size);

	accumulator_voltage_queue = xQueueCreate(
			accumulator_voltage_queue_info.element_count,
			accumulator_voltage_queue_info.element_size);

	vehicle_voltage_queue = xQueueCreate(
			vehicle_voltage_queue_info.element_count,
			vehicle_voltage_queue_info.element_size);

	accumulator_current_queue = xQueueCreate(
			accumulator_current_queue_info.element_count,
			accumulator_current_queue_info.element_size);

	cell_voltages_queue = xQueueCreate(cell_voltages_queue_info.element_count,
			cell_voltages_queue_info.element_size);

	cell_temperatures_queue = xQueueCreate(
			cell_temperatures_queue_info.element_count,
			cell_temperatures_queue_info.element_size);

	start_drive_queue = xQueueCreate(start_drive_queue_info.element_count,
			start_drive_queue_info.element_size);

	start_charge_queue = xQueueCreate(start_charge_queue_info.element_count,
			start_charge_queue_info.element_size);

	start_balance_queue = xQueueCreate(start_balance_queue_info.element_count,
			start_balance_queue_info.element_size);

	/* INITIALIZE TASKS */

	first_tick = 0.25 * TICK2HZ + osKernelGetTickCount(); // Wait for segger, etc

	SM_task_handle = osThreadNew(start_SM_task, NULL, &SM_task_info.attributes);

	SIM_task_handle = osThreadNew(start_SIM_task, NULL,
			&SIM_task_info.attributes);

	CSE_task_handle = osThreadNew(start_CSE_task, NULL,
			&CSE_task_info.attributes);

	IMD_task_handle = osThreadNew(start_IMD_task, NULL,
			&IMD_task_info.attributes);

	GPIO_task_handle = osThreadNew(start_GPIO_task, NULL,
			&GPIO_task_info.attributes);

	ADC_task_handle = osThreadNew(start_ADC_task, NULL,
			&ADC_task_info.attributes);

	COOL_task_handle = osThreadNew(start_COOL_task, NULL,
			&COOL_task_info.attributes);

	CAN_task_handle = osThreadNew(start_CAN_task, NULL,
			&CAN_task_info.attributes);

	COM_task_handle = osThreadNew(start_COM_task, NULL,
			&COM_task_info.attributes);

	IWDG_task_handle = osThreadNew(start_IWDG_task, NULL,
			&IWDG_task_info.attributes);

	event_handler_task_handle = osThreadNew(start_event_handler_task, NULL,
			&event_handler_task_info.attributes);

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
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
uint8_t vv;
uint8_t av;
uint16_t state;

/* ENTER TASKS */
void start_SM_task(void *argument) {
	SEGGER_SYSVIEW_Start();

	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * SM_task_info.periodicity;

	/* Make task-specific structures */
	ams_parameters.Ts_f32 = SM_task_info.periodicity;
	double *cell_voltages;

	xQueueReceive(start_drive_queue, &ams_inputs.drive_u8, 0);
	xQueueReceive(start_charge_queue, &ams_inputs.charge_u8, 0);
	xQueueReceive(start_balance_queue, &ams_inputs.balance_u8, 0);

	/* Wait until offset */
	next_tick += TICK2HZ * SM_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */
		xQueueReceive(cell_voltages_queue, &cell_voltages, 0);

		ams_inputs.AIR_minus_closed_u8 = get_air_minus_ext();
		ams_inputs.AIR_plus_closed_u8 = get_air_plus_ext();
		ams_inputs.precharge_closed_u8 = get_precharge_ext();
		ams_inputs.SC_u8 = get_sc_probe_ext();
		ams_inputs.accumulator_voltage_f64 = SIM0_Y.accumulator_voltage;
		ams_inputs.vehicle_voltage_f64 = SIM0_Y.vehicle_voltage;
		ams_inputs.drive_u8 = 0;
		ams_inputs.charge_u8 = 1;
		ams_inputs.charger_is_live_u8 = 1;
		ams(&state);
		set_air_minus_ext(ams_outputs.enable_AIR_minus_u8);
		set_air_plus_ext(ams_outputs.enable_AIR_plus_u8);
		set_precharge_ext(ams_outputs.enable_precharge_u8);

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
		GPIO.AMS_error_latched = get_ams_error_latched_ext();
		GPIO.IMD_error_latched = get_imd_error_latched_ext();
		GPIO.SC_probe = get_sc_probe_ext();
		GPIO.IMD_ok = get_imd_ok_ext();
		GPIO.AIR_plus_closed = get_air_plus_ext();
		GPIO.AIR_minus_closed = get_air_minus_ext();
		GPIO.precharge_closed = get_precharge_ext();

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
	uint16_t adc_readings[4 * 16];
	ADC_initialize(&htim3);
	temperatures_t temperatures;

	/* Wait until offset */
	next_tick += TICK2HZ * ADC_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */
		ADC_step((uint32_t*) &adc_readings, sizeof((uint32_t*) adc_readings));
		interpret_ADC_buffer(&temperatures, adc_readings, 16);

		xQueueOverwrite(temperatures_queue, &temperatures);

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
	const float valid_data_time_constraint = 5.0;

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

	uint16_t valid_data_sample_constraint = (valid_data_time_constraint
			/ COM_task_info.periodicity);
	if (valid_data_sample_constraint < 1) {
		valid_data_sample_constraint = 1;
	}

	double *cell_voltages;
	uint16_t cell_voltages_valid;
	double *cell_temperatures;
	uint16_t cell_temperatures_valid;
	double current;
	uint16_t current_valid;

	/* Wait until offset */
	next_tick += TICK2HZ * COM_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */

		LTC_acquire_data(1);

		cell_voltages = &SIM0_Y.cell_voltages;
		cell_temperatures = &SIM0_Y.cell_temperatures;
		cell_voltages_valid = 0x3FFF;
		cell_temperatures_valid = 0x3FFF;

		current_valid = xQueueReceive(accumulator_current_queue, &current, 0);

		//cell_voltages = LTC_voltages;
		//cell_temperatures = LTC_temperatures;
		if (!COM_data_valid_ok(
				(cell_voltages_valid == 0x03FF)
						&& (cell_temperatures_valid == 0x03FF) && current_valid,
				valid_data_sample_constraint)) {

			//Error_Handler();
		}

		if (!COM_voltages_ok_d(cell_voltages, cell_voltages_valid,
				voltage_sample_constraint)) {

			//Error_Handler();
		}

		if (!COM_temperatures_ok_d(cell_temperatures, cell_temperatures_valid,
				temperature_sample_constraint)) {

			//Error_Handler();
		}

		if (!COM_current_ok_d(&current, current_valid,
				current_sample_constraint)) {

			//Error_Handler();

		}

		xQueueOverwrite(cell_voltages_queue, &cell_voltages);
		xQueueOverwrite(cell_temperatures_queue, &cell_temperatures);

		/* Wait until next period */
		next_tick += tick_increment;
		osDelayUntil(next_tick);
	}
}

void start_CAN_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * CAN_task_info.periodicity;

	/* Make task-specific structures */

	/* Wait until offset */
	next_tick += TICK2HZ * CAN_task_info.offset;
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
		CSE_U.current = SIM0_Y.current;
		CSE_U.y = SIM0_Y.cell_voltages[0];

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

	double *temperatures;

	/* Wait until offset */
	next_tick += TICK2HZ * COOL_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */

		if (xQueuePeek(cell_temperatures_queue, &temperatures, 0)) {
			temperature = temperatures[0];
		}

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
	SIM0_initialize();

	SIM0_P.Ts = SIM_task_info.periodicity;
	SIM0_U.SC = 1;
	SIM0_U.drive = 0;
	SIM0_U.charge = 1;
	SIM0_U.drive_current = -100;
	{
		uint8_t start_drive = SIM0_U.drive > 0.5;
		xQueueOverwrite(start_drive_queue, &start_drive);
	}
	{
		uint8_t start_charge = SIM0_U.charge > 0.5;
		xQueueOverwrite(start_charge_queue, &start_charge);
	}
	{
		uint8_t start_balance = SIM0_U.drive + SIM0_U.charge > 1.5;
		xQueueOverwrite(start_balance_queue, &start_balance);
	}

	/* Wait until offset */
	next_tick += TICK2HZ * SIM_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */
		SIM0_step();

		xQueueOverwrite(accumulator_voltage_queue, &SIM0_Y.accumulator_voltage);
		xQueueOverwrite(vehicle_voltage_queue, &SIM0_Y.vehicle_voltage);
		xQueueOverwrite(accumulator_current_queue, &SIM0_Y.current);

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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
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
void Error_Handler(void)
{
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

