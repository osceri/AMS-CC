/*
 * SIM.c
 *
 *  Created on: Apr 11, 2022
 *      Author: oscar
 */
#include "main.h"
#include "programme_tasks.h"
#include "programme_queues.h"
#include "programme_data.h"

#include "Accumulator.h"
#include "Charger.h"
#include "SIM.h"

/* IMPLEMENT PROGRAMME FUNCTIONS IN THE SIMULATED ENVIRONMENT */
void set_AIR_plus_state(uint8_t closed) {
	SIM_state.set_air_plus_state = 0;
	SIM_state.air_plus_state_timer = 1;
}

void set_AIR_minus_state(uint8_t closed) {
	SIM_state.set_air_minus_state = 0;
	SIM_state.air_minus_state_timer = 1;
}

void set_precharge_state(uint8_t closed) {
	SIM_state.set_precharge_state = 0;
	SIM_state.precharge_state_timer = 1;
}

uint8_t get_AIR_plus_state() {
	return SIM_state.air_plus_state;
}

uint8_t get_AIR_minus_state() {
	return SIM_state.air_minus_state;
}

uint8_t get_precharge_state() {
	return SIM_state.precharge_state;
}

void set_charger_voltage(float level) {
	SIM_state.cc_level = level;
}

void set_charger_current(float level) {
	SIM_state.cv_level = level;
}

void set_ams_error(uint8_t raise_error) {
	SIM_state.ams_error_latched = raise_error;
}

uint8_t get_ams_error_latched() {
	return SIM_state.ams_error_latched;
}

void set_imd_error(uint8_t raise_error) {
	SIM_state.imd_error_latched = raise_error;
}

uint8_t get_imd_error_latched() {
	return SIM_state.imd_error_latched;
}

/* OK, COOL */

void toggle_SC() {
	SIM_state.SC_probe_timer = 1;
}

int dec_tim(uint8_t *ptr) {
	if (ptr[0] > 0) {
		ptr--;
		return !ptr[0];
	}
	return 0;
}

void initialize_SIM() {
	Accumulator_initialize();
	Charger_initialize();
}

void SIM_step() {
	/* INJECT ERROR */
	if (dec_tim(&SIM_state.SC_probe_timer)) {
		SIM_state.SC_probe ^= 1;
	}

	/* INJECT ERROR */
	if (SIM_state.ams_error_latched || SIM_state.imd_error_latched) {
		SIM_state.SC_probe = 0;
	}

	if (SIM_state.SC_probe == 0) {
		SIM_state.air_minus_state = 0;
		SIM_state.air_plus_state = 0;
		SIM_state.precharge_state = 0;

	} else {
		/* INJECT ERROR */
		if (dec_tim(&SIM_state.air_minus_state_timer)) {
			SIM_state.air_minus_state = SIM_state.set_air_minus_state;
		}

		/* INJECT ERROR */
		if (dec_tim(&SIM_state.air_plus_state_timer)) {
			SIM_state.air_plus_state = SIM_state.set_air_plus_state;
		}

		/* INJECT ERROR */
		if (dec_tim(&SIM_state.precharge_state_timer)) {
			SIM_state.precharge_state = SIM_state.set_precharge_state;
		}
	}

	SIM_state.U_accumulator = Accumulator_Y.VoltageSum;

	uint8_t connected = 0;
	uint8_t pconnected = 0;
	if (SIM_state.air_plus_state && SIM_state.air_minus_state) {
		connected = 1;
	}
	if (SIM_state.precharge_state && SIM_state.air_minus_state) {
		pconnected = 1;
	}

	uint8_t is_dbu = 1;
	uint8_t is_cu = 0;
	if (is_dbu) {
		if (dec_tim(&SIM_state.msg_timer)) {
			SIM_state.dbu = 1;
			dbu_status_1_t dbu_status_1;
			dbu_status_1.activate_ts_button = 1;
			xQueueOverwrite(dbu_status_1_queue, &dbu_status_1);
		}

		/* INJECT ERROR */
		if (pconnected) {
			SIM_state.U_vehicle += (SIM_state.U_accumulator
					- SIM_state.U_vehicle) * 0.6;
		} else {
			if (connected) {
				SIM_state.U_vehicle += (SIM_state.U_accumulator
						- SIM_state.U_vehicle) * 0.6;
			} else {
				SIM_state.U_vehicle *= 0.4;
			}
		}
	}
	if (is_cu) {
		if (dec_tim(&SIM_state.msg_timer)) {
			SIM_state.cu = 1;
			dbu_status_1_t dbu_status_1;
			dbu_status_1.ready_to_drive_button = 1;
			xQueueOverwrite(dbu_status_1_queue, &dbu_status_1);
		}

		/* INJECT ERROR */
		SIM_state.U_vehicle += (SIM_state.cv_level - SIM_state.U_vehicle) * 0.9;

		if (connected || pconnected) {
			Accumulator_U.Current = Charger_Y.Current;
		}
	}

	GPIO_t GPIO;
	GPIO.AIR_minus_closed = SIM_state.air_minus_state;
	GPIO.AIR_plus_closed = SIM_state.air_plus_state;
	GPIO.precharge_closed = SIM_state.precharge_state;
	GPIO.AMS_error_latched = SIM_state.ams_error_latched;
	GPIO.IMD_error_latched = SIM_state.imd_error_latched;
	GPIO.SC_probe = SIM_state.SC_probe;

	/* INJECT ERROR */
	GPIO.IMD_ok = 1;

	xQueueOverwrite(GPIO_queue, &GPIO);

	SIM_state.I_ts = Accumulator_Y.DisplayCurrent;


	ivt_msg_result_i_t I;
	ivt_msg_result_u1_t U1;
	ivt_msg_result_u3_t U3;

	I.i_ts = Accumulator_Y.DisplayCurrent;
	U1.u_cells = SIM_state.U_accumulator;
	U3.u_vehicle = SIM_state.U_vehicle;

	xQueueOverwrite(ivt_msg_result_i_queue, &I);
	xQueueOverwrite(ivt_msg_result_u1_queue, &U1);
	xQueueOverwrite(ivt_msg_result_u3_queue, &U3);

	Charger_U.CV_true = 1;
	Charger_U.CC_true = 1;
	Charger_U.CV_level = SIM_state.cv_level;
	Charger_U.CC_level = SIM_state.cc_level;
	Charger_U.y = Accumulator_Y.VoltageSum;


	Accumulator_step();
	Charger_step();

}
