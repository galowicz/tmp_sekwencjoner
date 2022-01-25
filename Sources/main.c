/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "main.h"

volatile uint16_t loop[LOOP_COUNT];
volatile uint16_t loop_pointer = 0;
volatile uint16_t loop_max_pointer = LOOP_COUNT;
struct WAV_sample sample[12]; //tablica z samplami
uint8_t sample_count = 0;
volatile uint8_t playstate = 1;
volatile uint8_t recording = 0;
volatile uint32_t sample_pos;
volatile struct WAV_sample act_sample;

int main(void) {
	//init: spi, timery, gpio, skan po pamieci,
	//init ledy
	GPIO_Init();
	PTB->PCOR = (1 << 8);
	//spi init i sprawdzenie pamieci
	SPI_Init();
	sample_count = SPI_Scan(sample);
//	sprawdz czy cos odczytane
	if (sample_count == 0) {
		GPIOB->PCOR = (3 << 8);
		while (1)
			;
	}
	//init tpm i pit
	TPM_Init();
//pit[0] - 32 hz
//pit[1] - samplerate
	PIT_Init(0, 32);
	PIT_Init(1, 48000);
	PIT_start(0);
	GPIOB->PSOR = (1 << 8);
	for (;;) {

		__WFI();

	}

	return 0;
}

void PIT_IRQHandler() {

	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 0
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;

		// Do ISR work
		GPIOB->PSOR = (1 << 8);
		if (++loop_pointer >= loop_max_pointer) {
			GPIOB->PCOR = (1 << 8);
			loop_pointer = 0;
		}
		if ((loop[loop_pointer] > 0) && playstate) {
			play_sample(sample[loop[loop_pointer] - 1].address, sample[loop[loop_pointer] - 1].length, sample[loop[loop_pointer] - 1].samplerate);

		}
	} else if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 1
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;

		sample_pos++;
		TPM_SetVal(2, SPI_send(0xff));
		if (sample_pos > act_sample.length) {
			sample_pos = 0;
			playstate = 1;
			TPM_SetVal(2, 0);
			KONIEC_TRANSMISJI;
			PIT_stop(1);
			GPIOB->PSOR = (1 << 9);
		}
	}
	//clear pending IRQ
	NVIC_ClearPendingIRQ(PIT_IRQn);
}
void PORTB_IRQHandler() {
	if (PORTB->ISFR & (1 << 1)) {
		PORTB->PCR[1] |= PORT_PCR_ISF_MASK;	//row1
		PTA->PSOR = 0xf00;
		PTA->PCOR = (1 << 8);
		if (!(PTB->PDIR & (1 << 1))) {
			//s1
			Button_pressed(1);
			PTA->PCOR = 0xf00;
			PORTB->PCR[1] |= PORT_PCR_ISF_MASK;	//row1
			return;
		}
		PTA->PSOR = 0xf00;
		PTA->PCOR = (1 << 9);
		if (!(PTB->PDIR & (1 << 1))) {
			//s2
			Button_pressed(2);
			PTA->PCOR = 0xf00;
			PORTB->PCR[1] |= PORT_PCR_ISF_MASK;	//row1
			return;
		}
		PTA->PSOR = 0xf00;
		PTA->PCOR = (1 << 10);
		if (!(PTB->PDIR & (1 << 1))) {
			//s3
			Button_pressed(3);
			PTA->PCOR = 0xf00;
			PORTB->PCR[1] |= PORT_PCR_ISF_MASK;	//row1
			return;
		}
		PTA->PSOR = 0xf00;
		PTA->PCOR = (1 << 11);
		if (!(PTB->PDIR & (1 << 1))) {
			//s1
			Button_pressed(4);
			PTA->PCOR = 0xf00;
			PORTB->PCR[1] |= PORT_PCR_ISF_MASK;	//row1
			return;
		}
		PTA->PCOR = 0xf00;
	}
	if (PORTB->ISFR & (1 << 2)) {
		PORTB->PCR[2] |= PORT_PCR_ISF_MASK;	//row2
		PTA->PSOR = 0xf00;
		PTA->PCOR = (1 << 8);
		if (!(PTB->PDIR & (1 << 2))) {
			//s1
			Button_pressed(5);
			PTA->PCOR = 0xf00;
			PORTB->PCR[2] |= PORT_PCR_ISF_MASK;	//row2
			return;
		}
		PTA->PSOR = 0xf00;
		PTA->PCOR = (1 << 9);
		if (!(PTB->PDIR & (1 << 2))) {
			//s2
			Button_pressed(6);
			PTA->PCOR = 0xf00;
			PORTB->PCR[2] |= PORT_PCR_ISF_MASK;	//row2
			return;
		}
		PTA->PSOR = 0xf00;
		PTA->PCOR = (1 << 10);
		if (!(PTB->PDIR & (1 << 2))) {
			//s3
			Button_pressed(7);
			PTA->PCOR = 0xf00;
			PORTB->PCR[2] |= PORT_PCR_ISF_MASK;	//row2
			return;
		}
		PTA->PSOR = 0xf00;
		PTA->PCOR = (1 << 11);
		if (!(PTB->PDIR & (1 << 2))) {
			//s1
			Button_pressed(8);
			PTA->PCOR = 0xf00;
			PORTB->PCR[2] |= PORT_PCR_ISF_MASK;	//row2
			return;
		}
		PTA->PCOR = 0xf00;
	}
	if (PORTB->ISFR & (1 << 3)) {
		PORTB->PCR[3] |= PORT_PCR_ISF_MASK;	//row3
		PTA->PSOR = 0xf00;
		PTA->PCOR = (1 << 8);
		if (!(PTB->PDIR & (1 << 3))) {
			//s1
			Button_pressed(9);
			PTA->PCOR = 0xf00;
			PORTB->PCR[3] |= PORT_PCR_ISF_MASK;	//row3
			return;
		}
		PTA->PSOR = 0xf00;
		PTA->PCOR = (1 << 9);
		if (!(PTB->PDIR & (1 << 3))) {
			//s2
			Button_pressed(10);
			PTA->PCOR = 0xf00;
			PORTB->PCR[3] |= PORT_PCR_ISF_MASK;	//row3
			return;
		}
		PTA->PSOR = 0xf00;
		PTA->PCOR = (1 << 10);
		if (!(PTB->PDIR & (1 << 3))) {
			//s3
			Button_pressed(11);
			PTA->PCOR = 0xf00;
			PORTB->PCR[3] |= PORT_PCR_ISF_MASK;	//row3
			return;
		}
		PTA->PSOR = 0xf00;
		PTA->PCOR = (1 << 11);
		if (!(PTB->PDIR & (1 << 3))) {
			//s1
			Button_pressed(12);
			PTA->PCOR = 0xf00;
			PORTB->PCR[3] |= PORT_PCR_ISF_MASK;	//row3
			return;
		}
		PTA->PCOR = 0xf00;
	}
	if (PORTB->ISFR & (1 << 4)) {
		PORTB->PCR[4] |= PORT_PCR_ISF_MASK;	//row1
		PTA->PSOR = 0xf00;
		PTA->PCOR = (1 << 8);
		if (!(PTB->PDIR & (1 << 4))) {
			//s1
			Button_pressed(13);
			PTA->PCOR = 0xf00;
			PORTB->PCR[4] |= PORT_PCR_ISF_MASK;	//row1
			return;
		}
		PTA->PSOR = 0xf00;
		PTA->PCOR = (1 << 9);
		if (!(PTB->PDIR & (1 << 4))) {
			//s2
			Button_pressed(14);
			PTA->PCOR = 0xf00;
			PORTB->PCR[4] |= PORT_PCR_ISF_MASK;	//row1
			return;
		}
		PTA->PSOR = 0xf00;
		PTA->PCOR = (1 << 10);
		if (!(PTB->PDIR & (1 << 4))) {
			//s3
			Button_pressed(15);
			PTA->PCOR = 0xf00;
			PORTB->PCR[4] |= PORT_PCR_ISF_MASK;	//row1
			return;
		}
		PTA->PSOR = 0xf00;
		PTA->PCOR = (1 << 11);
		if (!(PTB->PDIR & (1 << 4))) {
			//s1
			Button_pressed(16);
			PTA->PCOR = 0xf00;
			PORTB->PCR[4] |= PORT_PCR_ISF_MASK;	//row1
			return;
		}
		PTA->PCOR = 0xf00;
	}

}

