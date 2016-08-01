#ifndef time_h
#define time_h

#include "stm32f10x.h"

#define HRES_TICKS_IN_SEC   0xFF00*25

extern void initHRTimer();
extern void resetHRTimer();
extern uint32_t getHRTimerValue();

extern int delay(uint32_t val);

#define delay_us(val) delay((int)(val))
#define delay_ms(val) delay((int)(1000*val))

#endif
