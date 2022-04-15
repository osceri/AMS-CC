/*
 * programme_queues.h
 *
 *  Created on: Apr 10, 2022
 *      Author: oscar
 */

#ifndef INC_PROGRAMME_QUEUES_H_
#define INC_PROGRAMME_QUEUES_H_

#include "main.h"
#include "cmsis_os.h"

#include "queue.h"

/* QUEUE DEFINITIONS */

typedef struct {
	uint8_t element_count;		// The amount of elements in the queue
	uint16_t element_size;		// The size of every individual element
} queue_info;

QueueHandle_t GPIO_queue;

QueueHandle_t IMD_queue;

QueueHandle_t cell_voltages_queue;

QueueHandle_t cell_temperatures_queue;

QueueHandle_t accumulator_current_queue;

QueueHandle_t accumulator_voltage_queue;

QueueHandle_t vehicle_voltage_queue;

QueueHandle_t temperatures_queue;

QueueHandle_t start_drive_queue;

QueueHandle_t start_charge_queue;

QueueHandle_t start_balance_queue;


#endif /* INC_PROGRAMME_QUEUES_H_ */
