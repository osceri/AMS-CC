/*
 * "smile_data.h"
 *
 *  Created on: Mar 12, 2022
 *      Author: oscar
 */
#ifndef INC_SMILE_DATA_H_
#define INC_SMILE_DATA_H_

#include "smile_data.h"

struct {
   uint8_t precharge_drive_complete_u8;
   uint8_t precharge_drive_error_u8;
   uint8_t precharge_charge_error_u8;
   uint8_t precharge_charge_complete_u8;
   float Ts_f32;
} ams_parameters;

struct {
   double sought_voltage_f64;
   uint8_t AIR_minus_closed_u8;
   uint8_t drive_u8;
   uint8_t balance_u8;
   uint8_t AIR_plus_closed_u8;
   double U_variance_f64;
   uint8_t charge_u8;
   uint8_t precharge_closed_u8;
   uint8_t SC_u8;
   uint8_t error_u8;
   uint8_t charger_is_live_u8;
   double accumulator_voltage_f64;
   double vehicle_voltage_f64;
} ams_inputs;

struct {
   uint8_t enable_charge_u8;
   uint8_t enable_AIR_minus_u8;
   uint8_t enable_precharge_u8;
   uint8_t enable_driver_u8;
   uint8_t enable_charger_u8;
   uint8_t AMS_error_u8;
   uint8_t balances_126u8[126];
   uint8_t enable_AIR_plus_u8;
} ams_outputs;

#endif
