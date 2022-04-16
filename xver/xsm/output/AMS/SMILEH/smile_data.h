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
   uint8_t drive_complete;
   uint8_t precharge_charge_complete;
   uint8_t precharge_charge_error;
   uint8_t precharge_drive_complete;
   uint8_t precharge_drive_error;
   float Ts;
} ams_parameters;

struct {
   uint8_t precharge_closed;
   float accumulator_voltage;
   uint8_t air_plus_closed;
   uint8_t balance;
   uint8_t SC;
   float vehicle_voltage;
   uint8_t charge;
   uint8_t drive;
   float cell_voltage_variance;
   double accumulator_current;
   uint8_t drive_error;
   uint8_t air_minus_closed;
   uint8_t imd_error;
   uint8_t charge_error;
   uint8_t charger_is_awake;
   uint8_t charge_complete;
   uint8_t ams_error;
   uint8_t vehicle_voltage;
} ams_inputs;

struct {
   uint8_t close_air_minus;
   uint8_t close_air_plus;
   uint8_t error;
   uint8_t drive_force_quit;
   uint8_t close_precharge;
} ams_outputs;

#endif
