/*
 * timer.c
 *
 * Created: 21.03.2013 17:21:01
 *  Author: root
 */ 
#include "timer.h"
#include <avr/io.h>
#include "common.h"

void timerSetup(int f)
{
	switch(f)
	{
		case 0:
			TCCR0 = 0;
			OCR0 = 0;
			break;
		case 1:	// 50Gz
			TCCR0 = 0b00001000 | 0b111;
			OCR0 = 143;		
			break;
		case 2:	// 100Gz
			TCCR0 = 0b00001000 | 0b111;
			OCR0 = 71;		
			break;
		case 3:	// 150Gz
			TCCR0 = 0b00001000 | 0b110;
			OCR0 = 191;		
			break;
		case 4:	// 200Gz
			TCCR0 = 0b00001000 | 0b110;
			OCR0 = 143;		
			break;
		case 5:	// 250Gz
			TCCR0 = 0b00001000 | 0b101;
			OCR0 = 229;		
			break;
		case 6:	// 300Gz
			TCCR0 = 0b00001000 | 0b101;
			OCR0 = 191;		
			break;
		case 7:	// 350Gz
			TCCR0 = 0b00001000 | 0b101;
			OCR0 = 164;		
			break;
		case 8:	// 400Gz
			TCCR0 = 0b00001000 | 0b101;
			OCR0 = 143;		
			break;
		default:
			TCCR0 = 0;
			OCR0 = 0;
			break;
	}
}

void timerStart()
{
	TCNT0 = 0;
}

void timerWaitForEnd()
{
	while(TCNT0 == 0)
		asm("nop");
	while(TCNT0 != 0)
	{
		if(ChargerIsReady() == false)
			LCDLedStatus(1);
		else
			LCDLedStatus(0);
		//asm("nop");
	}		
}