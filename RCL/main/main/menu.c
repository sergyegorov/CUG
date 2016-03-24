#include <stdio.h>
#include "lcd.h"
#include "eeprom.h"
#include "tuglab.h"

char Cv = 1;
char Uv = 0;
char Iv = 0;
char Iv1 = 0;
char Lv = 0;
char Rv = 0;
char Phv = 1;
char Fv = 0;
char PreBlowTime = 1;

char CalcCRC()
{
	int crc = Cv;
	crc += Uv ^ 0x23; 
	crc += Iv ^ 0x84;
	crc += Iv1 ^ 0x84;
	crc += Lv ^ 0x21;
	crc += Rv ^ 0xA8;
	crc += Phv ^ 0xB0;
	crc += Fv ^ 0x31;
	crc += PreBlowTime ^ 0x44;
	crc += 2;
	return crc;
}

void sleep()
{ for(int i = 0;i<20;i++);}

#define BASE_EEPROM 30
void SaveToEPROM()
{
	/*for(int i = 0;i<10;i++)
	{
		eeprom_write_byte(20,0x5A);
		sleep();
		if((20) == 0x5A)
			break;
	} //while();*/
	EEPROM_write_byte(BASE_EEPROM+1,Cv);
	sleep();
	EEPROM_write_byte(BASE_EEPROM+2,Uv);
	sleep();
	EEPROM_write_byte(BASE_EEPROM+3,Iv);
	sleep();
	EEPROM_write_byte(BASE_EEPROM+4,Lv);
	sleep();
	EEPROM_write_byte(BASE_EEPROM+5,Rv);
	sleep();
	EEPROM_write_byte(BASE_EEPROM+6,Phv);
	sleep();
	EEPROM_write_byte(BASE_EEPROM+7,Fv);
	sleep();
	EEPROM_write_byte(BASE_EEPROM+8,PreBlowTime);
	sleep();
	EEPROM_write_byte(BASE_EEPROM+9,Iv1);
	sleep();
	EEPROM_write_byte(BASE_EEPROM+10,CalcCRC());
	sleep();
	EEPROM_write_byte(0,0);
	sleep();
}



void LoadFromEPROM()
{
	Cv = EEPROM_read_byte(BASE_EEPROM+1);
	Uv = EEPROM_read_byte(BASE_EEPROM+2);
	Iv = EEPROM_read_byte(BASE_EEPROM+3);
	Lv = EEPROM_read_byte(BASE_EEPROM+4);
	Rv = EEPROM_read_byte(BASE_EEPROM+5);
	Phv = EEPROM_read_byte(BASE_EEPROM+6);
	Fv = EEPROM_read_byte(BASE_EEPROM+7);
	PreBlowTime = EEPROM_read_byte(BASE_EEPROM+8);
	Iv1 = EEPROM_read_byte(BASE_EEPROM+9);
	EEPROM_read_byte(0);

	if(EEPROM_read_byte(BASE_EEPROM+10) != CalcCRC())
	{
		Cv = 1;
		Uv = 50;
		Iv = 0;
		Iv1 = 0;
		Lv = 1;
		Rv = 0;
		Phv = 30;
		Fv = 1;
		PreBlowTime = 1;

		SaveToEPROM();
	}
}

char CurValMin,CurValMax,CurValBigStep;
char *CurVal;

#define FMin 1
#define FMax 8
#define FBigStep 1
int PrintF(int x,int y,int val)
{
	if(val < FMin || val > FMax)
	{
		LCDPrintString("    ");
	}
	else
	{
		LCDMoveTo(y,x);
		//LCDPrintString("F");
		LCDPrintInt(val*50);
	}
	return 4;
}

#define CMin 0
#define CMax 15
#define CBigStep 1
int PrintC(int x,int y,int val)
{
	if(val < CMin || val > CMax)
	{
		LCDPrintString("    ");
	}
	else
	{
		LCDMoveTo(y,x);
		if(val > 0)
		{
			//LCDPrintString("C");
			LCDPrintInt(val);
		}
		else
			LCDPrintString("Дуг");
		//LCDPutChar(' ');
	}
	return 4;
}

#define UMin 0
#define UMax 100
#define UBigStep 50
int PrintU(int x,int y,int val)
{
	if(val < UMin || val > UMax)
	{
		LCDPrintString("    ");
	}
	else
	{
		LCDMoveTo(y,x);
		int v = 350 + val;
		//LCDPutChar(' ');
		//LCDPrintString("U");
		LCDPrintInt(v);
		//LCDPutChar(' ');
	}
	return 4;
}

