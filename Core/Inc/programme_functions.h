/*
 * programme_functions.h
 *
 *  Created on: Apr 11, 2022
 *      Author: oscar
 */

#ifndef INC_PROGRAMME_FUNCTIONS_H_
#define INC_PROGRAMME_FUNCTIONS_H_

void set_air_plus_ext(uint8_t closed);

uint8_t get_air_plus_ext();

void set_air_minus_ext(uint8_t closed);

uint8_t get_air_minus_ext();

void set_precharge_ext(uint8_t closed);

uint8_t get_precharge_ext();

void set_ams_error_ext(uint8_t raise_error);

uint8_t get_ams_error_latched_ext();

void set_imd_error_ext(uint8_t raise_error);

uint8_t get_imd_error_latched_ext();

uint8_t get_sc_probe_ext();

uint8_t get_imd_ok_ext();

void set_charger_voltage_limit_ext(float level);

void set_charger_current_limit_ext(float level);

void set_enable_charge_ext(uint8_t enable);

void set_balance_ext(uint8_t closed, uint8_t index, void* settings);


#endif /* INC_PROGRAMME_FUNCTIONS_H_ */
