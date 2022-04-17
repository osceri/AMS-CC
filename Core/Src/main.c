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
#include "queue.h"
#include "stdlib.h"

#include "SEGGER_SYSVIEW.h"

#include "programme_tasks.h"
#include "programme_queues.h"
#include "programme_data.h"
#include "programme_functions.h"

#include "rtos_LTC.h"
#include "COM.h"
#include "PID.h"
#include "FAN.h"
#include "IMD.h"
#include "CSE.h"
#include "queue.h"
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
		.priority = (osPriority_t) osPriorityLow3, }, .periodicity = 0.1,
		.offset = 0 * 0.000, .execution_time = 0.010, };

const task_info SIM_task_info = { { .name = "SIM_task", .stack_size = 3000 * 4,
		.priority = (osPriority_t) osPriorityLow1, }, .periodicity = 0.1,
		.offset = 0 * 0.230, .execution_time = 0.1, };

const task_info COOL_task_info = { { .name = "COOL_task", .stack_size = 160 * 4,
		.priority = (osPriority_t) osPriorityNormal6, }, .periodicity = 0.1,
		.offset = 0 * 0.220, .execution_time = 0.001, };

const task_info CSE_task_info = { { .name = "CSE_task", .stack_size = 1024 * 4,
		.priority = (osPriority_t) osPriorityNormal5, }, .periodicity = 0.1,
		.offset = 0 * 0.170, .execution_time = 0.035, };

const task_info CAN_task_info = { { .name = "CAN_task", .stack_size = 512 * 4,
		.priority = (osPriority_t) osPriorityBelowNormal2, },
		.periodicity = 0.1, .offset = 0 * 0.140, .execution_time = 0.020, };

const task_info COM_task_info = { { .name = "COM_task", .stack_size = 2048 * 4,
		.priority = (osPriority_t) osPriorityLow4, }, .periodicity = 0.1,
		.offset = 0 * 0.060, .execution_time = 0.040, };

const task_info ADC_task_info = { { .name = "ADC_task", .stack_size = 160 * 4,
		.priority = (osPriority_t) osPriorityNormal3, }, .periodicity = 0.1,
		.offset = 0 * 0.050, .execution_time = 0.001, };

const task_info GPIO_task_info = { { .name = "GPIO_task", .stack_size = 160 * 4,
		.priority = (osPriority_t) osPriorityNormal2, }, .periodicity = 0.1,
		.offset = 0 * 0.040, .execution_time = 0.001, };

const task_info IMD_task_info = { { .name = "IMD_task", .stack_size = 256 * 4,
		.priority = (osPriority_t) osPriorityLow5, }, .periodicity = 0.1,
		.offset = 0 * 0.60, .execution_time = 0.001, };

const task_info event_handler_task_info = { { .name = "event_handler_task",
		.stack_size = 128 * 4, .priority = (osPriority_t) osPriorityLow, },
		.periodicity = 0.1, .offset = 5, .execution_time = 0.001, };

const task_info IWDG_task_info = { { .name = "IWDG_task", .stack_size = 160 * 4,
		.priority = (osPriority_t) osPriorityLow2, }, .periodicity = 0.1,
		.offset = 0 * 0.010, .execution_time = 0.001, };

const queue_info GPIO_queue_info = { .element_count = 1, .element_size =
		sizeof(GPIO_t), };

const queue_info IMD_queue_info = { .element_count = 1, .element_size =
		sizeof(IMD_t), };

const queue_info CSE_queue_info = { .element_count = 1, .element_size =
		sizeof(CSE_t), };

const queue_info temperatures_queue_info = { .element_count = 1, .element_size =
		sizeof(temperatures_t), };

const queue_info PID_queue_info = { .element_count = 1, .element_size =
		sizeof(pPID_t), };

const queue_info charger_queue_info = { .element_count = 1, .element_size =
		sizeof(charger_t), };

const queue_info accumulator_current_queue_info = { .element_count = 1,
		.element_size = sizeof(double), };

const queue_info accumulator_voltage_queue_info = { .element_count = 1,
		.element_size = sizeof(double), };

const queue_info vehicle_voltage_queue_info = { .element_count = 1,
		.element_size = sizeof(double), };

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

const queue_info state_queue_info = { .element_count = 1, .element_size =
		sizeof(ams_state_t), };

const queue_info error_queue_info = { .element_count = 1, .element_size =
		sizeof(error_t), };

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

#ifndef SIMULATION
	initialize_CAN(&hcan1, &hcan2);
