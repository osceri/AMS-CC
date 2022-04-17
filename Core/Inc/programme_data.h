/*
 * programme_data.h
 *
 *  Created on: Apr 10, 2022
 *      Author: oscar
 */

#ifndef INC_PROGRAMME_DATA_H_
#define INC_PROGRAMME_DATA_H_

#include "canlib_data.h"

typedef struct {
	CAN_TxHeaderTypeDef tx_header;
	CAN_RxHeaderTypeDef rx_header;
	uint8_t data[8];
} can_queue_element_t;

typedef struct {
	uint8_t AMS_error_latched;
	uint8_t IMD_error_latched;
	uint8_t SC_probe;
	uint8_t IMD_ok;
	uint8_t AIR_plus_closed;
	uint8_t AIR_minus_closed;
	uint8_t precharge_closed;
} GPIO_t;

typedef struct {
	double duty_cycle;
	double frequency;
} IMD_t;

typedef struct {
	double duty_cycle;
} pPID_t;

typedef struct {
   double precharge_resistor;
   double fuse;
   double aux_1;
   double aux_2;
} temperatures_t;

typedef struct {
	double charger_current_limit;
	double charger_voltage_limit;
	uint8_t enable_charger;
} charger_t;

typedef struct {
	double SOC;
	double SOH;
} CSE_t;


typedef enum {
	ERROR_NO_ERROR = 0,
	ERROR_ANY = 1,
	ERROR_IMD = 2,
	ERROR_AMS = 3,
	ERROR_NO_LTC_DATA = 10,
	ERROR_NO_CURRENT_DATA = 11,
	ERROR_UNDER_VOLTAGE = 21,
	ERROR_OVER_VOLTAGE = 22,
	ERROR_UNDER_TEMPERATURE = 23,
	ERROR_OVER_TEMPERATURE = 24,
	ERROR_UNDER_CURRENT = 31,
	ERROR_OVER_CURRENT = 32,
	ERROR_NO_CURRENT = 33,
	ERROR_SM_PRECHARGE_DRIVE_CLOSE_AIR_MINUS = 111,
	ERROR_SM_PRECHARGE_DRIVE_CLOSE_PRECHARGE = 112,
	ERROR_SM_PRECHARGE_DRIVE_WAIT = 113,
	ERROR_SM_PRECHARGE_DRIVE_CLOSE_AIR_PLUS = 114,
	ERROR_SM_PRECHARGE_DRIVE_OPEN_PRECHARGE = 115,
	ERROR_SM_PRECHARGE_CHARGE_CLOSE_AIR_MINUS = 121,
	ERROR_SM_PRECHARGE_CHARGE_CLOSE_PRECHARGE = 122,
	ERROR_SM_PRECHARGE_CHARGE_WAIT = 123,
	ERROR_SM_PRECHARGE_CHARGE_CLOSE_AIR_PLUS = 124,
	ERROR_SM_PRECHARGE_CHARGE_OPEN_PRECHARGE = 125,
	ERROR_SM_DRIVE_FORCE_QUIT = 131,
	ERROR_SM_CHARGE_FORCE_QUIT = 141
} error_t;



#endif /* INC_PROGRAMME_DATA_H_ */
