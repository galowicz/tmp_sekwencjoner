/*
 * tpm.c
 *
 *  Created on: Jan 11, 2022
 *      Author: user
 */

#include "tpm.h"

void TPM_Init(void) {

	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // CLK ~ 41,9MHz (CLOCK_SETUP=0)

	TPM0->SC |= TPM_SC_PS(0); //no prescaling
	TPM0->SC |= TPM_SC_CMOD(1);					  // internal input clock source

	TPM0->MOD = 255; 										  // 8bit PCM
	// "Edge-aligned PWM true pulses" mode -> PCM output
	TPM0->SC &= ~TPM_SC_CPWMS_MASK;
	TPM0->CONTROLS[2].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
	TPM0->CONTROLS[2].CnV = 0;	//nie graj

}
void TPM_SetVal(uint8_t channel, uint32_t value) {
	TPM0->CONTROLS[channel].CnV = value;
}
