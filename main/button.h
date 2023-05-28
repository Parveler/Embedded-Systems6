/*
 * button.h
 *
 *  Created on: 25.05.2023
 *      Author: paul
 */
#include "my_gpio.h"

#include <inttypes.h>
#include <stdbool.h>

#ifndef MAIN_BUTTON_H_
#define MAIN_BUTTON_H_

#define MS_TO_U(i) (i*1000)
#define U_TO_MS(i) (i/1000)

enum buttonStates_e{
	WaitForPress,
	WaitForRelease,
	WaitForAnotherPress,
	ShortPressDetected,
	LongPressDetected,
	DoubleShortPressDetected,

};


void button_configure(configureIoMux_t* pButtonSettings);
bool button_getLevel(uint8_t gpioNumber);


#endif /* MAIN_BUTTON_H_ */
