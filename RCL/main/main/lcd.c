#include "lcd.h"
#include <avr/io.h>

const unsigned char WIN_TO_LCD[256];

void LCDInit()
{
	PORTD &= 0x3F;
	DDRD |= 0xC0;	
	PORTC = 0xFF;
	DDRC = 0xFF;

	_LCDSendCommand(1); // Reset
	_LCDSendCommand(0x6); // Increment cursor to 1
	//_LCDSendCommand(0xE); // TurnOn display, and show short cursor
	//_LCDSendCommand(0xC); // TurnOn display, no cursor
	_LCDSendCommand(0xD); // TurnOn display, with cursor
	_LCDSendCommand(0x38); // 8-bit,double string,5x8
}

void LCDLedStatus(char on)
{
	DDRD |= 0x20;
	if(on == 0)
		PORTD |= 0x20;
	else
		PORTD &= 0xDF;
}

void _LCDSendWaitNext()
{
	for(int i = 0;i<300;i++)
	;
}

void _LCDMiniWait()
{
	for(char i = 0;i<10;i++);
}

void _LCDSendCommand(char command)
{
	PORTD &= 0xBF;		// RS = 0
	_LCDMiniWait();
	PORTC = command;	// Setup command
	_LCDMiniWait();
	PORTD |= 0x80;		// Enable = 1
	_LCDMiniWait();
	PORTD &= 0x7F;		// Enable = 0
	_LCDMiniWait();
	PORTC = 0xFF;
	_LCDSendWaitNext();
}

void LCDPutByte(char data)
{
	PORTD |= 0x40;		// RS = 1
	_LCDMiniWait();
	PORTC = data;
	_LCDMiniWait();
	PORTD |= 0x80;		// Enable = 1
	_LCDMiniWait();
	PORTD &= 0x7F;		// Enable = 0
	_LCDMiniWait();
	PORTC = 0xFF;
	_LCDSendWaitNext();
}

void _LCDSendData(char data)
{
	PORTD |= 0x40;		// RS = 1
	_LCDMiniWait();
	PORTC = WIN_TO_LCD[data];
	_LCDMiniWait();
	PORTD |= 0x80;		// Enable = 1
	_LCDMiniWait();
	PORTD &= 0x7F;		// Enable = 0
	_LCDMiniWait();
	PORTC = 0xFF;
	_LCDSendWaitNext();
}

void LCDPutCharNoWait(char data)
{
	PORTD |= 0x40;		// RS = 1
	_LCDMiniWait();
	PORTC = WIN_TO_LCD[data];		// Setup data
	_LCDMiniWait();
	PORTD |= 0x80;		// Enable = 1
	_LCDMiniWait();
	PORTD |= 0x80;		// Enable = 1
	_LCDMiniWait();
	PORTD &= 0x7F;		// Enable = 0
	_LCDMiniWait();
	PORTC = 0xFF;
	_LCDMiniWait();
}

void LCDClear()
{
	_LCDSendCommand(1); // Reset
	_LCDSendCommand(2);
}

char LCDLineAdr[] = {0,64,20,84};
void LCDMoveTo(char str,char x)
{
	char adr = LCDLineAdr[str]+x;
	_LCDSendCommand(0x80|(adr&0x7F));
}

void LCDMoveToNoWait(char str,char x)
{
	char adr = LCDLineAdr[str]+x;
	adr = 0x80|(adr&0x7F);
	PORTD &= 0xBF;		// RS = 0
	PORTC = adr;	// Setup command
	PORTD |= 0x80;		// Enable = 1
	PORTD |= 0x80;		// Enable = 1
	PORTD &= 0x7F;		// Enable = 0
	PORTC = 0xFF;
}

void LCDPrintString(char *str)
{
	for(int i = 0;i<20 && str[i] != 0;i++)
		_LCDSendData(str[i]);
}

void LCDPutChar(char data)
{
	_LCDSendData(data);
}

void LCDPutLCDChar(char data)
{
	PORTD |= 0x40;		// RS = 1
	PORTC = data;
	PORTD |= 0x80;		// Enable = 1
	PORTD &= 0x7F;		// Enable = 0
	PORTC = 0xFF;
	_LCDSendWaitNext();
}


void LCDPrintInt(int val)
{
	int len = 0;
	int tmp;
	char str[5];

	if(val < 0)
	{
		_LCDSendData('-');
		val = -val;
	}

	if(val == 0)
	{
		_LCDSendData('0');
		return;
	}

	while(val > 0)
	{
		tmp = val;
		val /= 10;
		str[len] = '0'+(tmp-val*10);
		len ++;
	}

	len --;

	while(len >= 0)
	{
		_LCDSendData(str[len]);
		len --;		
	}
}


