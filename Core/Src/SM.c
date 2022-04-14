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
#include "programme_functions.h"
#include "SIM0.h"

#include "SM.h"

static state_t state = STATE_NONE;

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
void state_machine_step(void) {
	switch (state) {
	case STATE_NONE:
		state = STATE_ENTRY;
		break;
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
	case STATE_BALANCE:
		state = state_balance_step();
		break;
	}

	xQueueOverwrite(state_queue, &state);

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
	set_precharge_ext(0);
	set_air_plus_ext(0);
	set_air_minus_ext(0);
	set_enable_charge_ext(0);

	for (int i = 0; i < 126; i++) {
		set_balance_ext(0, i, NULL);
	}

	uint8_t SC;
	uint8_t start_drive;
	uint8_t start_charge;
	uint8_t start_balance;

	SC = get_sc_probe_ext();

	if (xQueueReceive(start_drive_queue, &start_drive, 0)) {
		if (SC && start_drive) {
			return STATE_PRECHARGE_DRIVE;
		}
	}
	if (xQueueReceive(start_charge_queue, &start_charge, 0)) {
		if (SC && start_charge) {
			return STATE_PRECHARGE_CHARGE;
		}
	}
	if (xQueueReceive(start_balance_queue, &start_balance, 0)) {
		if (!SC && start_balance) {
			return STATE_BALANCE;
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
	float DELAY = 4.0;
	double accumulator_voltage;
	double vehicle_voltage;

	uint16_t precharge_timed_out = 0;

	/* First we close the negative AIR */
	set_air_minus_ext(1);
	osDelay(TICK2HZ * DELAY);
	if (get_air_minus_ext() == 0) {

		return STATE_IDLE;
	}

	/* Then we close the precharge resistor relay */
	set_precharge_ext(1);
	osDelay(TICK2HZ * DELAY);
	if (get_precharge_ext() == 0) {

		return STATE_IDLE;
	}

	/*
	 * Now we wait a minimum of 0.5s and a maximum of 15s for the inverter (vehicle) voltage to
	 * reach 95% of the accumulator (cell) voltage.
	 */
	uint32_t initial_tick = osKernelGetTickCount();
	uint32_t current_tick = osKernelGetTickCount();
	uint32_t min_tick = TICK2HZ * 0.5; // Wait at least 0.5 seconds
	uint32_t max_tick = TICK2HZ * 25; // Wait at most 15 seconds

	/* We want the data to be up-to-date. Clear whatever is currently gathered */
	xQueueReceive(accumulator_voltage_queue, &accumulator_voltage, 0);
	xQueueReceive(vehicle_voltage_queue, &vehicle_voltage, 0);

	for (;; current_tick = osKernelGetTickCount()) {
		osDelay(TICK2HZ * 0.100);

		/* Skip the first few loops */
		if (min_tick > (current_tick - initial_tick)) {
			continue;
		}

		/* If 15s have gone by and we haven't left this loop, then we should exit */
		if ((current_tick - initial_tick) > max_tick) {
			precharge_timed_out = 1;
			break;
		}

		/* If no new voltage data is retrieved we shouldn't go on */
		if (!(xQueueReceive(accumulator_voltage_queue, &accumulator_voltage, 0)
				&& xQueueReceive(vehicle_voltage_queue, &vehicle_voltage, 0))) {
			continue;
		}

		/* If we are within time constraints and the data is up-to-date and satisfactory, then go on! */
		if (0.95 * accumulator_voltage < vehicle_voltage) {
			precharge_timed_out = 0;
			break;
		}

	}
	if (precharge_timed_out == 1) {

		return STATE_IDLE;
	}

	/* Now we close the positive AIR */
	set_air_plus_ext(1);
	osDelay(TICK2HZ * DELAY);
	if (get_air_plus_ext() == 0) {

		return STATE_IDLE;
	}

	/* At last we open the precharge resistor relay, so we don't burn it if the positive AIR opens unexpectedly */
	set_precharge_ext(0);
	osDelay(TICK2HZ * DELAY);
	if (get_precharge_ext() == 1) {

		return STATE_IDLE;
	}

	return STATE_DRIVE;
}

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_drive_step(void) {
	if (!get_sc_probe_ext()) {
		return STATE_IDLE;
	}
	return STATE_DRIVE;
}

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_precharge_charge_step(void) {
	float DELAY = 4.0;
	double accumulator_voltage;
	double vehicle_voltage;

	set_charger_voltage_limit_ext(4.15 * 126);
	set_charger_current_limit_ext(2 * 6.6);

	uint16_t precharge_timed_out = 0;

	/* First we close the negative AIR */
	set_air_minus_ext(1);
	osDelay(TICK2HZ * DELAY);
	if (get_air_minus_ext() == 0) {

		return STATE_IDLE;
	}

	/* Then we close the precharge resistor relay */
	set_precharge_ext(1);
	osDelay(TICK2HZ * DELAY);
	if (get_precharge_ext() == 0) {

		return STATE_IDLE;
	}

	/*
	 * Now we wait a minimum of 0.5s and a maximum of 15s for the inverter (vehicle) voltage to
	 * reach 95% of the accumulator (cell) voltage.
	 */
	uint32_t initial_tick = osKernelGetTickCount();
	uint32_t current_tick = osKernelGetTickCount();
	uint32_t min_tick = TICK2HZ * 0.5; // Wait at least 0.5 seconds
	uint32_t max_tick = TICK2HZ * 25; // Wait at most 15 seconds

	/* We want the data to be up-to-date. Clear whatever is currently gathered */
	xQueueReceive(accumulator_voltage_queue, &accumulator_voltage, 0);
	xQueueReceive(vehicle_voltage_queue, &vehicle_voltage, 0);

	for (;; current_tick = osKernelGetTickCount()) {
		osDelay(TICK2HZ * 0.100);

		/* Skip the first few loops */
		if (min_tick > (current_tick - initial_tick)) {
			continue;
		}

		/* If 15s have gone by and we haven't left this loop, then we should exit */
		if ((current_tick - initial_tick) > max_tick) {
			precharge_timed_out = 1;
			break;
		}

		/* If no new voltage data is retrieved we shouldn't go on */
		if (!(xQueueReceive(accumulator_voltage_queue, &accumulator_voltage, 0)
				&& xQueueReceive(vehicle_voltage_queue, &vehicle_voltage, 0))) {
			continue;
		}

		/* If we are within time constraints and the data is up-to-date and satisfactory, then go on! */
		if (0.95 * accumulator_voltage < vehicle_voltage) {
			precharge_timed_out = 0;
			break;
		}

	}
	if (precharge_timed_out == 1) {

		return STATE_IDLE;
	}

	/* Now we close the positive AIR */
	set_air_plus_ext(1);
	osDelay(TICK2HZ * DELAY);
	if (get_air_plus_ext() == 0) {

		return STATE_IDLE;
	}

	/* At last we open the precharge resistor relay, so we don't burn it if the positive AIR opens unexpectedly */
	set_precharge_ext(0);
	osDelay(TICK2HZ * DELAY);
	if (get_precharge_ext() == 1) {

		return STATE_IDLE;
	}

	return STATE_CHARGE;
}

static uint8_t I[126] = {
#define gWx1(x, y, z) (x + 7*y + 7*3*z)
#define gWx2(y, z) gWx1(0, y, z),gWx1(1, y, z),gWx1(2, y, z),gWx1(3, y, z),gWx1(4, y, z),gWx1(5, y, z),gWx1(6, y, z),
#define gWx3(z) gWx2(0, z)gWx2(1, z)gWx2(2, z)
		gWx3(0)gWx3(1)gWx3(2)gWx3(3)gWx3(4)gWx3(5) };

static double *cell_voltages;
static double mean;
static double var;

void gnome_sort() {
	uint8_t pos = 0;
	uint8_t tmp;
	while (pos < 126) {
		if (pos == 0) {
			pos++;
		} else if (cell_voltages[I[pos]] <= cell_voltages[I[pos - 1]]) {
			pos++;
		} else {
			tmp = I[pos];
			I[pos] = I[pos - 1];
			I[pos - 1] = tmp;
			pos--;
		}

	}
}

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_balance_step(void) {
	static int k = 0;

	if (xQueuePeek(cell_voltages_queue, &cell_voltages, 0)) {
		mean = 0;
		var = 0;
		double svar = 0;

		for (int i = 0; i < 126; i++) {
			mean += cell_voltages[i];
		}
		mean /= 126;
		for (int i = 0; i < 126; i++) {
			svar = cell_voltages[i] - mean;
			var = svar * svar;
		}

		if (var < 0.000001) {
			return STATE_ENTRY;
		}

		gnome_sort();

		if (k == 0) {
			for (int i = 0; i < 126; i++) {
				set_balance_ext(0, i, NULL);
			}
			for (int i = 0; i < 42; i++) {
				set_balance_ext(1, I[i], NULL);
			}

		}

		k = (k + 1) % 300;

	}

	return STATE_BALANCE;
}

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_charge_step(void) {
	float *cell_voltages;

	set_enable_charge_ext(1);
	set_charger_voltage_limit_ext(4.15 * 126);
	set_charger_current_limit_ext(2 * 6.6);

	if (xQueuePeek(cell_voltages_queue, &cell_voltages, 0)) {
		float max = 0;
		for (int i = 0; i < 126; i++) {
			if (cell_voltages[i] > max) {
				max = cell_voltages[i];
			}
		}
		if (max > 4.15) {
			return STATE_ENTRY;
		}
	}

	return STATE_CHARGE;
}
