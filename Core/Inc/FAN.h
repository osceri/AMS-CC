/*
 * FAN.h
 *
 *  Created on: Apr 10, 2022
 *      Author: oscar
 */

#ifndef INC_FAN_H_
#define INC_FAN_H_

void FAN_initialize(TIM_HandleTypeDef *htim);

void FAN_duty_cycle(TIM_HandleTypeDef *htim, float duty_cycle);

#endif /* INC_FAN_H_ */
