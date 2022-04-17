/*
 * "canlib_callbacks.h"
 *
 *  Created on: Mar 12, 2022
 *      Author: oscar
 */
#ifndef INC_CANLIB_CALLBACKS_H_
#define INC_CANLIB_CALLBACKS_H_

#include "canlib_data.h"

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
void can1_dbu_status_1_rx_callback(dbu_status_1_t* dbu_status_1);

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
void can2_amk1_setpoints_1_rx_callback(amk1_setpoints_1_t* amk1_setpoints_1);

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
void can2_ivt_msg_result_i_rx_callback(ivt_msg_result_i_t* ivt_msg_result_i);

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
void can2_ivt_msg_result_u1_rx_callback(ivt_msg_result_u1_t* ivt_msg_result_u1);

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
void can2_ivt_msg_result_u3_rx_callback(ivt_msg_result_u3_t* ivt_msg_result_u3);

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
void can2_cc_status_rx_callback(cc_status_t* cc_status);

/*
 * @Brief  
 * @Param  A pointer to a message which should be sent
 * @Retval Return 1 if the data entered in ams_status_1 should be sent
 */
uint8_t can1_ams_status_1_tx_callback(ams_status_1_t* ams_status_1);

/*
 * @Brief  
 * @Param  A pointer to a message which should be sent
 * @Retval Return 1 if the data entered in ams_temperatures should be sent
 */
uint8_t can1_ams_temperatures_tx_callback(ams_temperatures_t* ams_temperatures);

/*
 * @Brief  
 * @Param  A pointer to a message which should be sent
 * @Retval Return 1 if the data entered in ams_cell_voltages should be sent
 */
uint8_t can1_ams_cell_voltages_tx_callback(ams_cell_voltages_t* ams_cell_voltages);

/*
 * @Brief  
 * @Param  A pointer to a message which should be sent
 * @Retval Return 1 if the data entered in ams_cell_temperatures should be sent
 */
uint8_t can1_ams_cell_temperatures_tx_callback(ams_cell_temperatures_t* ams_cell_temperatures);

/*
 * @Brief  
 * @Param  A pointer to a message which should be sent
 * @Retval Return 1 if the data entered in charger_config should be sent
 */
uint8_t can2_charger_config_tx_callback(charger_config_t* charger_config);

#endif
