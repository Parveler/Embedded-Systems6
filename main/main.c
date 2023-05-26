

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

#define BUTTON 6
#define LONG_BUTTON_PRESS 1000
#define WAIT_FOR_ANOTHER_SHORT_PRESS 300



bool gPwmMode = true;
bool gLedOn = true;
uint8_t gMode = 1;

void app_main(void)
{
	ConfigureIoMux_t IoMuxSettings;
	IoMuxSettings.GpioNumber 		= 6;
	IoMuxSettings.OutputConfigure 	= 1;
	IoMuxSettings.PinCurrent 		= 2;
	IoMuxSettings.PinFunc 			= 1;
	IoMuxSettings.Pullup			= 0;
	IoMuxSettings.Pulldown			= 0;

	button_configure(&IoMuxSettings);

	static led_strip_handle_t led_strip;
	led_strip_configure(8, led_strip);
	led_strip_setLed(led_strip, 8, gMode, gLedOn);

	enum ButtonStates buttonstate = WaitForPress;
	uint64_t TimeButtonPressedBeginn = 0;
	uint64_t TimeButtonPressed = 0;
	uint64_t TimeButtonPressedReleased = 0;

    while (true) {

    	switch(buttonstate){

    	case WaitForPress:
    		if(button_getLevel(BUTTON)){
    			TimeButtonPressedBeginn = esp_timer_get_time();
    			buttonstate = WaitForRelease;
    		}
    		break;

    	case WaitForRelease:
    		printf("NIOT IN LOOP\n");
    		if(button_getLevel(BUTTON) == false){
    			printf(" IN LOOP\n");
    			TimeButtonPressedReleased = esp_timer_get_time();
    			TimeButtonPressed = TimeButtonPressedReleased -TimeButtonPressedBeginn;
    			TimeButtonPressed = U_TO_MS(TimeButtonPressed);
    			printf("TIME:%"PRIu64"\n",TimeButtonPressed);
    		}
    		if(TimeButtonPressed < LONG_BUTTON_PRESS){
    			buttonstate = WaitForAnotherPress;
    		}
    		else{
    			buttonstate = LongPressDetected;
    		}
    		break;
    	case WaitForAnotherPress:
    		TimeButtonPressedBeginn = esp_timer_get_time();
    		TimeButtonPressed = TimeButtonPressedBeginn -TimeButtonPressedReleased;
    		TimeButtonPressed = U_TO_MS(TimeButtonPressed);
    		if(TimeButtonPressed < WAIT_FOR_ANOTHER_SHORT_PRESS && button_getLevel(BUTTON)){
    		    buttonstate = DoubleShortPressDetected;
    		}
    		else if(TimeButtonPressed > WAIT_FOR_ANOTHER_SHORT_PRESS){
    		    buttonstate = ShortPressDetected;
    		}
    		break;
    	case ShortPressDetected:
    		gMode += 1;
    		if(gMode > 3){
    			gMode = 0;
    		}
    		led_strip_setLed(led_strip, 8, gMode, gLedOn);

    		break;
    	case LongPressDetected:
    		led_pwm(4, gPwmMode, gLedOn);
    		gPwmMode = !gPwmMode;
    		printf("Long button press detected!\n");
    		buttonstate = WaitForPress;
    		break;
    	case DoubleShortPressDetected:
    		gLedOn = !gLedOn;
    		break;
    	default:
    		break;
    	}
        usleep(MS_TO_U(50));
    }
}
