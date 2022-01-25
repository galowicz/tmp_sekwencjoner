/*
 * pit.c
 *
 *  Created on: Jan 12, 2022
 *      Author: user
 */
#include "pit.h"

void PIT_Init(uint8_t channel,uint32_t frequency) {

	// Enable clock to PIT module
	 SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; /* ToDo 6.2 Enable PIT mask in SCGC6 register */

	// Enable module, freeze timers in debug mode
	PIT->MCR &= ~PIT_MCR_MDIS_MASK; /* ToDo 6.3 Disable MDIS mask in MCR register */
	PIT->MCR |= PIT_MCR_FRZ_MASK;   /* ToDo 6.4 Enable FRZ mask in MCR register */

	// Initialize PIT0 to count down from argument
	PIT->CHANNEL[channel].LDVAL = PIT_LDVAL_TSV((SystemCoreClock/(frequency<<1))-1);

	// No chaining
	PIT->CHANNEL[channel].TCTRL &= PIT_TCTRL_CHN_MASK;

	// Generate interrupts
	PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TIE_MASK;

	/* Enable Interrupts */
	NVIC_SetPriority(PIT_IRQn, 0); // 0, 1, 2 or 3 /* ToDo 7.2b, ToDo 7.3: Set PIT interrupt priority level  */

	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn);

}
void PIT_start(uint8_t channel) {
// Enable counter
	PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void PIT_stop(uint8_t channel) {
// Enable counter
	PIT->CHANNEL[channel].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}
void PIT_set_freq(uint8_t channel,uint32_t frequency){
	PIT->CHANNEL[channel].LDVAL = PIT_LDVAL_TSV((uint32_t)(SystemCoreClock/frequency)-1);
}
