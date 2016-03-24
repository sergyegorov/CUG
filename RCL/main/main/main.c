#include <avr/io.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "lcd.h"
#include "tuglab.h"
#include "key.h"
#include "menu.h"
#include "rel.h"
#include "spi.h"
#include "spi_command.h"

#define DEBUG

void InitChargerInterface()
{
	SPI_Init();
}

void Init()
{
	PORTE |= 0x20;
	DDRE |= 0x20;
	PORTB |= 0x47;
	DDRB |= 0x47;
	PORTD |= 1;
	DDRD |= 0x11;
	InitChargerInterface();
}

void ChargerStart(char flag)
{
	/*if(flag == 0)
		PORTD |= 0x10;//0x40;
	else
		PORTD &= 0xEF;//0xBF;*/
	if(flag == 0)
		PORTE |= 0x20;//0x40;
	else
		PORTE &= 0xDF;//0xBF;
}

char ChargerIsReady()
{
	if((PINB & 0x10) != 0)
		return 0;
	else
		return 1;
}

char ReadChargerError()//ChargerHasError()
{
	//return (PIND & 0x4) == 0;
	SPI_Transmit(0,SPI_ERROR);
	sleep_ms(1);
	return SPI_Transmit(0,SPI_NOP);
}

unsigned int ReadU()
{
	unsigned int ul,uh;
	SPI_Transmit(0,SPI_ULo);
	sleep_ms(1);
	ul = SPI_Transmit(0,SPI_NOP);
	//sleep_ms(1);
	SPI_Transmit(0,SPI_UHi);
	sleep_ms(1);
	uh = SPI_Transmit(0,SPI_NOP);
	return (uh<<8)|ul;
}

void Error(char* str)
{
	ChargerStart(false);
	LCDClear();
	LCDMoveTo(0,0);
	LCDPrintString(str);
	sleep_ms(1000);
}

struct DebugInfo DInfo;
void ReadDebugInfo()
{
	int i;
	char *pointer = (char*)&DInfo;
	SPI_Transmit(0,SPI_DEBUG_INFO_START);
	sleep_ms(1);
	for(i = 0;i<DebugInfoSizeInWord;i++,pointer++)
	{
		SPI_Transmit(0,SPI_DEBUG_INFO_STEP_LO);
		sleep_ms(1);
		*pointer = SPI_Transmit(0,SPI_NOP);
		sleep_ms(1);

		pointer ++;

		SPI_Transmit(0,SPI_DEBUG_INFO_STEP_HI);
		sleep_ms(1);
		*pointer = SPI_Transmit(0,SPI_NOP);
		sleep_ms(1);
	}
}

void ErrorDebug()
{
	ChargerStart(false);

	LCDClear();

	LCDMoveTo(0,0);
	LCDPrintString("Cod");
	LCDPrintInt(DInfo.Code);
	LCDPrintString(" U");
	LCDPrintInt(DInfo.U);
	LCDPrintString(" ");
	LCDPrintInt(DInfo.UMust);

	LCDMoveTo(1,0);
	LCDPrintString("Stage:");
	LCDPrintInt(DInfo.Stage);
	LCDPrintString(" I");
	LCDPrintInt(DInfo.Impuls);
	LCDPrintString(" t");
	LCDPrintInt(DInfo.Time);

	LCDMoveTo(2,0);
	LCDPrintString(" Ok:");
	LCDPrintInt(DInfo.StepOk);
	LCDPrintString(" Err:");
	LCDPrintInt(DInfo.StepError);
	
	sleep_ms(1000);
}

void ErrorCode(char* str,char code,int last_u)
{
	int line = 1;
	ChargerStart(false);
	LCDClear();
	LCDMoveTo(0,0);
	LCDPrintString(str);
	LCDPrintString(" ");
	LCDPrintInt(code);
	LCDPrintString(" ");
	LCDPrintInt(last_u);

	if((code & SPI_ERROR_ULow) != 0)
	{
		LCDMoveTo(line,0);
		LCDPrintString(" Недозаряд");
		line ++;
	}
	if((code & SPI_ERROR_UHi) != 0)
	{
		LCDMoveTo(line,0);
		LCDPrintString(" Перезаряд");
		line ++;
	}
	if((code & SPI_ERROR_OverTime) != 0)
	{
		LCDMoveTo(line,0);
		LCDPrintString(" TimeOut");
		line ++;
	}
	if((code & SPI_ERROR_NO_DISCHARGE) != 0)
	{
		LCDMoveTo(line,0);
		LCDPrintString(" No Discharge");
		line ++;
	}
	sleep_ms(2000);
}

