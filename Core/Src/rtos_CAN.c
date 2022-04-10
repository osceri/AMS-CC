/*
 * rtos_CAN.c
 *
 *  Created on: Apr 10, 2022
 *      Author: oscar
 */

#include "main.h"
#include "cmsis_os.h"

#include "stdint.h"
#include "queue.h"
#include "can1.h"
#include "can2.h"
#include "RTOS_can.h"

/* These are the HAL type handles to be used for accessing mailboxes */
static CAN_HandleTypeDef *_hcan1;
static CAN_HandleTypeDef *_hcan2;

/* Global mailbox to hold information */

uint32_t tx_mailbox;

/* Task definitions a la cmsis */
osThreadId_t can_tx_task_handle;
osThreadId_t can1_tx_queue_task_handle;
osThreadId_t can2_tx_queue_task_handle;
osThreadId_t can_rx_queue_task_handle;

/* Generate a queue handle for every message used by the AMS */
QueueHandle_t ivt_msg_result_u3_queue;
QueueHandle_t ivt_msg_result_i_queue;
QueueHandle_t ivt_msg_result_u1_queue;
QueueHandle_t dbu_status_1_queue;
QueueHandle_t ivt_msg_result_u2_queue;
QueueHandle_t ams_status_1_queue;
QueueHandle_t ams_cell_voltages_queue;
QueueHandle_t ams_cell_temperatures_queue;
QueueHandle_t ams_temperatures_queue;
QueueHandle_t can_rx_queue;
QueueHandle_t can1_tx_queue;
QueueHandle_t can2_tx_queue;


double DECODER(uint64_t x) {
	x = (0x0 | ((x & 0xff0000000000) >> 40) | ((x & 0x00ff00000000) >> 24)
			| ((x & 0x0000ff000000) >> 8) | ((x & 0x000000ff0000) << 8));
	return (int32_t) x;
}

/*
 * @brief   Initialize all of the until now generated queue handles
 * @retval  None
 */
