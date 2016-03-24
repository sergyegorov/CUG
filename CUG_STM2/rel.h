#ifndef REL_H
#define REL_H

#include "common.h"

#define REL_STROB_PORT GPIOC
#define REL_STROB_PIN GPIO_Pin_9
#define REL_DATA_PORT GPIOC
#define REL_DATA_PIN GPIO_Pin_8
#define REL_CLK_PORT GPIOC
#define REL_CLK_PIN GPIO_Pin_7

class REL{
private:

protected:
	void initGPIO(){
		initGPIOBitOut(REL_STROB_PORT,REL_STROB_PIN);
		initGPIOBitOut(REL_DATA_PORT,REL_DATA_PIN);
		initGPIOBitOut(REL_CLK_PORT,REL_CLK_PIN);
	}
public:
	REL(){
		GPIO_ResetBits(REL_STROB_PORT,REL_STROB_PIN);
		GPIO_ResetBits(REL_DATA_PORT,REL_DATA_PIN);
		GPIO_ResetBits(REL_CLK_PORT,REL_CLK_PIN);

		initGPIO();
	}

	void send(int val){
		int mask = 1;
		for(int i = 0;i<16;i++){
			if((val & mask) != 0)
				GPIO_SetBits(REL_DATA_PORT,REL_DATA_PIN);
			else
				GPIO_ResetBits(REL_DATA_PORT,REL_DATA_PIN);
			delay_us(50);
			GPIO_SetBits(REL_CLK_PORT,REL_CLK_PIN);
			delay_us(100);
			GPIO_ResetBits(REL_CLK_PORT,REL_CLK_PIN);
			mask <<= 1;
		}
		GPIO_SetBits(REL_STROB_PORT,REL_STROB_PIN);
		delay_us(100);
		GPIO_ResetBits(REL_STROB_PORT,REL_STROB_PIN);
	}
};
#endif
