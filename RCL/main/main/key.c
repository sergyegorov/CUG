#include "key.h"
#include "tuglab.h"
#include "lcd.h"
#include <avr/io.h>

unsigned char PressedKey = 0;

void msleep()
{
	for(int i = 0;i<10;i++)
	;
}

unsigned char ReadKeyCode()
{
	unsigned char ret = 0;

	PORTC = 0x1;
	msleep();
	if((PIND & 0x8) != 0)
		ret |= 1;

	PORTC = 0x2;
	msleep();
	if((PIND & 0x8) != 0)
	{
		LCDClear();
		LCDMoveTo(0,0);
		LCDPrintString("Б Л О К И Р О В К А");

		do
		{
			sleep_ms(100);
		}while((PIND & 0x8) != 0);

		LCDMoveTo(2,1);
		LCDPrintString("для выхода нажмите");
		LCDMoveTo(3,3);
		LCDPrintString("кнопку 'Пауза'");

		do
		{
			sleep_ms(100);
		}while((PIND & 0x8) == 0);

		do
		{
			sleep_ms(100);
		}while((PIND & 0x8) != 0);

		ret |= 2;
	}//*/

	PORTC = 0x4;
	msleep();
	if((PIND & 0x8) != 0)
		ret |= 4;

	PORTC = 0x8;
	msleep();
	if((PIND & 0x8) != 0)
		ret |= 8;

	PORTC = 0x10;
	msleep();
	if((PIND & 0x8) != 0)
		ret |= 0x10;

	PORTC = 0x20;
	msleep();
	if((PIND & 0x8) != 0)
		ret |= 0x20;

	PORTC = 0x40;
	msleep();
	if((PIND & 0x8) != 0)
		ret |= 0x40;

	PORTC = 0x80;
	msleep();
	if((PIND & 0x8) != 0)
		ret |= 0x80;

	return ret;
}


unsigned char GetKeyCode()
{
	unsigned char ret;

	ret = ReadKeyCode();

	if(PressedKey == 0)
		PressedKey = ret;
	else
	{
		if(ret == 0)
		{
			sleep_ms(10);
			ret = PressedKey;
			PressedKey = 0;
			return ret;
		}
	}

	return 0;
}
