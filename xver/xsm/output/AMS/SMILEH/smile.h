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
   STATE_NONE = -1,
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
   STATE_AMS_ERROR = 18,
   STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_MINUS = 20,
   STATE_AMS_PRECHARGE_CHARGE_CLOSE_PRECHARGE = 21,
   STATE_AMS_PRECHARGE_CHARGE_ERROR = 22,
   STATE_AMS_PRECHARGE_CHARGE_WAIT = 23,
   STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_PLUS = 24,
   STATE_AMS_PRECHARGE_CHARGE_OPEN_PRECHARGE = 25,
   STATE_AMS_PRECHARGE_CHARGE_EXIT = 26,
   STATE_AMS_PRECHARGE_CHARGE_0 = 27,
   STATE_AMS_PRECHARGE_CHARGE_WAKE_CHARGER = 28,
   STATE_AMS_PRECHARGE_CHARGE = 19,
   STATE_AMS_CHARGE = 29,
   STATE_AMS_IDLE = 30,
   STATE_AMS_0 = 31,
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
ams_state_t ams_charge_function();
ams_state_t ams_idle_function();
ams_state_t ams_0_function();
ams_state_t ams_function();
#endif
