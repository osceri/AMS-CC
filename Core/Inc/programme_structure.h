/*
 * task_roster.h
 *
 *  Created on: Apr 10, 2022
 *      Author: oscar
 */

#ifndef INC_PROGRAMME_STRUCTURE_H_
#define INC_PROGRAMME_STRUCTURE_H_

#include "main.h"
#include "cmsis_os.h"

/* INITIALIZATION FUNCTION DECLARATIONS */

void initialize_can(CAN_HandleTypeDef* __hcan1, CAN_HandleTypeDef* __hcan2);

void initialize_LTC(SPI_HandleTypeDef *hspi);

/* TASK ENTRY POINT DECLARATIONS */
void start_SM_task(void *argument);
void start_IWDG_task(void *argument);
void start_event_handler_task(void *argument);
void start_IMD_task(void *argument);
void start_GPIO_task(void *argument);
void start_ADC_task(void *argument);
void start_COM_task(void *argument);
void start_CAN_rx_task(void *argument);
void start_CAN_tx_task(void *argument);
void start_CSE_task(void *argument);
void start_COOL_task(void *argument);
void start_SIM_task(void *argument);

/* TASK DEFINITIONS */

uint32_t first_tick;

typedef struct {
	float periodicity;		// [s] ((has to be a multiple of the systems lowest))
	float offset;			// [s] ((account for every preceeding task))
	float execution_time;	// [s] ((worst possible))
} task_info;

/* Main programme task */
osThreadId_t SM_task_handle;
const osThreadAttr_t SM_task_attributes = { .name = "SM_task", .stack_size = 1024
		* 4, .priority = (osPriority_t) osPriorityNormal, };
const task_info SM_task_info = { .periodicity = 0.4, .offset = 0.000,
		.execution_time = 0.010, };

/* Programme safety tasks */
osThreadId_t IWDG_task_handle;
const osThreadAttr_t IWDG_task_attributes = { .name = "IWDG_task", .stack_size =
		128 * 4, .priority = (osPriority_t) osPriorityLow, };
const task_info IWDG_task_info = { .periodicity = 0.4, .offset = 0.010,
		.execution_time = 0.001, };

osThreadId_t event_handler_task_handle;
const osThreadAttr_t event_handler_task_attributes = { .name = "event_handler_task",
		.stack_size = 128 * 4, .priority = (osPriority_t) osPriorityHigh, };
const task_info event_handler_task_info = { .periodicity = 0.4, .offset =
		0.011, .execution_time = 0.001, };

/* Data generating tasks */
osThreadId_t IMD_task_handle;
const osThreadAttr_t IMD_task_attributes = { .name = "IMD_task", .stack_size =
		128 * 4, .priority = (osPriority_t) osPriorityNormal1, };
const task_info IMD_task_info = { .periodicity = 0.4, .offset = 0.012,
		.execution_time = 0.001, };

osThreadId_t GPIO_task_handle;
const osThreadAttr_t GPIO_task_attributes = { .name = "GPIO_task", .stack_size =
		128 * 4, .priority = (osPriority_t) osPriorityNormal2, };
const task_info GPIO_task_info = { .periodicity = 0.4, .offset = 0.013,
		.execution_time = 0.001, };

osThreadId_t ADC_task_handle;
const osThreadAttr_t ADC_task_attributes = { .name = "ADC_task", .stack_size =
		128 * 4, .priority = (osPriority_t) osPriorityNormal3, };
const task_info ADC_task_info = { .periodicity = 0.4, .offset = 0.014,
		.execution_time = 0.001, };

osThreadId_t COM_task_handle;
const osThreadAttr_t COM_task_attributes = { .name = "COM_task", .stack_size =
		2048 * 4, .priority = (osPriority_t) osPriorityLow4, };
const task_info COM_task_info = { .periodicity = 0.4, .offset = 0.015,
		.execution_time = 0.040, };

osThreadId_t CAN_rx_task_handle;
const osThreadAttr_t CAN_rx_task_attributes =
		{ .name = "CAN_rx_task", .stack_size = 512 * 4, .priority =
				(osPriority_t) osPriorityBelowNormal1, };
const task_info CAN_rx_task_info = { .periodicity = 0.4, .offset = 0.055,
		.execution_time = 0.020, };

osThreadId_t CAN_tx_task_handle;
const osThreadAttr_t CAN_tx_task_attributes =
		{ .name = "CAN_tx_task", .stack_size = 512 * 4, .priority =
				(osPriority_t) osPriorityBelowNormal2, };
const task_info CAN_tx_task_info = { .periodicity = 0.4, .offset = 0.075,
		.execution_time = 0.020, };

/* Data consuming tasks */
osThreadId_t CSE_task_handle;
const osThreadAttr_t CSE_task_attributes = { .name = "CSE_task", .stack_size =
		1024 * 4, .priority = (osPriority_t) osPriorityNormal5, };
const task_info CSE_task_info = { .periodicity = 0.4, .offset = 0.095,
		.execution_time = 0.035, };

osThreadId_t COOL_task_handle;
const osThreadAttr_t COOL_task_attributes = { .name = "COOL_task", .stack_size =
		128 * 4, .priority = (osPriority_t) osPriorityNormal6, };
const task_info COOL_task_info = { .periodicity = 0.4, .offset = 0.130,
		.execution_time = 0.001, };

/* Data simulation tasks */
osThreadId_t SIM_task_handle;
const osThreadAttr_t SIM_task_attributes = { .name = "SIM_task", .stack_size =
		2048 * 4, .priority = (osPriority_t) osPriorityBelowNormal, };
const task_info SIM_task_info = { .periodicity = 0.4, .offset = 0.131,
		.execution_time = 0.1, };

/* QUEUE DEFINITIONS */



#endif /* INC_PROGRAMME_STRUCTURE_H_ */
