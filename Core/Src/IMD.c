/*
 * IMD.c
 *
 *  Created on: Apr 10, 2022
 *      Author: oscar
 */

#include "main.h"

#include "IMD.h"
#include "programme_data.h"
#include "programme_queues.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	IMD_t IMD;
	float ante = 0;
	float conq = 0;

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (htim->Instance == TIM2) {
		ante = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		conq = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

		IMD.duty_cycle = ante / conq;
		IMD.frequency = 100000000 / (conq + 1);

		xQueueOverwriteFromISR(IMD_queue, &IMD, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

void initialize_IMD(TIM_HandleTypeDef *htim) {
	htim->Instance->ARR = 42000000;

	if (HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_TIM_IC_Start(htim, TIM_CHANNEL_2) != HAL_OK) {
		Error_Handler();
	}
}
