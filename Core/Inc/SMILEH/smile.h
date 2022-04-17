#include "main.h"
/*
 * "smile.h"
 *
 *  Created on: Mar 12, 2022
 *      Author: oscar
 */
#ifndef INC_SMILE_H_
#define INC_SMILE_H_

#include "main.h"


typedef enum {
   STATE_NONE = 0,
   STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_MINUS = 3,
   STATE_AMS_PRECHARGE_DRIVE_CLOSE_PRECHARGE = 4,
   STATE_AMS_PRECHARGE_DRIVE_ERROR = 5,
   STATE_AMS_PRECHARGE_DRIVE_WAIT = 6,
   STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_PLUS = 7,
   STATE_AMS_PRECHARGE_DRIVE_OPEN_PRECHARGE = 8,
   STATE_AMS_PRECHARGE_DRIVE_EXIT = 9,
   STATE_AMS_PRECHARGE_DRIVE_0 = 10,
   STATE_AMS_PRECHARGE_DRIVE_ENTRY = 11,
   STATE_AMS_PRECHARGE_DRIVE = 2,
   STATE_AMS_DRIVE_0 = 13,
   STATE_AMS_DRIVE_DRIVE = 14,
   STATE_AMS_DRIVE_END_DRIVE = 15,
   STATE_AMS_DRIVE_EXIT = 16,
   STATE_AMS_DRIVE_FORCE_QUIT = 17,
   STATE_AMS_DRIVE = 12,
   STATE_AMS_ERROR_0 = 19,
   STATE_AMS_ERROR_ERROR_DETECTED = 20,
   STATE_AMS_ERROR_RELAYS_OPENED = 21,
   STATE_AMS_ERROR_AWAITING_ERROR_RESOLUTION = 22,
   STATE_AMS_ERROR = 18,
   STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_MINUS = 24,
   STATE_AMS_PRECHARGE_CHARGE_CLOSE_PRECHARGE = 25,
   STATE_AMS_PRECHARGE_CHARGE_ERROR = 26,
   STATE_AMS_PRECHARGE_CHARGE_WAIT = 27,
   STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_PLUS = 28,
   STATE_AMS_PRECHARGE_CHARGE_OPEN_PRECHARGE = 29,
   STATE_AMS_PRECHARGE_CHARGE_EXIT = 30,
   STATE_AMS_PRECHARGE_CHARGE_0 = 31,
   STATE_AMS_PRECHARGE_CHARGE_WAKE_CHARGER = 32,
   STATE_AMS_PRECHARGE_CHARGE = 23,
   STATE_AMS_CHARGE_0 = 34,
   STATE_AMS_CHARGE_CHARGE = 35,
   STATE_AMS_CHARGE_END_CHARGE = 36,
   STATE_AMS_CHARGE_EXIT = 37,
   STATE_AMS_CHARGE_FORCE_QUIT = 38,
   STATE_AMS_CHARGE = 33,
   STATE_AMS_IDLE = 39,
   STATE_AMS_BALANCE_0 = 41,
   STATE_AMS_BALANCE_BALANCE = 42,
   STATE_AMS_BALANCE_WAIT = 43,
   STATE_AMS_BALANCE = 40,
   STATE_AMS_0 = 44,
   STATE_AMS = 1,
} ams_state_t;

ams_state_t ams_precharge_drive_close_air_minus_function();
ams_state_t ams_precharge_drive_close_precharge_function();
ams_state_t ams_precharge_drive_error_function();
ams_state_t ams_precharge_drive_wait_function();
ams_state_t ams_precharge_drive_close_air_plus_function();
ams_state_t ams_precharge_drive_open_precharge_function();
ams_state_t ams_precharge_drive_exit_function();
ams_state_t ams_precharge_drive_0_function();
ams_state_t ams_precharge_drive_entry_function();
ams_state_t ams_precharge_drive_function();
ams_state_t ams_drive_0_function();
ams_state_t ams_drive_drive_function();
ams_state_t ams_drive_end_drive_function();
ams_state_t ams_drive_exit_function();
ams_state_t ams_drive_force_quit_function();
ams_state_t ams_drive_function();
ams_state_t ams_error_0_function();
ams_state_t ams_error_error_detected_function();
ams_state_t ams_error_relays_opened_function();
ams_state_t ams_error_awaiting_error_resolution_function();
ams_state_t ams_error_function();
ams_state_t ams_precharge_charge_close_air_minus_function();
ams_state_t ams_precharge_charge_close_precharge_function();
ams_state_t ams_precharge_charge_error_function();
ams_state_t ams_precharge_charge_wait_function();
ams_state_t ams_precharge_charge_close_air_plus_function();
ams_state_t ams_precharge_charge_open_precharge_function();
ams_state_t ams_precharge_charge_exit_function();
ams_state_t ams_precharge_charge_0_function();
ams_state_t ams_precharge_charge_wake_charger_function();
ams_state_t ams_precharge_charge_function();
ams_state_t ams_charge_0_function();
ams_state_t ams_charge_charge_function();
ams_state_t ams_charge_end_charge_function();
ams_state_t ams_charge_exit_function();
ams_state_t ams_charge_force_quit_function();
ams_state_t ams_charge_function();
ams_state_t ams_idle_function();
ams_state_t ams_balance_0_function();
ams_state_t ams_balance_balance_function();
ams_state_t ams_balance_wait_function();
ams_state_t ams_balance_function();
ams_state_t ams_0_function();
ams_state_t ams_function();
#endif
