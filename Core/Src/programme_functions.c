/*
 * programme_functions.c
 *
 *  Created on: Apr 11, 2022
 *      Author: oscar
 */

#include "main.h"

#include "canlib_callbacks.h"
#include "SIM0.h"

void set_air_plus_ext(uint8_t closed) {
#ifdef SIMULATION
	SIM0_U.EnableAirPlus = closed;
#elif
	HAL_GPIO_WritePin(enable_AIR_plus_Port, enable_AIR_plus_GPIO_Pin, closed);
#endif
}

uint8_t get_air_plus_ext() {
#ifdef SIMULATION
	return SIM0_Y.AIR_plus_closed;
#elif
	return HAL_GPIO_ReadPin(AIR_plus_closed_Port, AIR_plus_closed_GPIO_Pin);
#endif
}

void set_air_minus_ext(uint8_t closed) {
#ifdef SIMULATION
	SIM0_U.EnableAirMinus = closed;
#elif
	HAL_GPIO_WritePin(enable_AIR_minus_Port, enable_AIR_minus_GPIO_Pin, closed);
#endif
}

uint8_t get_air_minus_ext() {
#ifdef SIMULATION
	return SIM0_Y.AIR_minus_closed;
#elif
	return HAL_GPIO_ReadPin(AIR_minus_closed_Port, AIR_minus_closed_GPIO_Pin);
#endif
}

void set_precharge_ext(uint8_t closed) {
#ifdef SIMULATION
	SIM0_U.EnablePrecharge = closed;
#elif
	HAL_GPIO_WritePin(enable_precharge_Port, enable_precharge_GPIO_Pin, closed);
#endif
}

uint8_t get_precharge_ext() {
#ifdef SIMULATION
	return SIM0_Y.precharge_closed;
#elif
	return HAL_GPIO_ReadPin(precharge_closed_Port, precharge_closed_GPIO_Pin);
#endif

}

void set_ams_error_ext(uint8_t raise_error) {
#ifdef SIMULATION
	SIM0_U.AMS_error = raise_error;
#elif
	HAL_GPIO_WritePin(AMS_error_Port, AMS_error_GPIO_Pin, closed);
#endif
}

uint8_t get_ams_error_latched_ext() {
#ifdef SIMULATION
	return SIM0_Y.AMS_error_latched;
#elif
	return HAL_GPIO_ReadPin(AMS_error_latched_Port, AMS_error_latched_GPIO_Pin);
#endif
}

void set_imd_error_ext(uint8_t raise_error) {
#ifdef SIMULATION
	SIM0_U.IMD_error = raise_error;
#elif
	return HAL_GPIO_ReadPin(IMD_error_Port, IMD_error_GPIO_Pin);
#endif
}

uint8_t get_imd_error_latched_ext() {
#ifdef SIMULATION
	return SIM0_Y.IMD_error_latched;
#elif
	return HAL_GPIO_ReadPin(IMD_error_latched_Port, IMD_error_latched_GPIO_Pin);
#endif
}

uint8_t get_sc_probe_ext() {
#ifdef SIMULATION
	return SIM0_Y.SC_probe;
#elif
	return HAL_GPIO_ReadPin(SC_probe_latched_Port, SC_probe_GPIO_Pin);
#endif
}

uint8_t get_imd_ok_ext() {
#ifdef SIMULATION
	return 1;
#elif
	return HAL_GPIO_ReadPin(IMD_ok_Port, IMD_ok_GPIO_Pin);
#endif
}


void set_balance_ext(uint8_t closed, uint8_t index, void *settings) {
	SIM0_U.BalanceOut[index] = closed;

}