#define LMin 0
#define LMax 0xF
#define LBigStep 1
int PrintL(int x,int y,int val)
{
	if(val < LMin || val > LMax)
	{
		LCDPrintString("    ");
	}
	else
	{
		LCDMoveTo(y,x);
		int v = val * 25;
		//if(v < 10)
		//	LCDPutChar(' ');
		//if(v < 100)
			//LCDPutChar(' ');
		//LCDPrintString("L");
		LCDPrintInt(v);
		//LCDPutChar(' ');
	}
	return 4;
}

#define IMin 0
#define IMax 250
#define IBigStep 5
int PrintI(int x,int y,float val)
{
	if(val < IMin || val > IMax)
	{
		LCDPrintString("    ");
	}
	else
	{
		LCDMoveTo(y,x);
		int v = val;
		LCDPrintFInt(v,1);
	}
	return 4;
}

#define RMin 0
#define RMax 3
#define RBigStep 1
int PrintR(int x,int y,float val)
{
	if(val < RMin || val > RMax)
	{
		LCDPrintString("  ");
	}
	else
	{
		LCDMoveTo(y,x);
		//LCDPrintString("R");
		LCDPrintInt(val);
		//LCDPutChar(' ');
	}
	return 3;
}

#define PhMin 10
#define PhMax 255
#define PhBigStep 5
int PrintPh(int x,int y,int val)
{
	if(val < PhMin || val > PhMax)
	{
		LCDPrintString("    ");
	}
	else
	{
		LCDMoveTo(y,x);
		//int v = val;//100 / (val+1);
		LCDPrintInt(val);
		LCDPutChar(' ');
	}
	return 4;
}

