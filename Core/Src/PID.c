/*
 * PID.c
 *
 *  Created on: Mar 10, 2022
 *      Author: oscar
 */


#include "stdlib.h"
#include "stdint.h"
#include "PID.h"


/*
 * @brief	Progress the PID
 * @param	Input at current timestep
 * @retval	1 if it went through, 0 else
 */
uint8_t PID_progress(PID_t *ctrl, float input) {
	ctrl->input = input;
	ctrl->error = ctrl->input - ctrl->reference;

	ctrl->output = arm_pid_f32(&(ctrl->controller), ctrl->error);

	if (ctrl->output < ctrl->output_lower_bound) {
		ctrl->output = ctrl->output_lower_bound;
	}

	if (ctrl->output_upper_bound < ctrl->output) {
		ctrl->output = ctrl->output_upper_bound;
	}

	return 1;
}

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
uint8_t PID_initialize(PID_t *ctrl, float reference,
		float proportional, float integral, float derivative,
		float output_lower_bound, float output_upper_bound) {
	if (output_upper_bound < output_lower_bound) {
		return 0;
	}

	ctrl->error = 0.;
	ctrl->input = 0.;
	ctrl->output = 0.;
	ctrl->reference = reference;

	ctrl->output_upper_bound = output_upper_bound;
	ctrl->output_lower_bound = output_lower_bound;

	ctrl->controller.Kp = proportional;
	ctrl->controller.Ki = integral;
	ctrl->controller.Kd = derivative;

	arm_pid_init_f32(&(ctrl->controller), 1);

	return 1;
}

/*
 * @brief	Change output bounds, in a safe manner
 * @param	output lower bound, ex 0 for PWM
 * @param	output upper bound, ex 100 for PWM
 * @retval	1 if it went through, 0 else
 */
uint8_t PID_set_bounds(PID_t *ctrl, float output_lower_bound,
		float output_upper_bound) {
	if (output_upper_bound < output_lower_bound) {
		return 0;
	}

	ctrl->output_upper_bound = output_upper_bound;
	ctrl->output_lower_bound = output_lower_bound;

	return 1;
}

/*
 * @brief	Initialize the PID. Always resets the controller.
 * @param	Pointer to the PID struct to initialize
 * @param	proportionate component
 * @param	integral component
 * @param	derivative component
 * @retval	1 if it went through, 0 else
 */
uint8_t PID_set_parameters(PID_t *ctrl, float proportional,
		float integral, float derivative) {
	ctrl->controller.Kp = proportional;
	ctrl->controller.Ki = integral;
	ctrl->controller.Kd = derivative;

	arm_pid_init_f32(&(ctrl->controller), 0);

	return 1;
}

/*
 * @brief	Set the PID reference
 * @param	Pointer to the PID struct to initialize
 * @param	Reference value, sought value
 * @param	integral component
 * @param	derivative component
 * @retval	1 if it went through, 0 else
 */
uint8_t PID_set_reference(PID_t *ctrl, float reference) {

	ctrl->reference = reference;

	return 1;
}

/*
 * @brief	Resets the PID without altering parameters.
 * @retval	1 if it went through, 0 else
 */
uint8_t PID_wind_up(PID_t *ctrl) {

	arm_pid_init_f32(&(ctrl->controller), 1);

	return 1;
}
