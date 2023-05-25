/*
 * led.c
 *
 *  Created on: 25.05.2023
 *      Author: paul
 */
#include "led.h"

#include <inttypes.h>
#include "esp_log.h"
#include "led_strip.h"

void led_configure(uint8_t gpioNumber, uint8_t ammountLed){
	if(gpioNumber > 21){
			printf("Gpio number is incorrect! There are only 21 Gpios available\n");
	}
	if(ammountLed > 24){
				printf("LED number is incorrect! There are only 24 Leds available\n");
		}
	    led_strip_config_t strip_config = {
	        .strip_gpio_num = gpioNumber,
	        .max_leds = ammountLed, // at least one LED on board
	    };
	    led_strip_rmt_config_t rmt_config = {
	        .resolution_hz = 10 * 1000 * 1000, // 10MHz
	    };
	    led_strip_clear(led_strip);
}


