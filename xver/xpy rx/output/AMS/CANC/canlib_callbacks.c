/*
 * "canlib_callbacks.c"
 *
 *  Created on: Mar 12, 2022
 *      Author: oscar
 */

#include "main.h"
#include "canlib.h"

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
__weak void can1_ams_status_1_rx_callback(ams_status_1_t* ams_status_1) {

}

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
__weak void can1_dbu_status_1_rx_callback(dbu_status_1_t* dbu_status_1) {

}

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
__weak void can1_ams_temperatures_rx_callback(ams_temperatures_t* ams_temperatures) {

}

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
__weak void can1_ams_cell_voltages_rx_callback(ams_cell_voltages_t* ams_cell_voltages) {

}

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
__weak void can1_ams_cell_temperatures_rx_callback(ams_cell_temperatures_t* ams_cell_temperatures) {

}

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
__weak void can2_amk1_setpoints_1_rx_callback(amk1_setpoints_1_t* amk1_setpoints_1) {

}

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
__weak void can2_ivt_msg_result_u3_rx_callback(ivt_msg_result_u3_t* ivt_msg_result_u3) {

}

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
__weak void can2_ivt_msg_result_u2_rx_callback(ivt_msg_result_u2_t* ivt_msg_result_u2) {

}

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
__weak void can2_ivt_msg_result_u1_rx_callback(ivt_msg_result_u1_t* ivt_msg_result_u1) {

}

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
__weak void can2_ivt_msg_result_i_rx_callback(ivt_msg_result_i_t* ivt_msg_result_i) {

}

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
__weak void can2_charger_config_rx_callback(charger_config_t* charger_config) {

}

/*
 * @Brief  
 * @Param  A pointer to the message which was received
 * @Retval None
 */
__weak void can2_cc_status_rx_callback(cc_status_t* cc_status) {

}

/*
 * @Brief  
 * @Param  A pointer to a message which should be sent
 * @Retval Return 1 if the data entered in ams_status_1 should be sent
 */
__weak uint8_t can1_ams_status_1_tx_callback(ams_status_1_t* ams_status_1) {

}

/*
 * @Brief  
 * @Param  A pointer to a message which should be sent
 * @Retval Return 1 if the data entered in dbu_status_1 should be sent
 */
__weak uint8_t can1_dbu_status_1_tx_callback(dbu_status_1_t* dbu_status_1) {

}

/*
 * @Brief  
 * @Param  A pointer to a message which should be sent
 * @Retval Return 1 if the data entered in ams_temperatures should be sent
 */
__weak uint8_t can1_ams_temperatures_tx_callback(ams_temperatures_t* ams_temperatures) {

}

/*
 * @Brief  
 * @Param  A pointer to a message which should be sent
 * @Retval Return 1 if the data entered in ams_cell_voltages should be sent
 */
__weak uint8_t can1_ams_cell_voltages_tx_callback(ams_cell_voltages_t* ams_cell_voltages) {

}

/*
 * @Brief  
 * @Param  A pointer to a message which should be sent
 * @Retval Return 1 if the data entered in ams_cell_temperatures should be sent
 */
__weak uint8_t can1_ams_cell_temperatures_tx_callback(ams_cell_temperatures_t* ams_cell_temperatures) {

}

/*
 * @Brief  
 * @Param  A pointer to a message which should be sent
 * @Retval Return 1 if the data entered in ivt_msg_result_u3 should be sent
 */
__weak uint8_t can2_ivt_msg_result_u3_tx_callback(ivt_msg_result_u3_t* ivt_msg_result_u3) {

}

/*
 * @Brief  
 * @Param  A pointer to a message which should be sent
 * @Retval Return 1 if the data entered in ivt_msg_result_u2 should be sent
 */
__weak uint8_t can2_ivt_msg_result_u2_tx_callback(ivt_msg_result_u2_t* ivt_msg_result_u2) {

}

/*
 * @Brief  
 * @Param  A pointer to a message which should be sent
 * @Retval Return 1 if the data entered in ivt_msg_result_u1 should be sent
 */
__weak uint8_t can2_ivt_msg_result_u1_tx_callback(ivt_msg_result_u1_t* ivt_msg_result_u1) {

}

/*
 * @Brief  
 * @Param  A pointer to a message which should be sent
 * @Retval Return 1 if the data entered in ivt_msg_result_i should be sent
 */
__weak uint8_t can2_ivt_msg_result_i_tx_callback(ivt_msg_result_i_t* ivt_msg_result_i) {

}

/*
 * @Brief  
 * @Param  A pointer to a message which should be sent
 * @Retval Return 1 if the data entered in charger_config should be sent
 */
__weak uint8_t can2_charger_config_tx_callback(charger_config_t* charger_config) {

}

/*
 * @Brief  
 * @Param  A pointer to a message which should be sent
 * @Retval Return 1 if the data entered in cc_status should be sent
 */
__weak uint8_t can2_cc_status_tx_callback(cc_status_t* cc_status) {

}

