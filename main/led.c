/*
 * led.c
 *
 *  Created on: 25.05.2023
 *      Author: paul
 */
#include "led.h"
#include "button.h"

#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include "driver/ledc.h"
#include "led_strip.h"
#include "esp_log.h"

#define LEDC_TIMER				LEDC_TIMER_0
#define LEDC_MODE 				LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL			LEDC_CHANNEL_0
#define LEDC_DUTY_RES 			LEDC_TIMER_13_BIT
#define LEDC_DUTY 				4095
#define LEDC_FREQUENCY			5000
#define MAX_VALUE 				8192


void led_configure(uint8_t gpioNumber){
	if(gpioNumber > 21){
			printf("Gpio number is incorrect! There are only 21 Gpios available\n");
	}
	ledc_timer_config_t ledc_timer;
	ledc_timer.speed_mode 			= LEDC_MODE;
	ledc_timer.timer_num    		= LEDC_TIMER;
	ledc_timer.freq_hz				= LEDC_FREQUENCY;
	ledc_timer.duty_resolution		= LEDC_DUTY_RES;
	ledc_timer.clk_cfg				= LEDC_AUTO_CLK;

	ledc_timer_config(&ledc_timer);

	ledc_channel_config_t ledc_channel;
	ledc_channel.gpio_num 	= gpioNumber;
	ledc_channel.speed_mode = LEDC_MODE;
	ledc_channel.channel    = LEDC_CHANNEL;
	ledc_channel.timer_sel 	= LEDC_TIMER_0;
	ledc_channel.intr_type 	= LEDC_INTR_DISABLE;
	ledc_channel.duty 		= 0;
	ledc_channel.hpoint 	= 0;

	ledc_channel_config(&ledc_channel);
}

void led_pwm(uint8_t gpioNumber, bool PwmMode, bool LedOn){
	if(!LedOn){
    	ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0);
    	ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
		return;
	}
	static bool initialized = false;
	if(!initialized){
		led_configure(gpioNumber);
		initialized = true;
	}
	if(PwmMode){
		uint16_t counter = 0;
		while(counter <= MAX_VALUE){
			usleep(MS_TO_U(20)); //Needed for WATCHDOGTIMER
	    	ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, counter);
	    	ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
	    	counter += 10;
		}
	}
	else{
		int32_t counter = MAX_VALUE;
		while(counter > 0){
			usleep(1000);
			ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, counter);
			ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
			counter -= 10;
		}
	}
}

void led_strip_configure(uint8_t gpioNumber, led_strip_handle_t led_strip){
#ifdef CONFIG_BLINK_LED_RMT
	    led_strip_config_t strip_config = {
	        .strip_gpio_num = gpioNumber,
	        .max_leds = 25,
	    };
	    led_strip_rmt_config_t rmt_config = {
	        .resolution_hz = 10 * 1000 * 1000,
	    };
	    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
	    led_strip_clear(led_strip);
#elif CONFIG_BLINK_LED_GPIO
	    gpio_reset_pin(gpioNumber);
	    gpio_set_direction(gpioNumber, GPIO_MODE_OUTPUT);
#endif
}
void led_strip_setLed(led_strip_handle_t led_strip, uint8_t gpioNumber, uint8_t mode, bool LedOn){
	if(!LedOn){
			led_strip_clear(led_strip);
			led_strip_refresh(led_strip);
			return;
	}
	static bool initialized = false;
	if(!initialized){
		led_strip_configure(gpioNumber, led_strip);
		initialized = true;
	}

	uint8_t RGB [3] = {0};
	switch(mode){
	case 1:
		RGB [1] = 50;
		break;

	case 2:
		RGB [2] = 50;
		break;

	case 3:
		RGB [3] = 50;
		break;
	default:
		printf("Wrong Mode Entred!\n");
		return;
	}
	for (int i = 0; i < 25; i += 1){
	led_strip_set_pixel(led_strip, 24, RGB[0], RGB[1], RGB[2]);
	led_strip_refresh(led_strip);
	}
}






