#ifndef COMMON_H
#define COMMON_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#ifndef NULL
#define NULL 0
#endif

inline void delay_us(int mks){
	mks *= 24;
	for(int t = 0;t<mks;t++){
		asm("NOP");
	}
}

inline void delay_ms(int mks){
	mks *= 24000;
	for(int t = 0;t<mks;t++){
		asm("NOP");
	}
}

inline void initGPIOBitOut(GPIO_TypeDef* GPIOx, uint16_t pin){
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

#endif
