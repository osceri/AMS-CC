/*
 * "smile.c"
 *
 *  Created on: Mar 12, 2022
 *      Author: oscar
 */

#include "main.h"

#include "smile_callbacks.h"
#include "smile_data.h"
#include "smile.h"


__weak void zero_out_balance_v0() {
   /* Type your actual code somewhere else */
}
__weak void do_balance_v0() {
   /* Type your actual code somewhere else */
}
static state_t state_r0;
static state_t state_r1;
static state_t state_r2;
static float timer_r0;
static float timer_r1;
static float timer_r2;
ams_state_t ams_0_function() {
   if(1) {
       ams_outputs.balances_126u8;
       timer_r1 = 0;
       return STATE_AMS_ENTRY;
   }
   timer_r1 += ams_parameters.Ts_f32;
   return STATE_AMS_0;
}

ams_state_t ams_entry_function() {
   if(1) {
       ams_parameters.precharge_drive_complete_u8=0;
       ams_parameters.precharge_charge_complete_u8=0;
       zero_out_balance_v0();
       ams_outputs.enable_charge_u8=0;
       ams_outputs.enable_precharge_u8=0;
       ams_outputs.enable_AIR_plus_u8=0;
       ams_outputs.enable_AIR_minus_u8=0;
       timer_r1 = 0;
   }
   if((timer_r1 > 5)) {
       timer_r1 = 0;
       return STATE_AMS_IDLE;
   }
   timer_r1 += ams_parameters.Ts_f32;
   return STATE_AMS_ENTRY;
}

ams_state_t ams_idle_function() {
   if(1) {
       timer_r1 = 0;
   }
   if(ams_inputs.SC_u8&&ams_inputs.balance_u8) {
       timer_r1 = 0;
       return STATE_AMS_BALANCE;
   }
   if(ams_inputs.SC_u8&&ams_inputs.drive_u8) {
       timer_r1 = 0;
       return STATE_AMS_PRECHARGE_DRIVE;
   }
   if(ams_inputs.SC_u8&&ams_inputs.charge_u8) {
       timer_r1 = 0;
       return STATE_AMS_PRECHARGE_CHARGE;
   }
   timer_r1 += ams_parameters.Ts_f32;
   return STATE_AMS_IDLE;
}

ams_state_t ams_balance_function() {
   if((timer_r1 > 60)) {
       do_balance_v0();
       timer_r1 = 0;
   }
   if(ams_inputs.U_variance_f64<0.00001) {
       zero_out_balance_v0();
       timer_r1 = 0;
       return STATE_AMS_IDLE;
   }
   timer_r1 += ams_parameters.Ts_f32;
   return STATE_AMS_BALANCE;
}

ams_state_t ams_precharge_drive_0_function() {
   if(1) {
       ams_outputs.enable_AIR_minus_u8=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_MINUS;
   }
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_DRIVE_0;
}

ams_state_t ams_precharge_drive_close_air_minus_function() {
   if((timer_r2 < 2)&&(ams_inputs.AIR_minus_closed_u8==1)) {
       ams_outputs.enable_precharge_u8=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_CLOSE_PRECHARGE;
   }
   if((timer_r2 > 2)) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ERROR;
   }
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_MINUS;
}

ams_state_t ams_precharge_drive_close_precharge_function() {
   if((timer_r2 < 2)&&(ams_inputs.precharge_closed_u8==1)) {
       ams_outputs.enable_driver_u8=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_WAIT;
   }
   if((timer_r2 > 2)) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ERROR;
   }
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_DRIVE_CLOSE_PRECHARGE;
}

ams_state_t ams_precharge_drive_error_function() {
   if(1) {
       ams_parameters.precharge_drive_error_u8=1;
       timer_r2 = 0;
   }
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_DRIVE_ERROR;
}

ams_state_t ams_precharge_drive_wait_function() {
   if((timer_r2 > 2)&&(timer_r2 < 25)&&(0.95*ams_inputs.accumulator_voltage_f64<ams_inputs.vehicle_voltage_f64)) {
       ams_outputs.enable_AIR_plus_u8=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_PLUS;
   }
   if((timer_r2 > 25)) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ERROR;
   }
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_DRIVE_WAIT;
}

ams_state_t ams_precharge_drive_close_air_plus_function() {
   if((timer_r2 < 2)&&(ams_inputs.AIR_plus_closed_u8==1)) {
       ams_outputs.enable_precharge_u8=0;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_OPEN_PRECHARGE;
   }
   if((timer_r2 > 2)) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ERROR;
   }
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_PLUS;
}

ams_state_t ams_precharge_drive_open_precharge_function() {
   if((timer_r2 < 2)&&(ams_inputs.precharge_closed_u8==1)) {
       ams_parameters.precharge_drive_complete_u8=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_EXIT;
   }
   if((timer_r2 > 2)) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ERROR;
   }
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_DRIVE_OPEN_PRECHARGE;
}