//void SysTick_Disable() {
//	SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
//}

uint8_t play_sample(uint32_t address, uint32_t length, uint32_t samplerate) {

	//wyczysc playstate zeby nie gralo znowu
	playstate = 0;
	GPIOB->PCOR = (1 << 9);
	//ustaw systick na samplerate

	act_sample.address = address;
	act_sample.length = length;
	act_sample.samplerate = samplerate;

	sample_pos = 0;
	SPI_Start_Read_from_address(address + 43);
	PIT_set_freq(1, samplerate);
	PIT_start(1);

	return 1;
}
void Button_pressed(uint8_t button) {
	if (button == 16) {	//rec
		if (recording) {
			recording = 0;
			loop_max_pointer = loop_pointer;
			loop_pointer = 0;
			PTB->PSOR = (1 << 10);
		} else {
			recording = 1;
			loop_max_pointer=LOOP_COUNT;
			loop_pointer = 0;
			PTB->PCOR = (1 << 10);
		}
	} else if (button == 15) {
		loop_max_pointer = LOOP_COUNT;
		for (int i = 0; i < LOOP_COUNT; ++i) {
			loop[1]=0;
		}
	} else if (button == 14) {
		PIT_stop(0);	//stop playing
	} else if (button == 13) {
		PIT_start(0);
	} else if (button < 13) {
		if (recording) {
			loop[loop_pointer] = button;
			if (playstate && (button <= sample_count))
				play_sample(sample[button - 1].address, sample[button - 1].length, sample[button - 1].samplerate);
		} else {
			if (playstate && (button <= sample_count))
				play_sample(sample[button - 1].address, sample[button - 1].length, sample[button - 1].samplerate);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
