/*
 * rtos_LTC.c
 *
 *  Created on: Apr 10, 2022
 *      Author: oscar
 */

#include "main.h"
#include "cmsis_os.h"

#include "rtos_LTC.h"

#define cellstack_address_map(x) x
#define LTC_COM_TIMEOUT 0

SPI_HandleTypeDef *hltc;

uint8_t LTC_read_buffer[10], LTC_write_buffer[10], LTC_command_buffer[4];
uint16_t LTC_data[12 * 6 * 3];
double LTC_voltages[126];
double LTC_temperatures[60];

uint8_t LTC_data_valid;

static const unsigned int crc15Table[256] = { 0x0, 0xc599, 0xceab, 0xb32,
		0xd8cf, 0x1d56, 0x1664, 0xd3fd, 0xf407, 0x319e,
		0x3aac,  //!< precomputed CRC15 Table
		0xff35, 0x2cc8, 0xe951, 0xe263, 0x27fa, 0xad97, 0x680e, 0x633c, 0xa6a5,
		0x7558, 0xb0c1, 0xbbf3, 0x7e6a, 0x5990, 0x9c09, 0x973b, 0x52a2, 0x815f,
		0x44c6, 0x4ff4, 0x8a6d, 0x5b2e, 0x9eb7, 0x9585, 0x501c, 0x83e1, 0x4678,
		0x4d4a, 0x88d3, 0xaf29, 0x6ab0, 0x6182, 0xa41b, 0x77e6, 0xb27f, 0xb94d,
		0x7cd4, 0xf6b9, 0x3320, 0x3812, 0xfd8b, 0x2e76, 0xebef, 0xe0dd, 0x2544,
		0x2be, 0xc727, 0xcc15, 0x98c, 0xda71, 0x1fe8, 0x14da, 0xd143, 0xf3c5,
		0x365c, 0x3d6e, 0xf8f7, 0x2b0a, 0xee93, 0xe5a1, 0x2038, 0x7c2, 0xc25b,
		0xc969, 0xcf0, 0xdf0d, 0x1a94, 0x11a6, 0xd43f, 0x5e52, 0x9bcb, 0x90f9,
		0x5560, 0x869d, 0x4304, 0x4836, 0x8daf, 0xaa55, 0x6fcc, 0x64fe, 0xa167,
		0x729a, 0xb703, 0xbc31, 0x79a8, 0xa8eb, 0x6d72, 0x6640, 0xa3d9, 0x7024,
		0xb5bd, 0xbe8f, 0x7b16, 0x5cec, 0x9975, 0x9247, 0x57de, 0x8423, 0x41ba,
		0x4a88, 0x8f11, 0x57c, 0xc0e5, 0xcbd7, 0xe4e, 0xddb3, 0x182a, 0x1318,
		0xd681, 0xf17b, 0x34e2, 0x3fd0, 0xfa49, 0x29b4, 0xec2d, 0xe71f, 0x2286,
		0xa213, 0x678a, 0x6cb8, 0xa921, 0x7adc, 0xbf45, 0xb477, 0x71ee, 0x5614,
		0x938d, 0x98bf, 0x5d26, 0x8edb, 0x4b42, 0x4070, 0x85e9, 0xf84, 0xca1d,
		0xc12f, 0x4b6, 0xd74b, 0x12d2, 0x19e0, 0xdc79, 0xfb83, 0x3e1a, 0x3528,
		0xf0b1, 0x234c, 0xe6d5, 0xede7, 0x287e, 0xf93d, 0x3ca4, 0x3796, 0xf20f,
		0x21f2, 0xe46b, 0xef59, 0x2ac0, 0xd3a, 0xc8a3, 0xc391, 0x608, 0xd5f5,
		0x106c, 0x1b5e, 0xdec7, 0x54aa, 0x9133, 0x9a01, 0x5f98, 0x8c65, 0x49fc,
		0x42ce, 0x8757, 0xa0ad, 0x6534, 0x6e06, 0xab9f, 0x7862, 0xbdfb, 0xb6c9,
		0x7350, 0x51d6, 0x944f, 0x9f7d, 0x5ae4, 0x8919, 0x4c80, 0x47b2, 0x822b,
		0xa5d1, 0x6048, 0x6b7a, 0xaee3, 0x7d1e, 0xb887, 0xb3b5, 0x762c, 0xfc41,
		0x39d8, 0x32ea, 0xf773, 0x248e, 0xe117, 0xea25, 0x2fbc, 0x846, 0xcddf,
		0xc6ed, 0x374, 0xd089, 0x1510, 0x1e22, 0xdbbb, 0xaf8, 0xcf61, 0xc453,
		0x1ca, 0xd237, 0x17ae, 0x1c9c, 0xd905, 0xfeff, 0x3b66, 0x3054, 0xf5cd,
		0x2630, 0xe3a9, 0xe89b, 0x2d02, 0xa76f, 0x62f6, 0x69c4, 0xac5d, 0x7fa0,
		0xba39, 0xb10b, 0x7492, 0x5368, 0x96f1, 0x9dc3, 0x585a, 0x8ba7, 0x4e3e,
		0x450c, 0x8095 };

