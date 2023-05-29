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
#include "driver/gpio.h"





void led_configure(uint8_t gpioNumber){
	//configures LEDC Module
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
	gpio_reset_pin(gpioNumber);
    gpio_set_direction(gpioNumber, GPIO_MODE_OUTPUT);
	ledc_channel_config(&ledc_channel);
}

void led_pwm(uint8_t gpioNumber, bool pwmMode, bool ledOn){
	if(!ledOn){
    	ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0);
    	ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
		return;
	}
	if(pwmMode){
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

void led_strip_configure(uint8_t gpioNumber, led_strip_handle_t* led_strip){
	led_strip_config_t strip_config;
	strip_config.strip_gpio_num	    = BLINK_GPIO;
	strip_config.max_leds 			= LED_AMMOUNT;
	strip_config.led_pixel_format 	= LED_PIXEL_FORMAT_GRB;
	strip_config.led_model			= LED_MODEL_WS2812;
	strip_config.flags.invert_out 	= false;

	led_strip_rmt_config_t rmt_config;
	rmt_config.clk_src 		  = RMT_CLK_SRC_DEFAULT;
	rmt_config.resolution_hz  = 10000000;
	rmt_config.flags.with_dma = false;

	ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, led_strip));
}

void led_strip_setLed(led_strip_handle_t* led_strip, uint8_t gpioNumber, uint8_t mode, bool ledOn){
	if(!ledOn){
		for(int i = 0; i < LED_AMMOUNT; i += 1){
		led_strip_set_pixel(*led_strip, i, 0, 0, 0);
		}
		led_strip_refresh(*led_strip);
	}
	enum rgb_e rgb;
	rgb = mode;
	uint8_t RGB[3] = {0};
	switch(rgb){
		case Red:
			RGB[Red] = 50;
			break;
		case Green:
			RGB[Green] = 50;
			break;
		case Blue:
			RGB[Blue] = 50;
			break;
		default:
			return;
			break;
	}
	for(int i = 0; i < LED_AMMOUNT; i += 1){
		led_strip_set_pixel(*led_strip, i, RGB[0], RGB[1], RGB[2]);
	}
	led_strip_refresh(*led_strip);
}



