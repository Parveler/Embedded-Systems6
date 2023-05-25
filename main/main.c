

#include "my_gpio.h"
#include "button.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>
#include"esp_timer.h"

#define BUTTON 6
#define LONG_BUTTON_PRESS 1000
#define WAIT_FOR_ANOTHER_SHORT_PRESS 300


void app_main(void)
{

	ConfigureIoMux_t IoMuxSettings;
	IoMuxSettings.GpioNumber = 6;
	IoMuxSettings.OutputConfigure = 1;
	IoMuxSettings.PinCurrent = 2;
	IoMuxSettings.PinFunc = 1;
	button_configure(&IoMuxSettings);

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
    		if(!button_getLevel(BUTTON)){
    			TimeButtonPressedReleased = esp_timer_get_time();
    			TimeButtonPressed = TimeButtonPressedReleased -TimeButtonPressedBeginn;
    			TimeButtonPressed /= 1000;
    		}
    		if(TimeButtonPressed < LONG_BUTTON_PRESS){
    			buttonstate = ShortPressDetected;
    		}
    		else{
    			buttonstate = LongPressDetected;
    		}
    		break;
    	case WaitForAnotherPress:
    		break;
    	case ShortPressDetected:
    		TimeButtonPressedBeginn = esp_timer_get_time();
    		TimeButtonPressed = TimeButtonPressedBeginn -TimeButtonPressedReleased;
    		TimeButtonPressed /= 1000;
    		if(TimeButtonPressed < WAIT_FOR_ANOTHER_SHORT_PRESS && button_getLevel(BUTTON)){
    			buttonstate = DoubleShortPressDetected;
    		}
    		else if(TimeButtonPressed > WAIT_FOR_ANOTHER_SHORT_PRESS){
    			//Code für Short Press
    		}
    		break;
    	case LongPressDetected:
    		//Code für Long Press
    		break;
    	case DoubleShortPressDetected:
    		//Code für DoubleShort Press
    		break;
    	default:
    		break;
    	}
        sleep(1);
    }
}