/*
 * @brief	Is used to calculated the PEC.
 * @param	Length of the input data sequence with which to calculate the PEC
 * @param	The data used to calculate the PEC
 * @retval	The 16-bit PEC code
 */
uint16_t pec15_calc(uint8_t len, // Number of bytes that will be used to calculate a PEC
		uint8_t *data  // Array of data that will be used to calculate  a PEC
		) {
	uint16_t remainder, addr;
	remainder = 16;                    // initialize the PEC
	for (uint8_t i = 0; i < len; i++)  // loops for each byte in data array
			{
		addr = ((remainder >> 7) ^ data[i]) & 0xff; // calculate PEC table address
		remainder = (remainder << 8) ^ crc15Table[addr];
	}
	return (remainder * 2);  // The CRC15 has a 0 in the LSB so the remainder
							 // must be multiplied by 2
}

/*
 * @brief	Set the hspi which LTC uses
 * @param	The hspi to be used
 * @retval	None
 */
void initialize_LTC(SPI_HandleTypeDef *hspi) {
	hltc = hspi;
}

/*
 * @brief	Selects CS
 * @retval	None
 */
void LTC_CS_select(void) {
	HAL_GPIO_WritePin(LTC_CS_GPIO_Port, LTC_CS_Pin, 0);
}

/*
 * @brief	Deselects CS
 * @retval	None
 */
void LTC_CS_deselect(void) {
	HAL_GPIO_WritePin(LTC_CS_GPIO_Port, LTC_CS_Pin, 1);
}

/*
 * @brief	A helper function which interfaces with SPI. wakes the isoSPI port from sleep
 * @retval	LTC Status
 */
uint8_t LTC_wake() {
	uint16_t pec;

	// This is the ADSTAT command. Chosen for no specific reason
	uint16_t command = 0b0000010101101000;

	// This is how one would pack the command, calculate the PEC, and the pack the contents.
	LTC_command_buffer[0] = command >> 8;
	LTC_command_buffer[1] = command;
	pec = pec15_calc(2, LTC_command_buffer);
	LTC_command_buffer[2] = pec >> 8;
	LTC_command_buffer[3] = pec;

	// Simply lock the CS before transmitting and/or receiving
	LTC_CS_select();
	HAL_SPI_Transmit(hltc, LTC_command_buffer, 4, LTC_COM_TIMEOUT);
	LTC_CS_deselect();

	return 1;
}

/*
 * @brief	A helper function which interfaces with SPI.
 * @param	Wether or not to wake the isoSPI port first
 * @param	Which command to be sent
 * @retval	LTC Status
 */
uint8_t LTC_command(uint8_t wake, uint16_t command) {
	uint16_t pec;

	if (wake) {
		if (!LTC_wake()) {
			return 0;
		}
	}

	LTC_command_buffer[0] = command >> 8;
	LTC_command_buffer[1] = command;
	pec = pec15_calc(2, LTC_command_buffer);
	LTC_command_buffer[2] = pec >> 8;
	LTC_command_buffer[3] = pec;

	LTC_CS_select();
	HAL_SPI_Transmit(hltc, LTC_command_buffer, 4, LTC_COM_TIMEOUT);
	LTC_CS_deselect();

	return 1;
}

/*
 * @brief	A helper function which interfaces with SPI. Puts received data in LTC_read_buffer
 * @param	Whether or not to wake the isoSPI port first
 * @param	Which command the received data corresponds to
 * @retval	LTC Status
 */
uint8_t LTC_read_command(uint8_t wake, uint16_t command) {
	uint16_t pec;

	if (wake) {
		if (!LTC_wake()) {
			return 0;
		}
	}

	LTC_command_buffer[0] = command >> 8;
	LTC_command_buffer[1] = command;
	pec = pec15_calc(2, LTC_command_buffer);
	LTC_command_buffer[2] = pec >> 8;
	LTC_command_buffer[3] = pec;

	// We receive at the same instance
	LTC_CS_select();
	HAL_SPI_Transmit(hltc, LTC_command_buffer, 4, LTC_COM_TIMEOUT);
	HAL_SPI_Receive(hltc, LTC_read_buffer, 10, LTC_COM_TIMEOUT);
	LTC_CS_deselect();

	// We should make sure that the data is correct
	pec = pec15_calc(8, LTC_read_buffer);

	// If there is a discrepancy we return an error code
	if (!((((pec >> 8) & 0xff) != LTC_read_buffer[8])
			&& (((pec >> 0) & 0xff) != LTC_read_buffer[9]))) {
		return 0;
	}

	return 1;
}

