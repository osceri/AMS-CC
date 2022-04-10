/*
 * programme_data.h
 *
 *  Created on: Apr 10, 2022
 *      Author: oscar
 */

#ifndef INC_PROGRAMME_DATA_H_
#define INC_PROGRAMME_DATA_H_

typedef struct {
	CAN_TxHeaderTypeDef tx_header;
	CAN_RxHeaderTypeDef rx_header;
	uint8_t data[8];
} can_queue_element_t;

typedef struct {
	uint8_t AMS_error_latched;
	uint8_t IMD_error_latched;
	uint8_t SC_probe;
	uint8_t IMD_ok;
	uint8_t AIR_plus_closed;
	uint8_t AIR_minus_closed;
	uint8_t precharge_closed;
} GPIO_t;

typedef struct {
	float duty_cycle;
	float frequency;
} IMD_t;

typedef struct {
	double u_vehicle;
} ivt_msg_result_u3_t;

typedef struct {
	double i_ts;
} ivt_msg_result_i_t;

typedef struct {
	double u_cells;
} ivt_msg_result_u1_t;

typedef struct {
	uint8_t activate_ts_button;
	uint8_t ready_to_drive_button;
	float dbu_temperature;
} dbu_status_1_t;

typedef struct {
	double u_fuse;
} ivt_msg_result_u2_t;

typedef struct {
	uint8_t air1_closed;
	uint8_t air2_closed;
	uint8_t sc_closed;
	uint8_t pre_charge_status;
	uint8_t ams_error;
	uint8_t imd_error;
	float state_of_charge;
	float min_cell_voltage;
	float max_cell_voltage;
	float min_cell_temperature;
	float max_cell_temperature;
	float fan_speed;
} ams_status_1_t;

typedef struct {
	float v1s1;
	float v7s1;
	float v1s2;
	float v7s2;
	float v1s3;
	float v7s3;
	float v1s4;
	float v7s4;
	float v1s5;
	float v7s5;
	float v1s6;
	float v7s6;
	float v1s7;
	float v7s7;
	float v1s8;
	float v7s8;
	float v1s9;
	float v7s9;
	float v1s10;
	float v7s10;
	float v1s11;
	float v7s11;
	float v1s12;
	float v7s12;
	float v2s1;
	float v8s1;
	float v2s2;
	float v8s2;
	float v2s3;
	float v8s3;
	float v2s4;
	float v8s4;
	float v2s5;
	float v8s5;
	float v2s6;
	float v8s6;
	float v2s7;
	float v8s7;
	float v2s8;
	float v8s8;
	float v2s9;
	float v8s9;
	float v2s10;
	float v8s10;
	float v2s11;
	float v8s11;
	float v2s12;
	float v8s12;
	float v3s1;
	float v9s1;
	float v3s2;
	float v9s2;
	float v3s3;
	float v9s3;
	float v3s4;
	float v9s4;
	float v3s5;
	float v9s5;
	float v3s6;
	float v9s6;
	float v3s7;
	float v9s7;
	float v3s8;
	float v9s8;
	float v3s9;
	float v9s9;
	float v3s10;
	float v9s10;
	float v3s11;
	float v9s11;
	float v3s12;
	float v9s12;
	float v4s1;
	float v10s1;
	float v4s2;
	float v10s2;
	float v4s3;
	float v10s3;
	float v4s4;
	float v10s4;
	float v4s5;
	float v10s5;
	float v4s6;
	float v10s6;
	float v4s7;
	float v10s7;
	float v4s8;
	float v10s8;
	float v4s9;
	float v10s9;
	float v4s10;
	float v10s10;
	float v4s11;
	float v10s11;
	float v4s12;
	float v10s12;
	float v5s1;
	float v11s1;
	float v5s2;
	float v5s3;
	float v11s3;
	float v5s4;
	float v5s5;
	float v11s5;
	float v5s6;
	float v5s7;
	float v11s7;
	float v5s8;
	float v5s9;
	float v11s9;
	float v5s10;
	float v5s11;
	float v11s11;
	float v5s12;
	float v6s1;
	float v6s2;
	float v6s3;
	float v6s4;
	float v6s5;
	float v6s6;
	float v6s7;
	float v6s8;
	float v6s9;
	float v6s10;
	float v6s11;
	float v6s12;
} ams_cell_voltages_t;

typedef struct {
	float t1s1;
	float t1s2;
	float t1s3;
	float t1s4;
	float t1s5;
	float t1s6;
	float t1s7;
	float t1s8;
	float t1s9;
	float t1s10;
	float t1s11;
	float t1s12;
	float t2s1;
	float t2s2;
	float t2s3;
	float t2s4;
	float t2s5;
	float t2s6;
	float t2s7;
	float t2s8;
	float t2s9;
	float t2s10;
	float t2s11;
	float t2s12;
	float t3s1;
	float t3s2;
	float t3s3;
	float t3s4;
	float t3s5;
	float t3s6;
	float t3s7;
	float t3s8;
	float t3s9;
	float t3s10;
	float t3s11;
	float t3s12;
	float t4s1;
	float t4s2;
	float t4s3;
	float t4s4;
	float t4s5;
	float t4s6;
	float t4s7;
	float t4s8;
	float t4s9;
	float t4s10;
	float t4s11;
	float t4s12;
	float t5s1;
	float t5s2;
	float t5s3;
	float t5s4;
	float t5s5;
	float t5s6;
	float t5s7;
	float t5s8;
	float t5s9;
	float t5s10;
	float t5s11;
	float t5s12;
} ams_cell_temperatures_t;

typedef struct {
	float pre_charge_resistor_temperature;
	float fuse_resistor_temperature;
	float aux_1_temperature;
	float aux_2_temperature;
} ams_temperatures_t;

#endif /* INC_PROGRAMME_DATA_H_ */
