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
   double accumulator_voltage;
   uint8_t air_minus_closed;
   double cell_voltages_variance;
   uint8_t SC;
   double vehicle_voltage;
   uint8_t precharge_closed;
   double minimum_cell_voltage;
   uint8_t air_plus_closed;
   float Ts;
   double maximum_cell_voltage;
   uint8_t imd_error;
   uint8_t ams_error;
   uint8_t balance;
   double accumulator_current;
   uint8_t drive;
   uint8_t charge;
   uint8_t charger_is_awake;
} ams_inputs;

struct {
   uint8_t precharge_drive_complete;
   uint8_t charge_complete;
   uint8_t drive_complete;
   uint8_t close_air_plus;
   uint8_t enable_charger;
   uint8_t precharge_charge_complete;
   uint8_t close_air_minus;
   uint8_t close_precharge;
   uint8_t error;
} ams_outputs;

#endif
