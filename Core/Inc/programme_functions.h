/*
 * programme_functions.h
 *
 *  Created on: Apr 11, 2022
 *      Author: oscar
 */

#ifndef INC_PROGRAMME_FUNCTIONS_H_
#define INC_PROGRAMME_FUNCTIONS_H_


void set_AIR_plus_state_ext(uint8_t* closed);

uint8_t get_AIR_plus_state_ext(uint8_t* closed);

uint8_t take_AIR_plus_state_ext(uint8_t* closed);


void set_AIR_minus_state_ext(uint8_t* closed);

uint8_t get_AIR_minus_state_ext(uint8_t* closed);

uint8_t take_AIR_minus_state_ext(uint8_t* closed);


void set_precharge_state_ext(uint8_t* closed);

uint8_t get_precharge_state_ext(uint8_t* closed);

uint8_t take_precharge_state_ext(uint8_t* closed);


void set_ams_error_ext(uint8_t* raise_error);

uint8_t get_ams_error_latched_ext(uint8_t* raise_error);

uint8_t take_ams_error_latched_ext(uint8_t* raise_error);


void set_imd_error_ext(uint8_t* raise_error);

uint8_t get_imd_error_latched_ext(uint8_t* raise_error);

uint8_t take_imd_error_latched_ext(uint8_t* raise_error);


uint8_t get_SC_probe_ext(uint8_t* is_high);

uint8_t take_SC_probe_ext(uint8_t* is_high);


uint8_t get_imd_ok_ext(uint8_t* is_ok);

uint8_t take_imd_ok_ext(uint8_t* is_ok);


void set_charger_voltage_ext(float level);

void set_charger_current_ext(float level);



void set_voltages_int(float* voltages[]);

uint8_t get_voltages_int(float* voltages[]);

uint8_t take_voltages_int(float* voltages[]);

void set_voltages_d_int(double* voltages[]);

uint8_t get_voltages_d_int(double* voltages[]);

uint8_t take_voltages_d_int(float* voltages[]);


void set_temperatures_int(float* temperatures[]);

uint8_t get_temperatures_int(float* temperatures[]);

uint8_t take_temperatures_int(float* temperatures[]);

void set_temperatures_d_int(double* temperatures[]);

uint8_t get_temperatures_d_int(double* temperatures[]);

uint8_t take_temperatures_d_int(double* temperatures[]);


void set_accumulator_voltage_int(float* accumulator_voltage);

uint8_t get_accumulator_voltage_int(float* accumulator_voltage);

uint8_t take_accumulator_voltage_int(float* accumulator_voltage);

void set_accumulator_voltage_d_int(double* accumulator_voltage);

uint8_t get_accumulator_voltage_d_int(double* accumulator_voltage);

uint8_t take_accumulator_voltage_d_int(double* accumulator_voltage);


void set_vehicle_voltage_int(float* vehicle_voltage);

uint8_t get_vehicle_voltage_int(float* vehicle_voltage);

uint8_t take_vehicle_voltage_int(float* vehicle_voltage);

void set_vehicle_voltage_d_int(double* vehicle_voltage);

uint8_t get_vehicle_voltage_d_int(double* vehicle_voltage);

uint8_t take_vehicle_voltage_d_int(double* vehicle_voltage);


void set_accumulator_current_int(float* current);

uint8_t get_accumulator_current_int(float* current);

uint8_t take_accumulator_current_int(float* current);

void set_accumulator_current_d_int(double* current);

uint8_t get_accumulator_current_d_int(double* current);

uint8_t take_accumulator_current_d_int(double* current);





#endif /* INC_PROGRAMME_FUNCTIONS_H_ */
