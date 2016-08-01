#ifndef COMMON_H
#define COMMON_H
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

#define false 0
#define true 1

extern void initGPIOBitOut(GPIO_TypeDef* GPIOx, uint16_t pin);

#endif
