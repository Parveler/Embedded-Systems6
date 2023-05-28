/*
 * led.h
 *
 *  Created on: 25.05.2023
 *      Author: paul
 */
#include <inttypes.h>
#include <stdbool.h>
#include "led_strip.h"

#ifndef MAIN_LED_H_
#define MAIN_LED_H_

#define LEDC_TIMER				LEDC_TIMER_0
#define LEDC_MODE 				LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL			LEDC_CHANNEL_0
#define LEDC_DUTY_RES 			LEDC_TIMER_13_BIT
#define LEDC_DUTY 				4095
#define LEDC_FREQUENCY			5000
#define MAX_VALUE 				8192
#define BLINK_GPIO				8
#define LED_AMMOUNT				25

enum rgb_e{
	Red,
	Green,
	Blue,
};

void led_configure(uint8_t gpioNumber);
void led_pwm(uint8_t gpioNumber, bool PwmMode, bool LedOn);
void led_strip_configure(uint8_t gpioNumber, led_strip_handle_t* led_strip);
void led_strip_setLed(led_strip_handle_t* led_strip, uint8_t gpioNumber, uint8_t mode, bool LedOn);

#endif /* MAIN_LED_H_ */
