

#include "my_gpio.h"
#include "button.h"
#include "led.c"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>
#include "esp_timer.h"
#include "driver/ledc.h"
#include "led_strip.h"
#include "esp_attr.h"

#define BUTTON 6
#define BLINK_GPIO 8
#define PWM_GPIO 4
#define LONG_BUTTON_PRESS 1000
#define WAIT_FOR_ANOTHER_SHORT_PRESS 300



RTC_NOINIT_ATTR bool gPwmMode = true;	//Stores Variables in RTCNOINIT Memory
RTC_NOINIT_ATTR bool gLedOn = true;
RTC_NOINIT_ATTR uint8_t gMode = 0;

led_strip_handle_t led_strip;
void app_main(void)
{
	configureIoMux_t ioMuxSettings;
	ioMuxSettings.GpioNumber 		= 6;
	ioMuxSettings.OutputConfigure 	= 1;
	ioMuxSettings.PinCurrent 		= 2;
	ioMuxSettings.PinFunc 			= 1;
	ioMuxSettings.Pullup			= 0;
	ioMuxSettings.Pulldown			= 0;

	button_configure(&ioMuxSettings);


	enum buttonStates_e buttonstate = WaitForPress;
	uint64_t timeButtonPressedBeginn = 0;
	uint64_t timeButtonPressed = 0;
	uint64_t timeButtonPressedReleased = 0;

	led_strip_configure(BLINK_GPIO, &led_strip);
	led_strip_setLed(&led_strip, BLINK_GPIO, gMode, gLedOn);
	led_configure(PWM_GPIO);

    while (true) {

    	switch(buttonstate){

    	case WaitForPress:
    		if(button_getLevel(BUTTON)){
    			timeButtonPressedBeginn = esp_timer_get_time();
    			buttonstate = WaitForRelease;
    		}
    		break;

    	case WaitForRelease:
    		if(button_getLevel(BUTTON) == false){
    			timeButtonPressedReleased = esp_timer_get_time();
    			timeButtonPressed = timeButtonPressedReleased -timeButtonPressedBeginn;
    			timeButtonPressed = U_TO_MS(timeButtonPressed);
    			printf("TIME pressed:%"PRIu64"\n",timeButtonPressed);
    			if(timeButtonPressed < LONG_BUTTON_PRESS){
    				buttonstate = WaitForAnotherPress;
    			}
    			else{
    			    buttonstate = LongPressDetected;
    			}
    		}

    		break;
    	case WaitForAnotherPress:
    		timeButtonPressedBeginn = esp_timer_get_time();
    		timeButtonPressed = timeButtonPressedBeginn -timeButtonPressedReleased;
    		timeButtonPressed = U_TO_MS(timeButtonPressed);
    		if(timeButtonPressed < WAIT_FOR_ANOTHER_SHORT_PRESS && button_getLevel(BUTTON)){
    		    buttonstate = DoubleShortPressDetected;
    		}
    		else if(timeButtonPressed > WAIT_FOR_ANOTHER_SHORT_PRESS){
    		    buttonstate = ShortPressDetected;
    		}
    		break;
    	case ShortPressDetected:
    		gMode += 1;
    		if(gMode > 2){
    			gMode = 0;
    		}
    		led_strip_setLed(&led_strip, BLINK_GPIO, gMode, gLedOn);
    		printf("Short press detected\n");
    		buttonstate = WaitForPress;
    		break;
    	case LongPressDetected:
    		led_pwm(PWM_GPIO, gPwmMode, gLedOn);
    		gPwmMode = !gPwmMode;
    		printf("Long button press detected!\n");
    		buttonstate = WaitForPress;
    		break;
    	case DoubleShortPressDetected:
    		gLedOn = !gLedOn;
    		led_pwm(PWM_GPIO, gPwmMode, gLedOn);
    		led_strip_setLed(&led_strip, BLINK_GPIO, gMode, gLedOn);
    		printf("Double short press detected\n");
    		buttonstate = WaitForPress;
    		break;
    	default:
    		printf("Error occurred, button state is resetting...\n");
    		buttonstate = WaitForPress;
    		break;
    	}
        usleep(MS_TO_U(50));
    }
}
