/*
 * mygpio.h
 *
 *  Created on: 25.05.2023
 *      Author: paul
 */
#include <inttypes.h>

#ifndef MAIN_MY_GPIO_H_
#define MAIN_MY_GPIO_H_

#define GPIO_BASE 0x60004000
#define GPIO_OUT_OFFSET 0x04
#define GPIO_EN_OFFSET 0x20
#define GPIO_IN_REG_OFFSET 0x3C
#define gpioEnable *((volatile uint32_t*) (GPIO_BASE | GPIO_EN_OFFSET))
#define gpioOutput *((volatile uint32_t*) (GPIO_BASE | GPIO_OUT_OFFSET))
#define gpioInput *((volatile uint32_t*) (GPIO_BASE | GPIO_IN_REG_OFFSET))

#define IO_MUX_BASE 0x60009000
#define IO_MUX_OFFSET(i) (0x4 + 4 * i)
#define ioMuxGpioReg(i) *((volatile uint32_t*) (IO_MUX_BASE | IO_MUX_OFFSET(i)))

typedef struct IoMuxSettings{
	uint8_t GpioNumber 		;
	uint8_t PinFunc 		;
	uint8_t PinCurrent  	;
	uint8_t OutputConfigure ;
	uint8_t Pullup 			;
	uint8_t Pulldown 		;
	uint8_t RFU 			;
}ConfigureIoMux_t;

typedef union {
	struct{
		uint32_t ioMuxGpioMcuOe    : 1;  //bit 0
		uint32_t ioMuxGpioSlpSel   : 1;  //bit 1
		uint32_t ioMuxGpioMcuWpd   : 1;  //bit 2
		uint32_t ioMuxGpioMcuWpu   : 1;  //bit 3
		uint32_t ioMuxGpioMcuIe    : 1;  //bit 4
		uint32_t ioMuxGpioMcuDrv   : 2;  //bits 5-6
		uint32_t ioMuxGpioFunWpd   : 1;  //bit 7
		uint32_t ioMuxGpioFunWpu   : 1;  //bit 8
		uint32_t ioMuxGpioFunIe    : 1;  //bit 9
		uint32_t ioMuxGpioFunDrv   : 2;  //bits 10 - 11
		uint32_t ioMuxGpioMcuSel   : 3;  //bits 12 - 14
		uint32_t ioMuxGpioFilterEn : 1;  //bit 15
		uint32_t rfu			   : 16; //bits 16 - 31
	};
	uint32_t IoMuxGpioRegValue;
}IoMuxGpioReg_t;



void gpio_enable(uint8_t gpionumber);
void gpio_iomuxConfigure(ConfigureIoMux_t* pIoMuxSettings);
void gpio_outEnable(uint8_t gpioNumber);
void gpio_outDisable(uint8_t gpioNumber);
void gpio_outToggle(uint8_t gpioNumber);





#endif /* MAIN_MYGPIO_H_ */
