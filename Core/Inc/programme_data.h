/*
 * programme_data.h
 *
 *  Created on: Apr 10, 2022
 *      Author: oscar
 */

#ifndef INC_PROGRAMME_DATA_H_
#define INC_PROGRAMME_DATA_H_

#include "canlib_data.h"

typedef struct {
	CAN_TxHeaderTypeDef tx_header;
	CAN_RxHeaderTypeDef rx_header;
	uint8_t data[8];
} can_queue_element_t;

typedef struct {
	uint8_t AMS_error_latched;
	uint8_t IMD_error_latched;
	uint8_t SC_probe;
	uint8_t IMD_ok;
	uint8_t AIR_plus_closed;
	uint8_t AIR_minus_closed;
	uint8_t precharge_closed;
} GPIO_t;

typedef struct {
	float duty_cycle;
	float frequency;
} IMD_t;

typedef struct {
   float precharge_resistor;
   float fuse;
   float aux_1;
   float aux_2;
} temperatures_t;





#endif /* INC_PROGRAMME_DATA_H_ */
