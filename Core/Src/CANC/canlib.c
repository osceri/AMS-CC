/*
 * "canlib.c"
 *
 *  Created on: Mar 12, 2022
 *      Author: oscar
 */

#include "main.h"

#include "canlib.h"
#include "can1.h"
#include "can2.h"
#include "canlib_callbacks.h"
#include "canlib_data.h"

static uint32_t tx_mailbox;
static uint8_t rx_data[8];

static can1_tx_filo_max = 54;
static can1_tx_filo_index = 0;
static can_queue_element_t can1_tx_filo[54];

static can2_tx_filo_max = 6;
static can2_tx_filo_index = 0;
static can_queue_element_t can2_tx_filo[6];


static struct can1_ams_status_1_t can1_ams_status_1;
static struct can1_dbu_status_1_t can1_dbu_status_1;
static struct can1_ams_temperatures_t can1_ams_temperatures;
static struct can1_ams_cell_voltages_t can1_ams_cell_voltages;
static struct can1_ams_cell_temperatures_t can1_ams_cell_temperatures;

static struct can1_ams_status_1_t can1_ams_status_1;
static struct can1_dbu_status_1_t can1_dbu_status_1;
static struct can1_ams_temperatures_t can1_ams_temperatures;
static struct can1_ams_cell_voltages_t can1_ams_cell_voltages;
static struct can1_ams_cell_temperatures_t can1_ams_cell_temperatures;

static struct can2_amk1_setpoints_1_t can2_amk1_setpoints_1;
static struct can2_ivt_msg_result_i_t can2_ivt_msg_result_i;
static struct can2_ivt_msg_result_u1_t can2_ivt_msg_result_u1;
static struct can2_ivt_msg_result_u3_t can2_ivt_msg_result_u3;
static struct can2_charger_config_t can2_charger_config;
static struct can2_cc_status_t can2_cc_status;

static struct can2_ivt_msg_result_i_t can2_ivt_msg_result_i;
static struct can2_ivt_msg_result_u1_t can2_ivt_msg_result_u1;
static struct can2_ivt_msg_result_u3_t can2_ivt_msg_result_u3;
static struct can2_charger_config_t can2_charger_config;
static struct can2_cc_status_t can2_cc_status;

static uint8_t can1_ams_status_1_flag = 0;
static uint8_t can1_dbu_status_1_flag = 0;
static uint8_t can1_ams_temperatures_flag = 0;
static uint8_t can1_ams_cell_voltages_flag = 0;
static uint8_t can1_ams_cell_temperatures_flag = 0;

static uint8_t can2_amk1_setpoints_1_flag = 0;
static uint8_t can2_ivt_msg_result_i_flag = 0;
static uint8_t can2_ivt_msg_result_u1_flag = 0;
static uint8_t can2_ivt_msg_result_u3_flag = 0;
static uint8_t can2_charger_config_flag = 0;
static uint8_t can2_cc_status_flag = 0;

static CAN_HandleTypeDef* _hcan1;
static CAN_HandleTypeDef* _hcan2;

static void can1_ams_cell_voltages_array_raise(uint16_t _index) {
   static uint8_t array[1 +  24 / 8];
   static uint8_t count = 0;
   uint8_t index;

   switch(_index) {
       case 0: index = 0; break;
       case 1: index = 1; break;
       case 2: index = 2; break;
       case 3: index = 3; break;
       case 4: index = 4; break;
       case 5: index = 5; break;
       case 6: index = 6; break;
       case 7: index = 7; break;
       case 8: index = 8; break;
       case 9: index = 9; break;
       case 10: index = 10; break;
       case 11: index = 11; break;
       case 12: index = 12; break;
       case 13: index = 13; break;
       case 14: index = 14; break;
       case 15: index = 15; break;
       case 16: index = 16; break;
       case 17: index = 17; break;
       case 18: index = 18; break;
       case 19: index = 19; break;
       case 20: index = 20; break;
       case 21: index = 21; break;
       case 22: index = 22; break;
       case 23: index = 23; break;
       default: return;
   }

    if(index < 24) {
        uint8_t div = index / 8;
        uint8_t rem = index % 8;
        uint8_t flag = (1 << rem);
        
        if(!(array[div] & flag)) {
            array[div] |= flag;
            count = (count + 1) % 24;

            if(count == 0) {
                for(int i = 0; i < (1 + 24 / 8); i++) {
                    array[i] = 0;
                }
                can1_ams_cell_voltages_flag = 1;
            }
        }
    }
}
static void can1_ams_cell_temperatures_array_raise(uint16_t _index) {
   static uint8_t array[1 +  12 / 8];
   static uint8_t count = 0;
   uint8_t index;

   switch(_index) {
       case 0: index = 0; break;
       case 1: index = 1; break;
       case 2: index = 2; break;
       case 3: index = 3; break;
       case 4: index = 4; break;
       case 5: index = 5; break;
       case 6: index = 6; break;
       case 7: index = 7; break;
       case 8: index = 8; break;
       case 9: index = 9; break;
       case 10: index = 10; break;
       case 11: index = 11; break;
       default: return;
   }

    if(index < 12) {
        uint8_t div = index / 8;
        uint8_t rem = index % 8;
        uint8_t flag = (1 << rem);
        
        if(!(array[div] & flag)) {
            array[div] |= flag;
            count = (count + 1) % 12;

            if(count == 0) {
                for(int i = 0; i < (1 + 12 / 8); i++) {
                    array[i] = 0;
                }
                can1_ams_cell_temperatures_flag = 1;
            }
        }
    }
}
static void can2_charger_config_array_raise(uint16_t _index) {
   static uint8_t array[1 +  4 / 8];
   static uint8_t count = 0;
   uint8_t index;

   switch(_index) {
       case 64: index = 0; break;
       case 65: index = 1; break;
       case 66: index = 2; break;
       case 68: index = 3; break;
       default: return;
   }

    if(index < 4) {
        uint8_t div = index / 8;
        uint8_t rem = index % 8;
        uint8_t flag = (1 << rem);
        
        if(!(array[div] & flag)) {
            array[div] |= flag;
            count = (count + 1) % 4;

            if(count == 0) {
                for(int i = 0; i < (1 + 4 / 8); i++) {
                    array[i] = 0;
                }
                can2_charger_config_flag = 1;
            }
        }
    }
}
/*
 * @Brief  
 * @Retval None
 */
void can1_ams_status_1_decode() {
   can1_ams_status_1_unpack(&can1_ams_status_1, rx_data, 8);
   can1_ams_status_1_flag = 1;
   can1_ams_status_1_receive();
}

/*
 * @Brief  
 * @Retval None
 */
void can1_dbu_status_1_decode() {
   can1_dbu_status_1_unpack(&can1_dbu_status_1, rx_data, 8);
   can1_dbu_status_1_flag = 1;
   can1_dbu_status_1_receive();
}

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_temperatures_decode() {
   can1_ams_temperatures_unpack(&can1_ams_temperatures, rx_data, 6);
   can1_ams_temperatures_flag = 1;
   can1_ams_temperatures_receive();
}

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_cell_voltages_decode() {
   can1_ams_cell_voltages_unpack(&can1_ams_cell_voltages, rx_data, 7);
   can1_ams_cell_voltages_array_raise(can1_ams_cell_voltages.voltage_multiplexor);
   can1_ams_cell_voltages_receive();
}

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_cell_temperatures_decode() {
   can1_ams_cell_temperatures_unpack(&can1_ams_cell_temperatures, rx_data, 8);
   can1_ams_cell_temperatures_array_raise(can1_ams_cell_temperatures.temperature_multiplexor);
   can1_ams_cell_temperatures_receive();
}

/*
 * @Brief  
 * @Retval None
 */
void can2_amk1_setpoints_1_decode() {
   can2_amk1_setpoints_1_unpack(&can2_amk1_setpoints_1, rx_data, 8);
   can2_amk1_setpoints_1_flag = 1;
   can2_amk1_setpoints_1_receive();
}

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_i_decode() {
   can2_ivt_msg_result_i_unpack(&can2_ivt_msg_result_i, rx_data, 6);
   can2_ivt_msg_result_i_flag = 1;
   can2_ivt_msg_result_i_receive();
}

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_u1_decode() {
   can2_ivt_msg_result_u1_unpack(&can2_ivt_msg_result_u1, rx_data, 6);
   can2_ivt_msg_result_u1_flag = 1;
   can2_ivt_msg_result_u1_receive();
}

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_u3_decode() {
   can2_ivt_msg_result_u3_unpack(&can2_ivt_msg_result_u3, rx_data, 6);
   can2_ivt_msg_result_u3_flag = 1;
   can2_ivt_msg_result_u3_receive();
}

/*
 * @Brief  
 * @Retval None
 */
void can2_charger_config_decode() {
   can2_charger_config_unpack(&can2_charger_config, rx_data, 4);
   can2_charger_config_array_raise(can2_charger_config.set_mux);
   can2_charger_config_receive();
}

/*
 * @Brief  
 * @Retval None
 */
