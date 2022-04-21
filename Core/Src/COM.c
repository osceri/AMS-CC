/*
 * COM.c
 *
 *  Created on: Apr 10, 2022
 *      Author: oscar
 */

#include "main.h"
#include "programme_data.h"

static uint8_t over_voltages[126];
static uint8_t under_voltages[126];

static uint8_t over_temperatures[60];
static uint8_t under_temperatures[60];

static uint8_t over_current;
static uint8_t under_current;

/*
 * @Brief	Increments if true, resets (to zero) otherwise
 * @Param	A conditional statement which evaluates to true or false
 * @Param	A pointer to an integer which may be incremented or reset
 * @Retval	Void
 */
static void inc_res(uint8_t cnd, uint8_t *ptr) {
	if (cnd) {
		ptr[0]++;
	} else {
		ptr[0] = 0;
	}
}

/*
 * @Brief	Checks whether the voltages are within range, or if they have been indeterminate for too long.
 * @Param	A vector of voltages to check
 * @Param	The sample constraint. How many consecutive samples may be wrong before an error is triggered
 * @retval	An error code
 */
error_t COM_voltages_ok(double *voltages,
		uint16_t sample_constraint) {
	const float upper_boundary = 4.19;
	const float lower_boundary = 2.81;

	for (int p = 0; p < 126; p++) {
		inc_res(voltages[p] > upper_boundary, &over_voltages[p]);
		inc_res(lower_boundary > voltages[p], &under_voltages[p]);

		if (over_voltages[p] > sample_constraint) {
			return ERROR_OVER_VOLTAGE;
		}
		if (under_voltages[p] > sample_constraint) {
			return ERROR_UNDER_VOLTAGE;
		}
	}

	return 0;

}


/*
 * @Brief	Checks whether the temperatures are within range, or if they have been indeterminate for too long.
 * @Param	A vector of temperatures to check
 * @Param	The sample constraint. How many consecutive samples may be wrong before an error is triggered
 * @retval	An error code
 */
error_t COM_temperatures_ok(double *temperatures,
		uint16_t sample_constraint) {
	const float upper_boundary = 60;
	const float lower_boundary = -20;

	for (int p = 0; p < 60; p++) {
		inc_res(temperatures[p] > upper_boundary,
				&over_temperatures[p]);
		inc_res(lower_boundary > temperatures[p],
				&under_temperatures[p]);

		if (over_temperatures[p] > sample_constraint) {
			return ERROR_OVER_TEMPERATURE;
		}
		if (under_temperatures[p] > sample_constraint) {
			return ERROR_UNDER_TEMPERATURE;
		}
	}

	return 0;

}


/*
 * @Brief	Checks whether the current is within range, or if it has been indeterminate for too long.
 * @Param	A single current to check
 * @Param	The sample constraint. How many consecutive samples may be wrong before an error is triggered
 * @retval	An error code
 */
error_t COM_current_ok(double *current,
		uint16_t sample_constraint) {
	const float upper_boundary = 100;
	const float lower_boundary = -40;

	inc_res(current[0] > upper_boundary, &over_current);
	inc_res(lower_boundary > current[0], &under_current);

	if (over_current > sample_constraint) {
		return ERROR_OVER_CURRENT;
	}
	if (under_current > sample_constraint) {
		return ERROR_UNDER_CURRENT;
	}

	return 0;

}

