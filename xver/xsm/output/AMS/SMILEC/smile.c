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


uint8_t after(uint32_t* timer, float boundary) {
   return !(timer[0]*ams_parameters.Ts_f32 < boundary);
}

uint8_t before(uint32_t* timer, float boundary) {
   return (timer[0]*ams_parameters.Ts_f32 < boundary);
}

__weak void do_balance_v0() {
   /* Type your actual code somewhere else */
}
__weak void zero_out_balance_v0() {
   /* Type your actual code somewhere else */
}
void ams_0(uint32_t* super_timer, uint8_t* super_state) {
   if(1) {
       ams_outputs.balances_126u8;
       super_timer[0] = 0;
       super_state[0] = AMS_ENTRY_STATE;
   }

}
void ams_entry(uint32_t* super_timer, uint8_t* super_state) {
   static uint32_t timer = 0;
   timer++;
   if (super_timer[0] == 0) {
       timer = 0;
   }

   if(1) {
       ams_parameters.precharge_drive_complete_u8=0;
       ams_parameters.precharge_charge_complete_u8=0;
       zero_out_balance_v0();
       ams_outputs.enable_charge_u8=0;
       ams_outputs.enable_precharge_u8=0;
       ams_outputs.enable_AIR_plus_u8=0;
       ams_outputs.enable_AIR_minus_u8=0;
   }
   if(after(&timer,5)) {
       super_timer[0] = 0;
       super_state[0] = AMS_IDLE_STATE;
   }

}
void ams_idle(uint32_t* super_timer, uint8_t* super_state) {
   if(ams_inputs.SC_u8&&ams_inputs.balance_u8) {
       super_timer[0] = 0;
       super_state[0] = AMS_BALANCE_STATE;
   }
   if(ams_inputs.SC_u8&&ams_inputs.drive_u8) {
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_DRIVE_STATE;
   }
   if(ams_inputs.SC_u8&&ams_inputs.charge_u8) {
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_CHARGE_STATE;
   }

}
void ams_balance(uint32_t* super_timer, uint8_t* super_state) {
   static uint32_t timer = 0;
   timer++;
   if (super_timer[0] == 0) {
       timer = 0;
   }

   if(after(&timer,60)) {
       do_balance_v0();
   }
   if(ams_inputs.U_variance_f64<0.00001) {
       zero_out_balance_v0();
       super_timer[0] = 0;
       super_state[0] = AMS_IDLE_STATE;
   }

}
void ams_precharge_drive_0(uint32_t* super_timer, uint8_t* super_state) {
   if(1) {
       ams_outputs.enable_AIR_minus_u8=1;
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_DRIVE_CLOSE_AIR_MINUS_STATE;
   }

}
void ams_precharge_drive_close_air_minus(uint32_t* super_timer, uint8_t* super_state) {
   static uint32_t timer = 0;
   timer++;
   if (super_timer[0] == 0) {
       timer = 0;
   }

   if(before(&timer,2)&&(ams_inputs.AIR_minus_closed_u8==1)) {
       ams_outputs.enable_precharge_u8=1;
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_DRIVE_CLOSE_PRECHARGE_STATE;
   }
   if(after(&timer,2)) {
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_DRIVE_ERROR_STATE;
   }

}
void ams_precharge_drive_close_precharge(uint32_t* super_timer, uint8_t* super_state) {
   static uint32_t timer = 0;
   timer++;
   if (super_timer[0] == 0) {
       timer = 0;
   }

   if(before(&timer,2)&&(ams_inputs.precharge_closed_u8==1)) {
       ams_outputs.enable_driver_u8=1;
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_DRIVE_WAIT_STATE;
   }
   if(after(&timer,2)) {
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_DRIVE_ERROR_STATE;
   }

}
void ams_precharge_drive_error(uint32_t* super_timer, uint8_t* super_state) {
   if(1) {
       ams_parameters.precharge_drive_error_u8=1;
   }

}
void ams_precharge_drive_wait(uint32_t* super_timer, uint8_t* super_state) {
   static uint32_t timer = 0;
   timer++;
   if (super_timer[0] == 0) {
       timer = 0;
   }

   if(after(&timer,2)&&before(&timer,25)&&(0.95*ams_inputs.accumulator_voltage_f64<ams_inputs.vehicle_voltage_f64)) {
       ams_outputs.enable_AIR_plus_u8=1;
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_DRIVE_CLOSE_AIR_PLUS_STATE;
   }
   if(after(&timer,25)) {
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_DRIVE_ERROR_STATE;
   }

}
void ams_precharge_drive_close_air_plus(uint32_t* super_timer, uint8_t* super_state) {
   static uint32_t timer = 0;
   timer++;
   if (super_timer[0] == 0) {
       timer = 0;
   }

   if(before(&timer,2)&&(ams_inputs.AIR_plus_closed_u8==1)) {
       ams_outputs.enable_precharge_u8=0;
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_DRIVE_OPEN_PRECHARGE_STATE;
   }
   if(after(&timer,2)) {
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_DRIVE_ERROR_STATE;
   }

}
void ams_precharge_drive_open_precharge(uint32_t* super_timer, uint8_t* super_state) {
   static uint32_t timer = 0;
   timer++;
   if (super_timer[0] == 0) {
       timer = 0;
   }

   if(before(&timer,2)&&(ams_inputs.precharge_closed_u8==1)) {
       ams_parameters.precharge_drive_complete_u8=1;
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_DRIVE_EXIT_STATE;
   }
   if(after(&timer,2)) {
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_DRIVE_ERROR_STATE;
   }

}
void ams_precharge_drive_exit(uint32_t* super_timer, uint8_t* super_state) {

}
void ams_precharge_drive(uint32_t* super_timer, uint8_t* super_state) {
   static ams_precharge_drive_t state = AMS_PRECHARGE_DRIVE_0_STATE;
   static uint32_t timer = 0;
   timer++;
   if (super_timer[0] == 0) {
       timer = 0;
       state = AMS_PRECHARGE_DRIVE_0_STATE;
   }

   switch(state) {
       case AMS_PRECHARGE_DRIVE_0_STATE:
           ams_precharge_drive_0(&timer, &state);
           break;
       case AMS_PRECHARGE_DRIVE_CLOSE_AIR_MINUS_STATE:
           ams_precharge_drive_close_air_minus(&timer, &state);
           break;
       case AMS_PRECHARGE_DRIVE_CLOSE_PRECHARGE_STATE:
           ams_precharge_drive_close_precharge(&timer, &state);
           break;
       case AMS_PRECHARGE_DRIVE_ERROR_STATE:
           ams_precharge_drive_error(&timer, &state);
           break;
       case AMS_PRECHARGE_DRIVE_WAIT_STATE:
           ams_precharge_drive_wait(&timer, &state);
           break;
       case AMS_PRECHARGE_DRIVE_CLOSE_AIR_PLUS_STATE:
           ams_precharge_drive_close_air_plus(&timer, &state);
           break;
       case AMS_PRECHARGE_DRIVE_OPEN_PRECHARGE_STATE:
           ams_precharge_drive_open_precharge(&timer, &state);
           break;
       case AMS_PRECHARGE_DRIVE_EXIT_STATE:
           ams_precharge_drive_exit(&timer, &state);
           break;
   }

   if(ams_parameters.precharge_drive_complete_u8==1) {
       super_timer[0] = 0;
       super_state[0] = AMS_DRIVE_STATE;
   }
   if((ams_parameters.precharge_drive_error_u8==1)||ams_inputs.error_u8) {
       ams_outputs.AMS_error_u8=1;
       super_timer[0] = 0;
       super_state[0] = AMS_ERROR_STATE;
   }

}
void ams_drive(uint32_t* super_timer, uint8_t* super_state) {
   if(ams_inputs.drive_u8==0) {
       ams_outputs.enable_precharge_u8=0;
       ams_outputs.enable_AIR_plus_u8=0;
       ams_outputs.enable_AIR_minus_u8=0;
       super_timer[0] = 0;
       super_state[0] = AMS_IDLE_STATE;
   }
   if(ams_inputs.error_u8) {
       ams_outputs.enable_precharge_u8=0;
       ams_outputs.enable_AIR_plus_u8=0;
       ams_outputs.enable_AIR_minus_u8=0;
       super_timer[0] = 0;
       super_state[0] = AMS_ERROR_STATE;
   }

}
void ams_error(uint32_t* super_timer, uint8_t* super_state) {
   if(1) {
       ams_outputs.enable_precharge_u8=0;
       ams_outputs.enable_AIR_plus_u8=0;
       ams_outputs.enable_AIR_minus_u8=0;
   }
   if(!ams_inputs.error_u8) {
       super_timer[0] = 0;
       super_state[0] = AMS_IDLE_STATE;
   }

}
void ams_precharge_charge_0(uint32_t* super_timer, uint8_t* super_state) {

}
void ams_precharge_charge_start_charger(uint32_t* super_timer, uint8_t* super_state) {
   static uint32_t timer = 0;
   timer++;
   if (super_timer[0] == 0) {
       timer = 0;
   }

   if(before(&timer,10)&&(ams_inputs.charger_is_live_u8)) {
       ams_outputs.enable_AIR_minus_u8=1;
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_CHARGE_CLOSE_AIR_MINUS_STATE;
   }
   if(after(&timer,10)) {
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_CHARGE_ERROR_STATE;
   }

}
void ams_precharge_charge_close_air_minus(uint32_t* super_timer, uint8_t* super_state) {
   static uint32_t timer = 0;
   timer++;
   if (super_timer[0] == 0) {
       timer = 0;
   }

   if(before(&timer,2)&&(ams_inputs.AIR_minus_closed_u8==1)) {
       ams_outputs.enable_precharge_u8=1;
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_CHARGE_CLOSE_PRECHARGE_STATE;
   }
   if(after(&timer,2)) {
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_CHARGE_ERROR_STATE;
   }

}
void ams_precharge_charge_error(uint32_t* super_timer, uint8_t* super_state) {
   if(1) {
       ams_parameters.precharge_charge_error_u8=1;
   }

}
void ams_precharge_charge_close_precharge(uint32_t* super_timer, uint8_t* super_state) {
   static uint32_t timer = 0;
   timer++;
   if (super_timer[0] == 0) {
       timer = 0;
   }

   if(before(&timer,2)&&(ams_inputs.precharge_closed_u8==1)) {
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_CHARGE_WAIT_STATE;
   }
   if(after(&timer,2)) {
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_CHARGE_ERROR_STATE;
   }

}
void ams_precharge_charge_wait(uint32_t* super_timer, uint8_t* super_state) {
   static uint32_t timer = 0;
   timer++;
   if (super_timer[0] == 0) {
       timer = 0;
   }

   if(after(&timer,2)&&before(&timer,25)&&(0.95*ams_inputs.accumulator_voltage_f64<ams_inputs.vehicle_voltage_f64)) {
       ams_outputs.enable_AIR_plus_u8=1;
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_CHARGE_CLOSE_AIR_PLUS_STATE;
   }
   if(after(&timer,25)) {
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_CHARGE_ERROR_STATE;
   }

}
void ams_precharge_charge_close_air_plus(uint32_t* super_timer, uint8_t* super_state) {
   static uint32_t timer = 0;
   timer++;
   if (super_timer[0] == 0) {
       timer = 0;
   }

   if(before(&timer,2)&&(ams_inputs.AIR_plus_closed_u8==1)) {
       ams_outputs.enable_precharge_u8=0;
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_CHARGE_OPEN_PRECHARGE_STATE;
   }
   if(after(&timer,2)) {
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_CHARGE_ERROR_STATE;
   }

}
void ams_precharge_charge_open_precharge(uint32_t* super_timer, uint8_t* super_state) {
   static uint32_t timer = 0;
   timer++;
   if (super_timer[0] == 0) {
       timer = 0;
   }

   if(before(&timer,2)&&(ams_inputs.precharge_closed_u8==1)) {
       ams_parameters.precharge_charge_complete_u8=1;
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_CHARGE_EXIT_STATE;
   }
   if(after(&timer,2)) {
       super_timer[0] = 0;
       super_state[0] = AMS_PRECHARGE_CHARGE_ERROR_STATE;
   }

}
void ams_precharge_charge_exit(uint32_t* super_timer, uint8_t* super_state) {

}
void ams_precharge_charge(uint32_t* super_timer, uint8_t* super_state) {
   static ams_precharge_charge_t state = AMS_PRECHARGE_CHARGE_0_STATE;
   static uint32_t timer = 0;
   timer++;
   if (super_timer[0] == 0) {
       timer = 0;
       state = AMS_PRECHARGE_CHARGE_0_STATE;
   }

   switch(state) {
       case AMS_PRECHARGE_CHARGE_0_STATE:
           ams_precharge_charge_0(&timer, &state);
           break;
       case AMS_PRECHARGE_CHARGE_START_CHARGER_STATE:
           ams_precharge_charge_start_charger(&timer, &state);
           break;
       case AMS_PRECHARGE_CHARGE_CLOSE_AIR_MINUS_STATE:
           ams_precharge_charge_close_air_minus(&timer, &state);
           break;
       case AMS_PRECHARGE_CHARGE_ERROR_STATE:
           ams_precharge_charge_error(&timer, &state);
           break;
       case AMS_PRECHARGE_CHARGE_CLOSE_PRECHARGE_STATE:
           ams_precharge_charge_close_precharge(&timer, &state);
           break;
       case AMS_PRECHARGE_CHARGE_WAIT_STATE:
           ams_precharge_charge_wait(&timer, &state);
           break;
       case AMS_PRECHARGE_CHARGE_CLOSE_AIR_PLUS_STATE:
           ams_precharge_charge_close_air_plus(&timer, &state);
           break;
       case AMS_PRECHARGE_CHARGE_OPEN_PRECHARGE_STATE:
           ams_precharge_charge_open_precharge(&timer, &state);
           break;
       case AMS_PRECHARGE_CHARGE_EXIT_STATE:
           ams_precharge_charge_exit(&timer, &state);
           break;
   }

   if(ams_parameters.precharge_charge_complete_u8) {
       super_timer[0] = 0;
       super_state[0] = AMS_CHARGE_STATE;
   }

}
void ams_charge(uint32_t* super_timer, uint8_t* super_state) {
   if((ams_inputs.charge_u8==0)||(0.995*ams_inputs.sought_voltage_f64<ams_inputs.accumulator_voltage_f64)) {
       ams_outputs.enable_charger_u8=0;
       ams_outputs.enable_precharge_u8=0;
       ams_outputs.enable_AIR_plus_u8=0;
       ams_outputs.enable_AIR_minus_u8=0;
       super_timer[0] = 0;
       super_state[0] = AMS_IDLE_STATE;
   }
   if(ams_inputs.error_u8||ams_parameters.precharge_charge_error_u8) {
       ams_outputs.enable_precharge_u8=0;
       ams_outputs.enable_AIR_plus_u8=0;
       ams_outputs.enable_AIR_minus_u8=0;
       super_timer[0] = 0;
       super_state[0] = AMS_ERROR_STATE;
   }

}
void ams() {
   static ams_t state = AMS_0_STATE;
   static uint32_t timer = 0;
   timer++;
   switch(state) {
       case AMS_0_STATE:
           ams_0(&timer, &state);
           break;
       case AMS_ENTRY_STATE:
           ams_entry(&timer, &state);
           break;
       case AMS_IDLE_STATE:
           ams_idle(&timer, &state);
           break;
       case AMS_BALANCE_STATE:
           ams_balance(&timer, &state);
           break;
       case AMS_PRECHARGE_DRIVE_STATE:
           ams_precharge_drive(&timer, &state);
           break;
       case AMS_DRIVE_STATE:
           ams_drive(&timer, &state);
           break;
       case AMS_ERROR_STATE:
           ams_error(&timer, &state);
           break;
       case AMS_PRECHARGE_CHARGE_STATE:
           ams_precharge_charge(&timer, &state);
           break;
       case AMS_CHARGE_STATE:
           ams_charge(&timer, &state);
           break;
   }


}
