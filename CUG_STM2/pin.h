#ifndef PIN_H
#define PIN_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

class PIN{
protected:
	GPIO_TypeDef* GPIOx;
	uint16_t Pin;
public:
	PIN(GPIO_TypeDef* gpiox, uint16_t pin){
		GPIOx = gpiox;
		Pin = pin;
	}
	~PIN(){
	}

	void init();
};

class PINInput : public PIN{
public:
	PINInput(GPIO_TypeDef* gpiox, uint16_t pin) : PIN(gpiox,pin) {
	}

	void init(){
	}

	bool getState(){
		return GPIO_ReadInputDataBit(GPIOx,Pin);
	}
};

class PINOutput : public PIN{
public:
	PINOutput(GPIO_TypeDef* gpiox, uint16_t pin) : PIN(gpiox,pin) {
	}

	void init(){
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOx, &GPIO_InitStructure);
	}

	void setPin(){
		GPIO_SetBits(GPIOx,Pin);
	}

	void resetPin(){
		GPIO_ResetBits(GPIOx,Pin);
	}
};

#endif