#endif
	initialize_LTC(&hspi2);

	/* Initialize kernel */
	osKernelInitialize();

	/* INITIALIZE QUEUES */

	GPIO_queue = xQueueCreate(GPIO_queue_info.element_count,
			GPIO_queue_info.element_size);

	IMD_queue = xQueueCreate(IMD_queue_info.element_count,
			IMD_queue_info.element_size);

	CSE_queue = xQueueCreate(CSE_queue_info.element_count,
			CSE_queue_info.element_size);

	temperatures_queue = xQueueCreate(temperatures_queue_info.element_count,
			temperatures_queue_info.element_size);

	PID_queue = xQueueCreate(PID_queue_info.element_count,
			PID_queue_info.element_size);

	charger_queue = xQueueCreate(charger_queue_info.element_count,
			charger_queue_info.element_size);

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

	state_queue = xQueueCreate(state_queue_info.element_count,
			state_queue_info.element_size);

	error_queue = xQueueCreate(error_queue_info.element_count,
			error_queue_info.element_size);

	/* INITIALIZE TASKS */

	first_tick = 0.25 * TICK2HZ + osKernelGetTickCount(); // Wait for segger, etc

	SM_task_handle = osThreadNew(start_SM_task, NULL, &SM_task_info.attributes);

#ifdef SIMULATION
	SIM_task_handle = osThreadNew(start_SIM_task, NULL,
			&SIM_task_info.attributes);
#endif

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

#ifndef SIMULATION
	CAN_task_handle = osThreadNew(start_CAN_task, NULL,
			&CAN_task_info.attributes);
#endif

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
	RCC_OscInitStruct.PLL.PLLN = 160;
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
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV8;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

#define WAIT_FOR_CELL_VOLTAGES			0x0001
#define WAIT_FOR_CELL_TEMPERATURES		0x0002
#define WAIT_FOR_ACCUMULATOR_VOLTAGE	0x0004
#define WAIT_FOR_ACCUMULATOR_CURRENT	0x0008
#define WAIT_FOR_VEHICLE_VOLTAGE		0x0010
#define WAIT_FOR_DRIVE					0x0020
#define WAIT_FOR_CHARGE					0x0040
#define WAIT_FOR_BALANCE				0x0080
#define WAIT_FOR_ALL 					0xFFFF

void __wait_for_data(uint16_t FLAGS) {
	uint8_t NOT_OK = 1;

	double _d;
	double *_dptr;
	uint8_t _u8;
	while (NOT_OK) {
		osDelay(0.500 * TICK2HZ);
		NOT_OK &= !((FLAGS | WAIT_FOR_CELL_VOLTAGES)
				&& xQueuePeek(cell_voltages_queue, &_dptr, 0));
		NOT_OK &= !((FLAGS | WAIT_FOR_CELL_TEMPERATURES)
				&& xQueuePeek(cell_temperatures_queue, &_dptr, 0));
		NOT_OK &= !((FLAGS | WAIT_FOR_ACCUMULATOR_CURRENT)
				&& xQueuePeek(accumulator_current_queue, &_d, 0));
		NOT_OK &= !((FLAGS | WAIT_FOR_ACCUMULATOR_VOLTAGE)
				&& xQueuePeek(accumulator_voltage_queue, &_d, 0));
		NOT_OK &= !((FLAGS | WAIT_FOR_VEHICLE_VOLTAGE)
				&& xQueuePeek(vehicle_voltage_queue, &_d, 0));
		NOT_OK &= !((FLAGS | WAIT_FOR_DRIVE)
				&& xQueuePeek(start_drive_queue, &_u8, 0));
		NOT_OK &= !((FLAGS | WAIT_FOR_CHARGE)
				&& xQueuePeek(start_charge_queue, &_u8, 0));
		NOT_OK &= !((FLAGS | WAIT_FOR_BALANCE)
				&& xQueuePeek(start_balance_queue, &_u8, 0));
	}
}

error_t __error;
void __raise_error(error_t error) {
	__error = error;
	xQueueSend(error_queue, &error, portMAX_DELAY);
}

