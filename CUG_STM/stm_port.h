#ifndef STM_PORT_H
#define STM_PORT_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"

extern void initOutputPin(GPIO_TypeDef*  port,int pin);
extern void initInputPin(GPIO_TypeDef*  port,int pin);
extern void outPin(GPIO_TypeDef*  port,int mask,int val);
extern int getPin(GPIO_TypeDef*  port,int mask);

extern void initADC();

#endif
