#ifndef time_h
#define time_h

#include "stm32f4xx.h"

#define HRES_TICKS_IN_SEC   41868635L/2

extern void delay(uint32_t val);

extern void initHRTimer();
extern void resetHRTimer();
extern uint32_t getHRTimerValue();

#define delay_us(val) delay((int)(val))
#define delay_ms(val) delay((int)(1000*val))

#endif
