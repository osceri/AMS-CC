/*
 * SIM.h
 *
 *  Created on: Apr 11, 2022
 *      Author: oscar
 */

#ifndef INC_SIM_H_
#define INC_SIM_H_

struct _SIM_state {
	uint8_t dbu;
	uint8_t cu;
	uint8_t msg_timer;

	uint8_t SC_probe ;
	uint8_t SC_probe_timer;

	uint8_t set_air_plus_state ;
	uint8_t air_plus_state;
	uint8_t air_plus_state_timer;

	uint8_t set_air_minus_state;
	uint8_t air_minus_state;
	uint8_t air_minus_state_timer;

	uint8_t set_precharge_state;
	uint8_t precharge_state;
	uint8_t precharge_state_timer;

	uint8_t imd_error_latched;
	uint8_t ams_error_latched;

	float U_accumulator;
	float U_vehicle;
	float I_ts;

	float cc_level;
	float cv_level;
} SIM_state;

void initialize_SIM();

void SIM_step();

#endif /* INC_SIM_H_ */
