#include "tuglab.h"
#include <avr/io.h>
#include <avr/eeprom.h> 

void sleep_us(int mks)
{
	/*int i = 0;

	for(;i<mks;i++)
	{
		asm volatile("nop\n\t"::);
		asm volatile("nop\n\t"::);
		asm volatile("nop\n\t"::);
		asm volatile("nop\n\t"::);
	}*/
	int i;

	mks /= 3;

	if(mks == 0)
	{
		asm volatile("nop\n\t"::);
		return;
	}
	for(i = 0;i<mks;i++)
		asm volatile("nop\n\t"::);
}

void sleep_ms(int mls)
{
	int i = 0;

	/*TCNT0 = 0;
	TIMSK = 0;
	TIFR = 0;
	OCR0 = 255;
	TCCR0 = 4;
	ASSR = 0;*/

	for(;i<mls;i++)
	{
		int j = 0;
		while(j<(386))
			j ++;
	}
	/*while(1)
	{
		if(TCNT0 == 112)
		{
			TCNT0 = 0;
			break;
		}
	}*/
}

unsigned char EEPROM_read_byte(unsigned int addr)
{
	unsigned char ret;
	_EEGET(ret,addr);
	return ret;
	while(EECR & (1<<EEWE))
	EEAR = addr;
	EECR |= (1<<EERE);
	return EEDR;//*/
	//return eeprom_read_byte(addr);
}

void EEPROM_write_byte(unsigned int addr,unsigned char val)
{
	unsigned char ret;
	_EEPUT(addr,val);
	_EEGET(ret,addr);
	
	while(EECR & (1<<EEWE))
	EEAR = addr;
	EEDR = val;
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
	/*asm volatile("nop\n\t"
             "nop\n\t"
             "nop\n\t"
             "nop\n\t"
             "nop\n\t"
             "nop\n\t"
             "nop\n\t"
             "nop\n\t"
             "nop\n\t"
             "nop\n\t"
             ::); */
	EEPROM_read_byte(10);
	if(EEPROM_read_byte(addr) != val)
	{
		EEPROM_read_byte(addr);
	}//*/
	//eeprom_read_byte(addr);
}

unsigned short EEPROM_read_short(unsigned int addr)
{
	unsigned short ret = EEPROM_read_byte(addr);
	ret |= (EEPROM_read_byte(addr+1) << 8);
	return ret;
}

void EEPROM_write_short(unsigned int addr,unsigned short val)
{
	EEPROM_write_byte(addr,val&0xFF);
	EEPROM_write_byte(addr+1,val>>8);
}

