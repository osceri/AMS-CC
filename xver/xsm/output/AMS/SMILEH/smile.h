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
   STATE_AMS_ERROR_0 = 3,
   STATE_AMS_ERROR_ERROR_DETECTED = 4,
   STATE_AMS_ERROR_RELAYS_OPENED = 5,
   STATE_AMS_ERROR_AWAITING_ERROR_RESOLUTION = 6,
   STATE_AMS_ERROR = 2,
   STATE_AMS_MAIN_PRECHARGE_DRIVE_CLOSE_AIR_MINUS = 9,
   STATE_AMS_MAIN_PRECHARGE_DRIVE_CLOSE_PRECHARGE = 10,
   STATE_AMS_MAIN_PRECHARGE_DRIVE_ERROR = 11,
   STATE_AMS_MAIN_PRECHARGE_DRIVE_WAIT = 12,
   STATE_AMS_MAIN_PRECHARGE_DRIVE_CLOSE_AIR_PLUS = 13,
   STATE_AMS_MAIN_PRECHARGE_DRIVE_OPEN_PRECHARGE = 14,
   STATE_AMS_MAIN_PRECHARGE_DRIVE_EXIT = 15,
   STATE_AMS_MAIN_PRECHARGE_DRIVE_0 = 16,
   STATE_AMS_MAIN_PRECHARGE_DRIVE_ENTRY = 17,
   STATE_AMS_MAIN_PRECHARGE_DRIVE = 8,
   STATE_AMS_MAIN_DRIVE_0 = 19,
   STATE_AMS_MAIN_DRIVE_DRIVE = 20,
   STATE_AMS_MAIN_DRIVE_END_DRIVE = 21,
   STATE_AMS_MAIN_DRIVE_EXIT = 22,
   STATE_AMS_MAIN_DRIVE_FORCE_QUIT = 23,
   STATE_AMS_MAIN_DRIVE = 18,
   STATE_AMS_MAIN_PRECHARGE_CHARGE_CLOSE_AIR_MINUS = 25,
   STATE_AMS_MAIN_PRECHARGE_CHARGE_CLOSE_PRECHARGE = 26,
   STATE_AMS_MAIN_PRECHARGE_CHARGE_ERROR = 27,
   STATE_AMS_MAIN_PRECHARGE_CHARGE_WAIT = 28,
   STATE_AMS_MAIN_PRECHARGE_CHARGE_CLOSE_AIR_PLUS = 29,
   STATE_AMS_MAIN_PRECHARGE_CHARGE_OPEN_PRECHARGE = 30,
   STATE_AMS_MAIN_PRECHARGE_CHARGE_EXIT = 31,
   STATE_AMS_MAIN_PRECHARGE_CHARGE_0 = 32,
   STATE_AMS_MAIN_PRECHARGE_CHARGE_WAKE_CHARGER = 33,
   STATE_AMS_MAIN_PRECHARGE_CHARGE = 24,
   STATE_AMS_MAIN_CHARGE_0 = 35,
   STATE_AMS_MAIN_CHARGE_CHARGE = 36,
   STATE_AMS_MAIN_CHARGE_END_CHARGE = 37,
   STATE_AMS_MAIN_CHARGE_EXIT = 38,
   STATE_AMS_MAIN_CHARGE_FORCE_QUIT = 39,
   STATE_AMS_MAIN_CHARGE = 34,
   STATE_AMS_MAIN_IDLE = 40,
   STATE_AMS_MAIN_BALANCE_0 = 42,
   STATE_AMS_MAIN_BALANCE_BALANCE = 43,
   STATE_AMS_MAIN_BALANCE_WAIT = 44,
   STATE_AMS_MAIN_BALANCE = 41,
   STATE_AMS_MAIN_0 = 45,
   STATE_AMS_MAIN = 7,
   STATE_AMS_0 = 46,
   STATE_AMS = 1,
} ams_state_t;

ams_state_t ams_error_0_function();
ams_state_t ams_error_error_detected_function();
ams_state_t ams_error_relays_opened_function();
ams_state_t ams_error_awaiting_error_resolution_function();
ams_state_t ams_error_function();
ams_state_t ams_main_precharge_drive_close_air_minus_function();
ams_state_t ams_main_precharge_drive_close_precharge_function();
ams_state_t ams_main_precharge_drive_error_function();
ams_state_t ams_main_precharge_drive_wait_function();
ams_state_t ams_main_precharge_drive_close_air_plus_function();
ams_state_t ams_main_precharge_drive_open_precharge_function();
ams_state_t ams_main_precharge_drive_exit_function();
ams_state_t ams_main_precharge_drive_0_function();
ams_state_t ams_main_precharge_drive_entry_function();
ams_state_t ams_main_precharge_drive_function();
ams_state_t ams_main_drive_0_function();
ams_state_t ams_main_drive_drive_function();
ams_state_t ams_main_drive_end_drive_function();
ams_state_t ams_main_drive_exit_function();
ams_state_t ams_main_drive_force_quit_function();
ams_state_t ams_main_drive_function();
ams_state_t ams_main_precharge_charge_close_air_minus_function();
ams_state_t ams_main_precharge_charge_close_precharge_function();
ams_state_t ams_main_precharge_charge_error_function();
ams_state_t ams_main_precharge_charge_wait_function();
ams_state_t ams_main_precharge_charge_close_air_plus_function();
ams_state_t ams_main_precharge_charge_open_precharge_function();
ams_state_t ams_main_precharge_charge_exit_function();
ams_state_t ams_main_precharge_charge_0_function();
ams_state_t ams_main_precharge_charge_wake_charger_function();
ams_state_t ams_main_precharge_charge_function();
ams_state_t ams_main_charge_0_function();
ams_state_t ams_main_charge_charge_function();
ams_state_t ams_main_charge_end_charge_function();
ams_state_t ams_main_charge_exit_function();
ams_state_t ams_main_charge_force_quit_function();
ams_state_t ams_main_charge_function();
ams_state_t ams_main_idle_function();
ams_state_t ams_main_balance_0_function();
ams_state_t ams_main_balance_balance_function();
ams_state_t ams_main_balance_wait_function();
ams_state_t ams_main_balance_function();
ams_state_t ams_main_0_function();
ams_state_t ams_main_function();
ams_state_t ams_0_function();
ams_state_t ams_function();
#endif
