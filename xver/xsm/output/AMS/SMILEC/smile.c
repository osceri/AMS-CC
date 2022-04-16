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


__weak uint8_t zero_cells() {
   /* Type your actual code somewhere else */
}

__weak uint8_t balance_cells() {
   /* Type your actual code somewhere else */
}

static ams_state_t state_r0;
static ams_state_t state_r1;
static ams_state_t state_r2;
static float timer_r0;
static float timer_r1;
static float timer_r2;

ams_state_t ams_precharge_drive_close_air_minus_function() {
   if((timer_r2 < 2)&&ams_inputs.air_minus_closed) {
       ams_outputs.close_precharge=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_CLOSE_PRECHARGE;
   }

   if((timer_r2 > 2)) {
       ams_parameters.precharge_drive_error=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ERROR;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_MINUS;
}

ams_state_t ams_precharge_drive_close_precharge_function() {
   if((timer_r2 < 2)&&ams_inputs.precharge_closed) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_WAIT;
   }

   if((timer_r2 > 2)) {
       ams_parameters.precharge_drive_error=2;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ERROR;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_CLOSE_PRECHARGE;
}

ams_state_t ams_precharge_drive_error_function() {
   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_ERROR;
}

ams_state_t ams_precharge_drive_wait_function() {
   if((timer_r2 > 5)&&(timer_r2 < 30)&&(ams_inputs.accumulator_voltage*0.95<ams_inputs.vehicle_voltage)) {
       ams_outputs.close_air_plus=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_PLUS;
   }

   if((timer_r2 > 2)) {
       ams_parameters.precharge_drive_error=3;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ERROR;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_WAIT;
}

ams_state_t ams_precharge_drive_close_air_plus_function() {
   if((timer_r2 < 2)&&ams_inputs.air_plus_closed) {
       ams_outputs.close_precharge=0;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_OPEN_PRECHARGE;
   }

   if((timer_r2 > 30)) {
       ams_parameters.precharge_drive_error=4;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ERROR;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_PLUS;
}

ams_state_t ams_precharge_drive_open_precharge_function() {
   if((timer_r2 > 2)) {
       ams_parameters.precharge_drive_error=5;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ERROR;
   }

   if((timer_r2 < 2)&&!ams_inputs.precharge_closed) {
       ams_parameters.precharge_drive_complete=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_EXIT;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_OPEN_PRECHARGE;
}

ams_state_t ams_precharge_drive_exit_function() {
   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_EXIT;
}

ams_state_t ams_precharge_drive_0_function() {
   if(1) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ENTRY;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_0;
}

ams_state_t ams_precharge_drive_entry_function() {
   if(1) {
       ams_outputs.close_air_minus=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_MINUS;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_ENTRY;
}

ams_state_t ams_precharge_drive_function() {
   if(timer_r1 < 0.001*ams_parameters.Ts) {
       state_r1 = STATE_AMS_PRECHARGE_DRIVE_0;
   }

   switch(state_r2) {
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
       case STATE_AMS_PRECHARGE_DRIVE_0:
           state_r2 = ams_precharge_drive_0_function();
           break;
       case STATE_AMS_PRECHARGE_DRIVE_ENTRY:
           state_r2 = ams_precharge_drive_entry_function();
           break;
   }

   if(ams_parameters.precharge_drive_complete) {
       timer_r1 = 0;
       return STATE_AMS_DRIVE;
   }

   if(ams_parameters.precharge_drive_error||ams_inputs.ams_error||ams_inputs.imd_error) {
       ams_outputs.error=1;
       timer_r1 = 0;
       return STATE_AMS_ERROR;
   }

   if(1) {
       timer_r1 = 0;
   }

   timer_r1 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_DRIVE;
}

ams_state_t ams_drive_0_function() {
   if(1) {
       timer_r2 = 0;
       return STATE_AMS_DRIVE_DRIVE;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_DRIVE_0;
}

ams_state_t ams_drive_drive_function() {
   if(!ams_inputs.drive) {
       timer_r2 = 0;
       return STATE_AMS_DRIVE_END_DRIVE;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_DRIVE_DRIVE;
}

ams_state_t ams_drive_end_drive_function() {
   if((timer_r2 < 7)&&(abs(ams_inputs.accumulator_current)<0.001)) {
       ams_outputs.close_air_plus=0;
       ams_outputs.close_air_minus=0;
       ams_outputs.close_precharge=0;
       ams_parameters.drive_complete=1;
       timer_r2 = 0;
       return STATE_AMS_DRIVE_EXIT;
   }

   if((timer_r2 > 7)) {
       ams_outputs.close_air_plus=0;
       ams_outputs.close_air_minus=0;
       ams_outputs.close_precharge=0;
       ams_outputs.drive_force_quit=1;
       timer_r2 = 0;
       return STATE_AMS_DRIVE_FORCE_QUIT;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_DRIVE_END_DRIVE;
}

ams_state_t ams_drive_exit_function() {
   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_DRIVE_EXIT;
}

ams_state_t ams_drive_force_quit_function() {
   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_DRIVE_FORCE_QUIT;
}

ams_state_t ams_drive_function() {
   if(timer_r1 < 0.001*ams_parameters.Ts) {
       state_r1 = STATE_AMS_DRIVE_0;
   }

   switch(state_r2) {
       case STATE_AMS_DRIVE_0:
           state_r2 = ams_drive_0_function();
           break;
       case STATE_AMS_DRIVE_DRIVE:
           state_r2 = ams_drive_drive_function();
           break;
       case STATE_AMS_DRIVE_END_DRIVE:
           state_r2 = ams_drive_end_drive_function();
           break;
       case STATE_AMS_DRIVE_EXIT:
           state_r2 = ams_drive_exit_function();
           break;
       case STATE_AMS_DRIVE_FORCE_QUIT:
           state_r2 = ams_drive_force_quit_function();
           break;
   }

   if(ams_inputs.ams_error||ams_inputs.imd_error||ams_inputs.drive_error) {
       ams_outputs.error=1;
       timer_r1 = 0;
       return STATE_AMS_ERROR;
   }

   if(ams_parameters.drive_complete) {
       timer_r1 = 0;
       return STATE_AMS_IDLE;
   }

   if(1) {
       timer_r1 = 0;
   }

   timer_r1 += ams_parameters.Ts;
   return STATE_AMS_DRIVE;
}

ams_state_t ams_error_function() {
   if(!(ams_inputs.imd_error|ams_inputs.ams_error|ams_parameters.precharge_drive_error|ams_parameters.precharge_charge_error)) {
       timer_r1 = 0;
       return STATE_AMS_IDLE;
   }

   timer_r1 += ams_parameters.Ts;
   return STATE_AMS_ERROR;
}

ams_state_t ams_precharge_charge_close_air_minus_function() {
   if((timer_r2 < 2)&&ams_inputs.air_minus_closed) {
       ams_outputs.close_precharge=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_CLOSE_PRECHARGE;
   }

   if((timer_r2 > 2)) {
       ams_parameters.precharge_charge_error=2;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_MINUS;
}

ams_state_t ams_precharge_charge_close_precharge_function() {
   if((timer_r2 < 2)&&ams_inputs.precharge_closed) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_WAIT;
   }

   if((timer_r2 > 2)) {
       ams_parameters.precharge_charge_error=3;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_CLOSE_PRECHARGE;
}

ams_state_t ams_precharge_charge_error_function() {
   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_ERROR;
}

ams_state_t ams_precharge_charge_wait_function() {
   if((timer_r2 > 5)&&(timer_r2 < 30)&&(ams_inputs.accumulator_voltage*0.95<ams_inputs.vehicle_voltage)) {
       ams_outputs.close_air_plus=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_PLUS;
   }

   if((timer_r2 > 2)) {
       ams_parameters.precharge_charge_error=4;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_WAIT;
}

ams_state_t ams_precharge_charge_close_air_plus_function() {
   if((timer_r2 < 2)&&ams_inputs.air_plus_closed) {
       ams_outputs.close_precharge=0;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_OPEN_PRECHARGE;
   }

   if((timer_r2 > 30)) {
       ams_parameters.precharge_charge_error=5;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_PLUS;
}

ams_state_t ams_precharge_charge_open_precharge_function() {
   if((timer_r2 > 2)) {
       ams_parameters.precharge_charge_error=6;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }

   if((timer_r2 < 2)&&!ams_inputs.precharge_closed) {
       ams_parameters.precharge_charge_complete=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_EXIT;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_OPEN_PRECHARGE;
}

ams_state_t ams_precharge_charge_exit_function() {
   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_EXIT;
}

ams_state_t ams_precharge_charge_0_function() {
   if(1) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_WAKE_CHARGER;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_0;
}

ams_state_t ams_precharge_charge_wake_charger_function() {
   if((timer_r2 < 10)&&ams_inputs.charger_is_awake) {
       ams_outputs.close_air_minus=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_MINUS;
   }

   if((timer_r2 > 10)) {
       ams_parameters.precharge_charge_error=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }

   timer_r2 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_WAKE_CHARGER;
}

ams_state_t ams_precharge_charge_function() {
   if(timer_r1 < 0.001*ams_parameters.Ts) {
       state_r1 = STATE_AMS_PRECHARGE_CHARGE_0;
   }

   switch(state_r2) {
       case STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_MINUS:
           state_r2 = ams_precharge_charge_close_air_minus_function();
           break;
       case STATE_AMS_PRECHARGE_CHARGE_CLOSE_PRECHARGE:
           state_r2 = ams_precharge_charge_close_precharge_function();
           break;
       case STATE_AMS_PRECHARGE_CHARGE_ERROR:
           state_r2 = ams_precharge_charge_error_function();
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
       case STATE_AMS_PRECHARGE_CHARGE_0:
           state_r2 = ams_precharge_charge_0_function();
           break;
       case STATE_AMS_PRECHARGE_CHARGE_WAKE_CHARGER:
           state_r2 = ams_precharge_charge_wake_charger_function();
           break;
   }

   if(ams_parameters.precharge_charge_complete) {
       timer_r1 = 0;
       return STATE_AMS_CHARGE;
   }

   if(ams_parameters.precharge_charge_error||ams_inputs.ams_error||ams_inputs.imd_error) {
       ams_outputs.error=1;
       timer_r1 = 0;
       return STATE_AMS_ERROR;
   }

   if(1) {
       timer_r1 = 0;
   }

   timer_r1 += ams_parameters.Ts;
   return STATE_AMS_PRECHARGE_CHARGE;
}

ams_state_t ams_charge_function() {
   if(ams_inputs.ams_error||ams_inputs.imd_error||ams_inputs.charge_error) {
       ams_outputs.error=1;
       timer_r1 = 0;
       return STATE_AMS_ERROR;
   }

   if(ams_inputs.charge_complete) {
       timer_r1 = 0;
       return STATE_AMS_IDLE;
   }

   if(1) {
       timer_r1 = 0;
   }

   timer_r1 += ams_parameters.Ts;
   return STATE_AMS_CHARGE;
}

ams_state_t ams_idle_function() {
   if(ams_inputs.SC&&ams_inputs.drive) {
       timer_r1 = 0;
       return STATE_AMS_PRECHARGE_DRIVE;
   }

   if(ams_inputs.SC&&ams_inputs.charge) {
       timer_r1 = 0;
       return STATE_AMS_PRECHARGE_CHARGE;
   }

   if(!ams_inputs.SC&&ams_inputs.balance) {
       timer_r1 = 0;
   }

   if(ams_inputs.SC||!ams_inputs.balance||(ams_inputs.cell_voltage_variance<0.00001)) {
       zero_cells();
       timer_r1 = 0;
   }

   if((timer_r1 > 60)) {
       balance_cells();
       timer_r1 = 0;
   }

   timer_r1 += ams_parameters.Ts;
   return STATE_AMS_IDLE;
}

ams_state_t ams_0_function() {
   if(1) {
       timer_r1 = 0;
       return STATE_AMS_IDLE;
   }

   timer_r1 += ams_parameters.Ts;
   return STATE_AMS_0;
}

ams_state_t ams_function() {
   if(timer_r0 < 0.001*ams_parameters.Ts) {
       state_r0 = STATE_AMS_0;
   }

   switch(state_r2) {
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
       case STATE_AMS_IDLE:
           state_r2 = ams_idle_function();
           break;
       case STATE_AMS_0:
           state_r2 = ams_0_function();
           break;
   }

   timer_r0 += ams_parameters.Ts;
   return STATE_AMS;
}

