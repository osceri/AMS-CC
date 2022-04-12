/*
 * programme_states.h
 *
 *  Created on: Apr 10, 2022
 *      Author: oscar
 */

#ifndef INC_PROGRAMME_STATES_H_
#define INC_PROGRAMME_STATES_H_

typedef enum {
	STATE_ERROR = -1,
	STATE_NONE = 0,
	STATE_ENTRY = 1,
	STATE_IDLE = 2,
	STATE_PRECHARGE_DRIVE = 3,
	STATE_DRIVE = 4,
	STATE_PRECHARGE_CHARGE = 5,
	STATE_CHARGE = 6,
	STATE_BALANCE = 7,
} state_t;

typedef enum {
	CHARGE_STATE_ERROR = -1,
	CHARGE_STATE_NONE = 0,
	CHARGE_STATE_ENTRY = 1,
	CHARGE_STATE_IDLE = 2,
	CHARGE_STATE_CC = 3,
	CHARGE_STATE_CV = 4,
	CHARGE_STATE_CB = 5,
} charge_state_t;

#endif /* INC_PROGRAMME_STATES_H_ */
