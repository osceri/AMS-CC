/*
 * canlib_callbacks.c
 *
 *  Created on: Apr 17, 2022
 *      Author: oscar
 */

#include "main.h"

#include "programme_data.h"
#include "programme_functions.h"
#include "programme_queues.h"
#include "canlib_data.h"
#include "canlib_callbacks.h"
#include "SIM0.h"


uint8_t can2_ivt_msg_result_i_tx_callback(ivt_msg_result_i_t* ivt_msg_result_i){
	ivt_msg_result_i->ivt_result_i = SIM0_Y.current;
	return 1;
}

uint8_t can2_ivt_msg_result_u1_tx_callback(ivt_msg_result_u1_t* ivt_msg_result_u1) {
	ivt_msg_result_u1->ivt_result_u1 = SIM0_Y.accumulator_voltage;
	return 1;
}

uint8_t can2_ivt_msg_result_u3_tx_callback(ivt_msg_result_u3_t* ivt_msg_result_u3) {
	ivt_msg_result_u3->ivt_result_u3 = SIM0_Y.vehicle_voltage;
	return 1;
}

uint8_t can2_cc_status_tx_callback(cc_status_t* cc_status) {
	cc_status->enable_ams_charging = SIM0_U.charge && !SIM0_U.drive;
	return 1;
}

uint8_t can1_dbu_status_1_tx_callback(dbu_status_1_t* dbu_status_1) {
	dbu_status_1->activate_ts_button = SIM0_U.drive && !SIM0_U.charge;
	dbu_status_1->ready_to_drive_button = SIM0_U.charge && SIM0_U.drive;
	return 1;
}

void can2_charger_config_rx_callback(charger_config_t *charger_config) {
	SIM0_U.CurrentLimit = charger_config->msg_set_current_limit;
	SIM0_U.VoltageLimit = charger_config->msg_set_voltage_limit;
	SIM0_U.EnableCharge = charger_config->msg_set_enabled;
}