void LCDPrintUInt(unsigned int val)
{
	int len = 0;
	unsigned int tmp;
	char str[5];

	if(val == 0)
	{
		_LCDSendData('0');
		return;
	}

	while(val > 0)
	{
		tmp = val;
		val /= 10;
		str[len] = '0'+(tmp-val*10);
		len ++;
	}

	len --;

	while(len >= 0)
	{
		_LCDSendData(str[len]);
		len --;		
	}
}

void LCDPrintFInt(int val,int deximal)
{
	int rval = 0,dval = 0;

	if(val < 0)
	{
		LCDPrintString("-");
		val = -val;
	}

	rval = val;
	for(int i = 0;i<deximal;i++)
		rval /= 10;

	LCDPrintInt(rval);

	dval = rval;
	for(int i = 0;i<deximal;i++)
		dval *= 10;

	dval = val-dval;
	if(dval > 0)
	{
		LCDPrintString(".");
		if(deximal == 2)
		{
			if(dval < 10)
				LCDPrintString("0");
		}
		if(deximal == 3)
		{
			if(dval < 100)
				LCDPrintString("0");
			if(dval < 10)
				LCDPrintString("0");
		}
		LCDPrintInt(dval);
	}
}


//таблица перекодировки символов
  const unsigned char WIN_TO_LCD[256] = {
  //управляющие коды
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, //0 
  //управляющие коды
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, //1
  //        !     "     #     $     %     &     '     (     )     *     +     ,     -     .     /   //  ASCII символы
     ' ',  '!',  '"',  '#',  '$',  '%',  '&', 0x27,  '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/', //2 ЖКИ коды этих символов
  //  0     1     2     3     4     5     6     7     8     9     :     ;     <     =     >     ?   //  ASCII символы
     '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?', //3 ЖКИ коды этих символов
  //  @     A     B     C     D     E     F     G     H     I     J     K     L     M     N     O   //  ASCII символы
     '@',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O', //4 ЖКИ коды этих символов
  //  P     Q     R     S     T     U     V     W     X     Y     Z     [     \     ]     ^     _   //  ASCII символы
     'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  '[', '\\',  ']',  '^',  '_', //5 ЖКИ коды этих символов
  //  `     a     b     c     d     e     f     g     h     i     j     k     l     m     n     o   //  ASCII символы
     '`',  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o', //6 ЖКИ коды этих символов
  //  p     q     r     s     t     u     v     w     x     y     z     {      |     }    ~   0x7F, //  ASCII символы
     'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, //7 ЖКИ коды этих символов
  //0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, //  ASCII символы
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, //8 ЖКИ коды этих символов
  //0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, //  ASCII символы
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, //9 ЖКИ коды этих символов
  //  _     _     _     _     _     _     _     _     Ё     _     _     _     _     _     _     _   //  ASCII символы
     ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ', 0xA2,  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ', //A ЖКИ коды этих символов
  //  _     _     _     _     _     _     _     _     ё     _     _     _     _     _     _     _   //  ASCII символы
     ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ', 0xB5,  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ', //B ЖКИ коды этих символов
  //  А     Б     В     Г     Д     Е     Ж     З     И     Й     К     Л     М     Н     О     П   //  ASCII символы
     'A', 0xA0,  'B', 0xA1, 0xE0,  'E', 0xA3, 0xA4, 0xA5, 0xA6,  'K', 0xA7,  'M',  'H',  'O', 0xA8, //C ЖКИ коды этих символов   
  //  Р     С     Т     У     Ф     Х     Ц     Ч     Ш     Щ     Ъ     Ы     Ь     Э     Ю     Я   //  ASCII символы
     'P',  'C',  'T', 0xA9, 0xAA,  'X', 0xE1, 0xAB, 0xAC, 0xE2, 0xAD, 0xAE,  'b', 0xAF, 0xB0, 0xB1, //D ЖКИ коды этих символов   
  //  а     б     в     г     д     е     ж     з     и     й     к     л     м     н     о     п   //  ASCII символы
     'a', 0xB2, 0xB3, 0xB4, 0xE3,  'e', 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD,  'o', 0xBE, //E ЖКИ коды этих символов   
  //  р     с     т     у     ф     х     ц     ч     ш     щ     ъ     ы     ь     э     ю     я   //  ASCII символы
     'p',  'c', 0xBF,  'y', 0xE4,  'x', 0xE5, 0xC0, 0xC1, 0xE6, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7  //F ЖКИ коды этих символов
  };