void can2_cc_status_decode() {
   can2_cc_status_unpack(&can2_cc_status, rx_data, 1);
   can2_cc_status_flag = 1;
   can2_cc_status_receive();
}

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_status_1_transmit() {
   uint8_t tx_data[8];
   const CAN_TxHeaderTypeDef tx_header = {
       .DLC = 8,
       .IDE = CAN_ID_STD,
       .StdId = 32,
       .RTR = CAN_RTR_DATA,
       .TransmitGlobalTime = DISABLE,
   };
   ams_status_1_t ams_status_1;
   can1_ams_status_1_tx_callback(&ams_status_1);

   can1_ams_status_1.charging_status = can1_ams_status_1_charging_status_encode(ams_status_1.charging_status);
   can1_ams_status_1.air1_closed = can1_ams_status_1_air1_closed_encode(ams_status_1.air1_closed);
   can1_ams_status_1.air2_closed = can1_ams_status_1_air2_closed_encode(ams_status_1.air2_closed);
   can1_ams_status_1.sc_closed = can1_ams_status_1_sc_closed_encode(ams_status_1.sc_closed);
   can1_ams_status_1.pre_charge_status = can1_ams_status_1_pre_charge_status_encode(ams_status_1.pre_charge_status);
   can1_ams_status_1.ams_error = can1_ams_status_1_ams_error_encode(ams_status_1.ams_error);
   can1_ams_status_1.imd_error = can1_ams_status_1_imd_error_encode(ams_status_1.imd_error);
   can1_ams_status_1.state_of_charge = can1_ams_status_1_state_of_charge_encode(ams_status_1.state_of_charge);
   can1_ams_status_1.min_cell_voltage = can1_ams_status_1_min_cell_voltage_encode(ams_status_1.min_cell_voltage);
   can1_ams_status_1.max_cell_voltage = can1_ams_status_1_max_cell_voltage_encode(ams_status_1.max_cell_voltage);
   can1_ams_status_1.min_cell_temperature = can1_ams_status_1_min_cell_temperature_encode(ams_status_1.min_cell_temperature);
   can1_ams_status_1.max_cell_temperature = can1_ams_status_1_max_cell_temperature_encode(ams_status_1.max_cell_temperature);
   can1_ams_status_1.fan_speed = can1_ams_status_1_fan_speed_encode(ams_status_1.fan_speed);

   can1_ams_status_1_pack(tx_data, &can1_ams_status_1, 8);
   while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
   if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
       Error_Handler();
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can1_dbu_status_1_transmit() {
   uint8_t tx_data[8];
   const CAN_TxHeaderTypeDef tx_header = {
       .DLC = 8,
       .IDE = CAN_ID_STD,
       .StdId = 64,
       .RTR = CAN_RTR_DATA,
       .TransmitGlobalTime = DISABLE,
   };
   dbu_status_1_t dbu_status_1;
   can1_dbu_status_1_tx_callback(&dbu_status_1);

   can1_dbu_status_1.activate_ts_button = can1_dbu_status_1_activate_ts_button_encode(dbu_status_1.activate_ts_button);
   can1_dbu_status_1.ready_to_drive_button = can1_dbu_status_1_ready_to_drive_button_encode(dbu_status_1.ready_to_drive_button);
   can1_dbu_status_1.dbu_temperature = can1_dbu_status_1_dbu_temperature_encode(dbu_status_1.dbu_temperature);

   can1_dbu_status_1_pack(tx_data, &can1_dbu_status_1, 8);
   while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
   if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
       Error_Handler();
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_temperatures_transmit() {
   uint8_t tx_data[8];
   const CAN_TxHeaderTypeDef tx_header = {
       .DLC = 6,
       .IDE = CAN_ID_STD,
       .StdId = 35,
       .RTR = CAN_RTR_DATA,
       .TransmitGlobalTime = DISABLE,
   };
   ams_temperatures_t ams_temperatures;
   can1_ams_temperatures_tx_callback(&ams_temperatures);

   can1_ams_temperatures.pre_charge_resistor_temperature = can1_ams_temperatures_pre_charge_resistor_temperature_encode(ams_temperatures.pre_charge_resistor_temperature);
   can1_ams_temperatures.fuse_resistor_temperature = can1_ams_temperatures_fuse_resistor_temperature_encode(ams_temperatures.fuse_resistor_temperature);
   can1_ams_temperatures.aux_1_temperature = can1_ams_temperatures_aux_1_temperature_encode(ams_temperatures.aux_1_temperature);
   can1_ams_temperatures.aux_2_temperature = can1_ams_temperatures_aux_2_temperature_encode(ams_temperatures.aux_2_temperature);

   can1_ams_temperatures_pack(tx_data, &can1_ams_temperatures, 6);
   while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
   if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
       Error_Handler();
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_cell_voltages_transmit() {
   uint8_t tx_data[8];
   const CAN_TxHeaderTypeDef tx_header = {
       .DLC = 7,
       .IDE = CAN_ID_STD,
       .StdId = 33,
       .RTR = CAN_RTR_DATA,
       .TransmitGlobalTime = DISABLE,
   };
   ams_cell_voltages_t ams_cell_voltages;
   can1_ams_cell_voltages_tx_callback(&ams_cell_voltages);

   can1_ams_cell_voltages.voltage_multiplexor = can1_ams_cell_voltages_voltage_multiplexor_encode(ams_cell_voltages.voltage_multiplexor);
   can1_ams_cell_voltages.v1s1 = can1_ams_cell_voltages_v1s1_encode(ams_cell_voltages.v1s1);
   can1_ams_cell_voltages.v7s1 = can1_ams_cell_voltages_v7s1_encode(ams_cell_voltages.v7s1);
   can1_ams_cell_voltages.v1s2 = can1_ams_cell_voltages_v1s2_encode(ams_cell_voltages.v1s2);
   can1_ams_cell_voltages.v7s2 = can1_ams_cell_voltages_v7s2_encode(ams_cell_voltages.v7s2);
   can1_ams_cell_voltages.v1s3 = can1_ams_cell_voltages_v1s3_encode(ams_cell_voltages.v1s3);
   can1_ams_cell_voltages.v7s3 = can1_ams_cell_voltages_v7s3_encode(ams_cell_voltages.v7s3);
   can1_ams_cell_voltages.v1s4 = can1_ams_cell_voltages_v1s4_encode(ams_cell_voltages.v1s4);
   can1_ams_cell_voltages.v7s4 = can1_ams_cell_voltages_v7s4_encode(ams_cell_voltages.v7s4);
   can1_ams_cell_voltages.v1s5 = can1_ams_cell_voltages_v1s5_encode(ams_cell_voltages.v1s5);
   can1_ams_cell_voltages.v7s5 = can1_ams_cell_voltages_v7s5_encode(ams_cell_voltages.v7s5);
   can1_ams_cell_voltages.v1s6 = can1_ams_cell_voltages_v1s6_encode(ams_cell_voltages.v1s6);
   can1_ams_cell_voltages.v7s6 = can1_ams_cell_voltages_v7s6_encode(ams_cell_voltages.v7s6);
   can1_ams_cell_voltages.v1s7 = can1_ams_cell_voltages_v1s7_encode(ams_cell_voltages.v1s7);
   can1_ams_cell_voltages.v7s7 = can1_ams_cell_voltages_v7s7_encode(ams_cell_voltages.v7s7);
   can1_ams_cell_voltages.v1s8 = can1_ams_cell_voltages_v1s8_encode(ams_cell_voltages.v1s8);
   can1_ams_cell_voltages.v7s8 = can1_ams_cell_voltages_v7s8_encode(ams_cell_voltages.v7s8);
   can1_ams_cell_voltages.v1s9 = can1_ams_cell_voltages_v1s9_encode(ams_cell_voltages.v1s9);
   can1_ams_cell_voltages.v7s9 = can1_ams_cell_voltages_v7s9_encode(ams_cell_voltages.v7s9);
   can1_ams_cell_voltages.v1s10 = can1_ams_cell_voltages_v1s10_encode(ams_cell_voltages.v1s10);
   can1_ams_cell_voltages.v7s10 = can1_ams_cell_voltages_v7s10_encode(ams_cell_voltages.v7s10);
   can1_ams_cell_voltages.v1s11 = can1_ams_cell_voltages_v1s11_encode(ams_cell_voltages.v1s11);
   can1_ams_cell_voltages.v7s11 = can1_ams_cell_voltages_v7s11_encode(ams_cell_voltages.v7s11);
   can1_ams_cell_voltages.v1s12 = can1_ams_cell_voltages_v1s12_encode(ams_cell_voltages.v1s12);
   can1_ams_cell_voltages.v7s12 = can1_ams_cell_voltages_v7s12_encode(ams_cell_voltages.v7s12);
   can1_ams_cell_voltages.v2s1 = can1_ams_cell_voltages_v2s1_encode(ams_cell_voltages.v2s1);
   can1_ams_cell_voltages.v8s1 = can1_ams_cell_voltages_v8s1_encode(ams_cell_voltages.v8s1);
   can1_ams_cell_voltages.v2s2 = can1_ams_cell_voltages_v2s2_encode(ams_cell_voltages.v2s2);
   can1_ams_cell_voltages.v8s2 = can1_ams_cell_voltages_v8s2_encode(ams_cell_voltages.v8s2);
   can1_ams_cell_voltages.v2s3 = can1_ams_cell_voltages_v2s3_encode(ams_cell_voltages.v2s3);
   can1_ams_cell_voltages.v8s3 = can1_ams_cell_voltages_v8s3_encode(ams_cell_voltages.v8s3);
   can1_ams_cell_voltages.v2s4 = can1_ams_cell_voltages_v2s4_encode(ams_cell_voltages.v2s4);
   can1_ams_cell_voltages.v8s4 = can1_ams_cell_voltages_v8s4_encode(ams_cell_voltages.v8s4);
   can1_ams_cell_voltages.v2s5 = can1_ams_cell_voltages_v2s5_encode(ams_cell_voltages.v2s5);
   can1_ams_cell_voltages.v8s5 = can1_ams_cell_voltages_v8s5_encode(ams_cell_voltages.v8s5);
   can1_ams_cell_voltages.v2s6 = can1_ams_cell_voltages_v2s6_encode(ams_cell_voltages.v2s6);
   can1_ams_cell_voltages.v8s6 = can1_ams_cell_voltages_v8s6_encode(ams_cell_voltages.v8s6);
   can1_ams_cell_voltages.v2s7 = can1_ams_cell_voltages_v2s7_encode(ams_cell_voltages.v2s7);
   can1_ams_cell_voltages.v8s7 = can1_ams_cell_voltages_v8s7_encode(ams_cell_voltages.v8s7);
   can1_ams_cell_voltages.v2s8 = can1_ams_cell_voltages_v2s8_encode(ams_cell_voltages.v2s8);
   can1_ams_cell_voltages.v8s8 = can1_ams_cell_voltages_v8s8_encode(ams_cell_voltages.v8s8);
   can1_ams_cell_voltages.v2s9 = can1_ams_cell_voltages_v2s9_encode(ams_cell_voltages.v2s9);
   can1_ams_cell_voltages.v8s9 = can1_ams_cell_voltages_v8s9_encode(ams_cell_voltages.v8s9);
   can1_ams_cell_voltages.v2s10 = can1_ams_cell_voltages_v2s10_encode(ams_cell_voltages.v2s10);
   can1_ams_cell_voltages.v8s10 = can1_ams_cell_voltages_v8s10_encode(ams_cell_voltages.v8s10);
   can1_ams_cell_voltages.v2s11 = can1_ams_cell_voltages_v2s11_encode(ams_cell_voltages.v2s11);
   can1_ams_cell_voltages.v8s11 = can1_ams_cell_voltages_v8s11_encode(ams_cell_voltages.v8s11);
   can1_ams_cell_voltages.v2s12 = can1_ams_cell_voltages_v2s12_encode(ams_cell_voltages.v2s12);
   can1_ams_cell_voltages.v8s12 = can1_ams_cell_voltages_v8s12_encode(ams_cell_voltages.v8s12);
   can1_ams_cell_voltages.v3s1 = can1_ams_cell_voltages_v3s1_encode(ams_cell_voltages.v3s1);
   can1_ams_cell_voltages.v9s1 = can1_ams_cell_voltages_v9s1_encode(ams_cell_voltages.v9s1);
   can1_ams_cell_voltages.v3s2 = can1_ams_cell_voltages_v3s2_encode(ams_cell_voltages.v3s2);
   can1_ams_cell_voltages.v9s2 = can1_ams_cell_voltages_v9s2_encode(ams_cell_voltages.v9s2);
   can1_ams_cell_voltages.v3s3 = can1_ams_cell_voltages_v3s3_encode(ams_cell_voltages.v3s3);
   can1_ams_cell_voltages.v9s3 = can1_ams_cell_voltages_v9s3_encode(ams_cell_voltages.v9s3);
   can1_ams_cell_voltages.v3s4 = can1_ams_cell_voltages_v3s4_encode(ams_cell_voltages.v3s4);
   can1_ams_cell_voltages.v9s4 = can1_ams_cell_voltages_v9s4_encode(ams_cell_voltages.v9s4);
   can1_ams_cell_voltages.v3s5 = can1_ams_cell_voltages_v3s5_encode(ams_cell_voltages.v3s5);
   can1_ams_cell_voltages.v9s5 = can1_ams_cell_voltages_v9s5_encode(ams_cell_voltages.v9s5);
   can1_ams_cell_voltages.v3s6 = can1_ams_cell_voltages_v3s6_encode(ams_cell_voltages.v3s6);
   can1_ams_cell_voltages.v9s6 = can1_ams_cell_voltages_v9s6_encode(ams_cell_voltages.v9s6);
   can1_ams_cell_voltages.v3s7 = can1_ams_cell_voltages_v3s7_encode(ams_cell_voltages.v3s7);
   can1_ams_cell_voltages.v9s7 = can1_ams_cell_voltages_v9s7_encode(ams_cell_voltages.v9s7);
   can1_ams_cell_voltages.v3s8 = can1_ams_cell_voltages_v3s8_encode(ams_cell_voltages.v3s8);
   can1_ams_cell_voltages.v9s8 = can1_ams_cell_voltages_v9s8_encode(ams_cell_voltages.v9s8);
   can1_ams_cell_voltages.v3s9 = can1_ams_cell_voltages_v3s9_encode(ams_cell_voltages.v3s9);
   can1_ams_cell_voltages.v9s9 = can1_ams_cell_voltages_v9s9_encode(ams_cell_voltages.v9s9);
   can1_ams_cell_voltages.v3s10 = can1_ams_cell_voltages_v3s10_encode(ams_cell_voltages.v3s10);
   can1_ams_cell_voltages.v9s10 = can1_ams_cell_voltages_v9s10_encode(ams_cell_voltages.v9s10);
   can1_ams_cell_voltages.v3s11 = can1_ams_cell_voltages_v3s11_encode(ams_cell_voltages.v3s11);
   can1_ams_cell_voltages.v9s11 = can1_ams_cell_voltages_v9s11_encode(ams_cell_voltages.v9s11);
   can1_ams_cell_voltages.v3s12 = can1_ams_cell_voltages_v3s12_encode(ams_cell_voltages.v3s12);
   can1_ams_cell_voltages.v9s12 = can1_ams_cell_voltages_v9s12_encode(ams_cell_voltages.v9s12);
   can1_ams_cell_voltages.v4s1 = can1_ams_cell_voltages_v4s1_encode(ams_cell_voltages.v4s1);
   can1_ams_cell_voltages.v10s1 = can1_ams_cell_voltages_v10s1_encode(ams_cell_voltages.v10s1);
   can1_ams_cell_voltages.v4s2 = can1_ams_cell_voltages_v4s2_encode(ams_cell_voltages.v4s2);
   can1_ams_cell_voltages.v10s2 = can1_ams_cell_voltages_v10s2_encode(ams_cell_voltages.v10s2);
   can1_ams_cell_voltages.v4s3 = can1_ams_cell_voltages_v4s3_encode(ams_cell_voltages.v4s3);
   can1_ams_cell_voltages.v10s3 = can1_ams_cell_voltages_v10s3_encode(ams_cell_voltages.v10s3);
   can1_ams_cell_voltages.v4s4 = can1_ams_cell_voltages_v4s4_encode(ams_cell_voltages.v4s4);
   can1_ams_cell_voltages.v10s4 = can1_ams_cell_voltages_v10s4_encode(ams_cell_voltages.v10s4);
   can1_ams_cell_voltages.v4s5 = can1_ams_cell_voltages_v4s5_encode(ams_cell_voltages.v4s5);
   can1_ams_cell_voltages.v10s5 = can1_ams_cell_voltages_v10s5_encode(ams_cell_voltages.v10s5);
   can1_ams_cell_voltages.v4s6 = can1_ams_cell_voltages_v4s6_encode(ams_cell_voltages.v4s6);
   can1_ams_cell_voltages.v10s6 = can1_ams_cell_voltages_v10s6_encode(ams_cell_voltages.v10s6);
   can1_ams_cell_voltages.v4s7 = can1_ams_cell_voltages_v4s7_encode(ams_cell_voltages.v4s7);
   can1_ams_cell_voltages.v10s7 = can1_ams_cell_voltages_v10s7_encode(ams_cell_voltages.v10s7);
   can1_ams_cell_voltages.v4s8 = can1_ams_cell_voltages_v4s8_encode(ams_cell_voltages.v4s8);
   can1_ams_cell_voltages.v10s8 = can1_ams_cell_voltages_v10s8_encode(ams_cell_voltages.v10s8);
   can1_ams_cell_voltages.v4s9 = can1_ams_cell_voltages_v4s9_encode(ams_cell_voltages.v4s9);
   can1_ams_cell_voltages.v10s9 = can1_ams_cell_voltages_v10s9_encode(ams_cell_voltages.v10s9);
   can1_ams_cell_voltages.v4s10 = can1_ams_cell_voltages_v4s10_encode(ams_cell_voltages.v4s10);
   can1_ams_cell_voltages.v10s10 = can1_ams_cell_voltages_v10s10_encode(ams_cell_voltages.v10s10);
   can1_ams_cell_voltages.v4s11 = can1_ams_cell_voltages_v4s11_encode(ams_cell_voltages.v4s11);
   can1_ams_cell_voltages.v10s11 = can1_ams_cell_voltages_v10s11_encode(ams_cell_voltages.v10s11);
   can1_ams_cell_voltages.v4s12 = can1_ams_cell_voltages_v4s12_encode(ams_cell_voltages.v4s12);
   can1_ams_cell_voltages.v10s12 = can1_ams_cell_voltages_v10s12_encode(ams_cell_voltages.v10s12);
   can1_ams_cell_voltages.v5s1 = can1_ams_cell_voltages_v5s1_encode(ams_cell_voltages.v5s1);
   can1_ams_cell_voltages.v11s1 = can1_ams_cell_voltages_v11s1_encode(ams_cell_voltages.v11s1);
   can1_ams_cell_voltages.v5s2 = can1_ams_cell_voltages_v5s2_encode(ams_cell_voltages.v5s2);
   can1_ams_cell_voltages.v5s3 = can1_ams_cell_voltages_v5s3_encode(ams_cell_voltages.v5s3);
   can1_ams_cell_voltages.v11s3 = can1_ams_cell_voltages_v11s3_encode(ams_cell_voltages.v11s3);
   can1_ams_cell_voltages.v5s4 = can1_ams_cell_voltages_v5s4_encode(ams_cell_voltages.v5s4);
   can1_ams_cell_voltages.v5s5 = can1_ams_cell_voltages_v5s5_encode(ams_cell_voltages.v5s5);
   can1_ams_cell_voltages.v11s5 = can1_ams_cell_voltages_v11s5_encode(ams_cell_voltages.v11s5);
   can1_ams_cell_voltages.v5s6 = can1_ams_cell_voltages_v5s6_encode(ams_cell_voltages.v5s6);
   can1_ams_cell_voltages.v5s7 = can1_ams_cell_voltages_v5s7_encode(ams_cell_voltages.v5s7);
   can1_ams_cell_voltages.v11s7 = can1_ams_cell_voltages_v11s7_encode(ams_cell_voltages.v11s7);
   can1_ams_cell_voltages.v5s8 = can1_ams_cell_voltages_v5s8_encode(ams_cell_voltages.v5s8);
   can1_ams_cell_voltages.v5s9 = can1_ams_cell_voltages_v5s9_encode(ams_cell_voltages.v5s9);
   can1_ams_cell_voltages.v11s9 = can1_ams_cell_voltages_v11s9_encode(ams_cell_voltages.v11s9);
   can1_ams_cell_voltages.v5s10 = can1_ams_cell_voltages_v5s10_encode(ams_cell_voltages.v5s10);
   can1_ams_cell_voltages.v5s11 = can1_ams_cell_voltages_v5s11_encode(ams_cell_voltages.v5s11);
   can1_ams_cell_voltages.v11s11 = can1_ams_cell_voltages_v11s11_encode(ams_cell_voltages.v11s11);
   can1_ams_cell_voltages.v5s12 = can1_ams_cell_voltages_v5s12_encode(ams_cell_voltages.v5s12);
   can1_ams_cell_voltages.v6s1 = can1_ams_cell_voltages_v6s1_encode(ams_cell_voltages.v6s1);
   can1_ams_cell_voltages.v6s2 = can1_ams_cell_voltages_v6s2_encode(ams_cell_voltages.v6s2);
   can1_ams_cell_voltages.v6s3 = can1_ams_cell_voltages_v6s3_encode(ams_cell_voltages.v6s3);
   can1_ams_cell_voltages.v6s4 = can1_ams_cell_voltages_v6s4_encode(ams_cell_voltages.v6s4);
   can1_ams_cell_voltages.v6s5 = can1_ams_cell_voltages_v6s5_encode(ams_cell_voltages.v6s5);
   can1_ams_cell_voltages.v6s6 = can1_ams_cell_voltages_v6s6_encode(ams_cell_voltages.v6s6);
   can1_ams_cell_voltages.v6s7 = can1_ams_cell_voltages_v6s7_encode(ams_cell_voltages.v6s7);
   can1_ams_cell_voltages.v6s8 = can1_ams_cell_voltages_v6s8_encode(ams_cell_voltages.v6s8);
   can1_ams_cell_voltages.v6s9 = can1_ams_cell_voltages_v6s9_encode(ams_cell_voltages.v6s9);
   can1_ams_cell_voltages.v6s10 = can1_ams_cell_voltages_v6s10_encode(ams_cell_voltages.v6s10);
   can1_ams_cell_voltages.v6s11 = can1_ams_cell_voltages_v6s11_encode(ams_cell_voltages.v6s11);
   can1_ams_cell_voltages.v6s12 = can1_ams_cell_voltages_v6s12_encode(ams_cell_voltages.v6s12);

       can1_ams_cell_voltages.voltage_multiplexor = 0;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 1;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 2;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 3;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 4;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 5;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 6;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 7;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 8;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 9;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 10;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 11;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 12;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 13;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 14;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 15;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 16;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 17;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 18;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 19;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 20;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 21;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 22;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_voltages.voltage_multiplexor = 23;
       can1_ams_cell_voltages_pack(tx_data, &can1_ams_cell_voltages, 7);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
}

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_cell_temperatures_transmit() {
   uint8_t tx_data[8];
   const CAN_TxHeaderTypeDef tx_header = {
       .DLC = 8,
       .IDE = CAN_ID_STD,
       .StdId = 34,
       .RTR = CAN_RTR_DATA,
       .TransmitGlobalTime = DISABLE,
   };
   ams_cell_temperatures_t ams_cell_temperatures;
   can1_ams_cell_temperatures_tx_callback(&ams_cell_temperatures);

   can1_ams_cell_temperatures.temperature_multiplexor = can1_ams_cell_temperatures_temperature_multiplexor_encode(ams_cell_temperatures.temperature_multiplexor);
   can1_ams_cell_temperatures.t1s1 = can1_ams_cell_temperatures_t1s1_encode(ams_cell_temperatures.t1s1);
   can1_ams_cell_temperatures.t1s2 = can1_ams_cell_temperatures_t1s2_encode(ams_cell_temperatures.t1s2);
   can1_ams_cell_temperatures.t1s3 = can1_ams_cell_temperatures_t1s3_encode(ams_cell_temperatures.t1s3);
   can1_ams_cell_temperatures.t1s4 = can1_ams_cell_temperatures_t1s4_encode(ams_cell_temperatures.t1s4);
   can1_ams_cell_temperatures.t1s5 = can1_ams_cell_temperatures_t1s5_encode(ams_cell_temperatures.t1s5);
   can1_ams_cell_temperatures.t1s6 = can1_ams_cell_temperatures_t1s6_encode(ams_cell_temperatures.t1s6);
   can1_ams_cell_temperatures.t1s7 = can1_ams_cell_temperatures_t1s7_encode(ams_cell_temperatures.t1s7);
   can1_ams_cell_temperatures.t1s8 = can1_ams_cell_temperatures_t1s8_encode(ams_cell_temperatures.t1s8);
   can1_ams_cell_temperatures.t1s9 = can1_ams_cell_temperatures_t1s9_encode(ams_cell_temperatures.t1s9);
   can1_ams_cell_temperatures.t1s10 = can1_ams_cell_temperatures_t1s10_encode(ams_cell_temperatures.t1s10);
   can1_ams_cell_temperatures.t1s11 = can1_ams_cell_temperatures_t1s11_encode(ams_cell_temperatures.t1s11);
   can1_ams_cell_temperatures.t1s12 = can1_ams_cell_temperatures_t1s12_encode(ams_cell_temperatures.t1s12);
   can1_ams_cell_temperatures.t2s1 = can1_ams_cell_temperatures_t2s1_encode(ams_cell_temperatures.t2s1);
   can1_ams_cell_temperatures.t2s2 = can1_ams_cell_temperatures_t2s2_encode(ams_cell_temperatures.t2s2);
   can1_ams_cell_temperatures.t2s3 = can1_ams_cell_temperatures_t2s3_encode(ams_cell_temperatures.t2s3);
   can1_ams_cell_temperatures.t2s4 = can1_ams_cell_temperatures_t2s4_encode(ams_cell_temperatures.t2s4);
   can1_ams_cell_temperatures.t2s5 = can1_ams_cell_temperatures_t2s5_encode(ams_cell_temperatures.t2s5);
   can1_ams_cell_temperatures.t2s6 = can1_ams_cell_temperatures_t2s6_encode(ams_cell_temperatures.t2s6);
   can1_ams_cell_temperatures.t2s7 = can1_ams_cell_temperatures_t2s7_encode(ams_cell_temperatures.t2s7);
   can1_ams_cell_temperatures.t2s8 = can1_ams_cell_temperatures_t2s8_encode(ams_cell_temperatures.t2s8);
   can1_ams_cell_temperatures.t2s9 = can1_ams_cell_temperatures_t2s9_encode(ams_cell_temperatures.t2s9);
   can1_ams_cell_temperatures.t2s10 = can1_ams_cell_temperatures_t2s10_encode(ams_cell_temperatures.t2s10);
   can1_ams_cell_temperatures.t2s11 = can1_ams_cell_temperatures_t2s11_encode(ams_cell_temperatures.t2s11);
   can1_ams_cell_temperatures.t2s12 = can1_ams_cell_temperatures_t2s12_encode(ams_cell_temperatures.t2s12);
   can1_ams_cell_temperatures.t3s1 = can1_ams_cell_temperatures_t3s1_encode(ams_cell_temperatures.t3s1);
   can1_ams_cell_temperatures.t3s2 = can1_ams_cell_temperatures_t3s2_encode(ams_cell_temperatures.t3s2);
   can1_ams_cell_temperatures.t3s3 = can1_ams_cell_temperatures_t3s3_encode(ams_cell_temperatures.t3s3);
   can1_ams_cell_temperatures.t3s4 = can1_ams_cell_temperatures_t3s4_encode(ams_cell_temperatures.t3s4);
   can1_ams_cell_temperatures.t3s5 = can1_ams_cell_temperatures_t3s5_encode(ams_cell_temperatures.t3s5);
   can1_ams_cell_temperatures.t3s6 = can1_ams_cell_temperatures_t3s6_encode(ams_cell_temperatures.t3s6);
   can1_ams_cell_temperatures.t3s7 = can1_ams_cell_temperatures_t3s7_encode(ams_cell_temperatures.t3s7);
   can1_ams_cell_temperatures.t3s8 = can1_ams_cell_temperatures_t3s8_encode(ams_cell_temperatures.t3s8);
   can1_ams_cell_temperatures.t3s9 = can1_ams_cell_temperatures_t3s9_encode(ams_cell_temperatures.t3s9);
   can1_ams_cell_temperatures.t3s10 = can1_ams_cell_temperatures_t3s10_encode(ams_cell_temperatures.t3s10);
   can1_ams_cell_temperatures.t3s11 = can1_ams_cell_temperatures_t3s11_encode(ams_cell_temperatures.t3s11);
   can1_ams_cell_temperatures.t3s12 = can1_ams_cell_temperatures_t3s12_encode(ams_cell_temperatures.t3s12);
   can1_ams_cell_temperatures.t4s1 = can1_ams_cell_temperatures_t4s1_encode(ams_cell_temperatures.t4s1);
   can1_ams_cell_temperatures.t4s2 = can1_ams_cell_temperatures_t4s2_encode(ams_cell_temperatures.t4s2);
   can1_ams_cell_temperatures.t4s3 = can1_ams_cell_temperatures_t4s3_encode(ams_cell_temperatures.t4s3);
   can1_ams_cell_temperatures.t4s4 = can1_ams_cell_temperatures_t4s4_encode(ams_cell_temperatures.t4s4);
   can1_ams_cell_temperatures.t4s5 = can1_ams_cell_temperatures_t4s5_encode(ams_cell_temperatures.t4s5);
   can1_ams_cell_temperatures.t4s6 = can1_ams_cell_temperatures_t4s6_encode(ams_cell_temperatures.t4s6);
   can1_ams_cell_temperatures.t4s7 = can1_ams_cell_temperatures_t4s7_encode(ams_cell_temperatures.t4s7);
   can1_ams_cell_temperatures.t4s8 = can1_ams_cell_temperatures_t4s8_encode(ams_cell_temperatures.t4s8);
   can1_ams_cell_temperatures.t4s9 = can1_ams_cell_temperatures_t4s9_encode(ams_cell_temperatures.t4s9);
   can1_ams_cell_temperatures.t4s10 = can1_ams_cell_temperatures_t4s10_encode(ams_cell_temperatures.t4s10);
   can1_ams_cell_temperatures.t4s11 = can1_ams_cell_temperatures_t4s11_encode(ams_cell_temperatures.t4s11);
   can1_ams_cell_temperatures.t4s12 = can1_ams_cell_temperatures_t4s12_encode(ams_cell_temperatures.t4s12);
   can1_ams_cell_temperatures.t5s1 = can1_ams_cell_temperatures_t5s1_encode(ams_cell_temperatures.t5s1);
   can1_ams_cell_temperatures.t5s2 = can1_ams_cell_temperatures_t5s2_encode(ams_cell_temperatures.t5s2);
   can1_ams_cell_temperatures.t5s3 = can1_ams_cell_temperatures_t5s3_encode(ams_cell_temperatures.t5s3);
   can1_ams_cell_temperatures.t5s4 = can1_ams_cell_temperatures_t5s4_encode(ams_cell_temperatures.t5s4);
   can1_ams_cell_temperatures.t5s5 = can1_ams_cell_temperatures_t5s5_encode(ams_cell_temperatures.t5s5);
   can1_ams_cell_temperatures.t5s6 = can1_ams_cell_temperatures_t5s6_encode(ams_cell_temperatures.t5s6);
   can1_ams_cell_temperatures.t5s7 = can1_ams_cell_temperatures_t5s7_encode(ams_cell_temperatures.t5s7);
   can1_ams_cell_temperatures.t5s8 = can1_ams_cell_temperatures_t5s8_encode(ams_cell_temperatures.t5s8);
   can1_ams_cell_temperatures.t5s9 = can1_ams_cell_temperatures_t5s9_encode(ams_cell_temperatures.t5s9);
   can1_ams_cell_temperatures.t5s10 = can1_ams_cell_temperatures_t5s10_encode(ams_cell_temperatures.t5s10);
   can1_ams_cell_temperatures.t5s11 = can1_ams_cell_temperatures_t5s11_encode(ams_cell_temperatures.t5s11);
   can1_ams_cell_temperatures.t5s12 = can1_ams_cell_temperatures_t5s12_encode(ams_cell_temperatures.t5s12);

       can1_ams_cell_temperatures.temperature_multiplexor = 0;
       can1_ams_cell_temperatures_pack(tx_data, &can1_ams_cell_temperatures, 8);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_temperatures.temperature_multiplexor = 1;
       can1_ams_cell_temperatures_pack(tx_data, &can1_ams_cell_temperatures, 8);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_temperatures.temperature_multiplexor = 2;
       can1_ams_cell_temperatures_pack(tx_data, &can1_ams_cell_temperatures, 8);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_temperatures.temperature_multiplexor = 3;
       can1_ams_cell_temperatures_pack(tx_data, &can1_ams_cell_temperatures, 8);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_temperatures.temperature_multiplexor = 4;
       can1_ams_cell_temperatures_pack(tx_data, &can1_ams_cell_temperatures, 8);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_temperatures.temperature_multiplexor = 5;
       can1_ams_cell_temperatures_pack(tx_data, &can1_ams_cell_temperatures, 8);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_temperatures.temperature_multiplexor = 6;
       can1_ams_cell_temperatures_pack(tx_data, &can1_ams_cell_temperatures, 8);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_temperatures.temperature_multiplexor = 7;
       can1_ams_cell_temperatures_pack(tx_data, &can1_ams_cell_temperatures, 8);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_temperatures.temperature_multiplexor = 8;
       can1_ams_cell_temperatures_pack(tx_data, &can1_ams_cell_temperatures, 8);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_temperatures.temperature_multiplexor = 9;
       can1_ams_cell_temperatures_pack(tx_data, &can1_ams_cell_temperatures, 8);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_temperatures.temperature_multiplexor = 10;
       can1_ams_cell_temperatures_pack(tx_data, &can1_ams_cell_temperatures, 8);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can1_ams_cell_temperatures.temperature_multiplexor = 11;
       can1_ams_cell_temperatures_pack(tx_data, &can1_ams_cell_temperatures, 8);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan1));
       if (HAL_CAN_AddTxMessage(_hcan1, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
}

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_i_transmit() {
   uint8_t tx_data[8];
   const CAN_TxHeaderTypeDef tx_header = {
       .DLC = 6,
       .IDE = CAN_ID_STD,
       .StdId = 1313,
       .RTR = CAN_RTR_DATA,
       .TransmitGlobalTime = DISABLE,
   };
   ivt_msg_result_i_t ivt_msg_result_i;
   can2_ivt_msg_result_i_tx_callback(&ivt_msg_result_i);

   can2_ivt_msg_result_i.i_ts = can2_ivt_msg_result_i_i_ts_encode(ivt_msg_result_i.i_ts);

   can2_ivt_msg_result_i_pack(tx_data, &can2_ivt_msg_result_i, 6);
   while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan2));
   if (HAL_CAN_AddTxMessage(_hcan2, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
       Error_Handler();
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_u1_transmit() {
   uint8_t tx_data[8];
   const CAN_TxHeaderTypeDef tx_header = {
       .DLC = 6,
       .IDE = CAN_ID_STD,
       .StdId = 1314,
       .RTR = CAN_RTR_DATA,
       .TransmitGlobalTime = DISABLE,
   };
   ivt_msg_result_u1_t ivt_msg_result_u1;
   can2_ivt_msg_result_u1_tx_callback(&ivt_msg_result_u1);

   can2_ivt_msg_result_u1.u_cells = can2_ivt_msg_result_u1_u_cells_encode(ivt_msg_result_u1.u_cells);

   can2_ivt_msg_result_u1_pack(tx_data, &can2_ivt_msg_result_u1, 6);
   while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan2));
   if (HAL_CAN_AddTxMessage(_hcan2, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
       Error_Handler();
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_u3_transmit() {
   uint8_t tx_data[8];
   const CAN_TxHeaderTypeDef tx_header = {
       .DLC = 6,
       .IDE = CAN_ID_STD,
       .StdId = 1316,
       .RTR = CAN_RTR_DATA,
       .TransmitGlobalTime = DISABLE,
   };
   ivt_msg_result_u3_t ivt_msg_result_u3;
   can2_ivt_msg_result_u3_tx_callback(&ivt_msg_result_u3);

   can2_ivt_msg_result_u3.u_vehicle = can2_ivt_msg_result_u3_u_vehicle_encode(ivt_msg_result_u3.u_vehicle);

   can2_ivt_msg_result_u3_pack(tx_data, &can2_ivt_msg_result_u3, 6);
   while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan2));
   if (HAL_CAN_AddTxMessage(_hcan2, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
       Error_Handler();
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can2_charger_config_transmit() {
   uint8_t tx_data[8];
   const CAN_TxHeaderTypeDef tx_header = {
       .DLC = 4,
       .IDE = CAN_ID_STD,
       .StdId = 624,
       .RTR = CAN_RTR_DATA,
       .TransmitGlobalTime = DISABLE,
   };
   charger_config_t charger_config;
   can2_charger_config_tx_callback(&charger_config);

   can2_charger_config.set_mux = can2_charger_config_set_mux_encode(charger_config.set_mux);
   can2_charger_config.msg_set_voltage_limit = can2_charger_config_msg_set_voltage_limit_encode(charger_config.msg_set_voltage_limit);
   can2_charger_config.msg_set_current_limit = can2_charger_config_msg_set_current_limit_encode(charger_config.msg_set_current_limit);
   can2_charger_config.msg_set_power_limit = can2_charger_config_msg_set_power_limit_encode(charger_config.msg_set_power_limit);
   can2_charger_config.msg_set_enabled = can2_charger_config_msg_set_enabled_encode(charger_config.msg_set_enabled);
   can2_charger_config.msg_set_time_out = can2_charger_config_msg_set_time_out_encode(charger_config.msg_set_time_out);

       can2_charger_config.set_mux = 64;
       can2_charger_config_pack(tx_data, &can2_charger_config, 4);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan2));
       if (HAL_CAN_AddTxMessage(_hcan2, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can2_charger_config.set_mux = 65;
       can2_charger_config_pack(tx_data, &can2_charger_config, 4);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan2));
       if (HAL_CAN_AddTxMessage(_hcan2, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can2_charger_config.set_mux = 66;
       can2_charger_config_pack(tx_data, &can2_charger_config, 4);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan2));
       if (HAL_CAN_AddTxMessage(_hcan2, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
       can2_charger_config.set_mux = 68;
       can2_charger_config_pack(tx_data, &can2_charger_config, 4);
       while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan2));
       if (HAL_CAN_AddTxMessage(_hcan2, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
           Error_Handler();
       }
}

/*
 * @Brief  
 * @Retval None
 */
void can2_cc_status_transmit() {
   uint8_t tx_data[8];
   const CAN_TxHeaderTypeDef tx_header = {
       .DLC = 1,
       .IDE = CAN_ID_STD,
       .StdId = 592,
       .RTR = CAN_RTR_DATA,
       .TransmitGlobalTime = DISABLE,
   };
   cc_status_t cc_status;
   can2_cc_status_tx_callback(&cc_status);

   can2_cc_status.enable_ams_charging = can2_cc_status_enable_ams_charging_encode(cc_status.enable_ams_charging);

   can2_cc_status_pack(tx_data, &can2_cc_status, 1);
   while(!HAL_CAN_GetTxMailboxesFreeLevel(_hcan2));
   if (HAL_CAN_AddTxMessage(_hcan2, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
       Error_Handler();
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_status_1_receive() {
   if(can1_ams_status_1_flag) {
       ams_status_1_t ams_status_1;
       ams_status_1.charging_status = can1_ams_status_1_charging_status_decode(can1_ams_status_1.charging_status);
       ams_status_1.air1_closed = can1_ams_status_1_air1_closed_decode(can1_ams_status_1.air1_closed);
       ams_status_1.air2_closed = can1_ams_status_1_air2_closed_decode(can1_ams_status_1.air2_closed);
       ams_status_1.sc_closed = can1_ams_status_1_sc_closed_decode(can1_ams_status_1.sc_closed);
       ams_status_1.pre_charge_status = can1_ams_status_1_pre_charge_status_decode(can1_ams_status_1.pre_charge_status);
       ams_status_1.ams_error = can1_ams_status_1_ams_error_decode(can1_ams_status_1.ams_error);
       ams_status_1.imd_error = can1_ams_status_1_imd_error_decode(can1_ams_status_1.imd_error);
       ams_status_1.state_of_charge = can1_ams_status_1_state_of_charge_decode(can1_ams_status_1.state_of_charge);
       ams_status_1.min_cell_voltage = can1_ams_status_1_min_cell_voltage_decode(can1_ams_status_1.min_cell_voltage);
       ams_status_1.max_cell_voltage = can1_ams_status_1_max_cell_voltage_decode(can1_ams_status_1.max_cell_voltage);
       ams_status_1.min_cell_temperature = can1_ams_status_1_min_cell_temperature_decode(can1_ams_status_1.min_cell_temperature);
       ams_status_1.max_cell_temperature = can1_ams_status_1_max_cell_temperature_decode(can1_ams_status_1.max_cell_temperature);
       ams_status_1.fan_speed = can1_ams_status_1_fan_speed_decode(can1_ams_status_1.fan_speed);
       can1_ams_status_1_rx_callback(&ams_status_1);
       can1_ams_status_1_flag = 0;
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can1_dbu_status_1_receive() {
   if(can1_dbu_status_1_flag) {
       dbu_status_1_t dbu_status_1;
       dbu_status_1.activate_ts_button = can1_dbu_status_1_activate_ts_button_decode(can1_dbu_status_1.activate_ts_button);
       dbu_status_1.ready_to_drive_button = can1_dbu_status_1_ready_to_drive_button_decode(can1_dbu_status_1.ready_to_drive_button);
       dbu_status_1.dbu_temperature = can1_dbu_status_1_dbu_temperature_decode(can1_dbu_status_1.dbu_temperature);
       can1_dbu_status_1_rx_callback(&dbu_status_1);
       can1_dbu_status_1_flag = 0;
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_temperatures_receive() {
   if(can1_ams_temperatures_flag) {
       ams_temperatures_t ams_temperatures;
       ams_temperatures.pre_charge_resistor_temperature = can1_ams_temperatures_pre_charge_resistor_temperature_decode(can1_ams_temperatures.pre_charge_resistor_temperature);
       ams_temperatures.fuse_resistor_temperature = can1_ams_temperatures_fuse_resistor_temperature_decode(can1_ams_temperatures.fuse_resistor_temperature);
       ams_temperatures.aux_1_temperature = can1_ams_temperatures_aux_1_temperature_decode(can1_ams_temperatures.aux_1_temperature);
       ams_temperatures.aux_2_temperature = can1_ams_temperatures_aux_2_temperature_decode(can1_ams_temperatures.aux_2_temperature);
       can1_ams_temperatures_rx_callback(&ams_temperatures);
       can1_ams_temperatures_flag = 0;
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_cell_voltages_receive() {
   if(can1_ams_cell_voltages_flag) {
       ams_cell_voltages_t ams_cell_voltages;
       ams_cell_voltages.v1s1 = can1_ams_cell_voltages_v1s1_decode(can1_ams_cell_voltages.v1s1);
       ams_cell_voltages.v7s1 = can1_ams_cell_voltages_v7s1_decode(can1_ams_cell_voltages.v7s1);
       ams_cell_voltages.v1s2 = can1_ams_cell_voltages_v1s2_decode(can1_ams_cell_voltages.v1s2);
       ams_cell_voltages.v7s2 = can1_ams_cell_voltages_v7s2_decode(can1_ams_cell_voltages.v7s2);
       ams_cell_voltages.v1s3 = can1_ams_cell_voltages_v1s3_decode(can1_ams_cell_voltages.v1s3);
       ams_cell_voltages.v7s3 = can1_ams_cell_voltages_v7s3_decode(can1_ams_cell_voltages.v7s3);
       ams_cell_voltages.v1s4 = can1_ams_cell_voltages_v1s4_decode(can1_ams_cell_voltages.v1s4);
       ams_cell_voltages.v7s4 = can1_ams_cell_voltages_v7s4_decode(can1_ams_cell_voltages.v7s4);
       ams_cell_voltages.v1s5 = can1_ams_cell_voltages_v1s5_decode(can1_ams_cell_voltages.v1s5);
       ams_cell_voltages.v7s5 = can1_ams_cell_voltages_v7s5_decode(can1_ams_cell_voltages.v7s5);
       ams_cell_voltages.v1s6 = can1_ams_cell_voltages_v1s6_decode(can1_ams_cell_voltages.v1s6);
       ams_cell_voltages.v7s6 = can1_ams_cell_voltages_v7s6_decode(can1_ams_cell_voltages.v7s6);
       ams_cell_voltages.v1s7 = can1_ams_cell_voltages_v1s7_decode(can1_ams_cell_voltages.v1s7);
       ams_cell_voltages.v7s7 = can1_ams_cell_voltages_v7s7_decode(can1_ams_cell_voltages.v7s7);
       ams_cell_voltages.v1s8 = can1_ams_cell_voltages_v1s8_decode(can1_ams_cell_voltages.v1s8);
       ams_cell_voltages.v7s8 = can1_ams_cell_voltages_v7s8_decode(can1_ams_cell_voltages.v7s8);
       ams_cell_voltages.v1s9 = can1_ams_cell_voltages_v1s9_decode(can1_ams_cell_voltages.v1s9);
       ams_cell_voltages.v7s9 = can1_ams_cell_voltages_v7s9_decode(can1_ams_cell_voltages.v7s9);
       ams_cell_voltages.v1s10 = can1_ams_cell_voltages_v1s10_decode(can1_ams_cell_voltages.v1s10);
       ams_cell_voltages.v7s10 = can1_ams_cell_voltages_v7s10_decode(can1_ams_cell_voltages.v7s10);
       ams_cell_voltages.v1s11 = can1_ams_cell_voltages_v1s11_decode(can1_ams_cell_voltages.v1s11);
       ams_cell_voltages.v7s11 = can1_ams_cell_voltages_v7s11_decode(can1_ams_cell_voltages.v7s11);
       ams_cell_voltages.v1s12 = can1_ams_cell_voltages_v1s12_decode(can1_ams_cell_voltages.v1s12);
       ams_cell_voltages.v7s12 = can1_ams_cell_voltages_v7s12_decode(can1_ams_cell_voltages.v7s12);
       ams_cell_voltages.v2s1 = can1_ams_cell_voltages_v2s1_decode(can1_ams_cell_voltages.v2s1);
       ams_cell_voltages.v8s1 = can1_ams_cell_voltages_v8s1_decode(can1_ams_cell_voltages.v8s1);
       ams_cell_voltages.v2s2 = can1_ams_cell_voltages_v2s2_decode(can1_ams_cell_voltages.v2s2);
       ams_cell_voltages.v8s2 = can1_ams_cell_voltages_v8s2_decode(can1_ams_cell_voltages.v8s2);
       ams_cell_voltages.v2s3 = can1_ams_cell_voltages_v2s3_decode(can1_ams_cell_voltages.v2s3);
       ams_cell_voltages.v8s3 = can1_ams_cell_voltages_v8s3_decode(can1_ams_cell_voltages.v8s3);
       ams_cell_voltages.v2s4 = can1_ams_cell_voltages_v2s4_decode(can1_ams_cell_voltages.v2s4);
       ams_cell_voltages.v8s4 = can1_ams_cell_voltages_v8s4_decode(can1_ams_cell_voltages.v8s4);
       ams_cell_voltages.v2s5 = can1_ams_cell_voltages_v2s5_decode(can1_ams_cell_voltages.v2s5);
       ams_cell_voltages.v8s5 = can1_ams_cell_voltages_v8s5_decode(can1_ams_cell_voltages.v8s5);
       ams_cell_voltages.v2s6 = can1_ams_cell_voltages_v2s6_decode(can1_ams_cell_voltages.v2s6);
       ams_cell_voltages.v8s6 = can1_ams_cell_voltages_v8s6_decode(can1_ams_cell_voltages.v8s6);
       ams_cell_voltages.v2s7 = can1_ams_cell_voltages_v2s7_decode(can1_ams_cell_voltages.v2s7);
       ams_cell_voltages.v8s7 = can1_ams_cell_voltages_v8s7_decode(can1_ams_cell_voltages.v8s7);
       ams_cell_voltages.v2s8 = can1_ams_cell_voltages_v2s8_decode(can1_ams_cell_voltages.v2s8);
       ams_cell_voltages.v8s8 = can1_ams_cell_voltages_v8s8_decode(can1_ams_cell_voltages.v8s8);
       ams_cell_voltages.v2s9 = can1_ams_cell_voltages_v2s9_decode(can1_ams_cell_voltages.v2s9);
       ams_cell_voltages.v8s9 = can1_ams_cell_voltages_v8s9_decode(can1_ams_cell_voltages.v8s9);
       ams_cell_voltages.v2s10 = can1_ams_cell_voltages_v2s10_decode(can1_ams_cell_voltages.v2s10);
       ams_cell_voltages.v8s10 = can1_ams_cell_voltages_v8s10_decode(can1_ams_cell_voltages.v8s10);
       ams_cell_voltages.v2s11 = can1_ams_cell_voltages_v2s11_decode(can1_ams_cell_voltages.v2s11);
       ams_cell_voltages.v8s11 = can1_ams_cell_voltages_v8s11_decode(can1_ams_cell_voltages.v8s11);
       ams_cell_voltages.v2s12 = can1_ams_cell_voltages_v2s12_decode(can1_ams_cell_voltages.v2s12);
       ams_cell_voltages.v8s12 = can1_ams_cell_voltages_v8s12_decode(can1_ams_cell_voltages.v8s12);
       ams_cell_voltages.v3s1 = can1_ams_cell_voltages_v3s1_decode(can1_ams_cell_voltages.v3s1);
       ams_cell_voltages.v9s1 = can1_ams_cell_voltages_v9s1_decode(can1_ams_cell_voltages.v9s1);
       ams_cell_voltages.v3s2 = can1_ams_cell_voltages_v3s2_decode(can1_ams_cell_voltages.v3s2);
       ams_cell_voltages.v9s2 = can1_ams_cell_voltages_v9s2_decode(can1_ams_cell_voltages.v9s2);
       ams_cell_voltages.v3s3 = can1_ams_cell_voltages_v3s3_decode(can1_ams_cell_voltages.v3s3);
       ams_cell_voltages.v9s3 = can1_ams_cell_voltages_v9s3_decode(can1_ams_cell_voltages.v9s3);
       ams_cell_voltages.v3s4 = can1_ams_cell_voltages_v3s4_decode(can1_ams_cell_voltages.v3s4);
       ams_cell_voltages.v9s4 = can1_ams_cell_voltages_v9s4_decode(can1_ams_cell_voltages.v9s4);
       ams_cell_voltages.v3s5 = can1_ams_cell_voltages_v3s5_decode(can1_ams_cell_voltages.v3s5);
       ams_cell_voltages.v9s5 = can1_ams_cell_voltages_v9s5_decode(can1_ams_cell_voltages.v9s5);
       ams_cell_voltages.v3s6 = can1_ams_cell_voltages_v3s6_decode(can1_ams_cell_voltages.v3s6);
       ams_cell_voltages.v9s6 = can1_ams_cell_voltages_v9s6_decode(can1_ams_cell_voltages.v9s6);
       ams_cell_voltages.v3s7 = can1_ams_cell_voltages_v3s7_decode(can1_ams_cell_voltages.v3s7);
       ams_cell_voltages.v9s7 = can1_ams_cell_voltages_v9s7_decode(can1_ams_cell_voltages.v9s7);
       ams_cell_voltages.v3s8 = can1_ams_cell_voltages_v3s8_decode(can1_ams_cell_voltages.v3s8);
       ams_cell_voltages.v9s8 = can1_ams_cell_voltages_v9s8_decode(can1_ams_cell_voltages.v9s8);
       ams_cell_voltages.v3s9 = can1_ams_cell_voltages_v3s9_decode(can1_ams_cell_voltages.v3s9);
       ams_cell_voltages.v9s9 = can1_ams_cell_voltages_v9s9_decode(can1_ams_cell_voltages.v9s9);
       ams_cell_voltages.v3s10 = can1_ams_cell_voltages_v3s10_decode(can1_ams_cell_voltages.v3s10);
       ams_cell_voltages.v9s10 = can1_ams_cell_voltages_v9s10_decode(can1_ams_cell_voltages.v9s10);
       ams_cell_voltages.v3s11 = can1_ams_cell_voltages_v3s11_decode(can1_ams_cell_voltages.v3s11);
       ams_cell_voltages.v9s11 = can1_ams_cell_voltages_v9s11_decode(can1_ams_cell_voltages.v9s11);
       ams_cell_voltages.v3s12 = can1_ams_cell_voltages_v3s12_decode(can1_ams_cell_voltages.v3s12);
       ams_cell_voltages.v9s12 = can1_ams_cell_voltages_v9s12_decode(can1_ams_cell_voltages.v9s12);
       ams_cell_voltages.v4s1 = can1_ams_cell_voltages_v4s1_decode(can1_ams_cell_voltages.v4s1);
       ams_cell_voltages.v10s1 = can1_ams_cell_voltages_v10s1_decode(can1_ams_cell_voltages.v10s1);
       ams_cell_voltages.v4s2 = can1_ams_cell_voltages_v4s2_decode(can1_ams_cell_voltages.v4s2);
       ams_cell_voltages.v10s2 = can1_ams_cell_voltages_v10s2_decode(can1_ams_cell_voltages.v10s2);
       ams_cell_voltages.v4s3 = can1_ams_cell_voltages_v4s3_decode(can1_ams_cell_voltages.v4s3);
       ams_cell_voltages.v10s3 = can1_ams_cell_voltages_v10s3_decode(can1_ams_cell_voltages.v10s3);
       ams_cell_voltages.v4s4 = can1_ams_cell_voltages_v4s4_decode(can1_ams_cell_voltages.v4s4);
       ams_cell_voltages.v10s4 = can1_ams_cell_voltages_v10s4_decode(can1_ams_cell_voltages.v10s4);
       ams_cell_voltages.v4s5 = can1_ams_cell_voltages_v4s5_decode(can1_ams_cell_voltages.v4s5);
       ams_cell_voltages.v10s5 = can1_ams_cell_voltages_v10s5_decode(can1_ams_cell_voltages.v10s5);
       ams_cell_voltages.v4s6 = can1_ams_cell_voltages_v4s6_decode(can1_ams_cell_voltages.v4s6);
       ams_cell_voltages.v10s6 = can1_ams_cell_voltages_v10s6_decode(can1_ams_cell_voltages.v10s6);
       ams_cell_voltages.v4s7 = can1_ams_cell_voltages_v4s7_decode(can1_ams_cell_voltages.v4s7);
       ams_cell_voltages.v10s7 = can1_ams_cell_voltages_v10s7_decode(can1_ams_cell_voltages.v10s7);
       ams_cell_voltages.v4s8 = can1_ams_cell_voltages_v4s8_decode(can1_ams_cell_voltages.v4s8);
       ams_cell_voltages.v10s8 = can1_ams_cell_voltages_v10s8_decode(can1_ams_cell_voltages.v10s8);
       ams_cell_voltages.v4s9 = can1_ams_cell_voltages_v4s9_decode(can1_ams_cell_voltages.v4s9);
       ams_cell_voltages.v10s9 = can1_ams_cell_voltages_v10s9_decode(can1_ams_cell_voltages.v10s9);
       ams_cell_voltages.v4s10 = can1_ams_cell_voltages_v4s10_decode(can1_ams_cell_voltages.v4s10);
       ams_cell_voltages.v10s10 = can1_ams_cell_voltages_v10s10_decode(can1_ams_cell_voltages.v10s10);
       ams_cell_voltages.v4s11 = can1_ams_cell_voltages_v4s11_decode(can1_ams_cell_voltages.v4s11);
       ams_cell_voltages.v10s11 = can1_ams_cell_voltages_v10s11_decode(can1_ams_cell_voltages.v10s11);
       ams_cell_voltages.v4s12 = can1_ams_cell_voltages_v4s12_decode(can1_ams_cell_voltages.v4s12);
       ams_cell_voltages.v10s12 = can1_ams_cell_voltages_v10s12_decode(can1_ams_cell_voltages.v10s12);
       ams_cell_voltages.v5s1 = can1_ams_cell_voltages_v5s1_decode(can1_ams_cell_voltages.v5s1);
       ams_cell_voltages.v11s1 = can1_ams_cell_voltages_v11s1_decode(can1_ams_cell_voltages.v11s1);
       ams_cell_voltages.v5s2 = can1_ams_cell_voltages_v5s2_decode(can1_ams_cell_voltages.v5s2);
       ams_cell_voltages.v5s3 = can1_ams_cell_voltages_v5s3_decode(can1_ams_cell_voltages.v5s3);
       ams_cell_voltages.v11s3 = can1_ams_cell_voltages_v11s3_decode(can1_ams_cell_voltages.v11s3);
       ams_cell_voltages.v5s4 = can1_ams_cell_voltages_v5s4_decode(can1_ams_cell_voltages.v5s4);
       ams_cell_voltages.v5s5 = can1_ams_cell_voltages_v5s5_decode(can1_ams_cell_voltages.v5s5);
       ams_cell_voltages.v11s5 = can1_ams_cell_voltages_v11s5_decode(can1_ams_cell_voltages.v11s5);
       ams_cell_voltages.v5s6 = can1_ams_cell_voltages_v5s6_decode(can1_ams_cell_voltages.v5s6);
       ams_cell_voltages.v5s7 = can1_ams_cell_voltages_v5s7_decode(can1_ams_cell_voltages.v5s7);
       ams_cell_voltages.v11s7 = can1_ams_cell_voltages_v11s7_decode(can1_ams_cell_voltages.v11s7);
       ams_cell_voltages.v5s8 = can1_ams_cell_voltages_v5s8_decode(can1_ams_cell_voltages.v5s8);
       ams_cell_voltages.v5s9 = can1_ams_cell_voltages_v5s9_decode(can1_ams_cell_voltages.v5s9);
       ams_cell_voltages.v11s9 = can1_ams_cell_voltages_v11s9_decode(can1_ams_cell_voltages.v11s9);
       ams_cell_voltages.v5s10 = can1_ams_cell_voltages_v5s10_decode(can1_ams_cell_voltages.v5s10);
       ams_cell_voltages.v5s11 = can1_ams_cell_voltages_v5s11_decode(can1_ams_cell_voltages.v5s11);
       ams_cell_voltages.v11s11 = can1_ams_cell_voltages_v11s11_decode(can1_ams_cell_voltages.v11s11);
       ams_cell_voltages.v5s12 = can1_ams_cell_voltages_v5s12_decode(can1_ams_cell_voltages.v5s12);
       ams_cell_voltages.v6s1 = can1_ams_cell_voltages_v6s1_decode(can1_ams_cell_voltages.v6s1);
       ams_cell_voltages.v6s2 = can1_ams_cell_voltages_v6s2_decode(can1_ams_cell_voltages.v6s2);
       ams_cell_voltages.v6s3 = can1_ams_cell_voltages_v6s3_decode(can1_ams_cell_voltages.v6s3);
       ams_cell_voltages.v6s4 = can1_ams_cell_voltages_v6s4_decode(can1_ams_cell_voltages.v6s4);
       ams_cell_voltages.v6s5 = can1_ams_cell_voltages_v6s5_decode(can1_ams_cell_voltages.v6s5);
       ams_cell_voltages.v6s6 = can1_ams_cell_voltages_v6s6_decode(can1_ams_cell_voltages.v6s6);
       ams_cell_voltages.v6s7 = can1_ams_cell_voltages_v6s7_decode(can1_ams_cell_voltages.v6s7);
       ams_cell_voltages.v6s8 = can1_ams_cell_voltages_v6s8_decode(can1_ams_cell_voltages.v6s8);
       ams_cell_voltages.v6s9 = can1_ams_cell_voltages_v6s9_decode(can1_ams_cell_voltages.v6s9);
       ams_cell_voltages.v6s10 = can1_ams_cell_voltages_v6s10_decode(can1_ams_cell_voltages.v6s10);
       ams_cell_voltages.v6s11 = can1_ams_cell_voltages_v6s11_decode(can1_ams_cell_voltages.v6s11);
       ams_cell_voltages.v6s12 = can1_ams_cell_voltages_v6s12_decode(can1_ams_cell_voltages.v6s12);
       can1_ams_cell_voltages_rx_callback(&ams_cell_voltages);
       can1_ams_cell_voltages_flag = 0;
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_cell_temperatures_receive() {
   if(can1_ams_cell_temperatures_flag) {
       ams_cell_temperatures_t ams_cell_temperatures;
       ams_cell_temperatures.t1s1 = can1_ams_cell_temperatures_t1s1_decode(can1_ams_cell_temperatures.t1s1);
       ams_cell_temperatures.t1s2 = can1_ams_cell_temperatures_t1s2_decode(can1_ams_cell_temperatures.t1s2);
       ams_cell_temperatures.t1s3 = can1_ams_cell_temperatures_t1s3_decode(can1_ams_cell_temperatures.t1s3);
       ams_cell_temperatures.t1s4 = can1_ams_cell_temperatures_t1s4_decode(can1_ams_cell_temperatures.t1s4);
       ams_cell_temperatures.t1s5 = can1_ams_cell_temperatures_t1s5_decode(can1_ams_cell_temperatures.t1s5);
       ams_cell_temperatures.t1s6 = can1_ams_cell_temperatures_t1s6_decode(can1_ams_cell_temperatures.t1s6);
       ams_cell_temperatures.t1s7 = can1_ams_cell_temperatures_t1s7_decode(can1_ams_cell_temperatures.t1s7);
       ams_cell_temperatures.t1s8 = can1_ams_cell_temperatures_t1s8_decode(can1_ams_cell_temperatures.t1s8);
       ams_cell_temperatures.t1s9 = can1_ams_cell_temperatures_t1s9_decode(can1_ams_cell_temperatures.t1s9);
       ams_cell_temperatures.t1s10 = can1_ams_cell_temperatures_t1s10_decode(can1_ams_cell_temperatures.t1s10);
       ams_cell_temperatures.t1s11 = can1_ams_cell_temperatures_t1s11_decode(can1_ams_cell_temperatures.t1s11);
       ams_cell_temperatures.t1s12 = can1_ams_cell_temperatures_t1s12_decode(can1_ams_cell_temperatures.t1s12);
       ams_cell_temperatures.t2s1 = can1_ams_cell_temperatures_t2s1_decode(can1_ams_cell_temperatures.t2s1);
       ams_cell_temperatures.t2s2 = can1_ams_cell_temperatures_t2s2_decode(can1_ams_cell_temperatures.t2s2);
       ams_cell_temperatures.t2s3 = can1_ams_cell_temperatures_t2s3_decode(can1_ams_cell_temperatures.t2s3);
       ams_cell_temperatures.t2s4 = can1_ams_cell_temperatures_t2s4_decode(can1_ams_cell_temperatures.t2s4);
       ams_cell_temperatures.t2s5 = can1_ams_cell_temperatures_t2s5_decode(can1_ams_cell_temperatures.t2s5);
       ams_cell_temperatures.t2s6 = can1_ams_cell_temperatures_t2s6_decode(can1_ams_cell_temperatures.t2s6);
       ams_cell_temperatures.t2s7 = can1_ams_cell_temperatures_t2s7_decode(can1_ams_cell_temperatures.t2s7);
       ams_cell_temperatures.t2s8 = can1_ams_cell_temperatures_t2s8_decode(can1_ams_cell_temperatures.t2s8);
       ams_cell_temperatures.t2s9 = can1_ams_cell_temperatures_t2s9_decode(can1_ams_cell_temperatures.t2s9);
       ams_cell_temperatures.t2s10 = can1_ams_cell_temperatures_t2s10_decode(can1_ams_cell_temperatures.t2s10);
       ams_cell_temperatures.t2s11 = can1_ams_cell_temperatures_t2s11_decode(can1_ams_cell_temperatures.t2s11);
       ams_cell_temperatures.t2s12 = can1_ams_cell_temperatures_t2s12_decode(can1_ams_cell_temperatures.t2s12);
       ams_cell_temperatures.t3s1 = can1_ams_cell_temperatures_t3s1_decode(can1_ams_cell_temperatures.t3s1);
       ams_cell_temperatures.t3s2 = can1_ams_cell_temperatures_t3s2_decode(can1_ams_cell_temperatures.t3s2);
       ams_cell_temperatures.t3s3 = can1_ams_cell_temperatures_t3s3_decode(can1_ams_cell_temperatures.t3s3);
       ams_cell_temperatures.t3s4 = can1_ams_cell_temperatures_t3s4_decode(can1_ams_cell_temperatures.t3s4);
       ams_cell_temperatures.t3s5 = can1_ams_cell_temperatures_t3s5_decode(can1_ams_cell_temperatures.t3s5);
       ams_cell_temperatures.t3s6 = can1_ams_cell_temperatures_t3s6_decode(can1_ams_cell_temperatures.t3s6);
       ams_cell_temperatures.t3s7 = can1_ams_cell_temperatures_t3s7_decode(can1_ams_cell_temperatures.t3s7);
       ams_cell_temperatures.t3s8 = can1_ams_cell_temperatures_t3s8_decode(can1_ams_cell_temperatures.t3s8);
       ams_cell_temperatures.t3s9 = can1_ams_cell_temperatures_t3s9_decode(can1_ams_cell_temperatures.t3s9);
       ams_cell_temperatures.t3s10 = can1_ams_cell_temperatures_t3s10_decode(can1_ams_cell_temperatures.t3s10);
       ams_cell_temperatures.t3s11 = can1_ams_cell_temperatures_t3s11_decode(can1_ams_cell_temperatures.t3s11);
       ams_cell_temperatures.t3s12 = can1_ams_cell_temperatures_t3s12_decode(can1_ams_cell_temperatures.t3s12);
       ams_cell_temperatures.t4s1 = can1_ams_cell_temperatures_t4s1_decode(can1_ams_cell_temperatures.t4s1);
       ams_cell_temperatures.t4s2 = can1_ams_cell_temperatures_t4s2_decode(can1_ams_cell_temperatures.t4s2);
       ams_cell_temperatures.t4s3 = can1_ams_cell_temperatures_t4s3_decode(can1_ams_cell_temperatures.t4s3);
       ams_cell_temperatures.t4s4 = can1_ams_cell_temperatures_t4s4_decode(can1_ams_cell_temperatures.t4s4);
       ams_cell_temperatures.t4s5 = can1_ams_cell_temperatures_t4s5_decode(can1_ams_cell_temperatures.t4s5);
       ams_cell_temperatures.t4s6 = can1_ams_cell_temperatures_t4s6_decode(can1_ams_cell_temperatures.t4s6);
       ams_cell_temperatures.t4s7 = can1_ams_cell_temperatures_t4s7_decode(can1_ams_cell_temperatures.t4s7);
       ams_cell_temperatures.t4s8 = can1_ams_cell_temperatures_t4s8_decode(can1_ams_cell_temperatures.t4s8);
       ams_cell_temperatures.t4s9 = can1_ams_cell_temperatures_t4s9_decode(can1_ams_cell_temperatures.t4s9);
       ams_cell_temperatures.t4s10 = can1_ams_cell_temperatures_t4s10_decode(can1_ams_cell_temperatures.t4s10);
       ams_cell_temperatures.t4s11 = can1_ams_cell_temperatures_t4s11_decode(can1_ams_cell_temperatures.t4s11);
       ams_cell_temperatures.t4s12 = can1_ams_cell_temperatures_t4s12_decode(can1_ams_cell_temperatures.t4s12);
       ams_cell_temperatures.t5s1 = can1_ams_cell_temperatures_t5s1_decode(can1_ams_cell_temperatures.t5s1);
       ams_cell_temperatures.t5s2 = can1_ams_cell_temperatures_t5s2_decode(can1_ams_cell_temperatures.t5s2);
       ams_cell_temperatures.t5s3 = can1_ams_cell_temperatures_t5s3_decode(can1_ams_cell_temperatures.t5s3);
       ams_cell_temperatures.t5s4 = can1_ams_cell_temperatures_t5s4_decode(can1_ams_cell_temperatures.t5s4);
       ams_cell_temperatures.t5s5 = can1_ams_cell_temperatures_t5s5_decode(can1_ams_cell_temperatures.t5s5);
       ams_cell_temperatures.t5s6 = can1_ams_cell_temperatures_t5s6_decode(can1_ams_cell_temperatures.t5s6);
       ams_cell_temperatures.t5s7 = can1_ams_cell_temperatures_t5s7_decode(can1_ams_cell_temperatures.t5s7);
       ams_cell_temperatures.t5s8 = can1_ams_cell_temperatures_t5s8_decode(can1_ams_cell_temperatures.t5s8);
       ams_cell_temperatures.t5s9 = can1_ams_cell_temperatures_t5s9_decode(can1_ams_cell_temperatures.t5s9);
       ams_cell_temperatures.t5s10 = can1_ams_cell_temperatures_t5s10_decode(can1_ams_cell_temperatures.t5s10);
       ams_cell_temperatures.t5s11 = can1_ams_cell_temperatures_t5s11_decode(can1_ams_cell_temperatures.t5s11);
       ams_cell_temperatures.t5s12 = can1_ams_cell_temperatures_t5s12_decode(can1_ams_cell_temperatures.t5s12);
       can1_ams_cell_temperatures_rx_callback(&ams_cell_temperatures);
       can1_ams_cell_temperatures_flag = 0;
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can2_amk1_setpoints_1_receive() {
   if(can2_amk1_setpoints_1_flag) {
       amk1_setpoints_1_t amk1_setpoints_1;
       amk1_setpoints_1.amk1_control_inverter_on = can2_amk1_setpoints_1_amk1_control_inverter_on_decode(can2_amk1_setpoints_1.amk1_control_inverter_on);
       amk1_setpoints_1.amk1_control_dc_on = can2_amk1_setpoints_1_amk1_control_dc_on_decode(can2_amk1_setpoints_1.amk1_control_dc_on);
       amk1_setpoints_1.amk1_control_enable = can2_amk1_setpoints_1_amk1_control_enable_decode(can2_amk1_setpoints_1.amk1_control_enable);
       amk1_setpoints_1.amk1_control_error_reset = can2_amk1_setpoints_1_amk1_control_error_reset_decode(can2_amk1_setpoints_1.amk1_control_error_reset);
       amk1_setpoints_1.amk1_target_velocity = can2_amk1_setpoints_1_amk1_target_velocity_decode(can2_amk1_setpoints_1.amk1_target_velocity);
       amk1_setpoints_1.amk1_torque_limit_positiv = can2_amk1_setpoints_1_amk1_torque_limit_positiv_decode(can2_amk1_setpoints_1.amk1_torque_limit_positiv);
       amk1_setpoints_1.amk1_torque_limit_negativ = can2_amk1_setpoints_1_amk1_torque_limit_negativ_decode(can2_amk1_setpoints_1.amk1_torque_limit_negativ);
       can2_amk1_setpoints_1_rx_callback(&amk1_setpoints_1);
       can2_amk1_setpoints_1_flag = 0;
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_i_receive() {
   if(can2_ivt_msg_result_i_flag) {
       ivt_msg_result_i_t ivt_msg_result_i;
       ivt_msg_result_i.i_ts = can2_ivt_msg_result_i_i_ts_decode(can2_ivt_msg_result_i.i_ts);
       can2_ivt_msg_result_i_rx_callback(&ivt_msg_result_i);
       can2_ivt_msg_result_i_flag = 0;
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_u1_receive() {
   if(can2_ivt_msg_result_u1_flag) {
       ivt_msg_result_u1_t ivt_msg_result_u1;
       ivt_msg_result_u1.u_cells = can2_ivt_msg_result_u1_u_cells_decode(can2_ivt_msg_result_u1.u_cells);
       can2_ivt_msg_result_u1_rx_callback(&ivt_msg_result_u1);
       can2_ivt_msg_result_u1_flag = 0;
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_u3_receive() {
   if(can2_ivt_msg_result_u3_flag) {
       ivt_msg_result_u3_t ivt_msg_result_u3;
       ivt_msg_result_u3.u_vehicle = can2_ivt_msg_result_u3_u_vehicle_decode(can2_ivt_msg_result_u3.u_vehicle);
       can2_ivt_msg_result_u3_rx_callback(&ivt_msg_result_u3);
       can2_ivt_msg_result_u3_flag = 0;
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can2_charger_config_receive() {
   if(can2_charger_config_flag) {
       charger_config_t charger_config;
       charger_config.msg_set_voltage_limit = can2_charger_config_msg_set_voltage_limit_decode(can2_charger_config.msg_set_voltage_limit);
       charger_config.msg_set_current_limit = can2_charger_config_msg_set_current_limit_decode(can2_charger_config.msg_set_current_limit);
       charger_config.msg_set_power_limit = can2_charger_config_msg_set_power_limit_decode(can2_charger_config.msg_set_power_limit);
       charger_config.msg_set_enabled = can2_charger_config_msg_set_enabled_decode(can2_charger_config.msg_set_enabled);
       charger_config.msg_set_time_out = can2_charger_config_msg_set_time_out_decode(can2_charger_config.msg_set_time_out);
       can2_charger_config_rx_callback(&charger_config);
       can2_charger_config_flag = 0;
   }
}

/*
 * @Brief  
 * @Retval None
 */
void can2_cc_status_receive() {
   if(can2_cc_status_flag) {
       cc_status_t cc_status;
       cc_status.enable_ams_charging = can2_cc_status_enable_ams_charging_decode(can2_cc_status.enable_ams_charging);
       can2_cc_status_rx_callback(&cc_status);
       can2_cc_status_flag = 0;
   }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
   CAN_RxHeaderTypeDef rx_header;

   if (HAL_OK == HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data)) {
       switch(rx_header.StdId) {
           case 32:
               can1_ams_status_1_decode();
               break;
           case 64:
               can1_dbu_status_1_decode();
               break;
           case 35:
               can1_ams_temperatures_decode();
               break;
           case 33:
               can1_ams_cell_voltages_decode();
               break;
           case 34:
               can1_ams_cell_temperatures_decode();
               break;
           case 388:
               can2_amk1_setpoints_1_decode();
               break;
           case 1313:
               can2_ivt_msg_result_i_decode();
               break;
           case 1314:
               can2_ivt_msg_result_u1_decode();
               break;
           case 1316:
               can2_ivt_msg_result_u3_decode();
               break;
           case 624:
               can2_charger_config_decode();
               break;
           case 592:
               can2_cc_status_decode();
               break;
        }
    }
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {
   CAN_RxHeaderTypeDef rx_header;

   if (HAL_OK == HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &rx_header, rx_data)) {
       switch(rx_header.StdId) {
           case 32:
               can1_ams_status_1_decode();
               break;
           case 64:
               can1_dbu_status_1_decode();
               break;
           case 35:
               can1_ams_temperatures_decode();
               break;
           case 33:
               can1_ams_cell_voltages_decode();
               break;
           case 34:
               can1_ams_cell_temperatures_decode();
               break;
           case 388:
               can2_amk1_setpoints_1_decode();
               break;
           case 1313:
               can2_ivt_msg_result_i_decode();
               break;
           case 1314:
               can2_ivt_msg_result_u1_decode();
               break;
           case 1316:
               can2_ivt_msg_result_u3_decode();
               break;
           case 624:
               can2_charger_config_decode();
               break;
           case 592:
               can2_cc_status_decode();
               break;
        }
    }
}


void simple_filter() {
   CAN_FilterTypeDef filter_config;
   
   filter_config.FilterActivation = ENABLE;
   filter_config.FilterBank = 0;
   filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
   filter_config.FilterIdHigh = 0x0000;
   filter_config.FilterIdLow = 0x0000;
   filter_config.FilterMaskIdHigh = 0x0000;
   filter_config.FilterMaskIdLow = 0x0000;
   filter_config.FilterScale = CAN_FILTERSCALE_32BIT;

   filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
   if (HAL_CAN_ConfigFilter(_hcan1, &filter_config) != HAL_OK) {
       Error_Handler();
   }
   filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO1;
   if (HAL_CAN_ConfigFilter(_hcan1, &filter_config) != HAL_OK) {
       Error_Handler();
   }
   filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
   if (HAL_CAN_ConfigFilter(_hcan2, &filter_config) != HAL_OK) {
       Error_Handler();
   }
   filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO1;
   if (HAL_CAN_ConfigFilter(_hcan2, &filter_config) != HAL_OK) {
       Error_Handler();
   }


}

void initialize_CAN(CAN_HandleTypeDef* __hcan1, CAN_HandleTypeDef* __hcan2) {
   _hcan1 = __hcan1;

   if (HAL_CAN_Start(_hcan1) != HAL_OK) {
       Error_Handler();
   }
   if (HAL_CAN_WakeUp(_hcan1) != HAL_OK) {
       Error_Handler();
   }
   if (HAL_CAN_ActivateNotification(_hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK) {
       Error_Handler();
   }

   _hcan2 = __hcan2;

   if (HAL_CAN_Start(_hcan2) != HAL_OK) {
       Error_Handler();
   }
   if (HAL_CAN_WakeUp(_hcan2) != HAL_OK) {
       Error_Handler();
   }
   if (HAL_CAN_ActivateNotification(_hcan2, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK) {
       Error_Handler();
   }


   simple_filter();

}