ams_state_t ams_precharge_drive_exit_function() {
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_DRIVE_EXIT;
}

ams_state_t ams_precharge_drive_function() {
   if(timer_r1 < 0.001*Ts) {
       state_r1 = STATE_AMS_PRECHARGE_DRIVE_0;
   }

   switch(state_r2) {
       case STATE_AMS_PRECHARGE_DRIVE_0:
           state_r2 = ams_precharge_drive_0_function();
           break;
       case STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_MINUS:
           state_r2 = ams_precharge_drive_close_air_minus_function();
           break;
       case STATE_AMS_PRECHARGE_DRIVE_CLOSE_PRECHARGE:
           state_r2 = ams_precharge_drive_close_precharge_function();
           break;
       case STATE_AMS_PRECHARGE_DRIVE_ERROR:
           state_r2 = ams_precharge_drive_error_function();
           break;
       case STATE_AMS_PRECHARGE_DRIVE_WAIT:
           state_r2 = ams_precharge_drive_wait_function();
           break;
       case STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_PLUS:
           state_r2 = ams_precharge_drive_close_air_plus_function();
           break;
       case STATE_AMS_PRECHARGE_DRIVE_OPEN_PRECHARGE:
           state_r2 = ams_precharge_drive_open_precharge_function();
           break;
       case STATE_AMS_PRECHARGE_DRIVE_EXIT:
           state_r2 = ams_precharge_drive_exit_function();
           break;
   }

   if(1) {
       timer_r1 = 0;
   }
   if(ams_parameters.precharge_drive_complete_u8==1) {
       timer_r1 = 0;
       return STATE_AMS_DRIVE;
   }
   if((ams_parameters.precharge_drive_error_u8==1)||ams_inputs.error_u8) {
       ams_outputs.AMS_error_u8=1;
       timer_r1 = 0;
       return STATE_AMS_ERROR;
   }
   timer_r1 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_DRIVE;
}

ams_state_t ams_drive_function() {
   if(ams_inputs.drive_u8==0) {
       ams_outputs.enable_precharge_u8=0;
       ams_outputs.enable_AIR_plus_u8=0;
       ams_outputs.enable_AIR_minus_u8=0;
       timer_r1 = 0;
       return STATE_AMS_IDLE;
   }
   if(ams_inputs.error_u8) {
       ams_outputs.enable_precharge_u8=0;
       ams_outputs.enable_AIR_plus_u8=0;
       ams_outputs.enable_AIR_minus_u8=0;
       timer_r1 = 0;
       return STATE_AMS_ERROR;
   }
   timer_r1 += ams_parameters.Ts_f32;
   return STATE_AMS_DRIVE;
}

ams_state_t ams_error_function() {
   if(1) {
       ams_outputs.enable_precharge_u8=0;
       ams_outputs.enable_AIR_plus_u8=0;
       ams_outputs.enable_AIR_minus_u8=0;
       timer_r1 = 0;
   }
   if(!ams_inputs.error_u8) {
       timer_r1 = 0;
       return STATE_AMS_IDLE;
   }
   timer_r1 += ams_parameters.Ts_f32;
   return STATE_AMS_ERROR;
}

ams_state_t ams_precharge_charge_0_function() {
   if(1) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_START_CHARGER;
   }
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_CHARGE_0;
}

ams_state_t ams_precharge_charge_start_charger_function() {
   if((timer_r2 < 10)&&(ams_inputs.charger_is_live_u8)) {
       ams_outputs.enable_AIR_minus_u8=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_MINUS;
   }
   if((timer_r2 > 10)) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_CHARGE_START_CHARGER;
}

ams_state_t ams_precharge_charge_close_air_minus_function() {
   if((timer_r2 < 2)&&(ams_inputs.AIR_minus_closed_u8==1)) {
       ams_outputs.enable_precharge_u8=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_CLOSE_PRECHARGE;
   }
   if((timer_r2 > 2)) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_MINUS;
}

ams_state_t ams_precharge_charge_error_function() {
   if(1) {
       ams_parameters.precharge_charge_error_u8=1;
       timer_r2 = 0;
   }
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_CHARGE_ERROR;
}

ams_state_t ams_precharge_charge_close_precharge_function() {
   if((timer_r2 < 2)&&(ams_inputs.precharge_closed_u8==1)) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_WAIT;
   }
   if((timer_r2 > 2)) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_CHARGE_CLOSE_PRECHARGE;
}

ams_state_t ams_precharge_charge_wait_function() {
   if((timer_r2 > 2)&&(timer_r2 < 25)&&(0.95*ams_inputs.accumulator_voltage_f64<ams_inputs.vehicle_voltage_f64)) {
       ams_outputs.enable_AIR_plus_u8=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_PLUS;
   }
   if((timer_r2 > 25)) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_CHARGE_WAIT;
}