void Spark()
{
	DDRD |= 1;
	PORTD &= 0xFE;
	sleep_us(10);
	PORTD |= 1;	
}

char ChargeAndSpark()
{
	int read_error;
	
	read_error = SPI_Transmit(0,SPI_START);
	
	return read_error;
}

void Start(char by_key)
{
	int repl,repl_lo;
	char last_error = SPI_ERROR_NONE;
	int last_u = 0;

	LCDClear();
	LCDMoveTo(1,3);
	LCDPrintString("Р А З Р Я Д!!!");

	SPI_Transmit(0,SPI_COMMAND_START_NEW);
	sleep_ms(1);
	
	RelSetCode(Rv,Cv,Lv,1,1,0,1); // spak on
	
	SPI_Transmit(0,SPI_C_INFO);
	SPI_Transmit(0,Cv);
	sleep_ms(1);
	repl = SPI_Transmit(0,SPI_NOP);
	if(repl != Cv+1)
	{
		Error("Ош.Уст.Ёмкости");
		RelSet(0,0,0,0,0,0,0);
		return;
	}
	
	sleep_ms(200);
	
	/*for(int i = 0;i<100 || ReadKeyCode() != 0;i++)
	{
		Spark();
		sleep_ms(20);
		//sleep_us(50);
	}
	
	RelSet(0,0,0,0,0,0,0);
	
	return;//*/
	
	SPI_Transmit(0,SPI_SET_U);
	SPI_Transmit(0,Uv);
	sleep_ms(10);
	repl = SPI_Transmit(0,SPI_NOP);
	if(repl != Uv+1)
	{
		Error("Ош.Уст.Напряжения");
		RelSet(0,0,0,0,0,0,0);
		return;	
	}
	SPI_Transmit(0,SPI_SET_I);
	SPI_Transmit(0,Iv);
	sleep_ms(10);
	repl = SPI_Transmit(0,SPI_NOP);
	if(repl != Iv+1)
	{
		Error("Ош.Уст.Тока");
		RelSet(0,0,0,0,0,0,0);
		return;	
	}
	SPI_Transmit(0,SPI_SET_I1);
	SPI_Transmit(0,Iv1);
	sleep_ms(10);
	repl = SPI_Transmit(0,SPI_NOP);
	if(repl != Iv1+1)
	{
		Error("Ош.Уст.Тока1");
		RelSet(0,0,0,0,0,0,0);
		return;	
	}
	SPI_Transmit(0,SPI_SET_Ph);
	SPI_Transmit(0,Phv);
	sleep_ms(10);
	repl = SPI_Transmit(0,SPI_NOP);
	if(repl != Phv+1)
	{
		Error("Ош.Уст.Длит.");
		RelSet(0,0,0,0,0,0,0);
		return;
	}
	
	sleep_ms(PreBlowTime*1000);

	RelSetCode(Rv,Cv,Lv,1,1,1,0);
	RelSetCode(Rv,Cv,Lv,1,1,1,0);
	
	timerSetup(Fv);
	
	int error_count = 0;
	int count = 0;
	if(by_key)
	{
		//for(;ReadKeyCode() != 0;count ++)
		{
			timerStart();
			
			last_error = ChargeAndSpark();
			if(count > 100 && last_error != SPI_ERROR_NONE)
				error_count ++;
			else
				error_count /= 2;
			if(error_count > 10)
			{
				last_u = ReadU();
				ReadDebugInfo();
				ErrorCode("Ош.:",last_error,last_u);
				while(ReadKeyCode() != 0);
			}
			timerWaitForEnd();
		}
		ReadDebugInfo();
		ErrorDebug();
		while(ReadKeyCode() != 0);
	}
	else
	{
		//int positive = 0;
		int negative = 0;
		for(;IsStartSignal() != 0;count ++)
		//for(count = 0;count < 10;count ++)
		{
			timerStart();
			
			last_error = ChargeAndSpark();
			if(count > 200)
			{
				if(last_error != SPI_ERROR_NONE)
				{
					if(last_error == SPI_ERROR_ULow)
						error_count ++;
					else
						error_count += 2;
					negative ++;
				}
				else
				{
					error_count /= 2;
					//positive ++;
				}
			}
			
			if(error_count >= 40)
			{
				RelSet(0,0,0,0,0,0,0);
				last_u = ReadU();
				LCDClear();
				LCDMoveTo(0,0);
				LCDPrintString("Er/Al=");
				LCDPrintInt(negative);
				LCDPrintString("/");
				LCDPrintInt(count);
				LCDPrintString(" ");
				LCDPrintInt(error_count);
				
				ReadDebugInfo();
				ErrorDebug();
				while(IsStartSignal())
					sleep_ms(1);
			}//*/
			timerWaitForEnd();
		}
	}
	RelSet(0,0,0,0,0,0,0);
	
	SPI_Transmit(0,SPI_READ_STAT);
	sleep_ms(1);
	unsigned int ever_u = SPI_Transmit(0,SPI_NOP)&0xFF;	sleep_ms(1);
	unsigned int tmp = SPI_Transmit(0,SPI_NOP)&0xFF; 	sleep_ms(1);
	tmp <<= 8;
	ever_u |= tmp;
	
	unsigned int err_u = SPI_Transmit(0,SPI_NOP)&0xFF; sleep_ms(1);
	tmp = SPI_Transmit(0,SPI_NOP)&0xFF; 	sleep_ms(1);
	tmp <<= 8;
	ever_u |= tmp;
		
	LCDClear();
	LCDMoveTo(0,0);
	LCDPrintString("EvU");
	LCDPrintInt(ever_u);
	LCDPrintString("eru");
	LCDPrintInt(err_u);
	LCDPrintString("Cd");
	LCDPrintInt(last_error);
	
	//sleep_ms(1000);

	LCDMoveTo(1,0);
	SPI_Transmit(0,SPI_READ_U);
	sleep_ms(1);
	repl = (SPI_Transmit(0,SPI_NOP)&0xFF)<<8;
	repl |= (SPI_Transmit(0,SPI_NOP)&0xFF);
	LCDPrintString(" Uc     =");LCDPrintInt(repl);
	LCDMoveTo(2,0);
	repl = (SPI_Transmit(0,SPI_NOP)&0xFF)<<8;
	repl |= (SPI_Transmit(0,SPI_NOP)&0xFF);
	LCDPrintString(" Uin_rez=");LCDPrintInt(repl);
	LCDMoveTo(3,0);
	repl = (SPI_Transmit(0,SPI_NOP)&0xFF)<<8;
	repl |= (SPI_Transmit(0,SPI_NOP)&0xFF);
	LCDPrintString(" Uin_src=");LCDPrintInt(repl);
	sleep_ms(4000);

	timerSetup(0);

	SPI_Transmit(0,SPI_SET_I);
	SPI_Transmit(0,0);
	sleep_ms(10);
	repl = SPI_Transmit(0,SPI_NOP);
	if(repl != 1)
	{
		Error("Ош.Уст.Напряжения2");
		RelSet(0,0,0,0,0,0,0);
		return;
	}
	SPI_Transmit(0,SPI_SET_U);
	SPI_Transmit(0,0);
	sleep_ms(10);
	repl = SPI_Transmit(0,SPI_NOP);
	if(repl != 1)
	{
		Error("Ош.Уст.Тока2");
		RelSet(0,0,0,0,0,0,0);
		return;
	}

	for(int i = 0;i<30 && ReadKeyCode() != 0;i++)
		sleep_ms(100);
}

