#ifndef HR_TIMER_H
#define HR_TIMER_H
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

#define TICK_MS 53906
extern void initHRTimer();


inline void resetHRTimer(){
	TIM_SetCounter(TIM2,0);
}

inline uint16_t getHRTimerValue(){
	return TIM_GetCounter(TIM2);
}

inline void waitHR(int time){
	int to = getHRTimerValue() + time;
	while(to > getHRTimerValue())
		;
}

#endif