/* ENTER TASKS */
void start_SM_task(void *argument) {
	SEGGER_SYSVIEW_Start();

	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * SM_task_info.periodicity;

	/* Make task-specific structures */
	charger_t charger;
	ams_parameters.Ts = SM_task_info.periodicity;
	double *cell_voltages;
	double minimum;
	double mean;
	double variance;
	double maximum;

	/* Wait until offset */
	next_tick += TICK2HZ * SM_task_info.offset;
	osDelayUntil(next_tick);

	__wait_for_data(WAIT_FOR_ALL);

	set_charger_voltage_limit_ext(4.15 * 126);
	set_charger_current_limit_ext(6.6 * 2);

	for (;;) {
		/* Enter periodic behaviour */
		/* Receive every piece of data which is required to propagate the state machine */
		if (0x1 && xQueuePeek(cell_voltages_queue, &cell_voltages, 0)
				&& xQueuePeek(accumulator_current_queue,
						&ams_inputs.accumulator_current, 0)
				&& xQueuePeek(accumulator_voltage_queue,
						&ams_inputs.accumulator_voltage, 0)
				&& xQueuePeek(vehicle_voltage_queue,
						&ams_inputs.vehicle_voltage, 0)) {

			/* Calculate mean, maximum and variance of cell voltages */
			{
				mean = 0;
				variance = 0;
				maximum = cell_voltages[0];
				minimum = cell_voltages[0];
				for (int i = 0; i < 126; i++) {
					mean += cell_voltages[i];
					if (cell_voltages[i] > maximum) {
						maximum = cell_voltages[i];
					}
					if (cell_voltages[i] < minimum) {
						minimum = cell_voltages[i];
					}
				}
				mean /= 126;
				for (int i = 0; i < 126; i++) {
					double svar = cell_voltages[i] - mean;
					variance += svar * svar;
				}
			}

			/* Set the rest of the state machine inputs */
			ams_inputs.cell_voltages_variance = variance;
			ams_inputs.maximum_cell_voltage = maximum;
			ams_inputs.minimum_cell_voltage = minimum;
			ams_inputs.charger_is_awake = 1;
			ams_inputs.ams_error = get_ams_error_latched_ext();
			ams_inputs.imd_error = get_imd_error_latched_ext();
			ams_inputs.air_minus_closed = get_air_minus_ext();
			ams_inputs.air_plus_closed = get_air_plus_ext();
			ams_inputs.precharge_closed = get_precharge_ext();
			ams_inputs.SC = get_sc_probe_ext();

			/* Try to get the trigger signals */
			if (!xQueueReceive(start_drive_queue, &ams_inputs.drive, 0)) {
				ams_inputs.drive = 0;
			}
			if (!xQueueReceive(start_charge_queue, &ams_inputs.charge, 0)) {
				ams_inputs.charge = 0;
			}
			if (!xQueueReceive(start_balance_queue, &ams_inputs.balance, 0)) {
				ams_inputs.balance = 0;
			}

			ams_state_t state = ams_function(); // _step
			xQueueOverwrite(state_queue, &state);
		}

		/* If any error was produced by the state machine (1xx), then raise them */
		if (ams_outputs.set_error) {
			__raise_error((error_t) ams_outputs.set_error);

		}

		charger.charger_current_limit = 6.6 * 2; 		// 2 * C
		charger.charger_voltage_limit = 4.15 * 126;	// sought cell voltage times aount of cells
		charger.enable_charger = ams_outputs.enable_charger;

		xQueueOverwrite(charger_queue, &charger);

		set_air_minus_ext(ams_outputs.close_air_minus);
		set_air_plus_ext(ams_outputs.close_air_plus);
		set_precharge_ext(ams_outputs.close_precharge);

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

		if (xQueuePeek(IMD_queue, &IMD, 0)) {
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

		if (!GPIO.IMD_ok) {
			__raise_error(ERROR_IMD);
		}

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
	double *cell_voltages;
	double *cell_temperatures;
	double accumulator_current;
	error_t cell_voltages_error;
	error_t cell_temperatures_error;
	error_t accumulator_current_error;
	uint8_t cell_data_valid;
	uint8_t accumulator_current_valid;

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

	/* Wait until offset */
	next_tick += TICK2HZ * COM_task_info.offset;
	osDelayUntil(next_tick);

	__wait_for_data(WAIT_FOR_ACCUMULATOR_CURRENT);

	for (;;) {
		/* Enter periodic behaviour */
		/* Take the queue elements so that other tasks may not */
		xQueueReceive(cell_voltages_queue, &cell_voltages, 0);
		xQueueReceive(cell_temperatures_queue, &cell_temperatures, 0);

		/* Get new data (indirectly) form CAN */
		accumulator_current_valid = xQueuePeek(accumulator_current_queue,
				&accumulator_current, 0);
		/* Get new data over isoSPI */
		LTC_acquire_data(1);

		/* Get data from the program */
#ifdef SIMULATION
		cell_voltages = SIM0_Y.cell_voltages;
		cell_temperatures = SIM0_Y.cell_temperatures;
		cell_data_valid = 1;
#elif
		cell_voltages = LTC_voltages;
		cell_temperatures = LTC_temperatures;
		cell_data_valid = LTC_data_valid;
#endif

		/* If new cell data is available, supply the system with it */
		if (cell_data_valid) {
			xQueueOverwrite(cell_voltages_queue, &cell_voltages);
			xQueueOverwrite(cell_temperatures_queue, &cell_temperatures);
		} else {
			__raise_error(ERROR_NO_LTC_DATA);
		}

		/* Raise an error if cell voltage time constraints are not met,
		 * or if there is no data being received */
		cell_voltages_error = COM_voltages_ok(cell_voltages,
				voltage_sample_constraint);

		if (cell_voltages_error) {
			__raise_error(cell_voltages_error);
		}

		/* Raise an error if cell temperature time constraints are not met,
		 * or if there is no data being received */
		cell_temperatures_error = COM_temperatures_ok(cell_temperatures,
				!temperature_sample_constraint);

		if (cell_temperatures_error) {
			__raise_error(cell_temperatures_error);
		}

		/* If no new accumulator current was gathered, raise an error */
		if (!accumulator_current_valid) {
			__raise_error(ERROR_NO_CURRENT_DATA);
		}

		/* Raise an error if accumulator current time constraints are not met,
		 * or if there is no data being received */
		accumulator_current_error = COM_current_ok(&accumulator_current,
				current_sample_constraint);

		if (accumulator_current_error) {
			__raise_error(accumulator_current_error);
		}
		/* Wait until next period */
		while (next_tick < osKernelGetTickCount()) {
			next_tick += tick_increment;
		}
		osDelayUntil(next_tick);
	}
}

void start_CAN_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * CAN_task_info.periodicity;

	/* Make task-specific structures */
	uint32_t k = 0;

	/* Wait until offset */
	next_tick += TICK2HZ * CAN_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */
		if (!(k % 1)) {
			can1_dbu_status_1_receive();
			can2_ivt_msg_result_i_receive();
			can2_ivt_msg_result_u1_receive();
			can2_ivt_msg_result_u3_receive();
			can2_cc_status_receive();
		}
		if (!(k % 2)) {
			can1_ams_cell_temperatures_transmit();
			can1_ams_status_1_transmit();
		}
		if (!(k % 3)) {
			can2_charger_config_transmit();
		}
		if (!(k % 4)) {
			can1_ams_temperatures_transmit();
			can1_ams_cell_voltages_transmit();
		}
		/* Wait until next period */
		while (next_tick < osKernelGetTickCount()) {
			next_tick += tick_increment;
		}
		osDelayUntil(next_tick);
	}
}

