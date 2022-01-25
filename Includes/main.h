/*
 * main.h
 *
 *  Created on: Jan 11, 2022
 *      Author: user
 */

#ifndef INCLUDES_MAIN_H_
#define INCLUDES_MAIN_H_

#include "MKL05Z4.h"
#include "spi_nor.h"
#include "tpm.h"
#include "pit.h"
#include "gpio.h"

#define LOOP_COUNT 640
uint8_t play_sample(uint32_t address,uint32_t lenght,uint32_t samplerate);
void SysTick_Disable(void);
void Button_pressed(uint8_t button);

#endif /* INCLUDES_MAIN_H_ */