ams_state_t ams_precharge_charge_close_air_plus_function() {
   if((timer_r2 < 2)&&(ams_inputs.AIR_plus_closed_u8==1)) {
       ams_outputs.enable_precharge_u8=0;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_OPEN_PRECHARGE;
   }
   if((timer_r2 > 2)) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_PLUS;
}

ams_state_t ams_precharge_charge_open_precharge_function() {
   if((timer_r2 < 2)&&(ams_inputs.precharge_closed_u8==1)) {
       ams_parameters.precharge_charge_complete_u8=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_EXIT;
   }
   if((timer_r2 > 2)) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_CHARGE_OPEN_PRECHARGE;
}

ams_state_t ams_precharge_charge_exit_function() {
   timer_r2 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_CHARGE_EXIT;
}

ams_state_t ams_precharge_charge_function() {
   if(timer_r1 < 0.001*Ts) {
       state_r1 = STATE_AMS_PRECHARGE_CHARGE_0;
   }

   switch(state_r2) {
       case STATE_AMS_PRECHARGE_CHARGE_0:
           state_r2 = ams_precharge_charge_0_function();
           break;
       case STATE_AMS_PRECHARGE_CHARGE_START_CHARGER:
           state_r2 = ams_precharge_charge_start_charger_function();
           break;
       case STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_MINUS:
           state_r2 = ams_precharge_charge_close_air_minus_function();
           break;
       case STATE_AMS_PRECHARGE_CHARGE_ERROR:
           state_r2 = ams_precharge_charge_error_function();
           break;
       case STATE_AMS_PRECHARGE_CHARGE_CLOSE_PRECHARGE:
           state_r2 = ams_precharge_charge_close_precharge_function();
           break;
       case STATE_AMS_PRECHARGE_CHARGE_WAIT:
           state_r2 = ams_precharge_charge_wait_function();
           break;
       case STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_PLUS:
           state_r2 = ams_precharge_charge_close_air_plus_function();
           break;
       case STATE_AMS_PRECHARGE_CHARGE_OPEN_PRECHARGE:
           state_r2 = ams_precharge_charge_open_precharge_function();
           break;
       case STATE_AMS_PRECHARGE_CHARGE_EXIT:
           state_r2 = ams_precharge_charge_exit_function();
           break;
   }

   if(1) {
       timer_r1 = 0;
   }
   if(ams_parameters.precharge_charge_complete_u8) {
       timer_r1 = 0;
       return STATE_AMS_CHARGE;
   }
   timer_r1 += ams_parameters.Ts_f32;
   return STATE_AMS_PRECHARGE_CHARGE;
}

ams_state_t ams_charge_function() {
   if((ams_inputs.charge_u8==0)||(0.995*ams_inputs.sought_voltage_f64<ams_inputs.accumulator_voltage_f64)) {
       ams_outputs.enable_charger_u8=0;
       ams_outputs.enable_precharge_u8=0;
       ams_outputs.enable_AIR_plus_u8=0;
       ams_outputs.enable_AIR_minus_u8=0;
       timer_r1 = 0;
       return STATE_AMS_IDLE;
   }
   if(ams_inputs.error_u8||ams_parameters.precharge_charge_error_u8) {
       ams_outputs.enable_precharge_u8=0;
       ams_outputs.enable_AIR_plus_u8=0;
       ams_outputs.enable_AIR_minus_u8=0;
       timer_r1 = 0;
       return STATE_AMS_ERROR;
   }
   timer_r1 += ams_parameters.Ts_f32;
   return STATE_AMS_CHARGE;
}

ams_state_t ams_function() {
   if(timer_r0 < 0.001*Ts) {
       state_r0 = STATE_AMS_0;
   }

   switch(state_r2) {
       case STATE_AMS_0:
           state_r2 = ams_0_function();
           break;
       case STATE_AMS_ENTRY:
           state_r2 = ams_entry_function();
           break;
       case STATE_AMS_IDLE:
           state_r2 = ams_idle_function();
           break;
       case STATE_AMS_BALANCE:
           state_r2 = ams_balance_function();
           break;
       case STATE_AMS_PRECHARGE_DRIVE:
           state_r2 = ams_precharge_drive_function();
           break;
       case STATE_AMS_DRIVE:
           state_r2 = ams_drive_function();
           break;
       case STATE_AMS_ERROR:
           state_r2 = ams_error_function();
           break;
       case STATE_AMS_PRECHARGE_CHARGE:
           state_r2 = ams_precharge_charge_function();
           break;
       case STATE_AMS_CHARGE:
           state_r2 = ams_charge_function();
           break;
   }

   timer_r0 += ams_parameters.Ts_f32;
   return STATE_AMS;
}

