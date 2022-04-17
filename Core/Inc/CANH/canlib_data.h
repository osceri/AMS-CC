/*
 * "canlib_data.h"
 *
 *  Created on: Mar 12, 2022
 *      Author: oscar
 */

#ifndef INC_CANLIB_DATA_H_
#define INC_CANLIB_DATA_H_

typedef struct {
   uint8_t activate_ts_button;
   double dbu_temperature;
   uint8_t ready_to_drive_button;
} dbu_status_1_t;


typedef struct {
   uint8_t amk1_control_dc_on;
   uint8_t amk1_control_enable;
   uint8_t amk1_control_error_reset;
   uint8_t amk1_control_inverter_on;
   int16_t amk1_target_velocity;
   int16_t amk1_torque_limit_negativ;
   int16_t amk1_torque_limit_positiv;
} amk1_setpoints_1_t;

typedef struct {
   double i_ts;
} ivt_msg_result_i_t;

typedef struct {
   double u_cells;
} ivt_msg_result_u1_t;

typedef struct {
   double u_vehicle;
} ivt_msg_result_u3_t;

typedef struct {
   uint8_t enable_ams_charging;
} cc_status_t;


typedef struct {
   uint8_t air1_closed;
   uint8_t air2_closed;
   uint8_t ams_error;
   uint8_t charging_status;
   double fan_speed;
   uint8_t imd_error;
   double max_cell_temperature;
   double max_cell_voltage;
   double min_cell_temperature;
   double min_cell_voltage;
   uint8_t pre_charge_status;
   uint8_t sc_closed;
   double state_of_charge;
} ams_status_1_t;

typedef struct {
   double aux_1_temperature;
   double aux_2_temperature;
   double fuse_resistor_temperature;
   double pre_charge_resistor_temperature;
} ams_temperatures_t;

typedef struct {
   double v1s1;
   double v1s2;
   double v1s3;
   double v1s4;
   double v1s5;
   double v1s6;
   double v1s7;
   double v1s8;
   double v1s9;
   double v1s10;
   double v1s11;
   double v1s12;
   double v2s1;
   double v2s2;
   double v2s3;
   double v2s4;
   double v2s5;
   double v2s6;
   double v2s7;
   double v2s8;
   double v2s9;
   double v2s10;
   double v2s11;
   double v2s12;
   double v3s1;
   double v3s2;
   double v3s3;
   double v3s4;
   double v3s5;
   double v3s6;
   double v3s7;
   double v3s8;
   double v3s9;
   double v3s10;
   double v3s11;
   double v3s12;
   double v4s1;
   double v4s2;
   double v4s3;
   double v4s4;
   double v4s5;
   double v4s6;
   double v4s7;
   double v4s8;
   double v4s9;
   double v4s10;
   double v4s11;
   double v4s12;
   double v5s1;
   double v5s2;
   double v5s3;
   double v5s4;
   double v5s5;
   double v5s6;
   double v5s7;
   double v5s8;
   double v5s9;
   double v5s10;
   double v5s11;
   double v5s12;
   double v6s1;
   double v6s2;
   double v6s3;
   double v6s4;
   double v6s5;
   double v6s6;
   double v6s7;
   double v6s8;
   double v6s9;
   double v6s10;
   double v6s11;
   double v6s12;
   double v7s1;
   double v7s2;
   double v7s3;
   double v7s4;
   double v7s5;
   double v7s6;
   double v7s7;
   double v7s8;
   double v7s9;
   double v7s10;
   double v7s11;
   double v7s12;
   double v8s1;
   double v8s2;
   double v8s3;
   double v8s4;
   double v8s5;
   double v8s6;
   double v8s7;
   double v8s8;
   double v8s9;
   double v8s10;
   double v8s11;
   double v8s12;
   double v9s1;
   double v9s2;
   double v9s3;
   double v9s4;
   double v9s5;
   double v9s6;
   double v9s7;
   double v9s8;
   double v9s9;
   double v9s10;
   double v9s11;
   double v9s12;
   double v10s1;
   double v10s2;
   double v10s3;
   double v10s4;
   double v10s5;
   double v10s6;
   double v10s7;
   double v10s8;
   double v10s9;
   double v10s10;
   double v10s11;
   double v10s12;
   double v11s1;
   double v11s3;
   double v11s5;
   double v11s7;
   double v11s9;
   double v11s11;
   uint8_t voltage_multiplexor;
} ams_cell_voltages_t;

typedef struct {
   double t1s1;
   double t1s2;
   double t1s3;
   double t1s4;
   double t1s5;
   double t1s6;
   double t1s7;
   double t1s8;
   double t1s9;
   double t1s10;
   double t1s11;
   double t1s12;
   double t2s1;
   double t2s2;
   double t2s3;
   double t2s4;
   double t2s5;
   double t2s6;
   double t2s7;
   double t2s8;
   double t2s9;
   double t2s10;
   double t2s11;
   double t2s12;
   double t3s1;
   double t3s2;
   double t3s3;
   double t3s4;
   double t3s5;
   double t3s6;
   double t3s7;
   double t3s8;
   double t3s9;
   double t3s10;
   double t3s11;
   double t3s12;
   double t4s1;
   double t4s2;
   double t4s3;
   double t4s4;
   double t4s5;
   double t4s6;
   double t4s7;
   double t4s8;
   double t4s9;
   double t4s10;
   double t4s11;
   double t4s12;
   double t5s1;
   double t5s2;
   double t5s3;
   double t5s4;
   double t5s5;
   double t5s6;
   double t5s7;
   double t5s8;
   double t5s9;
   double t5s10;
   double t5s11;
   double t5s12;
   uint8_t temperature_multiplexor;
} ams_cell_temperatures_t;


typedef struct {
   double msg_set_current_limit;
   uint8_t msg_set_enabled;
   double msg_set_power_limit;
   uint8_t msg_set_time_out;
   double msg_set_voltage_limit;
   uint8_t set_mux;
} charger_config_t;


#endif
