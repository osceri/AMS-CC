/*
 * LTC_rtos.h
 *
 *  Created on: Apr 10, 2022
 *      Author: oscar
 */

#ifndef INC_LTC_H_
#define INC_LTC_H_

#include "main.h"

uint16_t LTC_data[12 * 6 * 3];
double LTC_voltages[126];
double LTC_temperatures[60];

/*
 * @brief	Is used to calculated the PEC.
 * @param	Length of the input data sequence with which to calculate the PEC
 * @param	The data used to calculate the PEC
 * @retval	The 16-bit PEC code
 */
uint16_t pec15_calc(uint8_t len, // Number of bytes that will be used to calculate a PEC
		uint8_t *data  // Array of data that will be used to calculate  a PEC
		);

/*
 * @brief	Set the hspi which LTC uses
 * @param	The hspi to be used
 * @retval	None
 */
void initialize_LTC(SPI_HandleTypeDef *hspi);

/*
 * @brief	Selects CS
 * @retval	None
 */
void LTC_CS_select(void);

/*
 * @brief	Deselects CS
 * @retval	None
 */
void LTC_CS_deselect(void);

/*
 * @brief	A helper function which interfaces with SPI. wakes the isoSPI port from sleep
 * @retval	1 if successful
 */
uint8_t LTC_wake();

/*
 * @brief	A helper function which interfaces with SPI.
 * @param	Wether or not to wake the isoSPI port first
 * @param	Which command to be sent
 * @retval	1 if successful
 */
uint8_t LTC_command(uint8_t wake, uint16_t command);

/*
 * @brief	A helper function which interfaces with SPI. Puts received data in LTC_read_buffer
 * @param	Whether or not to wake the isoSPI port first
 * @param	Which command the received data corresponds to
 * @retval	1 if successful
 */
uint8_t LTC_read_command(uint8_t wake, uint16_t command);

/*
 * @brief	A helper function which interfaces with SPI. Sends the data stored in LTC_write_buffer
 * @param	Whether or not to wake the isoSPI port first
 * @param	Which command the transmitted data corresponds to
 * @retval	1 if successful
 */
uint8_t LTC_write_command(uint8_t wake, uint16_t command);

/*
 * @brief	The function which acquires all of the cell temperatures and voltages, and places them in cell_voltages and cell_temperatures
 * @param	Whether or not to wake the isoSPI port beforehand
 * @retval	1 if successful
 */
uint8_t LTC_acquire_data(uint8_t wake);

/*
 * @brief	A function which takes data from LTC_data and translates it into LTC_voltages
 * @retval	1 if successful
 */
uint8_t LTC_make_voltages(void);

/*
 * @brief	A function which takes data from LTC_data and translates it into LTC_temperatures
 * @retval	1 if successful
 */
uint8_t LTC_make_temperatures(void);

#endif /* INC_LTC_H_ */
