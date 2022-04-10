/*
 * FAN.c
 *
 *  Created on: Apr 10, 2022
 *      Author: oscar
 */

#ifndef SRC_FAN_C_
#define SRC_FAN_C_

#include "main.h"
#include "programme_data.h"

#include "FAN.h"

void FAN_initialize(TIM_HandleTypeDef *htim) {
	const float frequency = 1000;

	htim->Instance->ARR = 65535 / frequency;
	htim->Instance->CCR3 = htim->Instance->ARR / 2;
	htim->Instance->CNT = 0;
	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_3);

}

void FAN_duty_cycle(TIM_HandleTypeDef *htim, float duty_cycle) {
	if (duty_cycle < 0) {
		duty_cycle = 0;
	}
	if (100 < duty_cycle) {
		duty_cycle = 100;
	}

	htim->Instance->CCR3 = (duty_cycle / 100) * htim->Instance->ARR;
	htim->Instance->CNT = 0;
}

#endif /* SRC_FAN_C_ */
