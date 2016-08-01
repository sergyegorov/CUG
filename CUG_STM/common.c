#include "common.h"


int overInitProtectorData[] = {0,0,0,0,0,0};

void initGPIOBitOut(GPIO_TypeDef* GPIOx, uint16_t pin){
		int protInd;
		if(GPIOx == GPIOA)
			protInd = 0;
		else{
			if(GPIOx == GPIOB)
				protInd = 1;
			else {
				if(GPIOx == GPIOC)
					protInd = 2;
				else {
					if(GPIOx == GPIOD)
						protInd = 3;
					else
						return;
				}
			}
		}
		if((overInitProtectorData[protInd]&pin) != 0)
			return;
		else
			overInitProtectorData[protInd] |= pin;

		GPIO_InitTypeDef GPIO_InitStructure;

		GPIO_InitStructure.GPIO_Pin = pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_Init(GPIOx, &GPIO_InitStructure);
	}
