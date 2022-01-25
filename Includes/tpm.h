/*
 * tpm.h
 *
 *  Created on: Jan 11, 2022
 *      Author: user
 */

#ifndef INCLUDES_TPM_H_
#define INCLUDES_TPM_H_
#include "MKL05Z4.h"
void TPM_Init(void) ;

void TPM_SetVal(uint8_t channel, uint32_t value);


#endif /* INCLUDES_TPM_H_ */