/*
 * @brief	A helper function which interfaces with SPI. Sends the data stored in LTC_write_buffer
 * @param	Whether or not to wake the isoSPI port first
 * @param	Which command the transmitted data corresponds to
 * @retval	LTC Status
 */
uint8_t LTC_write_command(uint8_t wake, uint16_t command) {
	uint16_t pec;

	if (wake) {
		if (!LTC_wake()) {
			return 0;
		}
	}

	LTC_command_buffer[0] = command >> 8;
	LTC_command_buffer[1] = command;
	pec = pec15_calc(2, LTC_command_buffer);
	LTC_command_buffer[2] = pec >> 8;
	LTC_command_buffer[3] = pec;

	pec = pec15_calc(8, LTC_write_buffer);
	LTC_write_buffer[8] = pec >> 8;
	LTC_write_buffer[9] = pec;

	// We transmit in the same instance
	LTC_CS_select();
	HAL_SPI_Transmit(hltc, LTC_command_buffer, 4, LTC_COM_TIMEOUT);
	HAL_SPI_Transmit(hltc, LTC_write_buffer, 10, LTC_COM_TIMEOUT);
	LTC_CS_deselect();

	return 1;

}

/*
 * @brief	The function which acquires all of the cell temperatures and voltages, and places them in cell_voltages and cell_temperatures
 * @param	Whether or not to wake the isoSPI port beforehand
 * @param	LTC_data_valid[n] is high if the n:th slave has given valid data
 * @retval	LTC Status
 */
uint8_t LTC_acquire_data(uint8_t wake) {
	int p, k, i, j, command;
	uint16_t all_read; /* LTC_data_valid[n] might only be high if slave n was read (PEC correct, among other things) */
	uint16_t all_zeros; /* LTC_data_valid[n] might only be high if the bits are not all zero, as this would constitute a zero voltage reference (or zeroed/floating cell voltages) */
	uint16_t all_ones; /* LTC_data_valid[n] might only be high if the bits are not all ones, as this is indicative of the message being corrupted */

	LTC_data_valid = 0;

	/* ADCV, MD = 10, PUP = 1, DCP = 0, CH = 000 */
	command = 0b0000001101110000;

	LTC_command(wake, command);

	//osDelay(configTICK_RATE_HZ * 0.02); // ~2 ms
	HAL_Delay(2);

	/* ADAX : MD = 10, PUP = 1, CH = 000 */
	command = 0b0000010101100000;

	LTC_command(0, command);

	//osDelay(configTICK_RATE_HZ * 0.02); // ~2 ms
	HAL_Delay(2);

	// First we fill the cell_voltage buffer with the integer values
	p = 0;

	for (k = 0; k < 12; k++) { // 12 segments
		all_read = 1;
		all_zeros = 1;
		all_ones = 1;

		for (i = 0; i < 6; i++) {
			// We use the RDCVA command and increment it every pass to get RDCVB, RDCVC .. RDAUXB
			command = 0b1000000000000100 | (cellstack_address_map(k) << 11);
			command += 2 * i;

			all_read &= LTC_read_command(0, command);

			for (j = 0; j < 3; j++) {
				LTC_data[p] = LTC_read_buffer[2 * j]
						| (LTC_read_buffer[2 * j + 1] << 8);

				//all_zeros &= (LTC_data[p] == 0);
				//all_ones &= (LTC_data[p] == -1);

				p++;
			}
		}

		LTC_data_valid = (all_read && !all_zeros && !all_ones);
	}

	LTC_make_voltages();
	LTC_make_temperatures();

	return 1;
}

#define cellstack_voltage_count(cellstack) (11 - (cellstack % 2))
/*
 * @brief	A function which takes data from LTC_data and translates it into LTC_voltages
 * @retval	1 if successful
 */
uint8_t LTC_make_voltages(void) {
	uint16_t cellstack, K, k, p;

	p = 0;

	for (cellstack = 0; cellstack < 12; cellstack++) {
		K = cellstack_voltage_count(cellstack);
		for (k = 0; k < K; k++) {
			LTC_voltages[p] = 0.0001 * LTC_data[18 * cellstack + k];
		}
	}

	return 1;
}

/*
 * @brief	A function which takes data from LTC_data and translates it into LTC_temperatures
 * @retval	1 if successful
 */
uint8_t LTC_make_temperatures(void) {
	uint16_t cellstack, K, k, p;

	double beta = 3500;
	double temp = 25 + 273.15;
	double R0 = 10000 * exp(-beta / temp);
	double R = 10000;

	p = 0;

	for (cellstack = 0; cellstack < 12; cellstack++) {
		K = 5;

		for (k = 0; k < K; k++) {
			LTC_temperatures[p] = beta
					/ (log(R * LTC_data[18 * cellstack + 12 + k])
							- log(
									-R0
											* (LTC_data[18 * cellstack + 12 + k]
													- LTC_data[18 * cellstack
															+ 12 + K])))
					- 273.15;

		}
	}

	return 1;
}
