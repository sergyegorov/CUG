/*
 * signal.c
 *
 * Created: 13.03.2013 16:57:18
 *  Author: root
 */ 

#include<avr\io.h>
#include "signal.h"

char IsStartSignal()
{
	DDRD &= 0b11111011;
	if((PIND&0b00000100) != 0)
		return 0;
	return 1;
}