void initialize_CAN(CAN_HandleTypeDef *__hcan1, CAN_HandleTypeDef *__hcan2) {
	_hcan1 = __hcan1;

	if (HAL_CAN_Start(_hcan1) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_CAN_WakeUp(_hcan1) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_CAN_ActivateNotification(_hcan1,
	CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK) {
		Error_Handler();
	}

	_hcan2 = __hcan2;

	if (HAL_CAN_Start(_hcan2) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_CAN_WakeUp(_hcan2) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_CAN_ActivateNotification(_hcan2,
	CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK) {
		Error_Handler();
	}

	{
		struct ivt_msg_result_u3_t _ivt_msg_result_u3;
		ivt_msg_result_u3_queue = xQueueCreate(1, sizeof(_ivt_msg_result_u3));
	}
	{
		struct ivt_msg_result_i_t _ivt_msg_result_i;
		ivt_msg_result_i_queue = xQueueCreate(1, sizeof(_ivt_msg_result_i));
	}
	{
		struct ivt_msg_result_u1_t _ivt_msg_result_u1;
		ivt_msg_result_u1_queue = xQueueCreate(1, sizeof(_ivt_msg_result_u1));
	}
	{
		struct dbu_status_1_t _dbu_status_1;
		dbu_status_1_queue = xQueueCreate(1, sizeof(_dbu_status_1));
	}
	{
		struct ivt_msg_result_u2_t _ivt_msg_result_u2;
		ivt_msg_result_u2_queue = xQueueCreate(1, sizeof(_ivt_msg_result_u2));
	}
	{
		struct ams_status_1_t _ams_status_1;
		ams_status_1_queue = xQueueCreate(1, sizeof(_ams_status_1));
	}
	{
		struct ams_cell_voltages_t _ams_cell_voltages;
		ams_cell_voltages_queue = xQueueCreate(1, sizeof(_ams_cell_voltages));
	}
	{
		struct ams_cell_temperatures_t _ams_cell_temperatures;
		ams_cell_temperatures_queue = xQueueCreate(1,
				sizeof(_ams_cell_temperatures));
	}
	{
		struct ams_temperatures_t _ams_temperatures;
		ams_temperatures_queue = xQueueCreate(1, sizeof(_ams_temperatures));
	}

	struct can_queue_element_t _can_queue_element;

	/* The queue is slightly bigger than the greatest amount of receiveable messages */
	can_rx_queue = xQueueCreate(10, sizeof(_can_queue_element));
	/* The queues are slightly bigger than the greatest amount of sendeable messages in the respective channels */
	can1_tx_queue = xQueueCreate(72, sizeof(_can_queue_element));
	can2_tx_queue = xQueueCreate(8, sizeof(_can_queue_element));

	simple_filter();
	//can1_filter_config();
	//can2_filter_config();

}

void tx_send_can() {
	static struct can_queue_element_t can_queue_element;

	while (xQueueReceive(can1_tx_queue, &can_queue_element, 0)
			&& (0 < HAL_CAN_GetTxMailboxesFreeLevel(_hcan1))) {
		if (HAL_CAN_AddTxMessage(_hcan1, &can_queue_element.tx_header,
				can_queue_element.data, &tx_mailbox) != HAL_OK) {
			Error_Handler();
		}
	}
	while (xQueueReceive(can2_tx_queue, &can_queue_element, 0)
			&& (0 < HAL_CAN_GetTxMailboxesFreeLevel(_hcan2))) {
		if (HAL_CAN_AddTxMessage(_hcan2, &can_queue_element.tx_header,
				can_queue_element.data, &tx_mailbox) != HAL_OK) {
			Error_Handler();
		}
	}
}

void tx_send_can_fromISR() {
	static struct can_queue_element_t can_queue_element;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	while (xQueueReceiveFromISR(can1_tx_queue, &can_queue_element,
			&xHigherPriorityTaskWoken)
			&& (0 < HAL_CAN_GetTxMailboxesFreeLevel(_hcan1))) {
		if (HAL_CAN_AddTxMessage(_hcan1, &can_queue_element.tx_header,
				can_queue_element.data, &tx_mailbox) != HAL_OK) {
			Error_Handler();
		}
	}
	while (xQueueReceiveFromISR(can2_tx_queue, &can_queue_element,
			&xHigherPriorityTaskWoken)
			&& (0 < HAL_CAN_GetTxMailboxesFreeLevel(_hcan2))) {
		if (HAL_CAN_AddTxMessage(_hcan2, &can_queue_element.tx_header,
				can_queue_element.data, &tx_mailbox) != HAL_OK) {
			Error_Handler();
		}
	}

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	return;
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan) {
	tx_send_can_fromISR();
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan) {
	tx_send_can_fromISR();
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan) {
	tx_send_can_fromISR();
}

/*
 * @brief   Configure the first HAL_CAN callback to place
 * @param   A pointer to the appropriate HAL-type can handle
 * @retval  None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	static struct can_queue_element_t can_queue_element;

	if (HAL_OK
			== HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0,
					&can_queue_element.rx_header, can_queue_element.data)) {
		xQueueSendFromISR(can_rx_queue, &can_queue_element,
				&xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

/*
 * @brief   Configure the second HAL_CAN callback to place
 * @param   A pointer to the appropriate HAL-type can handle
 * @retval  None
 */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	static struct can_queue_element_t can_queue_element;

	if (HAL_OK
			== HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1,
					&can_queue_element.rx_header, can_queue_element.data)) {
		xQueueSendFromISR(can_rx_queue, &can_queue_element,
				&xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

/*
 * @brief   A function which unpacks a received can2_ivt_msg_result_u3 component (including every sub-multiplex),
 *          and places the resulting message in ivt_msg_result_u3_queue
 * @param   A pointer to a can queue element which was retrieved from the SW RX CAN queue
 * @retval  None
 */
void ivt_msg_result_u3_unpack_and_send(
		struct can_queue_element_t *can_queue_element) {
	static struct can2_ivt_msg_result_u3_t can2_ivt_msg_result_u3;
	static struct ivt_msg_result_u3_t ivt_msg_result_u3;
	can2_ivt_msg_result_u3_unpack(&can2_ivt_msg_result_u3,
			can_queue_element->data, CAN2_IVT_MSG_RESULT_U3_LENGTH);

	ivt_msg_result_u3.u_vehicle = can2_ivt_msg_result_u3_u_vehicle_decode(
			can2_ivt_msg_result_u3.u_vehicle);
	ivt_msg_result_u3.u_vehicle = DECODER(can2_ivt_msg_result_u3.u_vehicle)
			/ 1000;

	xQueueOverwrite(ivt_msg_result_u3_queue, &ivt_msg_result_u3);
}

/*
 * @brief   A function which unpacks a received can2_ivt_msg_result_i component (including every sub-multiplex),
 *          and places the resulting message in ivt_msg_result_i_queue
 * @param   A pointer to a can queue element which was retrieved from the SW RX CAN queue
 * @retval  None
 */
void ivt_msg_result_i_unpack_and_send(
		struct can_queue_element_t *can_queue_element) {
	static struct can2_ivt_msg_result_i_t can2_ivt_msg_result_i;
	static struct ivt_msg_result_i_t ivt_msg_result_i;
	can2_ivt_msg_result_i_unpack(&can2_ivt_msg_result_i,
			can_queue_element->data, CAN2_IVT_MSG_RESULT_I_LENGTH);

	ivt_msg_result_i.i_ts = can2_ivt_msg_result_i_i_ts_decode(
			can2_ivt_msg_result_i.i_ts);
	ivt_msg_result_i.i_ts = DECODER(can2_ivt_msg_result_i.i_ts) / 1000;

	xQueueOverwrite(ivt_msg_result_i_queue, &ivt_msg_result_i);
}

/*
 * @brief   A function which unpacks a received can2_ivt_msg_result_u1 component (including every sub-multiplex),
 *          and places the resulting message in ivt_msg_result_u1_queue
 * @param   A pointer to a can queue element which was retrieved from the SW RX CAN queue
 * @retval  None
 */
void ivt_msg_result_u1_unpack_and_send(
		struct can_queue_element_t *can_queue_element) {
	static struct can2_ivt_msg_result_u1_t can2_ivt_msg_result_u1;
	static struct ivt_msg_result_u1_t ivt_msg_result_u1;
	can2_ivt_msg_result_u1_unpack(&can2_ivt_msg_result_u1,
			can_queue_element->data, CAN2_IVT_MSG_RESULT_U1_LENGTH);

	ivt_msg_result_u1.u_cells = can2_ivt_msg_result_u1_u_cells_decode(
			can2_ivt_msg_result_u1.u_cells);
	ivt_msg_result_u1.u_cells = DECODER(can2_ivt_msg_result_u1.u_cells) / 1000;

	xQueueOverwrite(ivt_msg_result_u1_queue, &ivt_msg_result_u1);
}

/*
 * @brief   A function which unpacks a received can1_dbu_status_1 component (including every sub-multiplex),
 *          and places the resulting message in dbu_status_1_queue
 * @param   A pointer to a can queue element which was retrieved from the SW RX CAN queue
 * @retval  None
 */
void dbu_status_1_unpack_and_send(struct can_queue_element_t *can_queue_element) {
	static struct can1_dbu_status_1_t can1_dbu_status_1;
	static struct dbu_status_1_t dbu_status_1;
	can1_dbu_status_1_unpack(&can1_dbu_status_1, can_queue_element->data,
	CAN1_DBU_STATUS_1_LENGTH);

	dbu_status_1.activate_ts_button =
			can1_dbu_status_1_activate_ts_button_decode(
					can1_dbu_status_1.activate_ts_button);
	dbu_status_1.ready_to_drive_button =
			can1_dbu_status_1_ready_to_drive_button_decode(
					can1_dbu_status_1.ready_to_drive_button);
	dbu_status_1.dbu_temperature = can1_dbu_status_1_dbu_temperature_decode(
			can1_dbu_status_1.dbu_temperature);

	xQueueOverwrite(dbu_status_1_queue, &dbu_status_1);
}

/*
 * @brief   A function which unpacks a received can2_ivt_msg_result_u2 component (including every sub-multiplex),
 *          and places the resulting message in ivt_msg_result_u2_queue
 * @param   A pointer to a can queue element which was retrieved from the SW RX CAN queue
 * @retval  None
 */
void ivt_msg_result_u2_unpack_and_send(
		struct can_queue_element_t *can_queue_element) {
	static struct can2_ivt_msg_result_u2_t can2_ivt_msg_result_u2;
	static struct ivt_msg_result_u2_t ivt_msg_result_u2;
	can2_ivt_msg_result_u2_unpack(&can2_ivt_msg_result_u2,
			can_queue_element->data, CAN2_IVT_MSG_RESULT_U2_LENGTH);

	ivt_msg_result_u2.u_fuse = can2_ivt_msg_result_u2_u_fuse_decode(
			can2_ivt_msg_result_u2.u_fuse);
	ivt_msg_result_u2.u_fuse = DECODER(can2_ivt_msg_result_u2.u_fuse) / 1000;

	xQueueOverwrite(ivt_msg_result_u2_queue, &ivt_msg_result_u2);
}

/*
 * @brief   A function which packs a can1_ams_status_1 to be sent (including every sub-multiplex) into the
 *          appropriate SW TX CAN queue. Make sure that the ams_status_1_queue queue is filled with data before hand !
 * @retval  None
 */
void ams_status_1_pack_and_send() {
	static struct can1_ams_status_1_t can1_ams_status_1;
	static struct ams_status_1_t ams_status_1;
	static struct can_queue_element_t can_queue_element = { .tx_header = {
			.DLC = CAN1_AMS_STATUS_1_LENGTH, .IDE = CAN_ID_STD, .StdId =
			CAN1_AMS_STATUS_1_FRAME_ID, .RTR = CAN_RTR_DATA,
			.TransmitGlobalTime = DISABLE } };
	if (xQueuePeek(ams_status_1_queue, &ams_status_1, 0)) {
		can1_ams_status_1.air1_closed = can1_ams_status_1_air1_closed_encode(
				ams_status_1.air1_closed);
		can1_ams_status_1.air2_closed = can1_ams_status_1_air2_closed_encode(
				ams_status_1.air2_closed);
		can1_ams_status_1.sc_closed = can1_ams_status_1_sc_closed_encode(
				ams_status_1.sc_closed);
		can1_ams_status_1.pre_charge_status =
				can1_ams_status_1_pre_charge_status_encode(
						ams_status_1.pre_charge_status);
		can1_ams_status_1.ams_error = can1_ams_status_1_ams_error_encode(
				ams_status_1.ams_error);
		can1_ams_status_1.imd_error = can1_ams_status_1_imd_error_encode(
				ams_status_1.imd_error);
		can1_ams_status_1.state_of_charge =
				can1_ams_status_1_state_of_charge_encode(
						ams_status_1.state_of_charge);
		can1_ams_status_1.min_cell_voltage =
				can1_ams_status_1_min_cell_voltage_encode(
						ams_status_1.min_cell_voltage);
		can1_ams_status_1.max_cell_voltage =
				can1_ams_status_1_max_cell_voltage_encode(
						ams_status_1.max_cell_voltage);
		can1_ams_status_1.min_cell_temperature =
				can1_ams_status_1_min_cell_temperature_encode(
						ams_status_1.min_cell_temperature);
		can1_ams_status_1.max_cell_temperature =
				can1_ams_status_1_max_cell_temperature_encode(
						ams_status_1.max_cell_temperature);
		can1_ams_status_1.fan_speed = can1_ams_status_1_fan_speed_encode(
				ams_status_1.fan_speed);

		can1_ams_status_1_pack(can_queue_element.data, &can1_ams_status_1,
		CAN1_AMS_STATUS_1_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);
	}
}

/*
 * @brief   A function which packs a can1_ams_cell_voltages to be sent (including every sub-multiplex) into the
 *          appropriate SW TX CAN queue. Make sure that the ams_cell_voltages_queue queue is filled with data before hand !
 * @retval  None
 */
void ams_cell_voltages_pack_and_send() {
	static struct can1_ams_cell_voltages_t can1_ams_cell_voltages;
	static struct ams_cell_voltages_t ams_cell_voltages;
	static struct can_queue_element_t can_queue_element = { .tx_header = {
			.DLC = CAN1_AMS_CELL_VOLTAGES_LENGTH, .IDE = CAN_ID_STD, .StdId =
			CAN1_AMS_CELL_VOLTAGES_FRAME_ID, .RTR = CAN_RTR_DATA,
			.TransmitGlobalTime = DISABLE } };
	if (xQueuePeek(ams_cell_voltages_queue, &ams_cell_voltages, 0)) {
		can1_ams_cell_voltages.v1s1 = can1_ams_cell_voltages_v1s1_encode(
				ams_cell_voltages.v1s1);
		can1_ams_cell_voltages.v7s1 = can1_ams_cell_voltages_v7s1_encode(
				ams_cell_voltages.v7s1);
		can1_ams_cell_voltages.v1s2 = can1_ams_cell_voltages_v1s2_encode(
				ams_cell_voltages.v1s2);
		can1_ams_cell_voltages.v7s2 = can1_ams_cell_voltages_v7s2_encode(
				ams_cell_voltages.v7s2);
		can1_ams_cell_voltages.v1s3 = can1_ams_cell_voltages_v1s3_encode(
				ams_cell_voltages.v1s3);
		can1_ams_cell_voltages.v7s3 = can1_ams_cell_voltages_v7s3_encode(
				ams_cell_voltages.v7s3);
		can1_ams_cell_voltages.v1s4 = can1_ams_cell_voltages_v1s4_encode(
				ams_cell_voltages.v1s4);
		can1_ams_cell_voltages.v7s4 = can1_ams_cell_voltages_v7s4_encode(
				ams_cell_voltages.v7s4);
		can1_ams_cell_voltages.v1s5 = can1_ams_cell_voltages_v1s5_encode(
				ams_cell_voltages.v1s5);
		can1_ams_cell_voltages.v7s5 = can1_ams_cell_voltages_v7s5_encode(
				ams_cell_voltages.v7s5);
		can1_ams_cell_voltages.v1s6 = can1_ams_cell_voltages_v1s6_encode(
				ams_cell_voltages.v1s6);
		can1_ams_cell_voltages.v7s6 = can1_ams_cell_voltages_v7s6_encode(
				ams_cell_voltages.v7s6);
		can1_ams_cell_voltages.v1s7 = can1_ams_cell_voltages_v1s7_encode(
				ams_cell_voltages.v1s7);
		can1_ams_cell_voltages.v7s7 = can1_ams_cell_voltages_v7s7_encode(
				ams_cell_voltages.v7s7);
		can1_ams_cell_voltages.v1s8 = can1_ams_cell_voltages_v1s8_encode(
				ams_cell_voltages.v1s8);
		can1_ams_cell_voltages.v7s8 = can1_ams_cell_voltages_v7s8_encode(
				ams_cell_voltages.v7s8);
		can1_ams_cell_voltages.v1s9 = can1_ams_cell_voltages_v1s9_encode(
				ams_cell_voltages.v1s9);
		can1_ams_cell_voltages.v7s9 = can1_ams_cell_voltages_v7s9_encode(
				ams_cell_voltages.v7s9);
		can1_ams_cell_voltages.v1s10 = can1_ams_cell_voltages_v1s10_encode(
				ams_cell_voltages.v1s10);
		can1_ams_cell_voltages.v7s10 = can1_ams_cell_voltages_v7s10_encode(
				ams_cell_voltages.v7s10);
		can1_ams_cell_voltages.v1s11 = can1_ams_cell_voltages_v1s11_encode(
				ams_cell_voltages.v1s11);
		can1_ams_cell_voltages.v7s11 = can1_ams_cell_voltages_v7s11_encode(
				ams_cell_voltages.v7s11);
		can1_ams_cell_voltages.v1s12 = can1_ams_cell_voltages_v1s12_encode(
				ams_cell_voltages.v1s12);
		can1_ams_cell_voltages.v7s12 = can1_ams_cell_voltages_v7s12_encode(
				ams_cell_voltages.v7s12);
		can1_ams_cell_voltages.v2s1 = can1_ams_cell_voltages_v2s1_encode(
				ams_cell_voltages.v2s1);
		can1_ams_cell_voltages.v8s1 = can1_ams_cell_voltages_v8s1_encode(
				ams_cell_voltages.v8s1);
		can1_ams_cell_voltages.v2s2 = can1_ams_cell_voltages_v2s2_encode(
				ams_cell_voltages.v2s2);
		can1_ams_cell_voltages.v8s2 = can1_ams_cell_voltages_v8s2_encode(
				ams_cell_voltages.v8s2);
		can1_ams_cell_voltages.v2s3 = can1_ams_cell_voltages_v2s3_encode(
				ams_cell_voltages.v2s3);
		can1_ams_cell_voltages.v8s3 = can1_ams_cell_voltages_v8s3_encode(
				ams_cell_voltages.v8s3);
		can1_ams_cell_voltages.v2s4 = can1_ams_cell_voltages_v2s4_encode(
				ams_cell_voltages.v2s4);
		can1_ams_cell_voltages.v8s4 = can1_ams_cell_voltages_v8s4_encode(
				ams_cell_voltages.v8s4);
		can1_ams_cell_voltages.v2s5 = can1_ams_cell_voltages_v2s5_encode(
				ams_cell_voltages.v2s5);
		can1_ams_cell_voltages.v8s5 = can1_ams_cell_voltages_v8s5_encode(
				ams_cell_voltages.v8s5);
		can1_ams_cell_voltages.v2s6 = can1_ams_cell_voltages_v2s6_encode(
				ams_cell_voltages.v2s6);
		can1_ams_cell_voltages.v8s6 = can1_ams_cell_voltages_v8s6_encode(
				ams_cell_voltages.v8s6);
		can1_ams_cell_voltages.v2s7 = can1_ams_cell_voltages_v2s7_encode(
				ams_cell_voltages.v2s7);
		can1_ams_cell_voltages.v8s7 = can1_ams_cell_voltages_v8s7_encode(
				ams_cell_voltages.v8s7);
		can1_ams_cell_voltages.v2s8 = can1_ams_cell_voltages_v2s8_encode(
				ams_cell_voltages.v2s8);
		can1_ams_cell_voltages.v8s8 = can1_ams_cell_voltages_v8s8_encode(
				ams_cell_voltages.v8s8);
		can1_ams_cell_voltages.v2s9 = can1_ams_cell_voltages_v2s9_encode(
				ams_cell_voltages.v2s9);
		can1_ams_cell_voltages.v8s9 = can1_ams_cell_voltages_v8s9_encode(
				ams_cell_voltages.v8s9);
		can1_ams_cell_voltages.v2s10 = can1_ams_cell_voltages_v2s10_encode(
				ams_cell_voltages.v2s10);
		can1_ams_cell_voltages.v8s10 = can1_ams_cell_voltages_v8s10_encode(
				ams_cell_voltages.v8s10);
		can1_ams_cell_voltages.v2s11 = can1_ams_cell_voltages_v2s11_encode(
				ams_cell_voltages.v2s11);
		can1_ams_cell_voltages.v8s11 = can1_ams_cell_voltages_v8s11_encode(
				ams_cell_voltages.v8s11);
		can1_ams_cell_voltages.v2s12 = can1_ams_cell_voltages_v2s12_encode(
				ams_cell_voltages.v2s12);
		can1_ams_cell_voltages.v8s12 = can1_ams_cell_voltages_v8s12_encode(
				ams_cell_voltages.v8s12);
		can1_ams_cell_voltages.v3s1 = can1_ams_cell_voltages_v3s1_encode(
				ams_cell_voltages.v3s1);
		can1_ams_cell_voltages.v9s1 = can1_ams_cell_voltages_v9s1_encode(
				ams_cell_voltages.v9s1);
		can1_ams_cell_voltages.v3s2 = can1_ams_cell_voltages_v3s2_encode(
				ams_cell_voltages.v3s2);
		can1_ams_cell_voltages.v9s2 = can1_ams_cell_voltages_v9s2_encode(
				ams_cell_voltages.v9s2);
		can1_ams_cell_voltages.v3s3 = can1_ams_cell_voltages_v3s3_encode(
				ams_cell_voltages.v3s3);
		can1_ams_cell_voltages.v9s3 = can1_ams_cell_voltages_v9s3_encode(
				ams_cell_voltages.v9s3);
		can1_ams_cell_voltages.v3s4 = can1_ams_cell_voltages_v3s4_encode(
				ams_cell_voltages.v3s4);
		can1_ams_cell_voltages.v9s4 = can1_ams_cell_voltages_v9s4_encode(
				ams_cell_voltages.v9s4);
		can1_ams_cell_voltages.v3s5 = can1_ams_cell_voltages_v3s5_encode(
				ams_cell_voltages.v3s5);
		can1_ams_cell_voltages.v9s5 = can1_ams_cell_voltages_v9s5_encode(
				ams_cell_voltages.v9s5);
		can1_ams_cell_voltages.v3s6 = can1_ams_cell_voltages_v3s6_encode(
				ams_cell_voltages.v3s6);
		can1_ams_cell_voltages.v9s6 = can1_ams_cell_voltages_v9s6_encode(
				ams_cell_voltages.v9s6);
		can1_ams_cell_voltages.v3s7 = can1_ams_cell_voltages_v3s7_encode(
				ams_cell_voltages.v3s7);
		can1_ams_cell_voltages.v9s7 = can1_ams_cell_voltages_v9s7_encode(
				ams_cell_voltages.v9s7);
		can1_ams_cell_voltages.v3s8 = can1_ams_cell_voltages_v3s8_encode(
				ams_cell_voltages.v3s8);
		can1_ams_cell_voltages.v9s8 = can1_ams_cell_voltages_v9s8_encode(
				ams_cell_voltages.v9s8);
		can1_ams_cell_voltages.v3s9 = can1_ams_cell_voltages_v3s9_encode(
				ams_cell_voltages.v3s9);
		can1_ams_cell_voltages.v9s9 = can1_ams_cell_voltages_v9s9_encode(
				ams_cell_voltages.v9s9);
		can1_ams_cell_voltages.v3s10 = can1_ams_cell_voltages_v3s10_encode(
				ams_cell_voltages.v3s10);
		can1_ams_cell_voltages.v9s10 = can1_ams_cell_voltages_v9s10_encode(
				ams_cell_voltages.v9s10);
		can1_ams_cell_voltages.v3s11 = can1_ams_cell_voltages_v3s11_encode(
				ams_cell_voltages.v3s11);
		can1_ams_cell_voltages.v9s11 = can1_ams_cell_voltages_v9s11_encode(
				ams_cell_voltages.v9s11);
		can1_ams_cell_voltages.v3s12 = can1_ams_cell_voltages_v3s12_encode(
				ams_cell_voltages.v3s12);
		can1_ams_cell_voltages.v9s12 = can1_ams_cell_voltages_v9s12_encode(
				ams_cell_voltages.v9s12);
		can1_ams_cell_voltages.v4s1 = can1_ams_cell_voltages_v4s1_encode(
				ams_cell_voltages.v4s1);
		can1_ams_cell_voltages.v10s1 = can1_ams_cell_voltages_v10s1_encode(
				ams_cell_voltages.v10s1);
		can1_ams_cell_voltages.v4s2 = can1_ams_cell_voltages_v4s2_encode(
				ams_cell_voltages.v4s2);
		can1_ams_cell_voltages.v10s2 = can1_ams_cell_voltages_v10s2_encode(
				ams_cell_voltages.v10s2);
		can1_ams_cell_voltages.v4s3 = can1_ams_cell_voltages_v4s3_encode(
				ams_cell_voltages.v4s3);
		can1_ams_cell_voltages.v10s3 = can1_ams_cell_voltages_v10s3_encode(
				ams_cell_voltages.v10s3);
		can1_ams_cell_voltages.v4s4 = can1_ams_cell_voltages_v4s4_encode(
				ams_cell_voltages.v4s4);
		can1_ams_cell_voltages.v10s4 = can1_ams_cell_voltages_v10s4_encode(
				ams_cell_voltages.v10s4);
		can1_ams_cell_voltages.v4s5 = can1_ams_cell_voltages_v4s5_encode(
				ams_cell_voltages.v4s5);
		can1_ams_cell_voltages.v10s5 = can1_ams_cell_voltages_v10s5_encode(
				ams_cell_voltages.v10s5);
		can1_ams_cell_voltages.v4s6 = can1_ams_cell_voltages_v4s6_encode(
				ams_cell_voltages.v4s6);
		can1_ams_cell_voltages.v10s6 = can1_ams_cell_voltages_v10s6_encode(
				ams_cell_voltages.v10s6);
		can1_ams_cell_voltages.v4s7 = can1_ams_cell_voltages_v4s7_encode(
				ams_cell_voltages.v4s7);
		can1_ams_cell_voltages.v10s7 = can1_ams_cell_voltages_v10s7_encode(
				ams_cell_voltages.v10s7);
		can1_ams_cell_voltages.v4s8 = can1_ams_cell_voltages_v4s8_encode(
				ams_cell_voltages.v4s8);
		can1_ams_cell_voltages.v10s8 = can1_ams_cell_voltages_v10s8_encode(
				ams_cell_voltages.v10s8);
		can1_ams_cell_voltages.v4s9 = can1_ams_cell_voltages_v4s9_encode(
				ams_cell_voltages.v4s9);
		can1_ams_cell_voltages.v10s9 = can1_ams_cell_voltages_v10s9_encode(
				ams_cell_voltages.v10s9);
		can1_ams_cell_voltages.v4s10 = can1_ams_cell_voltages_v4s10_encode(
				ams_cell_voltages.v4s10);
		can1_ams_cell_voltages.v10s10 = can1_ams_cell_voltages_v10s10_encode(
				ams_cell_voltages.v10s10);
		can1_ams_cell_voltages.v4s11 = can1_ams_cell_voltages_v4s11_encode(
				ams_cell_voltages.v4s11);
		can1_ams_cell_voltages.v10s11 = can1_ams_cell_voltages_v10s11_encode(
				ams_cell_voltages.v10s11);
		can1_ams_cell_voltages.v4s12 = can1_ams_cell_voltages_v4s12_encode(
				ams_cell_voltages.v4s12);
		can1_ams_cell_voltages.v10s12 = can1_ams_cell_voltages_v10s12_encode(
				ams_cell_voltages.v10s12);
		can1_ams_cell_voltages.v5s1 = can1_ams_cell_voltages_v5s1_encode(
				ams_cell_voltages.v5s1);
		can1_ams_cell_voltages.v11s1 = can1_ams_cell_voltages_v11s1_encode(
				ams_cell_voltages.v11s1);
		can1_ams_cell_voltages.v5s2 = can1_ams_cell_voltages_v5s2_encode(
				ams_cell_voltages.v5s2);
		can1_ams_cell_voltages.v5s3 = can1_ams_cell_voltages_v5s3_encode(
				ams_cell_voltages.v5s3);
		can1_ams_cell_voltages.v11s3 = can1_ams_cell_voltages_v11s3_encode(
				ams_cell_voltages.v11s3);
		can1_ams_cell_voltages.v5s4 = can1_ams_cell_voltages_v5s4_encode(
				ams_cell_voltages.v5s4);
		can1_ams_cell_voltages.v5s5 = can1_ams_cell_voltages_v5s5_encode(
				ams_cell_voltages.v5s5);
		can1_ams_cell_voltages.v11s5 = can1_ams_cell_voltages_v11s5_encode(
				ams_cell_voltages.v11s5);
		can1_ams_cell_voltages.v5s6 = can1_ams_cell_voltages_v5s6_encode(
				ams_cell_voltages.v5s6);
		can1_ams_cell_voltages.v5s7 = can1_ams_cell_voltages_v5s7_encode(
				ams_cell_voltages.v5s7);
		can1_ams_cell_voltages.v11s7 = can1_ams_cell_voltages_v11s7_encode(
				ams_cell_voltages.v11s7);
		can1_ams_cell_voltages.v5s8 = can1_ams_cell_voltages_v5s8_encode(
				ams_cell_voltages.v5s8);
		can1_ams_cell_voltages.v5s9 = can1_ams_cell_voltages_v5s9_encode(
				ams_cell_voltages.v5s9);
		can1_ams_cell_voltages.v11s9 = can1_ams_cell_voltages_v11s9_encode(
				ams_cell_voltages.v11s9);
		can1_ams_cell_voltages.v5s10 = can1_ams_cell_voltages_v5s10_encode(
				ams_cell_voltages.v5s10);
		can1_ams_cell_voltages.v5s11 = can1_ams_cell_voltages_v5s11_encode(
				ams_cell_voltages.v5s11);
		can1_ams_cell_voltages.v11s11 = can1_ams_cell_voltages_v11s11_encode(
				ams_cell_voltages.v11s11);
		can1_ams_cell_voltages.v5s12 = can1_ams_cell_voltages_v5s12_encode(
				ams_cell_voltages.v5s12);
		can1_ams_cell_voltages.v6s1 = can1_ams_cell_voltages_v6s1_encode(
				ams_cell_voltages.v6s1);
		can1_ams_cell_voltages.v6s2 = can1_ams_cell_voltages_v6s2_encode(
				ams_cell_voltages.v6s2);
		can1_ams_cell_voltages.v6s3 = can1_ams_cell_voltages_v6s3_encode(
				ams_cell_voltages.v6s3);
		can1_ams_cell_voltages.v6s4 = can1_ams_cell_voltages_v6s4_encode(
				ams_cell_voltages.v6s4);
		can1_ams_cell_voltages.v6s5 = can1_ams_cell_voltages_v6s5_encode(
				ams_cell_voltages.v6s5);
		can1_ams_cell_voltages.v6s6 = can1_ams_cell_voltages_v6s6_encode(
				ams_cell_voltages.v6s6);
		can1_ams_cell_voltages.v6s7 = can1_ams_cell_voltages_v6s7_encode(
				ams_cell_voltages.v6s7);
		can1_ams_cell_voltages.v6s8 = can1_ams_cell_voltages_v6s8_encode(
				ams_cell_voltages.v6s8);
		can1_ams_cell_voltages.v6s9 = can1_ams_cell_voltages_v6s9_encode(
				ams_cell_voltages.v6s9);
		can1_ams_cell_voltages.v6s10 = can1_ams_cell_voltages_v6s10_encode(
				ams_cell_voltages.v6s10);
		can1_ams_cell_voltages.v6s11 = can1_ams_cell_voltages_v6s11_encode(
				ams_cell_voltages.v6s11);
		can1_ams_cell_voltages.v6s12 = can1_ams_cell_voltages_v6s12_encode(
				ams_cell_voltages.v6s12);

		can1_ams_cell_voltages.voltage_multiplexor = 0;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 1;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 2;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 3;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 4;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 5;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 6;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 7;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 8;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 9;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 10;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 11;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 12;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 13;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 14;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 15;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 16;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 17;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 18;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 19;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 20;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 21;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_voltages.voltage_multiplexor = 22;
		can1_ams_cell_voltages_pack(can_queue_element.data,
				&can1_ams_cell_voltages, CAN1_AMS_CELL_VOLTAGES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

	}
}

/*
 * @brief   A function which packs a can1_ams_cell_temperatures to be sent (including every sub-multiplex) into the
 *          appropriate SW TX CAN queue. Make sure that the ams_cell_temperatures_queue queue is filled with data before hand !
 * @retval  None
 */
void ams_cell_temperatures_pack_and_send() {
	static struct can1_ams_cell_temperatures_t can1_ams_cell_temperatures;
	static struct ams_cell_temperatures_t ams_cell_temperatures;
	static struct can_queue_element_t can_queue_element = { .tx_header = {
			.DLC = CAN1_AMS_CELL_TEMPERATURES_LENGTH, .IDE = CAN_ID_STD,
			.StdId = CAN1_AMS_CELL_TEMPERATURES_FRAME_ID, .RTR = CAN_RTR_DATA,
			.TransmitGlobalTime = DISABLE } };
	if (xQueuePeek(ams_cell_temperatures_queue, &ams_cell_temperatures, 0)) {
		can1_ams_cell_temperatures.t1s1 =
				can1_ams_cell_temperatures_t1s1_encode(
						ams_cell_temperatures.t1s1);
		can1_ams_cell_temperatures.t1s2 =
				can1_ams_cell_temperatures_t1s2_encode(
						ams_cell_temperatures.t1s2);
		can1_ams_cell_temperatures.t1s3 =
				can1_ams_cell_temperatures_t1s3_encode(
						ams_cell_temperatures.t1s3);
		can1_ams_cell_temperatures.t1s4 =
				can1_ams_cell_temperatures_t1s4_encode(
						ams_cell_temperatures.t1s4);
		can1_ams_cell_temperatures.t1s5 =
				can1_ams_cell_temperatures_t1s5_encode(
						ams_cell_temperatures.t1s5);
		can1_ams_cell_temperatures.t1s6 =
				can1_ams_cell_temperatures_t1s6_encode(
						ams_cell_temperatures.t1s6);
		can1_ams_cell_temperatures.t1s7 =
				can1_ams_cell_temperatures_t1s7_encode(
						ams_cell_temperatures.t1s7);
		can1_ams_cell_temperatures.t1s8 =
				can1_ams_cell_temperatures_t1s8_encode(
						ams_cell_temperatures.t1s8);
		can1_ams_cell_temperatures.t1s9 =
				can1_ams_cell_temperatures_t1s9_encode(
						ams_cell_temperatures.t1s9);
		can1_ams_cell_temperatures.t1s10 =
				can1_ams_cell_temperatures_t1s10_encode(
						ams_cell_temperatures.t1s10);
		can1_ams_cell_temperatures.t1s11 =
				can1_ams_cell_temperatures_t1s11_encode(
						ams_cell_temperatures.t1s11);
		can1_ams_cell_temperatures.t1s12 =
				can1_ams_cell_temperatures_t1s12_encode(
						ams_cell_temperatures.t1s12);
		can1_ams_cell_temperatures.t2s1 =
				can1_ams_cell_temperatures_t2s1_encode(
						ams_cell_temperatures.t2s1);
		can1_ams_cell_temperatures.t2s2 =
				can1_ams_cell_temperatures_t2s2_encode(
						ams_cell_temperatures.t2s2);
		can1_ams_cell_temperatures.t2s3 =
				can1_ams_cell_temperatures_t2s3_encode(
						ams_cell_temperatures.t2s3);
		can1_ams_cell_temperatures.t2s4 =
				can1_ams_cell_temperatures_t2s4_encode(
						ams_cell_temperatures.t2s4);
		can1_ams_cell_temperatures.t2s5 =
				can1_ams_cell_temperatures_t2s5_encode(
						ams_cell_temperatures.t2s5);
		can1_ams_cell_temperatures.t2s6 =
				can1_ams_cell_temperatures_t2s6_encode(
						ams_cell_temperatures.t2s6);
		can1_ams_cell_temperatures.t2s7 =
				can1_ams_cell_temperatures_t2s7_encode(
						ams_cell_temperatures.t2s7);
		can1_ams_cell_temperatures.t2s8 =
				can1_ams_cell_temperatures_t2s8_encode(
						ams_cell_temperatures.t2s8);
		can1_ams_cell_temperatures.t2s9 =
				can1_ams_cell_temperatures_t2s9_encode(
						ams_cell_temperatures.t2s9);
		can1_ams_cell_temperatures.t2s10 =
				can1_ams_cell_temperatures_t2s10_encode(
						ams_cell_temperatures.t2s10);
		can1_ams_cell_temperatures.t2s11 =
				can1_ams_cell_temperatures_t2s11_encode(
						ams_cell_temperatures.t2s11);
		can1_ams_cell_temperatures.t2s12 =
				can1_ams_cell_temperatures_t2s12_encode(
						ams_cell_temperatures.t2s12);
		can1_ams_cell_temperatures.t3s1 =
				can1_ams_cell_temperatures_t3s1_encode(
						ams_cell_temperatures.t3s1);
		can1_ams_cell_temperatures.t3s2 =
				can1_ams_cell_temperatures_t3s2_encode(
						ams_cell_temperatures.t3s2);
		can1_ams_cell_temperatures.t3s3 =
				can1_ams_cell_temperatures_t3s3_encode(
						ams_cell_temperatures.t3s3);
		can1_ams_cell_temperatures.t3s4 =
				can1_ams_cell_temperatures_t3s4_encode(
						ams_cell_temperatures.t3s4);
		can1_ams_cell_temperatures.t3s5 =
				can1_ams_cell_temperatures_t3s5_encode(
						ams_cell_temperatures.t3s5);
		can1_ams_cell_temperatures.t3s6 =
				can1_ams_cell_temperatures_t3s6_encode(
						ams_cell_temperatures.t3s6);
		can1_ams_cell_temperatures.t3s7 =
				can1_ams_cell_temperatures_t3s7_encode(
						ams_cell_temperatures.t3s7);
		can1_ams_cell_temperatures.t3s8 =
				can1_ams_cell_temperatures_t3s8_encode(
						ams_cell_temperatures.t3s8);
		can1_ams_cell_temperatures.t3s9 =
				can1_ams_cell_temperatures_t3s9_encode(
						ams_cell_temperatures.t3s9);
		can1_ams_cell_temperatures.t3s10 =
				can1_ams_cell_temperatures_t3s10_encode(
						ams_cell_temperatures.t3s10);
		can1_ams_cell_temperatures.t3s11 =
				can1_ams_cell_temperatures_t3s11_encode(
						ams_cell_temperatures.t3s11);
		can1_ams_cell_temperatures.t3s12 =
				can1_ams_cell_temperatures_t3s12_encode(
						ams_cell_temperatures.t3s12);
		can1_ams_cell_temperatures.t4s1 =
				can1_ams_cell_temperatures_t4s1_encode(
						ams_cell_temperatures.t4s1);
		can1_ams_cell_temperatures.t4s2 =
				can1_ams_cell_temperatures_t4s2_encode(
						ams_cell_temperatures.t4s2);
		can1_ams_cell_temperatures.t4s3 =
				can1_ams_cell_temperatures_t4s3_encode(
						ams_cell_temperatures.t4s3);
		can1_ams_cell_temperatures.t4s4 =
				can1_ams_cell_temperatures_t4s4_encode(
						ams_cell_temperatures.t4s4);
		can1_ams_cell_temperatures.t4s5 =
				can1_ams_cell_temperatures_t4s5_encode(
						ams_cell_temperatures.t4s5);
		can1_ams_cell_temperatures.t4s6 =
				can1_ams_cell_temperatures_t4s6_encode(
						ams_cell_temperatures.t4s6);
		can1_ams_cell_temperatures.t4s7 =
				can1_ams_cell_temperatures_t4s7_encode(
						ams_cell_temperatures.t4s7);
		can1_ams_cell_temperatures.t4s8 =
				can1_ams_cell_temperatures_t4s8_encode(
						ams_cell_temperatures.t4s8);
		can1_ams_cell_temperatures.t4s9 =
				can1_ams_cell_temperatures_t4s9_encode(
						ams_cell_temperatures.t4s9);
		can1_ams_cell_temperatures.t4s10 =
				can1_ams_cell_temperatures_t4s10_encode(
						ams_cell_temperatures.t4s10);
		can1_ams_cell_temperatures.t4s11 =
				can1_ams_cell_temperatures_t4s11_encode(
						ams_cell_temperatures.t4s11);
		can1_ams_cell_temperatures.t4s12 =
				can1_ams_cell_temperatures_t4s12_encode(
						ams_cell_temperatures.t4s12);
		can1_ams_cell_temperatures.t5s1 =
				can1_ams_cell_temperatures_t5s1_encode(
						ams_cell_temperatures.t5s1);
		can1_ams_cell_temperatures.t5s2 =
				can1_ams_cell_temperatures_t5s2_encode(
						ams_cell_temperatures.t5s2);
		can1_ams_cell_temperatures.t5s3 =
				can1_ams_cell_temperatures_t5s3_encode(
						ams_cell_temperatures.t5s3);
		can1_ams_cell_temperatures.t5s4 =
				can1_ams_cell_temperatures_t5s4_encode(
						ams_cell_temperatures.t5s4);
		can1_ams_cell_temperatures.t5s5 =
				can1_ams_cell_temperatures_t5s5_encode(
						ams_cell_temperatures.t5s5);
		can1_ams_cell_temperatures.t5s6 =
				can1_ams_cell_temperatures_t5s6_encode(
						ams_cell_temperatures.t5s6);
		can1_ams_cell_temperatures.t5s7 =
				can1_ams_cell_temperatures_t5s7_encode(
						ams_cell_temperatures.t5s7);
		can1_ams_cell_temperatures.t5s8 =
				can1_ams_cell_temperatures_t5s8_encode(
						ams_cell_temperatures.t5s8);
		can1_ams_cell_temperatures.t5s9 =
				can1_ams_cell_temperatures_t5s9_encode(
						ams_cell_temperatures.t5s9);
		can1_ams_cell_temperatures.t5s10 =
				can1_ams_cell_temperatures_t5s10_encode(
						ams_cell_temperatures.t5s10);
		can1_ams_cell_temperatures.t5s11 =
				can1_ams_cell_temperatures_t5s11_encode(
						ams_cell_temperatures.t5s11);
		can1_ams_cell_temperatures.t5s12 =
				can1_ams_cell_temperatures_t5s12_encode(
						ams_cell_temperatures.t5s12);

		can1_ams_cell_temperatures.temperature_multiplexor = 0;
		can1_ams_cell_temperatures_pack(can_queue_element.data,
				&can1_ams_cell_temperatures, CAN1_AMS_CELL_TEMPERATURES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_temperatures.temperature_multiplexor = 1;
		can1_ams_cell_temperatures_pack(can_queue_element.data,
				&can1_ams_cell_temperatures, CAN1_AMS_CELL_TEMPERATURES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_temperatures.temperature_multiplexor = 2;
		can1_ams_cell_temperatures_pack(can_queue_element.data,
				&can1_ams_cell_temperatures, CAN1_AMS_CELL_TEMPERATURES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_temperatures.temperature_multiplexor = 3;
		can1_ams_cell_temperatures_pack(can_queue_element.data,
				&can1_ams_cell_temperatures, CAN1_AMS_CELL_TEMPERATURES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_temperatures.temperature_multiplexor = 4;
		can1_ams_cell_temperatures_pack(can_queue_element.data,
				&can1_ams_cell_temperatures, CAN1_AMS_CELL_TEMPERATURES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_temperatures.temperature_multiplexor = 5;
		can1_ams_cell_temperatures_pack(can_queue_element.data,
				&can1_ams_cell_temperatures, CAN1_AMS_CELL_TEMPERATURES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_temperatures.temperature_multiplexor = 6;
		can1_ams_cell_temperatures_pack(can_queue_element.data,
				&can1_ams_cell_temperatures, CAN1_AMS_CELL_TEMPERATURES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_temperatures.temperature_multiplexor = 7;
		can1_ams_cell_temperatures_pack(can_queue_element.data,
				&can1_ams_cell_temperatures, CAN1_AMS_CELL_TEMPERATURES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_temperatures.temperature_multiplexor = 8;
		can1_ams_cell_temperatures_pack(can_queue_element.data,
				&can1_ams_cell_temperatures, CAN1_AMS_CELL_TEMPERATURES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_temperatures.temperature_multiplexor = 9;
		can1_ams_cell_temperatures_pack(can_queue_element.data,
				&can1_ams_cell_temperatures, CAN1_AMS_CELL_TEMPERATURES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

		can1_ams_cell_temperatures.temperature_multiplexor = 10;
		can1_ams_cell_temperatures_pack(can_queue_element.data,
				&can1_ams_cell_temperatures, CAN1_AMS_CELL_TEMPERATURES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);

	}
}

/*
 * @brief   A function which packs a can1_ams_temperatures to be sent (including every sub-multiplex) into the
 *          appropriate SW TX CAN queue. Make sure that the ams_temperatures_queue queue is filled with data before hand !
 * @retval  None
 */
void ams_temperatures_pack_and_send() {
	static struct can1_ams_temperatures_t can1_ams_temperatures;
	static struct ams_temperatures_t ams_temperatures;
	static struct can_queue_element_t can_queue_element = { .tx_header = {
			.DLC = CAN1_AMS_TEMPERATURES_LENGTH, .IDE = CAN_ID_STD, .StdId =
			CAN1_AMS_TEMPERATURES_FRAME_ID, .RTR = CAN_RTR_DATA,
			.TransmitGlobalTime = DISABLE } };
	if (xQueuePeek(ams_temperatures_queue, &ams_temperatures, 0)) {
		can1_ams_temperatures.pre_charge_resistor_temperature =
				can1_ams_temperatures_pre_charge_resistor_temperature_encode(
						ams_temperatures.pre_charge_resistor_temperature);
		can1_ams_temperatures.fuse_resistor_temperature =
				can1_ams_temperatures_fuse_resistor_temperature_encode(
						ams_temperatures.fuse_resistor_temperature);
		can1_ams_temperatures.aux_1_temperature =
				can1_ams_temperatures_aux_1_temperature_encode(
						ams_temperatures.aux_1_temperature);
		can1_ams_temperatures.aux_2_temperature =
				can1_ams_temperatures_aux_2_temperature_encode(
						ams_temperatures.aux_2_temperature);

		can1_ams_temperatures_pack(can_queue_element.data,
				&can1_ams_temperatures, CAN1_AMS_TEMPERATURES_LENGTH);
		xQueueSend(can1_tx_queue, &can_queue_element, 0);
	}
}


/*
 * @brief   This function configures the AMS to only receive the messages that it is specified to receive
 *          Currently, the system is not smart enough to use masks or group messages in pairs, and will
 *          therefore use an excessive amount of filters.
 * @retval  None
 */
void can1_filter_config() {
	CAN_FilterTypeDef filter_config;

	filter_config.FilterActivation = ENABLE;
	filter_config.FilterBank = 0;
	filter_config.FilterMode = CAN_FILTERMODE_IDLIST;
	filter_config.FilterIdHigh = CAN1_DBU_STATUS_1_FRAME_ID << 5;
	filter_config.FilterIdLow = 0x0000;
	filter_config.FilterMaskIdHigh = 0x0000;
	filter_config.FilterMaskIdLow = 0x0000;
	filter_config.FilterScale = CAN_FILTERSCALE_16BIT;

	filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	if (HAL_CAN_ConfigFilter(_hcan1, &filter_config) != HAL_OK) {
		Error_Handler();
	}

	filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO1;
	if (HAL_CAN_ConfigFilter(_hcan1, &filter_config) != HAL_OK) {
		Error_Handler();
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

	if (HAL_CAN_ConfigFilter(_hcan2, &filter_config) != HAL_OK) {
		Error_Handler();
	}

	filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO1;
	if (HAL_CAN_ConfigFilter(_hcan2, &filter_config) != HAL_OK) {
		Error_Handler();
	}
}

/*
 * @brief   This function configures the AMS to only receive the messages that it is specified to receive
 *          Currently, the system is not smart enough to use masks or group messages in pairs, and will
 *          therefore use an excessive amount of filters.
 * @retval  None
 */
void can2_filter_config() {
	CAN_FilterTypeDef filter_config;

	filter_config.FilterActivation = ENABLE;
	filter_config.FilterBank = 0;
	filter_config.FilterMode = CAN_FILTERMODE_IDLIST;
	filter_config.FilterIdHigh = CAN2_IVT_MSG_RESULT_U3_FRAME_ID << 5;
	filter_config.FilterIdLow = 0x0000;
	filter_config.FilterMaskIdHigh = 0x0000;
	filter_config.FilterMaskIdLow = 0x0000;
	filter_config.FilterScale = CAN_FILTERSCALE_16BIT;

	filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	if (HAL_CAN_ConfigFilter(_hcan2, &filter_config) != HAL_OK) {
		Error_Handler();
	}

	filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO1;
	if (HAL_CAN_ConfigFilter(_hcan2, &filter_config) != HAL_OK) {
		Error_Handler();
	}
	filter_config.FilterActivation = ENABLE;
	filter_config.FilterBank = 1;
	filter_config.FilterMode = CAN_FILTERMODE_IDLIST;
	filter_config.FilterIdHigh = CAN2_IVT_MSG_RESULT_I_FRAME_ID << 5;
	filter_config.FilterIdLow = 0x0000;
	filter_config.FilterMaskIdHigh = 0x0000;
	filter_config.FilterMaskIdLow = 0x0000;
	filter_config.FilterScale = CAN_FILTERSCALE_16BIT;

	filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	if (HAL_CAN_ConfigFilter(_hcan2, &filter_config) != HAL_OK) {
		Error_Handler();
	}

	filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO1;
	if (HAL_CAN_ConfigFilter(_hcan2, &filter_config) != HAL_OK) {
		Error_Handler();
	}
	filter_config.FilterActivation = ENABLE;
	filter_config.FilterBank = 2;
	filter_config.FilterMode = CAN_FILTERMODE_IDLIST;
	filter_config.FilterIdHigh = CAN2_IVT_MSG_RESULT_U1_FRAME_ID << 5;
	filter_config.FilterIdLow = 0x0000;
	filter_config.FilterMaskIdHigh = 0x0000;
	filter_config.FilterMaskIdLow = 0x0000;
	filter_config.FilterScale = CAN_FILTERSCALE_16BIT;

	filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	if (HAL_CAN_ConfigFilter(_hcan2, &filter_config) != HAL_OK) {
		Error_Handler();
	}

	filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO1;
	if (HAL_CAN_ConfigFilter(_hcan2, &filter_config) != HAL_OK) {
		Error_Handler();
	}
	filter_config.FilterActivation = ENABLE;
	filter_config.FilterBank = 3;
	filter_config.FilterMode = CAN_FILTERMODE_IDLIST;
	filter_config.FilterIdHigh = CAN2_IVT_MSG_RESULT_U2_FRAME_ID << 5;
	filter_config.FilterIdLow = 0x0000;
	filter_config.FilterMaskIdHigh = 0x0000;
	filter_config.FilterMaskIdLow = 0x0000;
	filter_config.FilterScale = CAN_FILTERSCALE_16BIT;

	filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	if (HAL_CAN_ConfigFilter(_hcan2, &filter_config) != HAL_OK) {
		Error_Handler();
	}

	filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO1;
	if (HAL_CAN_ConfigFilter(_hcan2, &filter_config) != HAL_OK) {
		Error_Handler();
	}
}
