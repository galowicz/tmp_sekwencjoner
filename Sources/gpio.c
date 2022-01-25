/*
 * gpio.c
 *
 *  Created on: Jan 21, 2022
 *      Author: user
 */
#include "gpio.h"

void GPIO_Init() {
	//porta
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; //enable clock to porta
	//pa5-ss, pa6-miso, pa7-mosi,
	PORTA->PCR[5] = PORT_PCR_MUX(1);
	PORTA->PCR[6] = PORT_PCR_MUX(3);
	PORTA->PCR[7] = PORT_PCR_MUX(3);

	PTA->PSOR |= (1 << 5);
	FPTA->PDDR |= (1 << 1) | (1 << 5) | (1 << 7); //pa1,pa5,pa6 as output

	//portb
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	//pb0-sck, pb7-tpm_out, pb8-red_led
	PORTB->PCR[0] = PORT_PCR_MUX(3);
	PORTB->PCR[7] = PORT_PCR_MUX(2);
	PTB->PDDR |= (7 << 8);
	PTB->PSOR |= (7 << 8);
	PORTB->PCR[8] = PORT_PCR_MUX(1);
	PORTB->PCR[9] = PORT_PCR_MUX(1);
	PORTB->PCR[10] = PORT_PCR_MUX(1);

	PTB->PSOR |= (1 << 8); //turn off led

	PTB->PDDR |= 1 | (1 << 7) | (1 << 8);
	//KOLUMNY przyciskow
	PORTB->PCR[1] = PORT_PCR_MUX(1) | PORT_PCR_IRQC(10) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;
	PORTB->PCR[2] = PORT_PCR_MUX(1) | PORT_PCR_IRQC(10) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;
	PORTB->PCR[3] = PORT_PCR_MUX(1) | PORT_PCR_IRQC(10) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;
	PORTB->PCR[4] = PORT_PCR_MUX(1) | PORT_PCR_IRQC(10) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;

	//wiersze przyciskow wejscia,
	PORTA->PCR[8] = PORT_PCR_MUX(1);
	PORTA->PCR[9] = PORT_PCR_MUX(1);
	PORTA->PCR[10] = PORT_PCR_MUX(1);
	PORTA->PCR[11] = PORT_PCR_MUX(1);
	PTA->PDDR |= (0xf << 8);
	PTA->PCOR = (0xf << 8);
	NVIC_ClearPendingIRQ(PORTB_IRQn); /* Clear NVIC any pending interrupts on PORTC_B */
	NVIC_EnableIRQ(PORTB_IRQn); /* Enable NVIC interrupts source for PORTC_B module */

}

