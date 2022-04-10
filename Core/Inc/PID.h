/*
 * PID.h
 *
 *  Created on: Mar 10, 2022
 *      Author: oscar
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include "main.h"

#define ARM_MATH_CM4

#include "arm_math.h"

/*
 * A fairly general purpose pid controller built on the arm_math.h library, utilizing the FPU on the micro controller.
 * Has functions for initialization, progression, wind-up, parameter assertion
 */

typedef struct {
	/* Simply input and reference, their difference (error), and the output */
	float error;
	float input;
	float output;
	float reference;

	/* One should put a lower and upper bound on the output. For pid one might use 0 and 100 */
	float output_upper_bound;
	float output_lower_bound;

	/* The arm_math core structure */
	arm_pid_instance_f32 controller;
} PID_t;


/*
 * @brief	Progress the PID
 * @param	Input at current timestep
 * @retval	1 if it went through, 0 else
 */
uint8_t PID_progress(PID_t*, float);

/*
 * @brief	Initialize the PID. Always resets the controller.
 * @param	Pointer to the PID struct to initialize
 * @param	Reference value, sought value
 * @param	proportionate component
 * @param	integral component
 * @param	derivative component
 * @param	output lower bound, ex 0 for PWM
 * @param	output upper bound, ex 100 for PWM
 * @retval	1 if it went through, 0 else
 */
uint8_t PID_initialize(PID_t*, float, float, float, float,
		float, float);

/*
 * @brief	Change output bounds, in a safe manner
 * @param	output lower bound, ex 0 for PWM
 * @param	output upper bound, ex 100 for PWM
 * @retval	1 if it went through, 0 else
 */
uint8_t PID_set_bounds(PID_t*, float, float);

/*
 * @brief	Initialize the PID. Always resets the controller.
 * @param	Pointer to the PID struct to initialize
 * @param	proportionate component
 * @param	integral component
 * @param	derivative component
 * @retval	1 if it went through, 0 else
 */
uint8_t PID_set_parameters(PID_t*, float, float, float);

/*
 * @brief	Set the PID reference
 * @param	Pointer to the PID struct to initialize
 * @param	Reference value, sought value
 * @param	integral component
 * @param	derivative component
 * @retval	1 if it went through, 0 else
 */
uint8_t PID_set_reference(PID_t*, float);

/*
 * @brief	Resets the PID without altering parameters.
 * @retval	1 if it went through, 0 else
 */
uint8_t PID_wind_up(PID_t*);

#endif /* INC_PID_H_ */
