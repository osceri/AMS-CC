/*
 * COM.h
 *
 *  Created on: Apr 10, 2022
 *      Author: oscar
 */

#ifndef INC_COM_H_
#define INC_COM_H_


/*
 * @Brief	Checks whether the voltages are within range, or if they have been indeterminate for too long.
 * @Param	A vector of voltages to check
 * @Param	A flag which says whether the values are valid or not (due to corruption, etc.)
 * @Param	The sample constraint. How many consecutive samples may be wrong before an error is triggered
 * @retval	1 if there is no error
 */
uint8_t COM_voltages_ok(float *voltages, uint8_t valid,
		uint16_t sample_constraint);


/*
 * @Brief	Checks whether the voltages are within range, or if they have been indeterminate for too long.
 * @Param	A vector of voltages to check
 * @Param	A flag which says whether the values are valid or not (due to corruption, etc.)
 * @Param	The sample constraint. How many consecutive samples may be wrong before an error is triggered
 * @retval	1 if there is no error
 */
uint8_t COM_voltages_ok_d(double *voltages, uint8_t valid,
		uint16_t sample_constraint);

/*
 * @Brief	Checks whether the temperatures are within range, or if they have been indeterminate for too long.
 * @Param	A vector of temperatures to check
 * @Param	A flag which says whether the values are valid or not (due to corruption, etc.)
 * @Param	The sample constraint. How many consecutive samples may be wrong before an error is triggered
 * @retval	1 if there is no error
 */
uint8_t COM_temperatures_ok(float *temperatures, uint8_t valid,
		uint16_t sample_constraint);

/*
 * @Brief	Checks whether the temperatures are within range, or if they have been indeterminate for too long.
 * @Param	A vector of temperatures to check
 * @Param	A flag which says whether the values are valid or not (due to corruption, etc.)
 * @Param	The sample constraint. How many consecutive samples may be wrong before an error is triggered
 * @retval	1 if there is no error
 */
uint8_t COM_temperatures_ok_d(double *temperatures, uint8_t valid,
		uint16_t sample_constraint);


/*
 * @Brief	Checks whether the current is within range, or if it has been indeterminate for too long.
 * @Param	A single current to check
 * @Param	A flag which says whether the value is valid or not (due to corruption, etc.)
 * @Param	The sample constraint. How many consecutive samples may be wrong before an error is triggered
 * @retval	1 if there is no error
 */
uint8_t COM_current_ok(float *current, uint8_t valid,
		uint16_t sample_constraint);


/*
 * @Brief	Checks whether the current is within range, or if it has been indeterminate for too long.
 * @Param	A single current to check
 * @Param	A flag which says whether the value is valid or not (due to corruption, etc.)
 * @Param	The sample constraint. How many consecutive samples may be wrong before an error is triggered
 * @retval	1 if there is no error
 */
uint8_t COM_current_ok_d(double *current, uint8_t valid,
		uint16_t sample_constraint);



#endif /* INC_COM_H_ */
