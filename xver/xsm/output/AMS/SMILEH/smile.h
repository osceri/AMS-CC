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
  AMS_PRECHARGE_DRIVE_0_STATE = 0,
  AMS_PRECHARGE_DRIVE_CLOSE_AIR_MINUS_STATE = 1,
  AMS_PRECHARGE_DRIVE_CLOSE_PRECHARGE_STATE = 2,
  AMS_PRECHARGE_DRIVE_ERROR_STATE = 3,
  AMS_PRECHARGE_DRIVE_WAIT_STATE = 4,
  AMS_PRECHARGE_DRIVE_CLOSE_AIR_PLUS_STATE = 5,
  AMS_PRECHARGE_DRIVE_OPEN_PRECHARGE_STATE = 6,
  AMS_PRECHARGE_DRIVE_EXIT_STATE = 7,
} ams_precharge_drive_t;

typedef enum {
  AMS_PRECHARGE_CHARGE_0_STATE = 0,
  AMS_PRECHARGE_CHARGE_START_CHARGER_STATE = 1,
  AMS_PRECHARGE_CHARGE_CLOSE_AIR_MINUS_STATE = 2,
  AMS_PRECHARGE_CHARGE_ERROR_STATE = 3,
  AMS_PRECHARGE_CHARGE_CLOSE_PRECHARGE_STATE = 4,
  AMS_PRECHARGE_CHARGE_WAIT_STATE = 5,
  AMS_PRECHARGE_CHARGE_CLOSE_AIR_PLUS_STATE = 6,
  AMS_PRECHARGE_CHARGE_OPEN_PRECHARGE_STATE = 7,
  AMS_PRECHARGE_CHARGE_EXIT_STATE = 8,
} ams_precharge_charge_t;

typedef enum {
  AMS_0_STATE = 0,
  AMS_ENTRY_STATE = 1,
  AMS_IDLE_STATE = 2,
  AMS_BALANCE_STATE = 3,
  AMS_PRECHARGE_DRIVE_STATE = 4,
  AMS_DRIVE_STATE = 5,
  AMS_ERROR_STATE = 6,
  AMS_PRECHARGE_CHARGE_STATE = 7,
  AMS_CHARGE_STATE = 8,
} ams_t;


uint8_t after(uint32_t* timer, float boundary);

uint8_t before(uint32_t* timer, float boundary);

void ams_0(uint32_t* super_timer, uint8_t* super_state);
void ams_entry(uint32_t* super_timer, uint8_t* super_state);
void ams_idle(uint32_t* super_timer, uint8_t* super_state);
void ams_balance(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_drive_0(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_drive_close_air_minus(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_drive_close_precharge(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_drive_error(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_drive_wait(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_drive_close_air_plus(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_drive_open_precharge(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_drive_exit(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_drive(uint32_t* super_timer, uint8_t* super_state);
void ams_drive(uint32_t* super_timer, uint8_t* super_state);
void ams_error(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_charge_0(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_charge_start_charger(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_charge_close_air_minus(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_charge_error(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_charge_close_precharge(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_charge_wait(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_charge_close_air_plus(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_charge_open_precharge(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_charge_exit(uint32_t* super_timer, uint8_t* super_state);
void ams_precharge_charge(uint32_t* super_timer, uint8_t* super_state);
void ams_charge(uint32_t* super_timer, uint8_t* super_state);
void ams();
#endif