void start_CSE_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * CSE_task_info.periodicity;

	/* Make task-specific structures */
	CSE_initialize();
	CSE_t CSE;
	double *cell_voltages;
	double accumulator_current;
	double mean_cell_voltage;

	/* Wait until offset */
	next_tick += TICK2HZ * CSE_task_info.offset;
	osDelayUntil(next_tick);

	__wait_for_data(WAIT_FOR_ALL);

	for (;;) {
		/* Enter periodic behaviour */

		/* Collect cell voltages and accumulator current -- then calcualte SOC/SOH */
		if (xQueuePeek(accumulator_current_queue, &accumulator_current, 0)
				&& xQueuePeek(cell_voltages_queue, &cell_voltages, 0)) {
			/*
			 * Calculate the mean cell voltage -- if the periodicity is high enough
			 * one might calculate them all individually at some point (and/or do them interleaving)
			 */
			mean_cell_voltage = 0;
			for (int i = 0; i < 126; i++) {
				mean_cell_voltage += cell_voltages[i];
			}
			mean_cell_voltage /= 126;

			CSE_U.current = accumulator_current;
			CSE_U.y = mean_cell_voltage;

			CSE_step();

			CSE.SOC = CSE_Y.soc;
			CSE.SOH = CSE_Y.capacity / (6.6 * 3600);
			xQueueOverwrite(CSE_queue, &CSE);

		}

		/* Wait until next period */
		while (next_tick < osKernelGetTickCount()) {
			next_tick += tick_increment;
		}
		osDelayUntil(next_tick);
	}
}