#define PreBlowTimeMin 0
#define PreBlowTimeMax 9
#define PreBlowTimeBigStep 1
int PrintPreBlowTime(int x,int y,float val)
{
	if(val < PreBlowTimeMin || val > PreBlowTimeMax)
	{
		LCDPrintString("  ");
	}
	else
	{
		LCDMoveTo(y,x);
		//LCDPrintString("R");
		LCDPrintInt(val);
		//LCDPutChar(' ');
	}
	return 3;
}
int PrintScr(int col,char is_big_step)
{
	int x = 0;
	int pos = 0;
	int step = 1;

	if(Cv == 0)
	{
		if(col < 0)
			col = 5;
		if(col > 5)
			col = 0;
	}
	else
	{
		if(col < 0)
			col = 6;
		if(col > 6)
			col = 0;
	}

	LCDClear();
	LCDMoveTo(0,0);
	if(Cv > 0)
		LCDPrintString("C F");
	else
		LCDPrintString("d  F");

	CurVal = 0;
	CurValBigStep = 1;
	PrintC(x,2,Cv);
	if(col == 0)
	{
		if(is_big_step)
		CurValBigStep = CBigStep;
		PrintC(x,3,Cv-CurValBigStep);
		PrintC(x,1,Cv+CurValBigStep);
		pos = x;
		CurVal = &Cv;
		CurValMax = CMax;
		CurValMin = CMin;
	}
	x += 2;
	if(Cv == 0)
		x += 1; 
	PrintF(x,2,Fv);
	if(col == 1)
	{
		if(is_big_step)
		CurValBigStep = FBigStep;
		PrintF(x,3,Fv-CurValBigStep);
		PrintF(x,1,Fv+CurValBigStep);
		pos = x;
		CurVal = &Fv;
		CurValMax = FMax;
		CurValMin = FMin;
	}
	x += 4;
	if(Cv == 0)
	{
		LCDMoveTo(0,x);
		LCDPrintString("I1");
		PrintI(x,2,Iv);
		if(col == 2)
		{
			if(is_big_step)
				CurValBigStep = IBigStep;
			else
				CurValBigStep = 1;
			PrintI(x,3,Iv-CurValBigStep);
			PrintI(x,1,Iv+CurValBigStep);
			pos = x;
			CurVal = &Iv;
			CurValMin = IMin;
			CurValMax = IMax;
		}
		x +=4;

		LCDMoveTo(0,x);
		LCDPrintString("I2");
		PrintI(x,2,Iv1);
		if(col == 3)
		{
			if(is_big_step)
			CurValBigStep = IBigStep;
			else
			CurValBigStep = 1;
			PrintI(x,3,Iv1-CurValBigStep);
			PrintI(x,1,Iv1+CurValBigStep);
			pos = x;
			CurVal = &Iv1;
			CurValMin = IMin;
			CurValMax = IMax;
		}
		x +=4;

		LCDMoveTo(0,x);
		LCDPrintString("t");
		PrintPh(x,2,Phv);
		if(col == 4)
		{
			if(is_big_step)
				CurValBigStep = PhBigStep;
			PrintPh(x,3,Phv-CurValBigStep);
			PrintPh(x,1,Phv+CurValBigStep);
			pos = x;
			CurVal = &Phv;
			CurValMin = PhMin;
			CurValMax = PhMax;
		}
		x +=3;
	}
	else
	{
		LCDMoveTo(0,x);
		LCDPrintString("U");
		PrintU(x,2,Uv);
		if(col == 2)
		{
			if(is_big_step)
				CurValBigStep = UBigStep;
			PrintU(x,3,Uv-CurValBigStep);
			PrintU(x,1,Uv+CurValBigStep);
			pos = x;
			CurVal = &Uv;
			CurValMin = UMin;
			CurValMax = UMax;
		}
		x += 4;

		LCDMoveTo(0,x);
		LCDPrintString("L");
		PrintL(x,2,Lv);
		if(col == 3)
		{
			if(is_big_step)
				CurValBigStep = LBigStep;
			PrintL(x,3,Lv-CurValBigStep);
			PrintL(x,1,Lv+CurValBigStep);
			pos = x;
			CurVal = &Lv;
			CurValMin = LMin;
			CurValMax = LMax;
		}
		x += 3;

		LCDMoveTo(0,x);
		LCDPrintString("R");
		PrintR(x,2,Rv);
		if(col == 4)
		{
			if(is_big_step)
				CurValBigStep = RBigStep;
			PrintR(x,3,Rv-CurValBigStep);
			PrintR(x,1,Rv+CurValBigStep);
			pos = x;
			CurVal = &Rv;
			CurValMin = RMin;
			CurValMax = RMax;
		}
		x += 2;

		LCDMoveTo(0,x);
		LCDPrintString("I");
		PrintI(x,2,Iv);
		if(col == 5)
		{
			if(is_big_step)
				CurValBigStep = IBigStep;
			else
				CurValBigStep = 2;
			PrintI(x,3,Iv-CurValBigStep);
			PrintI(x,1,Iv+CurValBigStep);
			pos = x;
			CurVal = &Iv;
			CurValMin = IMin;
			CurValMax = IMax;
		}
		x +=4;
	}
	
	LCDMoveTo(0,x);
	LCDPrintString("п");
	PrintPreBlowTime(x,2,PreBlowTime);
	if((Cv > 0 && col == 6) || (Cv == 0 && col == 5))
	{
		if(is_big_step)
		CurValBigStep = PreBlowTimeBigStep;
		PrintPreBlowTime(x,3,PreBlowTime-CurValBigStep);
		PrintPreBlowTime(x,1,PreBlowTime+CurValBigStep);
		pos = x;
		CurVal = &PreBlowTime;
		CurValMin = PreBlowTimeMin;
		CurValMax = PreBlowTimeMax;
	}
	//LCDMoveTo(1,0);
	//LCDPrintString(">");
	LCDMoveTo(2,pos);

	return col;
}

void PrintInfoScr()
{
	LCDClear();
	LCDMoveTo(0,0);
	LCDPrintString("ПУСК=============шаг");
	LCDMoveTo(1,1);
	if(Cv == 0)
	{
		LCDPrintString("Дуга ");
		LCDPrintFInt(Iv,1);
		LCDPrintString("/");
		LCDPrintFInt(Iv1,1);
		LCDPrintString("A ");
		LCDPrintInt(Fv*50);
		LCDPrintString("Gz ");
		LCDPrintInt(Phv);
		LCDPrintString("t");
	}		
	else
	{
		LCDPrintString("Искра ");
		LCDPrintInt(Fv*50);
		LCDPrintString("Gz ");
		LCDPrintInt(350+Uv);
		LCDPrintString("V ");
		
		LCDMoveTo(2,1);
		LCDPrintInt(Cv);
		LCDPrintString("uF ");
		LCDPrintInt(Lv*25);
		LCDPrintString("uGn ");
		LCDPrintInt(Rv);
		LCDPrintString("Om ");
		if(Iv > 0)
		{
			LCDPrintFInt(Iv,1);
			LCDPrintString("A ");			
		}
	}
	LCDMoveTo(3,0);
	LCDPrintString("СТОП=======Настройка");
}
