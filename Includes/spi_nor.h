/*
 * spi_nor.h
 *
 *  Created on: Dec 11, 2021
 *      Author: Krzysztof Gałowicz
 *     Version: 0.1
 *
 * Simple library prowiding easy access to typical spi nor flash memory chip from seriers 25(for ex. w25q80)
 * requires manual CS handling
 */

#ifndef INCLUDES_SPI_NOR_H_
#define INCLUDES_SPI_NOR_H_
//#include <inttypes.h>
#include "MKL05Z4.h"
#include "pit.h"
#include "gpio.h"
struct WAV_sample{
	uint32_t address;
	uint32_t length;
	uint32_t samplerate;

};
#define CS_PIN 5

#define START_TRANSMISJI GPIOA->PCOR = (1 << CS_PIN)
#define KONIEC_TRANSMISJI GPIOA->PSOR = (1 << CS_PIN)
//Available commands:
/* \brief Read data at given address
 * \param 24bit wide address
 * \return byte written at given address
 */
#define READ_DATA 0x03U
/*
 * \brief enable write operation
 */
#define WRITE_ENABLE 0x06U
/*
 * \brief disable write operation
 */
#define WRITE_DISABLE 0x04U
/*
 * \brief read lower half of status register(S7-S0)
 * \return lower half of status register
 */
#define READ_STATUS_REG_1 0x05U
/*
 * \brief read higher half of status register(S15-S8)
 * \return higher half of status register
 */
#define READ_STATUS_REG_2 0x35U
/*
 * \brief read manufacturer id
 * \return menufacturer id
 */
#define READ_MANUFACTURER_ID 0x90U
/** \brief Initialize SPI
 */
void SPI_Init();

/* \brief Read data at given address
 * \param 24bit wide address
 * \return byte written at given address
 */
uint8_t SPI_read_byte(uint32_t addr);
/* \brief Read data at given address
 * \param pointer to where write data,size of data,24bit wide address
 * \return date of given written at given address
 */
uint8_t SPI_read_data(uint8_t*data, uint32_t size, uint32_t addr);
/*
 * \brief send data via spi
 * \param data to send
 * \return received data
 */
uint8_t SPI_send(uint8_t data);
void SPI_Start_Read_from_address(uint32_t address);

uint8_t SPI_Scan(struct WAV_sample*);

#endif /* INCLUDES_SPI_NOR_H_ */
