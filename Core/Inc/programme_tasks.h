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

void initialize_can(CAN_HandleTypeDef *__hcan1, CAN_HandleTypeDef *__hcan2);

void initialize_LTC(SPI_HandleTypeDef *hspi);

/* TASK ENTRY POINT DECLARATIONS */
void start_SM_task(void *argument);
void start_IWDG_task(void *argument);
void start_event_handler_task(void *argument);
void start_IMD_task(void *argument);
void start_GPIO_task(void *argument);
void start_ADC_task(void *argument);
void start_COM_task(void *argument);
void start_CAN_task(void *argument);
void start_CSE_task(void *argument);
void start_COOL_task(void *argument);
void start_SIM_task(void *argument);

/* TASK DEFINITIONS */

uint32_t first_tick;

typedef struct {
	osThreadAttr_t attributes;
	float periodicity;	// [s] ((has to be a multiple of the systems lowest))
	float offset;			// [s] ((account for every preceeding task))
	float execution_time;	// [s] ((worst possible))
} task_info;

/* Main programme task */
osThreadId_t SM_task_handle;

/* Programme safety tasks */
osThreadId_t IWDG_task_handle;

osThreadId_t event_handler_task_handle;

/* Data generating tasks */
osThreadId_t IMD_task_handle;

osThreadId_t GPIO_task_handle;

osThreadId_t ADC_task_handle;

osThreadId_t COM_task_handle;

osThreadId_t CAN_task_handle;

/* Data consuming tasks */
osThreadId_t CSE_task_handle;

osThreadId_t COOL_task_handle;

/* Data simulation tasks */
osThreadId_t SIM_task_handle;



#endif /* INC_PROGRAMME_STRUCTURE_H_ */
