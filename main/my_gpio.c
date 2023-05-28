/*
 * mygpio.c
 *
 *  Created on: 25.05.2023
 *      Author: paul
 */

#include <stdio.h>
#include <inttypes.h>
#include "my_gpio.h"



void gpio_enable(uint8_t gpioNumber){
	if(gpioNumber > 21){
		printf("Gpio number is incorrect! There are only 21 Gpios available\n");
		return;
	}
	gpioEnable |= (1 << gpioNumber);
}

void gpio_iomuxConfigure(configureIoMux_t* pIoMuxSettings){
	if(pIoMuxSettings->GpioNumber > 21){
		printf("Gpio number is incorrect! There are only 21 Gpios available\n");
	}
volatile IoMuxGpioReg_t* pIoMuxGpioReg = (volatile IoMuxGpioReg_t*) ((uintptr_t)IO_MUX_BASE | IO_MUX_OFFSET(pIoMuxSettings->GpioNumber));
pIoMuxGpioReg->ioMuxGpioMcuSel = pIoMuxSettings->PinFunc;
pIoMuxGpioReg->ioMuxGpioFunDrv = pIoMuxSettings->PinCurrent;
pIoMuxGpioReg->ioMuxGpioFunIe  = pIoMuxSettings->OutputConfigure;
pIoMuxGpioReg->ioMuxGpioFunWpd = pIoMuxSettings->Pulldown;
pIoMuxGpioReg->ioMuxGpioFunWpu = pIoMuxSettings->Pullup;
}

void gpio_outEnable(uint8_t gpioNumber){
	if(gpioNumber > 21){
		printf("Gpio number is incorrect! There are only 21 Gpios available\n");
		return;
	}
	gpioOutput |= (1 << gpioNumber);
}

void gpio_outDisable(uint8_t gpioNumber){
	if(gpioNumber > 21){
		printf("Gpio number is incorrect! There are only 21 Gpios available\n");
		return;
	}
	gpioOutput &= ~(1 << gpioNumber);
}


void gpio_outToggle(uint8_t gpioNumber){
	if(gpioNumber > 21){
		printf("Gpio number is incorrect! There are only 21 Gpios available\n");
		return;
	}
	gpioOutput ^= (1 << gpioNumber);
}



