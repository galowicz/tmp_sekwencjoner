/*
 * spi_nor.c
 *
 *  Created on: Jan 11, 2022
 *      Author: user
 */

#include "spi_nor.h"
/** \brief Initialize SPI
 */
void SPI_Init() {
	//enable spi0 module
	SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK;
	//porta
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; //enable clock to porta
	//pa5-ss, pa6-miso, pa7-mosi,
	PORTA->PCR[5] = PORT_PCR_MUX(1);
	GPIOA->PSOR |= (1 << 5); //ustaw ss na 1- wylacz komunikacje
	GPIOA->PDDR |= (1 << 5);
	PORTA->PCR[6] = PORT_PCR_MUX(3);
	PORTA->PCR[7] = PORT_PCR_MUX(3);

	//portb
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	//pb0-sck

	PORTB->PCR[0] = PORT_PCR_MUX(3);

	SPI0->C1 |= SPI_C1_SPE_MASK | SPI_C1_MSTR_MASK; //enable spi as master
	SPI0->C1 &= ~SPI_C1_CPHA_MASK;
//	SPI0->C2 |=0
	SPI0->BR = 0; //full speed
}

/* \brief Read data at given address
 * \param 24bit wide address
 * \return byte written at given address
 */
uint8_t SPI_read_byte(uint32_t addr) {
	START_TRANSMISJI;
	uint8_t data = 0;
	SPI_send(READ_DATA);
	SPI_send(addr >> 16);
	SPI_send(addr >> 8);
	SPI_send(addr);
	data = SPI_send(0xff);
	//addr=SPI0_D;
	KONIEC_TRANSMISJI;
	return data;
}
/* \brief Read data at given address
 * \param pointer to where write data,size of data,24bit wide address
 * \return date of given written at given address
 */
uint8_t SPI_read_data(uint8_t*data, uint32_t size, uint32_t addr) {
	START_TRANSMISJI;
	SPI_send(READ_DATA);
	SPI_send(addr >> 16);
	SPI_send(addr >> 8);
	SPI_send(addr);
	for (uint8_t i = 0; i < size; ++i) {
		data[i] = SPI_send(0xff);
	}
	KONIEC_TRANSMISJI;
	return 0;
}
/*
 * \brief send data via spi
 * \param data to send
 * \return received data
 */
uint8_t SPI_send(uint8_t data) {
	while (!((1 << SPI_S_SPTEF_SHIFT) & SPI0_S))
		;
	SPI0_D = data;
	while (!((1 << SPI_S_SPRF_SHIFT) & SPI0_S))
		;
	data = SPI0_D;
	return data;
}

void SPI_Start_Read_from_address(uint32_t address) {
	START_TRANSMISJI;
	SPI_send(READ_DATA);
	SPI_send(address >> 16);
	SPI_send(address >> 8);
	SPI_send(address);
}

uint8_t SPI_Scan(struct WAV_sample* tablica) {
	//sprawdz rozmiar pamieci
	START_TRANSMISJI; // start transmisji cs=0
	SPI_send(READ_MANUFACTURER_ID);
	SPI_send(0);
	SPI_send(0);
	SPI_send(0);
	SPI_send(0);
	uint32_t capacity = SPI_send(0);
	switch (capacity) {
	case 0x13:
		capacity = 0xfffff;
		break;
	case 0x14:
		capacity = 0x1fffff;
		break;
	case 0x15:
		capacity = 0x3fffff;
		break;
	default:
		capacity = 0xfffff;
		break;
	}
	KONIEC_TRANSMISJI; //koniec transmisji
	for (uint32_t i = SystemCoreClock >> 9; i > 0; --i)
		; //czekaj miedzy poleceniami ~250us
	//skan pamieci wlasciwy
	uint8_t temp, buf[4], j = 0;
	START_TRANSMISJI;
	SPI_send(READ_DATA);
	SPI_send(0);
	SPI_send(0);
	SPI_send(0);

	for (uint32_t i = 0; i < capacity; ++i) {
		temp = SPI_send(0xff);
		if (temp == 'R') {
			temp = SPI_send(0xff);
			i++;
			if (temp == 'I') {
				temp = SPI_send(0xff);
				i++;
				if (temp == 'F') {
					temp = SPI_send(0xff);
					i++;
					if (temp == 'F') {
						//jest plik w pamieci
						tablica[j].address = i - 3;	//todo nwm czy napewno 3
						//czytaj rozmiar
						buf[3] = SPI_send(0xff);
						i++;
						buf[3] -= 36;
						buf[2] = SPI_send(0xff);
						i++;
						buf[1] = SPI_send(0xff);
						i++;
						buf[0] = SPI_send(0xff);
						i++;
						tablica[j].length = buf[0];
						tablica[j].length = tablica[j].length << 8;
						tablica[j].length |= buf[1];
						tablica[j].length = tablica[j].length << 8;
						tablica[j].length |= buf[2];
						tablica[j].length = tablica[j].length << 8;
						tablica[j].length |= buf[3];
						//samplerate
						for (int k = 0; k < 20; ++k) {		//pomin 20 komorek
							SPI_send(0xff);
							i++;
						}
						buf[3] = SPI_send(0xff);
						i++;
						buf[2] = SPI_send(0xff);
						i++;
						buf[1] = SPI_send(0xff);
						i++;
						buf[0] = SPI_send(0xff);
						i++;
						tablica[j].samplerate = buf[0];
						tablica[j].samplerate = tablica[j].samplerate << 8;
						tablica[j].samplerate |= buf[1];
						tablica[j].samplerate = tablica[j].samplerate << 8;
						tablica[j].samplerate |= buf[2];
						tablica[j].samplerate = tablica[j].samplerate << 8;
						tablica[j].samplerate |= buf[3];
						//zwieksz ilosc sampli
						j++;

					}
				}
			}
		}
	}
	KONIEC_TRANSMISJI;
	return j;

}

