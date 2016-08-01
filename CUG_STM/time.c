#include "time.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

void initHRTimer(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 66;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 0xFFFFFF;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);
	TIM_Cmd(TIM2, ENABLE);
}

void resetHRTimer(){
	TIM_SetCounter(TIM2,0);
}

uint32_t getHRTimerValue(){
	return TIM_GetCounter(TIM2);
}

int delay(uint32_t val){
	uint16_t from = getHRTimerValue(),to;
	int64_t step = HRES_TICKS_IN_SEC;
	step *= val;
	step /=  1000000;
	if(step == 0)
		step = 1;
	while(step > 0){
		to = getHRTimerValue();
		if(to >= from)
			step -= (to - from);
		else
			step -= ((0xFFFF-from) + to + 1);
		from = to;
	}
	return from;
}
