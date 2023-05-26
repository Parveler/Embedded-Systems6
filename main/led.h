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

void led_configure(uint8_t gpioNumber);
void led_pwm(uint8_t gpioNumber, bool PwmMode, bool LedOn);
void led_strip_configure(uint8_t gpioNumber, led_strip_handle_t led_strip);
void led_strip_setLed(led_strip_handle_t led_strip, uint8_t gpioNumber, uint8_t mode, bool LedOn);

#endif /* MAIN_LED_H_ */
