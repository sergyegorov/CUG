#include "rel.h"
#include <avr/io.h>

void RelRefresh()
{
	for(int j = 0;j<4;j++)
	{
		for(int i = 800;i>=100;i-=100)
		{
			RelSendDataTch(0x5555,i);
			RelSendDataTch(0xAAAA,i);
		}
	}
	RelSendDataTch(0,500);
}

void RelSendData(unsigned int val)
{
	RelSendDataTch(val,550);
}

void RelSendDataTch(unsigned int val,int wait_time)
{
	// setup external device shift register
	PORTA = 7;
	DDRA |= 7;
	int time_const = 100;
	int tmp = val;
	for(int i = 0;i<16;i++)
	{
		if(tmp & 1)
			PORTA &= 0xFD;
		else
			PORTA |= 2;
		for(int j = 0;j<time_const;j++);
		PORTA &= 0xFB;
		for(int j = 0;j<time_const;j++);
		PORTA |= 4;
		for(int j = 0;j<time_const;j++);

		tmp >>= 1;
	}
	PORTA &= 0xFE;
	for(int j = 0;j<time_const;j++);
	PORTA |= 1;
	for(int j = 0;j<wait_time;j++);
	PORTA |= 1;
}

int RelSetCode(char r,char c,char l,
	unsigned char is_out_enable,unsigned char is_spark_enable,
	unsigned char gas_on,unsigned char gas_hi)
{
	int val = 0;//c&0xF;
	if(gas_on != 0)
		val |= 1;
	if(gas_hi != 0)
		val |= 2;
		
	if((r&2) == 0)
		val |= 0b00100000;
	if((r&4) == 0)
		val |= 0b00010000;
	if(is_out_enable)
		val |= 0b01000000;
	if(is_spark_enable)
		val |= 0b10000000;
		
	if((c&8) != 0)
		val |= 0b1000000000000000;
	if((c&4) != 0)
		val |= 0b0100000000000000;
	if((c&2) != 0)
		val |= 0b0010000000000000;
	if((c&1) != 0)
		val |= 0b0001000000000000;
		
	if((l&8) == 0)
		val |= 0b0000000100000000;
	if((l&4) == 0)
		val |= 0b0000001000000000;
	if((l&2) == 0)
		val |= 0b0000010000000000;
	if((l&1) == 0)
		val |= 0b0000100000000000;
		
	RelSendData(val);

	return val;	
}

int RelSet(int r,int c,int l,
	unsigned char is_out_enable,unsigned char is_spark_enable,
	unsigned char gas_on,unsigned char gas_hi)
{
	return RelSetCode(r,c,l/25,is_out_enable,is_spark_enable,gas_on,gas_hi);
}