void start_COOL_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * COOL_task_info.periodicity;

	/* Make task-specific structures */
	PID_t PID;
	pPID_t pPID;

	PID_initialize(&PID, // *ptr
			20,			// Reference temperature (sought)
			100 / 30, 	// Proportionate gain
			0,			// Integral gain
			0,			// Differential gain
			20,			// Lower PID output bound
			100			// Upper PID output bound
			);
	FAN_initialize(&htim1);

	double *cell_temperatures;
	double max_cell_temperature;

	/* Wait until offset */
	next_tick += TICK2HZ * COOL_task_info.offset;
	osDelayUntil(next_tick);

	__wait_for_data(WAIT_FOR_CELL_TEMPERATURES);

	for (;;) {
		/* Enter periodic behaviour */

		/* If the cell temperatures are received one may input them into the PID */
		if (xQueuePeek(cell_temperatures_queue, &cell_temperatures, 0)) {
			/* Calculate the maximum cell temperature */
			max_cell_temperature = 0;
			for (int i = 0; i < 126; i++) {
				max_cell_temperature += cell_temperatures[i];
			}
			max_cell_temperature /= 126;

			/* Progress the PID and set the fan duty cycle accordingly */
			PID_progress(&PID, max_cell_temperature);
			FAN_duty_cycle(&htim1, PID.output);

			pPID.duty_cycle = PID.output;
			xQueueOverwrite(PID_queue, &pPID);
		}

		/* Wait until next period */
		while (next_tick < osKernelGetTickCount()) {
			next_tick += tick_increment;
		}
		osDelayUntil(next_tick);
	}
}
void start_event_handler_task(void *argument) {
	__wait_for_data(WAIT_FOR_ALL);

	error_t error;

	for (;;) {
		/* Always unlatch the errors after this time has ended
		 * -- this will of course not affect the actual XXX_error_latched signal */
		set_ams_error_ext(0);
		set_imd_error_ext(0);
		/* Wait forever for an error to be raised */
		if (xQueueReceive(error_queue, &error, portMAX_DELAY)) {
			switch (error) {
			case ERROR_IMD:
				set_imd_error_ext(1);
				break;
			default:
				set_ams_error_ext(1);
				break;
			}
		}

		/* Latch the ams_error for 500 ms */
		osDelay(0.500 * TICK2HZ);
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

	__wait_for_data(WAIT_FOR_ALL);
	//initialize_IWDG(&hiwdg, IWDG_task_info.periodicity);

	for (;;) {
		/* Enter periodic behaviour */
		HAL_IWDG_Refresh(&hiwdg);

		/* Wait until next period */
		while (next_tick < osKernelGetTickCount()) {
			next_tick += tick_increment;
		}
		osDelayUntil(next_tick);
	}
}

void start_SIM_task(void *argument) {
	/* Set up task-specific timing parameters */
	uint32_t next_tick = first_tick;
	uint32_t tick_increment = TICK2HZ * SIM_task_info.periodicity;

	/* Make task-specific structures */
	SIM0_initialize();
	charger_t charger;
	uint16_t k = 0;

	uint8_t SC = 1;
	uint8_t start_drive = 0;
	uint8_t start_charge = 1;
	uint8_t start_balance = 0;

	SIM0_P.Ts = SIM_task_info.periodicity;
	SIM0_U.SC = SC;
	SIM0_U.drive = start_drive ^ start_balance;
	SIM0_U.charge = start_charge ^ start_balance;
	SIM0_U.drive_current = -5;

	/* Wait until offset */
	next_tick += TICK2HZ * SIM_task_info.offset;
	osDelayUntil(next_tick);

	for (;;) {
		/* Enter periodic behaviour */

		if (k == 10) {
			xQueueOverwrite(start_drive_queue, &start_drive);
			xQueueOverwrite(start_charge_queue, &start_charge);
			xQueueOverwrite(start_balance_queue, &start_balance);
		}
		k++;

		xQueuePeek(charger_queue, &charger, 0);
		SIM0_U.CurrentLimit = charger.charger_current_limit;
		SIM0_U.VoltageLimit = charger.charger_voltage_limit;
		SIM0_U.EnableCharge = charger.enable_charger;

		SIM0_step();

		xQueueOverwrite(accumulator_voltage_queue, &SIM0_Y.accumulator_voltage);
		xQueueOverwrite(vehicle_voltage_queue, &SIM0_Y.vehicle_voltage);
		xQueueOverwrite(accumulator_current_queue, &SIM0_Y.current);

		/* Wait until next period */
		while (next_tick < osKernelGetTickCount()) {
			next_tick += tick_increment;
		}
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

