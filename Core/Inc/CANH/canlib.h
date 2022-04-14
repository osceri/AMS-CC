/*
 * "canlib.h"
 *
 *  Created on: Mar 12, 2022
 *      Author: oscar
 */
#ifndef INC_CANLIB_H_
#define INC_CANLIB_H_

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
void can1_dbu_status_1_receive();


/*
 * @Brief  
 * @Retval None
 */
void can2_amk1_setpoints_1_receive();

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_i_receive();

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_u1_receive();

/*
 * @Brief  
 * @Retval None
 */
void can2_ivt_msg_result_u3_receive();

/*
 * @Brief  
 * @Retval None
 */
void can2_cc_status_receive();


#endif
