/*
 * "canlib.h"
 *
 *  Created on: Mar 12, 2022
 *      Author: oscar
 */
#ifndef INC_CANLIB_H_
#define INC_CANLIB_H_

#include "can1.h"
#include "can2.h"
#include "canlib_data.h"
#include "canlib_callbacks.h"

void initialize_CAN(CAN_HandleTypeDef* __hcan1, CAN_HandleTypeDef* __hcan2);

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_status_1_transmit();

/*
 * @Brief  
 * @Retval None
 */
void can1_dbu_status_1_transmit();

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_temperatures_transmit();

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_cell_voltages_transmit();

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_cell_temperatures_transmit();


/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_u3_transmit();

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_u2_transmit();

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_u1_transmit();

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_i_transmit();

/*
 * @Brief  
 * @Retval None
 */
void can2_charger_config_transmit();

/*
 * @Brief  
 * @Retval None
 */
void can2_cc_status_transmit();


/*
 * @Brief  
 * @Retval None
 */
void can1_ams_status_1_receive();

/*
 * @Brief  
 * @Retval None
 */
void can1_dbu_status_1_receive();

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_temperatures_receive();

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_cell_voltages_receive();

/*
 * @Brief  
 * @Retval None
 */
void can1_ams_cell_temperatures_receive();


/*
 * @Brief  
 * @Retval None
 */
void can2_amk1_setpoints_1_receive();

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_u3_receive();

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_u2_receive();

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_u1_receive();

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_i_receive();

/*
 * @Brief  
 * @Retval None
 */
void can2_charger_config_receive();

/*
 * @Brief  
 * @Retval None
 */
void can2_cc_status_receive();


#endif
