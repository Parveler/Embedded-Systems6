/*
 * button.c
 *
 *  Created on: 25.05.2023
 *      Author: paul
 */

#include "my_gpio.h"
#include "button.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
void button_configure(configureIoMux_t* pButtonSettings){
	gpio_enable(pButtonSettings->GpioNumber);
	gpio_iomuxConfigure(pButtonSettings);
}

bool button_getLevel(uint8_t gpioNumber){
	if(gpioNumber > 21){
		printf("Gpio number is incorrect! There are only 21 Gpios available\n");
		return false;
	}
	bool ButtonLevelFirst = (gpioInput & (1 << gpioNumber));
	if(ButtonLevelFirst){
		usleep(MS_TO_U(10)); //waits 10 ms
		bool ButtonLevelLast  = (gpioInput & (1 << gpioNumber));
		if(ButtonLevelFirst == ButtonLevelLast){
			return true;
		}
	}
	return false;
}
