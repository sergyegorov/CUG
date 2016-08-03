#include "common.h"
#include "rel.h"
#include "time.h"

#define _REL_STROB_PORT GPIOC
#define REL_STROB_PIN GPIO_Pin_9
#define REL_DATA_PORT GPIOC
#define REL_DATA_PIN GPIO_Pin_8
#define REL_CLK_PORT GPIOC
#define REL_CLK_PIN GPIO_Pin_7

//unsigned long RelTickCount = 0;
char IsGasOn = 0;

unsigned int NoGasTickCounter = 0;

#define REL_WAIT(times) delay_ms(200*times)
//#define MSB_FIRST
int relInited = 0;
//int prevVal = -1;
void setRelayN(int val){
	int i;

	//if((val & REL_GAS_MASK) != 0 || ((prevVal & REL_GAS_MASK) != 0 && prevVal != val))
	//	RelTickCount = 0;

	//prevVal = val;
	if((val & REL_GAS_MASK) != 0){
		IsGasOn = 1;
		NoGasTickCounter = 0;
	}else
		IsGasOn = 0;

	if(relInited == 0){
		GPIO_ResetBits(_REL_STROB_PORT,REL_STROB_PIN);
		GPIO_ResetBits(REL_DATA_PORT,REL_DATA_PIN);
		GPIO_ResetBits(REL_CLK_PORT,REL_CLK_PIN);

		initGPIOBitOut(_REL_STROB_PORT,REL_STROB_PIN);
		initGPIOBitOut(REL_DATA_PORT,REL_DATA_PIN);
		initGPIOBitOut(REL_CLK_PORT,REL_CLK_PIN);
		relInited = 1;
	}

#ifdef MSB_FIRST
	int mask = 0x80;
#else
	int mask = 1;
#endif
	for(i = 0;i<8;i++){
		if((val & mask) != 0)
			GPIO_SetBits(REL_DATA_PORT,REL_DATA_PIN);
		else
			GPIO_ResetBits(REL_DATA_PORT,REL_DATA_PIN);
		delay_us(100);
		GPIO_SetBits(REL_CLK_PORT,REL_CLK_PIN);
		delay_us(100);
		GPIO_ResetBits(REL_CLK_PORT,REL_CLK_PIN);
		delay_us(50);
	#ifdef MSB_FIRST
		val <<= 1;
	#else
		val >>= 1;
	#endif
	}

	GPIO_SetBits(_REL_STROB_PORT,REL_STROB_PIN);
	delay_us(100);
	GPIO_ResetBits(_REL_STROB_PORT,REL_STROB_PIN);
	delay_us(100);
}

#define R_DELAY 200
void testRel(){
	int i;
	for(i = 0;i<8;i++){
		setRelayN(1<<i);
		delay_ms(R_DELAY);
	}//*/
	setRelayN(REL_GAS_LOW);
	delay_ms(R_DELAY);
	setRelayN(REL_GAS_MAX);
	delay_ms(R_DELAY);
	setRelayN(REL_GAS_HI);
	delay_ms(R_DELAY);
	setRelayN(REL_MAIN);
	delay_ms(300);
	setRelayN(REL_600V);
	delay_ms(R_DELAY);
	setRelayN(REL_NONE);
	delay_ms(R_DELAY);
}
