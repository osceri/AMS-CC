/*
 * "smile_data.h"
 *
 *  Created on: Mar 12, 2022
 *      Author: oscar
 */
#ifndef INC_SMILE_DATA_H_
#define INC_SMILE_DATA_H_

#include "main.h"

#include "smile_data.h"

struct {
   uint8_t error;
   uint8_t precharge_charge_complete;
   uint8_t drive_complete;
   uint8_t charge_complete;
   uint8_t precharge_drive_complete;
   float Ts;
} ams_parameters;

struct {
   uint8_t ams_error;
   double accumulator_current;
   double minimum_cell_voltage;
   double cell_voltages_variance;
   uint8_t balance;
   uint8_t air_minus_closed;
   double maximum_cell_voltage;
   uint8_t imd_error;
   uint8_t charge_error;
   double vehicle_voltage;
   uint8_t charge;
   uint8_t air_plus_closed;
   double accumulator_voltage;
   uint8_t drive;
   uint8_t charger_is_awake;
   uint8_t drive_error;
   uint8_t precharge_closed;
   uint8_t SC;
} ams_inputs;

struct {
   uint8_t enable_charger;
   uint8_t close_precharge;
   uint8_t close_air_minus;
   uint8_t close_air_plus;
   uint8_t set_error;
} ams_outputs;

#endif
