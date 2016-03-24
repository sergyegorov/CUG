#include "time.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

void initHRTimer(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 1;
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
