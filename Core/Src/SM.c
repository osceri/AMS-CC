/*
 * SM.c
 *
 *  Created on: Apr 10, 2022
 *      Author: oscar
 */

#include "main.h"
#include "programme_states.h"
#include "programme_data.h"
#include "programme_queues.h"

#include "SM.h"

static GPIO_t GPIO;
static dbu_status_1_t dbu_status_1;
static ivt_msg_result_i_t ivt_msg_result_i;
static ivt_msg_result_u1_t ivt_msg_result_u1;
static ivt_msg_result_u3_t ivt_msg_result_u3;

static state_t state = STATE_NONE;
static charge_state_t charge_state = CHARGE_STATE_NONE;

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
void state_machine_step(void) {
	static __called = 0;
	if (!__called++) {
		state = STATE_ENTRY;
	}

	switch (state) {
	case STATE_ERROR:
		state = state_error_step();
		break;
	case STATE_ENTRY:
		state = state_entry_step();
		break;
	case STATE_IDLE:
		state = state_idle_step();
		break;
	case STATE_PRECHARGE_DRIVE:
		state = state_precharge_drive_step();
		break;
	case STATE_DRIVE:
		state = state_drive_step();
		break;
	case STATE_PRECHARGE_CHARGE:
		state = state_precharge_charge_step();
		break;
	case STATE_CHARGE:
		state = state_charge_step();
		break;
	}

	xQueueOverwrite(state_queue, &state);
	xQueueOverwrite(charge_state_queue, &charge_state);

}

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_error_step(void) {
	/* WAIT FOR WATCHDOG TO KILL PROGRAMME */
	__disable_irq();
	for (;;)
		;

	return STATE_ERROR;
}

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_entry_step(void) {

	/* ALL PERIPHERALS SHOULD BE INITIALIZED AT THIS POINT. */
	/* .. IT WOULD BE A GOOD IDEA TO xQueueReceive() EVERY QUEUE/DATA PORT TO MAKE SURE THAT THEY ARE RUNNING*/

	return STATE_IDLE;
}

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_idle_step(void) {
	if (xQueueReceive(dbu_status_1_queue, &dbu_status_1, 0)
			&& xQueueReceive(GPIO_queue, &GPIO, 0)) {
		if (GPIO.SC_probe && dbu_status_1.activate_ts_button) {
			return STATE_PRECHARGE_DRIVE;
		}
	}
	return STATE_IDLE;
}

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_precharge_drive_step(void) {

	return STATE_DRIVE;
}

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_drive_step(void) {
	if (xQueueReceive(GPIO_queue, &GPIO, 0)) {
		if (GPIO.SC_probe) {
			return STATE_IDLE;
		}
	}
	return STATE_DRIVE;
}

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_precharge_charge_step(void) {
	return STATE_CHARGE;
}

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_charge_step(void) {
	static __called = 0;
	if (!__called++) {
		state = STATE_ENTRY;
	}

	switch (charge_state) {
	case CHARGE_STATE_ERROR: {
		__disable_irq();
		for (;;)
			;
		charge_state = CHARGE_STATE_ERROR;
	}
		break;
	case CHARGE_STATE_ENTRY: {
		/* NOTHING, REALLY */
	}
		break;
	case CHARGE_STATE_IDLE: {

	}
		break;
	case CHARGE_STATE_CC: {

	}
		break;
	case CHARGE_STATE_CV: {

	}
		break;
	case CHARGE_STATE_CB: {

	}
		break;
	case CHARGE_STATE_NONE:
		__called = 0;
		return STATE_IDLE;
	}

	return STATE_CHARGE;
}