int main()
{
	//int i;
	int selected_col = 0;
	int prev_key = -1;
	char is_big_step = 1;
	int key,mode = 0;

	/*for(int i = 0;i<5;i++)
	{
		LCDLedStatus(1);
		RelSet(6,15,375,1,1,1,1);
		LCDLedStatus(0);
		RelSet(0,0,0,0,0,0,0);
	}//*/
	LCDLedStatus(1);

	/*while(1)
	{
		sleep_ms(25);
		selected_col ++;
		if((selected_col&1) != 0)
			PORTD |= 0x20;
		else
			PORTD &= 0xDF;
	}
	/*timerSetup(8);
	timerStart();
	while(1)
	{
		//timerStart();
		timerWaitForEnd();
		selected_col ++;
		if((selected_col&1) != 0)
			PORTD |= 0x20;
		else
			PORTD &= 0xDF;
	}*/
	/*while(1)
	{
		//RelSet(0,0,0,0,0,1,0); // gas on
		//RelSet(0,0,0,0,0,0,1); // gas hi
		//RelSet(0,0,0,0,0,0,0); // R0
		//RelSet(2,0,0,0,0,0,0); // R2
		//RelSet(4,0,0,0,0,0,0); // R4
		//RelSet(6,0,0,0,0,0,0); // R6
		//RelSet(0,0,0,1,0,0,0); // pow on
		//RelSet(0,0,0,0,1,0,0); // spak on
		//RelSendData(0xFFFF);
		//RelSet(0,0,25,0,0,0,0); // L25
		//RelSet(0,0,50,0,0,0,0); // L50
		//RelSet(0,0,100,0,0,0,0); // L100
		//RelSet(0,0,200,0,0,0,0); // L200
		//RelSet(0,0,375,0,0,0,0); // L375
		//RelSet(0,1,0,0,0,0,0); // C1
		//RelSet(0,2,0,0,0,0,0); // C2
		//RelSet(0,4,0,0,0,0,0); // C4
		//RelSet(0,8,0,0,0,0,0); // C8
		//RelSet(0,15,0,0,0,0,0); // C15
		for(int j = 0;j<5;j++)
		for(int i = 0;i<32000;i++);
		RelSet(0,0,0,0,0,0,0);
		for(int j = 0;j<5;j++)
		for(int i = 0;i<32000;i++);
	}*/
	
	PORTE |= 0x20;
	DDRE |= 0x20;

	for(int i = 0;i<10000;i++)
	;

	LCDInit();

	Init();

	/*while(1)
	{
		LCDLedStatus(0);
		ChargerStart(false);
		for(int j = 0;j<5;j++)
		for(int i = 0;i<32000;i++);
		LCDLedStatus(1);
		ChargerStart(true);
		for(int j = 0;j<5;j++)
		for(int i = 0;i<32000;i++);
	}//*/

	/*LCDClear();
	while(1)
	{
		LCDMoveTo(0,5);
		if(IsStartSignal() == 0)
			LCDPrintString("0");
		else
			LCDPrintString("1");
	}//*/

	LCDClear();
	RelSet(0,0,0,0,0,0,0);
	LCDMoveTo(0,5);
	LCDPrintString("Ц У Г - III");
	#ifndef DEBUG
	sleep_ms(500);
	#endif
	LCDMoveTo(1,6);
	LCDPrintString("Цифровой");
	#ifndef DEBUG
	sleep_ms(1500);
	#endif
	LCDMoveTo(2,3);
	LCDPrintString("Универсальный");
	#ifndef DEBUG
	sleep_ms(1500);
	#endif
	LCDMoveTo(3,6);
	LCDPrintString("Генератор");

	#ifndef DEBUG
	sleep_ms(3000);
	#else
	sleep_ms(500);
	#endif

	ChargerStart(false);
	
	/*while(SPI_Transmit(0,SPI_COMMAND_HELLO) != SPI_COMMAND_HREPLY)
		sleep_ms(10);

	while(ChargerIsReady() == 0)
		;//*/

	LoadFromEPROM();
	
	LCDClear();
	LCDLedStatus(0);

	//Start(1);
	while(1)
	{
		key = ReadKeyCode();
		if(mode == 0)
		{
			switch(key)
			{
			case 0x1:
				Start(1);
				break;
			case 0x80:	// down left SETUP
				mode = 1;
				break;
			}
		}
		else
		{
			switch(key)
			{
			case 0x4:
				selected_col --;
				break;
			case 0x20:
				selected_col ++;
				break;
			case 0x8:
				if(CurVal != 0)
				{
					if(*CurVal < CurValMax)
					{
						if(is_big_step)
							(*CurVal) += CurValBigStep;
						else
							(*CurVal) ++;
						SaveToEPROM();
					}
				}
				break;
			case 0x10:
				if(CurVal != 0)
				{
					if(*CurVal > CurValMin)
					{
						if(is_big_step)
							(*CurVal) -= CurValBigStep;
						else
							(*CurVal) --;
						SaveToEPROM();
					}
				}
				break;
			case 0x40:	// up left STEP
				if(is_big_step == 0)
					is_big_step = 1;
				else
					is_big_step = 0;
				break;
			case 0x80:	// down left SETUP
				mode = 0;
				break;
			}
		}
		
		if(IsStartSignal())
		{
			Start(0);
			PrintInfoScr();
		}			

		if(prev_key == key)
			continue;
		for(int i = 0;i<200;i++)
		{
			key = ReadKeyCode();
			if(key != 0)
				i = 0;
		}
		if(mode == 1)
			selected_col = PrintScr(selected_col,is_big_step);			
		else
			PrintInfoScr();
		prev_key = key;
	}
	return 0;
}

