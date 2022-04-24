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
#include "CSE.h"

void can1_dbu_status_1_rx_callback(dbu_status_1_t *dbu_status_1) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xQueueOverwriteFromISR(start_drive_queue,
			&dbu_status_1->activate_ts_button, &xHigherPriorityTaskWoken);
	xQueueOverwriteFromISR(start_balance_queue,
			&dbu_status_1->ready_to_drive_button, &xHigherPriorityTaskWoken);
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void can2_ivt_msg_result_i_rx_callback(ivt_msg_result_i_t *ivt_msg_result_i) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xQueueOverwriteFromISR(accumulator_current_queue, &ivt_msg_result_i->i_ts,
			&xHigherPriorityTaskWoken);
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void can2_ivt_msg_result_u1_rx_callback(ivt_msg_result_u1_t *ivt_msg_result_u1) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xQueueOverwriteFromISR(accumulator_voltage_queue,
			&ivt_msg_result_u1->u_cells, &xHigherPriorityTaskWoken);
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void can2_ivt_msg_result_u3_rx_callback(ivt_msg_result_u3_t *ivt_msg_result_u3) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xQueueOverwriteFromISR(vehicle_voltage_queue, &ivt_msg_result_u3->u_vehicle,
			&xHigherPriorityTaskWoken);
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void can2_cc_status_rx_callback(cc_status_t *cc_status) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xQueueOverwriteFromISR(start_charge_queue, &cc_status->enable_ams_charging,
			&xHigherPriorityTaskWoken);
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

uint8_t can2_charger_config_tx_callback(charger_config_t *charger_config) {
	charger_config->msg_set_current_limit = charger.charger_current_limit;
	charger_config->msg_set_voltage_limit = charger.charger_voltage_limit;
	charger_config->msg_set_power_limit = charger.charger_current_limit * charger.charger_voltage_limit;
	charger_config->msg_set_time_out = 60;
	charger_config->msg_set_enabled = charger.enable_charger;
	return 1;
}

uint8_t can1_ams_status_1_tx_callback(ams_status_1_t *ams_status_1) {
	double *cell_voltages;
	double *cell_temperatures;
	double maximum_cell_temperature;
	double minimum_cell_temperature;
	double maximum_cell_voltage;
	double minimum_cell_voltage;
	temperatures_t temperatures;

	if (xQueuePeek(cell_voltages_queue, &cell_voltages, 0)
			&& xQueuePeek(cell_temperatures_queue, &cell_temperatures, 0)
			&& xQueuePeek(temperatures_queue, &temperatures, 0)) {
		maximum_cell_temperature = cell_temperatures[0];
		minimum_cell_temperature = cell_temperatures[0];
		maximum_cell_voltage = cell_voltages[0];
		minimum_cell_voltage = cell_voltages[0];
		for (int i = 0; i < 126; i++) {
			if (maximum_cell_temperature < cell_temperatures[0]) {
				maximum_cell_temperature = cell_temperatures[0];
			}
			if (minimum_cell_temperature > cell_temperatures[0]) {
				minimum_cell_temperature = cell_temperatures[0];
			}
			if (maximum_cell_voltage < cell_voltages[0]) {
				maximum_cell_voltage = cell_voltages[0];
			}
			if (minimum_cell_voltage > cell_voltages[0]) {
				minimum_cell_voltage = cell_voltages[0];
			}

		}

		ams_status_1->air1_closed = get_air_minus_ext();
		ams_status_1->air2_closed = get_air_plus_ext();
		ams_status_1->ams_error = get_ams_error_latched_ext();
		ams_status_1->imd_error = get_imd_error_latched_ext();
		ams_status_1->charging_status = charger.enable_charger;
		ams_status_1->fan_speed = pPID.duty_cycle;
		ams_status_1->max_cell_temperature = maximum_cell_temperature;
		ams_status_1->max_cell_voltage = maximum_cell_voltage;
		ams_status_1->min_cell_temperature = minimum_cell_temperature;
		ams_status_1->min_cell_voltage = minimum_cell_voltage;
		ams_status_1->pre_charge_status = 1;
		ams_status_1->sc_closed = get_sc_probe_ext();
		ams_status_1->state_of_charge = CSE_Y.soc;

		return 1;
	} else {
		return 0;
	}

}

uint8_t can1_ams_temperatures_tx_callback(ams_temperatures_t *ams_temperatures) {
	return xQueuePeek(temperatures_queue, &ams_temperatures->aux_1_temperature,
			0);
}

uint8_t can1_ams_cell_voltages_tx_callback(
		ams_cell_voltages_t *ams_cell_voltages) {
	return xQueuePeek(cell_voltages_queue, &ams_cell_voltages->v1s1, 0);
}

uint8_t can1_ams_cell_temperatures_tx_callback(
		ams_cell_temperatures_t *ams_cell_temperatures) {
	return xQueuePeek(cell_temperatures_queue, &ams_cell_temperatures->t1s1, 0);
}
