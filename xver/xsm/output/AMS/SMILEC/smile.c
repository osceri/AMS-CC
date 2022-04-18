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


__weak uint8_t end_cell_balancing() {
   /* Type your actual code somewhere else */
}

__weak uint8_t do_cell_balancing() {
   /* Type your actual code somewhere else */
}

__weak void ams_error() {
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
       ams_outputs.air_minus_closed_s=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_CLOSE_PRECHARGE;
   }

   if((timer_r2 > 2)) {
       ams_outputs.error=111;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ERROR;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_MINUS;
}

ams_state_t ams_precharge_drive_close_precharge_function() {
   if((timer_r2 < 2)&&ams_inputs.precharge_closed) {
       ams_outputs.precharge_closed_s=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_WAIT;
   }

   if((timer_r2 > 2)) {
       ams_outputs.error=112;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ERROR;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_CLOSE_PRECHARGE;
}

ams_state_t ams_precharge_drive_error_function() {
   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_ERROR;
}

ams_state_t ams_precharge_drive_wait_function() {
   if((timer_r2 > 5)&&(timer_r2 < 30)&&(ams_inputs.accumulator_voltage*0.95<ams_inputs.vehicle_voltage)) {
       ams_outputs.close_air_plus=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_PLUS;
   }

   if((timer_r2 > 30)) {
       ams_outputs.error=113;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ERROR;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_WAIT;
}

ams_state_t ams_precharge_drive_close_air_plus_function() {
   if((timer_r2 < 2)&&ams_inputs.air_plus_closed) {
       ams_outputs.close_precharge=0;
       ams_outputs.air_plus_closed_s=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_OPEN_PRECHARGE;
   }

   if((timer_r2 > 2)) {
       ams_outputs.error=114;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ERROR;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_PLUS;
}

ams_state_t ams_precharge_drive_open_precharge_function() {
   if((timer_r2 > 2)) {
       ams_outputs.error=115;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ERROR;
   }

   if((timer_r2 < 2)&&!ams_inputs.precharge_closed) {
       ams_outputs.precharge_drive_complete=1;
       ams_outputs.precharge_closed_s=0;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_EXIT;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_OPEN_PRECHARGE;
}

ams_state_t ams_precharge_drive_exit_function() {
   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_EXIT;
}

ams_state_t ams_precharge_drive_0_function() {
   if(1) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_ENTRY;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_0;
}

ams_state_t ams_precharge_drive_entry_function() {
   if(1) {
       ams_outputs.close_air_minus=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_DRIVE_CLOSE_AIR_MINUS;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_DRIVE_ENTRY;
}

ams_state_t ams_precharge_drive_function() {
   if(timer_r1 < 0.001*ams_inputs.Ts) {
       state_r2 = STATE_AMS_PRECHARGE_DRIVE_0;
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
       default:
           ams_error();
           break;
   }

   if(ams_outputs.precharge_drive_complete) {
       ams_outputs.precharge_drive_complete=0;
       timer_r1 = 0;
       return STATE_AMS_DRIVE;
   }

   if(ams_outputs.error||ams_inputs.ams_error||ams_inputs.imd_error) {
       timer_r1 = 0;
       return STATE_AMS_ERROR;
   }


   timer_r1 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_DRIVE;
}

ams_state_t ams_drive_0_function() {
   if(1) {
       timer_r2 = 0;
       return STATE_AMS_DRIVE_DRIVE;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_DRIVE_0;
}

ams_state_t ams_drive_drive_function() {
   if((ams_inputs.minimum_cell_voltage<2.85)||!ams_inputs.drive) {
       timer_r2 = 0;
       return STATE_AMS_DRIVE_END_DRIVE;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_DRIVE_DRIVE;
}

ams_state_t ams_drive_end_drive_function() {
   if((timer_r2 < 7)&&(fabsf(ams_inputs.accumulator_current)<0.001)) {
       ams_outputs.close_air_plus=0;
       ams_outputs.air_plus_closed_s=0;
       ams_outputs.close_air_minus=0;
       ams_outputs.air_minus_closed_s=0;
       ams_outputs.close_precharge=0;
       ams_outputs.precharge_closed_s=0;
       ams_outputs.drive_complete=1;
       timer_r2 = 0;
       return STATE_AMS_DRIVE_EXIT;
   }

   if((timer_r2 > 7)) {
       ams_outputs.close_air_plus=0;
       ams_outputs.air_plus_closed_s=0;
       ams_outputs.close_air_minus=0;
       ams_outputs.air_minus_closed_s=0;
       ams_outputs.close_precharge=0;
       ams_outputs.precharge_closed_s=0;
       ams_outputs.error=131;
       timer_r2 = 0;
       return STATE_AMS_DRIVE_FORCE_QUIT;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_DRIVE_END_DRIVE;
}

ams_state_t ams_drive_exit_function() {
   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_DRIVE_EXIT;
}

ams_state_t ams_drive_force_quit_function() {
   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_DRIVE_FORCE_QUIT;
}

ams_state_t ams_drive_function() {
   if(timer_r1 < 0.001*ams_inputs.Ts) {
       state_r2 = STATE_AMS_DRIVE_0;
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
       default:
           ams_error();
           break;
   }

   if(ams_inputs.ams_error||ams_inputs.imd_error||ams_outputs.error) {
       timer_r1 = 0;
       return STATE_AMS_ERROR;
   }

   if(ams_outputs.drive_complete) {
       ams_outputs.drive_complete=0;
       timer_r1 = 0;
       return STATE_AMS_IDLE;
   }


   timer_r1 += ams_inputs.Ts;
   return STATE_AMS_DRIVE;
}

ams_state_t ams_error_0_function() {
   if(1) {
       timer_r2 = 0;
       return STATE_AMS_ERROR_ERROR_DETECTED;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_ERROR_0;
}

ams_state_t ams_error_error_detected_function() {
   if(1) {
       ams_outputs.close_air_plus=0;
       ams_outputs.air_plus_closed_s=0;
       ams_outputs.close_air_minus=0;
       ams_outputs.air_minus_closed_s=0;
       ams_outputs.close_precharge=0;
       ams_outputs.precharge_closed_s=0;
       timer_r2 = 0;
       return STATE_AMS_ERROR_RELAYS_OPENED;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_ERROR_ERROR_DETECTED;
}

ams_state_t ams_error_relays_opened_function() {
   if((timer_r2 > 5)) {
       ams_outputs.error=0;
       timer_r2 = 0;
       return STATE_AMS_ERROR_AWAITING_ERROR_RESOLUTION;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_ERROR_RELAYS_OPENED;
}

ams_state_t ams_error_awaiting_error_resolution_function() {
   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_ERROR_AWAITING_ERROR_RESOLUTION;
}

ams_state_t ams_error_function() {
   if(timer_r1 < 0.001*ams_inputs.Ts) {
       state_r2 = STATE_AMS_ERROR_0;
   }

   switch(state_r2) {
       case STATE_AMS_ERROR_0:
           state_r2 = ams_error_0_function();
           break;
       case STATE_AMS_ERROR_ERROR_DETECTED:
           state_r2 = ams_error_error_detected_function();
           break;
       case STATE_AMS_ERROR_RELAYS_OPENED:
           state_r2 = ams_error_relays_opened_function();
           break;
       case STATE_AMS_ERROR_AWAITING_ERROR_RESOLUTION:
           state_r2 = ams_error_awaiting_error_resolution_function();
           break;
       default:
           ams_error();
           break;
   }

   if(!(ams_outputs.error||ams_inputs.ams_error||ams_inputs.imd_error)) {
       timer_r1 = 0;
       return STATE_AMS_IDLE;
   }


   timer_r1 += ams_inputs.Ts;
   return STATE_AMS_ERROR;
}

ams_state_t ams_precharge_charge_close_air_minus_function() {
   if((timer_r2 < 2)&&ams_inputs.air_minus_closed) {
       ams_outputs.close_precharge=1;
       ams_outputs.air_minus_closed_s=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_CLOSE_PRECHARGE;
   }

   if((timer_r2 > 2)) {
       ams_outputs.error=122;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_MINUS;
}

ams_state_t ams_precharge_charge_close_precharge_function() {
   if((timer_r2 < 2)&&ams_inputs.precharge_closed) {
       ams_outputs.precharge_closed_s=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_WAIT;
   }

   if((timer_r2 > 2)) {
       ams_outputs.error=123;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_CLOSE_PRECHARGE;
}

ams_state_t ams_precharge_charge_error_function() {
   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_ERROR;
}

ams_state_t ams_precharge_charge_wait_function() {
   if((timer_r2 > 5)&&(timer_r2 < 30)&&(ams_inputs.accumulator_voltage*0.95<ams_inputs.vehicle_voltage)) {
       ams_outputs.close_air_plus=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_PLUS;
   }

   if((timer_r2 > 30)) {
       ams_outputs.error=124;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_WAIT;
}

ams_state_t ams_precharge_charge_close_air_plus_function() {
   if((timer_r2 < 2)&&ams_inputs.air_plus_closed) {
       ams_outputs.close_precharge=0;
       ams_outputs.air_plus_closed_s=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_OPEN_PRECHARGE;
   }

   if((timer_r2 > 2)) {
       ams_outputs.error=125;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_PLUS;
}

ams_state_t ams_precharge_charge_open_precharge_function() {
   if((timer_r2 > 2)) {
       ams_outputs.error=126;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }

   if((timer_r2 < 2)&&!ams_inputs.precharge_closed) {
       ams_outputs.precharge_charge_complete=1;
       ams_outputs.precharge_closed_s=0;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_EXIT;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_OPEN_PRECHARGE;
}

ams_state_t ams_precharge_charge_exit_function() {
   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_EXIT;
}

ams_state_t ams_precharge_charge_0_function() {
   if(1) {
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_WAKE_CHARGER;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_0;
}

ams_state_t ams_precharge_charge_wake_charger_function() {
   if((timer_r2 < 10)&&ams_inputs.charger_is_awake) {
       ams_outputs.close_air_minus=1;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_CLOSE_AIR_MINUS;
   }

   if((timer_r2 > 10)) {
       ams_outputs.error=121;
       timer_r2 = 0;
       return STATE_AMS_PRECHARGE_CHARGE_ERROR;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_CHARGE_WAKE_CHARGER;
}

ams_state_t ams_precharge_charge_function() {
   if(timer_r1 < 0.001*ams_inputs.Ts) {
       state_r2 = STATE_AMS_PRECHARGE_CHARGE_0;
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
       default:
           ams_error();
           break;
   }

   if(ams_outputs.precharge_charge_complete) {
       ams_outputs.precharge_charge_complete=0;
       timer_r1 = 0;
       return STATE_AMS_CHARGE;
   }

   if(ams_outputs.error||ams_inputs.ams_error||ams_inputs.imd_error) {
       timer_r1 = 0;
       return STATE_AMS_ERROR;
   }


   timer_r1 += ams_inputs.Ts;
   return STATE_AMS_PRECHARGE_CHARGE;
}

ams_state_t ams_charge_0_function() {
   if(1) {
       timer_r2 = 0;
       return STATE_AMS_CHARGE_CHARGE;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_CHARGE_0;
}

ams_state_t ams_charge_charge_function() {
   if((ams_inputs.maximum_cell_voltage>4.15)||!ams_inputs.charge) {
       ams_outputs.enable_charger=0;
       timer_r2 = 0;
       return STATE_AMS_CHARGE_END_CHARGE;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_CHARGE_CHARGE;
}

ams_state_t ams_charge_end_charge_function() {
   if((timer_r2 < 7)&&(fabsf(ams_inputs.accumulator_current)<0.001)) {
       ams_outputs.close_air_plus=0;
       ams_outputs.air_plus_closed_s=0;
       ams_outputs.close_air_minus=0;
       ams_outputs.air_minus_closed_s=0;
       ams_outputs.close_precharge=0;
       ams_outputs.precharge_closed_s=0;
       ams_outputs.charge_complete=1;
       timer_r2 = 0;
       return STATE_AMS_CHARGE_EXIT;
   }

   if((timer_r2 > 7)) {
       ams_outputs.close_air_plus=0;
       ams_outputs.air_plus_closed_s=0;
       ams_outputs.close_air_minus=0;
       ams_outputs.air_minus_closed_s=0;
       ams_outputs.close_precharge=0;
       ams_outputs.precharge_closed_s=0;
       ams_outputs.error=141;
       timer_r2 = 0;
       return STATE_AMS_CHARGE_FORCE_QUIT;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_CHARGE_END_CHARGE;
}

ams_state_t ams_charge_exit_function() {
   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_CHARGE_EXIT;
}

ams_state_t ams_charge_force_quit_function() {
   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_CHARGE_FORCE_QUIT;
}

ams_state_t ams_charge_function() {
   if(timer_r1 < 0.001*ams_inputs.Ts) {
       state_r2 = STATE_AMS_CHARGE_0;
   }

   switch(state_r2) {
       case STATE_AMS_CHARGE_0:
           state_r2 = ams_charge_0_function();
           break;
       case STATE_AMS_CHARGE_CHARGE:
           state_r2 = ams_charge_charge_function();
           break;
       case STATE_AMS_CHARGE_END_CHARGE:
           state_r2 = ams_charge_end_charge_function();
           break;
       case STATE_AMS_CHARGE_EXIT:
           state_r2 = ams_charge_exit_function();
           break;
       case STATE_AMS_CHARGE_FORCE_QUIT:
           state_r2 = ams_charge_force_quit_function();
           break;
       default:
           ams_error();
           break;
   }

   if(ams_inputs.ams_error||ams_inputs.imd_error||ams_outputs.error) {
       timer_r1 = 0;
       return STATE_AMS_ERROR;
   }

   if(ams_outputs.charge_complete) {
       ams_outputs.charge_complete=0;
       ams_outputs.enable_charger=0;
       timer_r1 = 0;
       return STATE_AMS_IDLE;
   }


   timer_r1 += ams_inputs.Ts;
   return STATE_AMS_CHARGE;
}

ams_state_t ams_idle_function() {
   if(ams_inputs.SC&&ams_inputs.drive) {
       timer_r1 = 0;
       return STATE_AMS_PRECHARGE_DRIVE;
   }

   if(ams_inputs.SC&&ams_inputs.charge) {
       ams_outputs.enable_charger=1;
       timer_r1 = 0;
       return STATE_AMS_PRECHARGE_CHARGE;
   }

   if(!ams_inputs.SC&&ams_inputs.balance&&(ams_inputs.cell_voltages_variance>0.01)) {
       timer_r1 = 0;
       return STATE_AMS_BALANCE;
   }

   timer_r1 += ams_inputs.Ts;
   return STATE_AMS_IDLE;
}

ams_state_t ams_balance_0_function() {
   if(1) {
       timer_r2 = 0;
       return STATE_AMS_BALANCE_BALANCE;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_BALANCE_0;
}

ams_state_t ams_balance_balance_function() {
   if(1) {
       do_cell_balancing();
       timer_r2 = 0;
       return STATE_AMS_BALANCE_WAIT;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_BALANCE_BALANCE;
}

ams_state_t ams_balance_wait_function() {
   if((timer_r2 > 60)) {
       timer_r2 = 0;
       return STATE_AMS_BALANCE_BALANCE;
   }

   timer_r2 += ams_inputs.Ts;
   return STATE_AMS_BALANCE_WAIT;
}

ams_state_t ams_balance_function() {
   if(timer_r1 < 0.001*ams_inputs.Ts) {
       state_r2 = STATE_AMS_BALANCE_0;
   }

   switch(state_r2) {
       case STATE_AMS_BALANCE_0:
           state_r2 = ams_balance_0_function();
           break;
       case STATE_AMS_BALANCE_BALANCE:
           state_r2 = ams_balance_balance_function();
           break;
       case STATE_AMS_BALANCE_WAIT:
           state_r2 = ams_balance_wait_function();
           break;
       default:
           ams_error();
           break;
   }

   if(ams_inputs.SC||(!ams_inputs.balance)||(ams_inputs.cell_voltages_variance<0.005)) {
       end_cell_balancing();
       timer_r1 = 0;
       return STATE_AMS_IDLE;
   }


   timer_r1 += ams_inputs.Ts;
   return STATE_AMS_BALANCE;
}

ams_state_t ams_0_function() {
   if(1) {
       timer_r1 = 0;
       return STATE_AMS_IDLE;
   }

   timer_r1 += ams_inputs.Ts;
   return STATE_AMS_0;
}

ams_state_t ams_function() {
   if(timer_r0 < 0.001*ams_inputs.Ts) {
       state_r1 = STATE_AMS_0;
   }

   switch(state_r1) {
       case STATE_AMS_PRECHARGE_DRIVE:
           state_r1 = ams_precharge_drive_function();
           break;
       case STATE_AMS_DRIVE:
           state_r1 = ams_drive_function();
           break;
       case STATE_AMS_ERROR:
           state_r1 = ams_error_function();
           break;
       case STATE_AMS_PRECHARGE_CHARGE:
           state_r1 = ams_precharge_charge_function();
           break;
       case STATE_AMS_CHARGE:
           state_r1 = ams_charge_function();
           break;
       case STATE_AMS_IDLE:
           state_r1 = ams_idle_function();
           break;
       case STATE_AMS_BALANCE:
           state_r1 = ams_balance_function();
           break;
       case STATE_AMS_0:
           state_r1 = ams_0_function();
           break;
       default:
           ams_error();
           break;
   }

   timer_r0 += ams_inputs.Ts;
   return STATE_AMS;
}

