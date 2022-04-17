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

void can1_dbu_status_1_rx_callback(dbu_status_1_t *dbu_status_1) {
	xQueueOverwrite(start_drive_queue, &dbu_status_1->ready_to_drive_button);
}

void can2_amk1_setpoints_1_rx_callback(amk1_setpoints_1_t *amk1_setpoints_1) {

}

void can2_ivt_msg_result_i_rx_callback(ivt_msg_result_i_t *ivt_msg_result_i) {
	xQueueOverwrite(accumulator_voltage_queue, &ivt_msg_result_i->i_ts);
}

void can2_ivt_msg_result_u1_rx_callback(ivt_msg_result_u1_t *ivt_msg_result_u1) {
	xQueueOverwrite(accumulator_voltage_queue, &ivt_msg_result_u1->u_cells);
}

void can2_ivt_msg_result_u3_rx_callback(ivt_msg_result_u3_t *ivt_msg_result_u3) {
	xQueueOverwrite(vehicle_voltage_queue, &ivt_msg_result_u3->u_vehicle);
}

void can2_cc_status_rx_callback(cc_status_t *cc_status) {
	xQueueOverwrite(start_charge_queue, &cc_status->enable_ams_charging);
}

uint8_t can2_charger_config_tx_callback(charger_config_t *charger_config) {
	charger_t charger;
	if (xQueuePeek(charger_queue, &charger, 0)) {
		charger_config->msg_set_current_limit = charger.charger_current_limit;
		charger_config->msg_set_voltage_limit = charger.charger_voltage_limit;
		charger_config->msg_set_power_limit = charger.charger_current_limit
				* charger.charger_voltage_limit;
		charger_config->msg_set_time_out = 60;
		charger_config->msg_set_enabled = charger.enable_charger;
	}
}

uint8_t can1_ams_status_1_tx_callback(ams_status_1_t *ams_status_1) {
	double *cell_voltages;
	double *cell_temperatures;
	double maximum_cell_temperature;
	double minimum_cell_temperature;
	double maximum_cell_voltage;
	double minimum_cell_voltage;
	if (xQueuePeek(cell_voltages_queue, &cell_voltages, 0)
			&& xQueuePeek(cell_temperatures_queue, &cell_temperatures, 0)) {
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

	}

	temperatures_t temperatures;
	charger_t charger;
	GPIO_t GPIO;
	pPID_t PID;
	CSE_t CSE;

	xQueuePeek(temperatures_queue, &temperatures, 0);
	xQueuePeek(charger_queue, &charger, 0);
	xQueuePeek(GPIO_queue, &GPIO, 0);
	xQueuePeek(PID_queue, &PID, 0);
	xQueuePeek(CSE_queue, &CSE, 0);

	ams_status_1->air1_closed = GPIO.AIR_minus_closed;
	ams_status_1->air2_closed = GPIO.AIR_plus_closed;
	ams_status_1->ams_error = GPIO.AMS_error_latched;
	ams_status_1->imd_error = GPIO.IMD_error_latched;
	ams_status_1->charging_status = charger.enable_charger;
	ams_status_1->fan_speed = PID.duty_cycle;
	ams_status_1->max_cell_temperature = maximum_cell_temperature;
	ams_status_1->max_cell_voltage = maximum_cell_voltage;
	ams_status_1->min_cell_temperature = minimum_cell_temperature;
	ams_status_1->min_cell_voltage = minimum_cell_voltage;
	ams_status_1->pre_charge_status = 1;
	ams_status_1->sc_closed = GPIO.SC_probe;
	ams_status_1->state_of_charge = CSE.SOC*100;
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
