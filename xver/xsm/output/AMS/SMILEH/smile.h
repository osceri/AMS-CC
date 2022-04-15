#include "main.h"
/*
 * "smile.h"
 *
 *  Created on: Mar 12, 2022
 *      Author: oscar
 */
#ifndef INC_SMILE_H_
#define INC_SMILE_H_


typedef enum {
   STATE_NONE = -1;
   STATE_AMS_0 = 2,
   STATE_AMS_ENTRY = 3,
   STATE_AMS_IDLE = 4,
   STATE_AMS_BALANCE = 5,
   STATE_AMS_PRECHARGE_DRIVE_0 = 7,
   STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_MINUS = 8,
   STATE_AMS_PRECHARGE_DRIVE_CLOSE_PRECHARGE = 9,
   STATE_AMS_PRECHARGE_DRIVE_ERROR = 10,
   STATE_AMS_PRECHARGE_DRIVE_WAIT = 11,
   STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_PLUS = 12,
   STATE_AMS_PRECHARGE_DRIVE_OPEN_PRECHARGE = 13,
   STATE_AMS_PRECHARGE_DRIVE_EXIT = 14,
   STATE_AMS_PRECHARGE_DRIVE = 6,
   STATE_AMS_DRIVE = 15,
   STATE_AMS_ERROR = 16,
   STATE_AMS_PRECHARGE_CHARGE_0 = 18,
   STATE_AMS_PRECHARGE_CHARGE_START_CHARGER = 19,
   STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_MINUS = 20,
   STATE_AMS_PRECHARGE_CHARGE_ERROR = 21,
   STATE_AMS_PRECHARGE_CHARGE_CLOSE_PRECHARGE = 22,
   STATE_AMS_PRECHARGE_CHARGE_WAIT = 23,
   STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_PLUS = 24,
   STATE_AMS_PRECHARGE_CHARGE_OPEN_PRECHARGE = 25,
   STATE_AMS_PRECHARGE_CHARGE_EXIT = 26,
   STATE_AMS_PRECHARGE_CHARGE = 17,
   STATE_AMS_CHARGE = 27,
   STATE_AMS = 1,
} ams_state_t;

void ams_0_function();
void ams_entry_function();
void ams_idle_function();
void ams_balance_function();
void ams_precharge_drive_0_function();
void ams_precharge_drive_close_air_minus_function();
void ams_precharge_drive_close_precharge_function();
void ams_precharge_drive_error_function();
void ams_precharge_drive_wait_function();
void ams_precharge_drive_close_air_plus_function();
void ams_precharge_drive_open_precharge_function();
void ams_precharge_drive_exit_function();
void ams_precharge_drive_function();
void ams_drive_function();
void ams_error_function();
void ams_precharge_charge_0_function();
void ams_precharge_charge_start_charger_function();
void ams_precharge_charge_close_air_minus_function();
void ams_precharge_charge_error_function();
void ams_precharge_charge_close_precharge_function();
void ams_precharge_charge_wait_function();
void ams_precharge_charge_close_air_plus_function();
void ams_precharge_charge_open_precharge_function();
void ams_precharge_charge_exit_function();
void ams_precharge_charge_function();
void ams_charge_function();
void ams_function();
#endif
