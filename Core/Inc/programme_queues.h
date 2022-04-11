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

QueueHandle_t state_queue;

QueueHandle_t charge_state_queue;

QueueHandle_t GPIO_queue;

QueueHandle_t IMD_queue;

QueueHandle_t can_rx_queue;

QueueHandle_t can1_tx_queue;

QueueHandle_t can2_tx_queue;

QueueHandle_t ams_temperatures_queue;

QueueHandle_t ivt_msg_result_u3_queue;

QueueHandle_t ivt_msg_result_i_queue;

QueueHandle_t ivt_msg_result_u1_queue;

QueueHandle_t dbu_status_1_queue;

QueueHandle_t ivt_msg_result_u2_queue;

QueueHandle_t ams_status_1_queue;

QueueHandle_t ams_cell_voltages_queue;

QueueHandle_t ams_cell_temperatures_queue;


#endif /* INC_PROGRAMME_QUEUES_H_ */
