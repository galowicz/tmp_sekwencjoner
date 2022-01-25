/*
 * pit.h
 *
 *  Created on: Jan 12, 2022
 *      Author: user
 */

#ifndef INCLUDES_PIT_H_
#define INCLUDES_PIT_H_
#include "MKL05Z4.h"
//void PIT_INIT(uint32_t frequency);
void PIT_Init(uint8_t channel, uint32_t frequency);
void PIT_start(uint8_t channel);
void PIT_stop(uint8_t channel);
void PIT_set_freq(uint8_t channel,uint32_t frequency);
#endif /* INCLUDES_PIT_H_ */
