/*
 * programme_functions.c
 *
 *  Created on: Apr 11, 2022
 *      Author: oscar
 */

#include "main.h"

#include "canlib_callbacks.h"
#include "SIM0.h"

void set_air_plus_ext(uint8_t closed){
	SIM0_U.EnableAirPlus = closed;
}

uint8_t get_air_plus_ext(){
	return SIM0_Y.AIR_plus_closed;
}

void set_air_minus_ext(uint8_t closed){
	SIM0_U.EnableAirMinus = closed;
}

uint8_t get_air_minus_ext(){
	return SIM0_Y.AIR_minus_closed;
}

void set_precharge_ext(uint8_t closed){
	SIM0_U.EnablePrecharge = closed;
}

uint8_t get_precharge_ext(){
	return SIM0_Y.precharge_closed;

}

void set_ams_error_ext(uint8_t raise_error){
	SIM0_U.AMS_error = raise_error;
}

uint8_t get_ams_error_latched_ext(){
	return SIM0_Y.AMS_error_latched;
}

void set_imd_error_ext(uint8_t raise_error){
	SIM0_U.IMD_error = raise_error;
}

uint8_t get_imd_error_latched_ext(){
	return SIM0_Y.IMD_error_latched;
}

uint8_t get_sc_probe_ext(){
	return SIM0_Y.SC_probe;
}

uint8_t get_imd_ok_ext(){
	return SIM0_Y.IMD_ok;
}

void set_charger_voltage_limit_ext(float level){
	SIM0_U.VoltageLimit = level;
}

void set_charger_current_limit_ext(float level){
	SIM0_U.CurrentLimit = level;
}

void set_enable_charge_ext(uint8_t enable){
	SIM0_U.EnableCharge = enable;
}

void set_balance_ext(uint8_t closed, uint8_t index, void* settings){
	SIM0_U.BalanceOut[index] = closed;

}


