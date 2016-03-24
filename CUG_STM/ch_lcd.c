#include "ch_lcd.h"
#include "stm_port.h"
#include "time.h"

const unsigned char WIN_TO_LCD[256];

const unsigned int DataPins = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
const unsigned int ControlPins = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
const unsigned int KeyPin = GPIO_Pin_7;

#define RS_On GPIO_SetBits(GPIOC,GPIO_Pin_10)
#define RS_Off GPIO_ResetBits(GPIOC,GPIO_Pin_10)
#define EN_On GPIO_SetBits(GPIOC,GPIO_Pin_11)
#define EN_Off GPIO_ResetBits(GPIOC,GPIO_Pin_11)

#define IS_KEY_ON GPIO_ReadInputDataBit(GPIOB, KeyPin)

//extern void delay(int val);
void _LCDSendCommand(char command);

int GetKeyPressed(){
	int ret = 0;
	int key = 1;
	int i;
	EN_Off;
	outPin(GPIOD,0xFF,0xFF);
	delay_us(100);
	if(IS_KEY_ON == 0)
		return ret;
	for(i = 0;i<8;i++){
		outPin(GPIOD,0xFF,key);
		delay_us(100);
		if(IS_KEY_ON)
			ret |= key;
		key <<= 1;
	}
	return ret;
}

int KeyTime = 0;
#define REPEAT_TIME 1000;
#define MIN_REPEAT_TIME 200
int RepeatTime = REPEAT_TIME;
int Key = 0;
int GetKeyCode(){
	int ret = GetKeyPressed();

	if(ret == 0){
		Key = ret;
		return ret;
	}

	if(Key == ret)
		KeyTime ++;
	else{
		KeyTime = 0;
		RepeatTime = REPEAT_TIME;
		Key = ret;
	}

	if(KeyTime == 2)
		return ret;

	if(KeyTime > RepeatTime){
		if(RepeatTime > MIN_REPEAT_TIME)
			RepeatTime -= MIN_REPEAT_TIME;
		KeyTime = 0;
	}

	return 0;
}

int LineSize = 20;
int CurStr = 0,CurX = 0;
void LCDInit(int line_size)
{
	LineSize = line_size;
	RS_On;
	EN_Off;
	outPin(GPIOD,0xFF,0xFF);//PORTC = command;        // Setup command
	//int i = 0;
	initOutputPin(GPIOD,DataPins);
	initOutputPin(GPIOC,ControlPins);
	initInputPin(GPIOB,KeyPin);

	/*while(1){
		GPIOC->BSRRL = GPIO_Pin_10;//RS_On;
		delay(10000);
		GPIOC->BSRRH = GPIO_Pin_10;//RS_Off;
		delay(10000);
	}//*/

	/*while(1)
	{
		GPIOC->BSRRL = GPIO_Pin_10;//RS_On;
		i ++;
		i ++;
		i ++;
		GPIOC->BSRRH = GPIO_Pin_10;//RS_Off;
		i ++;
		i ++;
		i ++;
	}//*/
	RS_Off;
	RS_On;
	//for(i = 0;i<10000 MULT;i++);
	delay_us(1000);
    _LCDSendCommand(0x38); // 8-bit,double string,5x8
    delay_us(1000);//for(i = 0;i<1000 MULT;i++);
    _LCDSendCommand(0b1100); // Display on
    delay_us(1000);//for(i = 0;i<1000 MULT;i++);
    _LCDSendCommand(1); // Reset
    _LCDSendCommand(0x6); // Increment cursor to 1
}

void LCDLedStatus(char on)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_12);//DDRD |= 0x20;
    if(on == 0)
      	GPIO_SetBits(GPIOC,GPIO_Pin_12);//	PORTD |= 0x20;
    else
      	GPIO_ResetBits(GPIOC,GPIO_Pin_12);//    PORTD &= 0xDF;
}

inline void _LCDSendWaitNext()
{
	delay_us(20);
}

inline void _LCDMiniWait()
{
	delay_us(6);
}

void _LCDSendCommand(char command)
{
	//int i;
    RS_Off;//PORTD &= 0xBF;          // RS = 0
    _LCDMiniWait();
    outPin(GPIOD,0xFF,command);//PORTC = command;        // Setup command
    _LCDMiniWait();
    EN_On;//PORTD |= 0x80;          // Enable = 1
    _LCDMiniWait();
    EN_Off;//PORTD &= 0x7F;          // Enable = 0
    _LCDMiniWait();
    outPin(GPIOD,0xFF,0xFF);//PORTC = 0xFF;
    _LCDSendWaitNext();
    if(command == 1){
    	delay_us(3000);
    	CurStr = 0;
    	CurX = 0;
    }
}

void _LCDSendData(char data)
{
	if(CurX >= LineSize)
		return;
    RS_On;//PORTD |= 0x40;          // RS = 1
    _LCDMiniWait();
    outPin(GPIOD,0xFF,WIN_TO_LCD[(unsigned int)data]);//PORTC = WIN_TO_LCD[data];
    _LCDMiniWait();
    EN_On;//PORTD |= 0x80;          // Enable = 1
    _LCDMiniWait();
    EN_Off;//PORTD &= 0x7F;          // Enable = 0
    _LCDMiniWait();
    outPin(GPIOD,0xFF,0xFF);//PORTC = 0xFF;
    _LCDSendWaitNext();
    CurX ++;
}

void LCDClear()
{
	_LCDSendCommand(1); // Reset
    _LCDSendCommand(2);
}

char LCDLineAdr[] = {0,64,20,84};
void LCDMoveTo(char str,char x)
{
    char adr = LCDLineAdr[(unsigned int)str]+x;
    _LCDSendCommand(0x80|(adr&0x7F));
    CurStr = str;
    CurX = x;
}

void LCDMoveToNoWait(char str,char x)
{
    char adr = (char)(LCDLineAdr[(unsigned int)str]+x);
    adr = 0x80|(adr&0x7F);
    RS_Off;//PORTD &= 0xBF;          // RS = 0
    outPin(GPIOD,0xFF,adr);//PORTC = adr;    // Setup command
    EN_On;//PORTD |= 0x80;          // Enable = 1
    EN_On;//PORTD |= 0x80;          // Enable = 1
    EN_Off;//PORTD &= 0x7F;          // Enable = 0
    outPin(GPIOD,0xFF,0xFF);//PORTC = 0xFF;
}

void LCDPrintString(const char *str)
{
	int i;
    for(i = 0;i<20 && str[i] != 0;i++)
    	_LCDSendData(str[i]);
}

void LCDPutChar(char data)
{
	_LCDSendData(data);
}

void LCDPutLCDChar(char data)
{
	RS_On;//PORTD |= 0x40;          // RS = 1
	outPin(GPIOD,0xFF,data);//PORTC = data;
    EN_On;//PORTD |= 0x80;          // Enable = 1
    EN_Off;//PORTD &= 0x7F;          // Enable = 0
    outPin(GPIOD,0xFF,0xFF);//PORTC = 0xFF;
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
	if(deximal == 0)
		return LCDPrintInt(val);
	int rval = 0,dval = 0;

    if(val < 0)
    {
    	LCDPrintString("-");
        val = -val;
    }

    rval = val;
    int i;
    for(i = 0;i<deximal;i++)
        rval /= 10;

    LCDPrintInt(rval);

    dval = rval;
    for(i = 0;i<deximal;i++)
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
    else
    {
    	/*LCDPrintString(".");
    	for(i = 0;i<deximal;i++)
    		LCDPrintString("0");*/
    	LCDPrintString(" ");
    	for(i = 0;i<deximal;i++)
    	    LCDPrintString(" ");
    }
}

void Beep(){
	int i;
	initOutputPin(GPIOB,GPIO_Pin_10);
	initOutputPin(GPIOB,GPIO_Pin_11);
	for(i = 0;i<100;i++){
		outPin(GPIOB,GPIO_Pin_10 | GPIO_Pin_11,GPIO_Pin_11);
		delay_ms(1);
		outPin(GPIOB,GPIO_Pin_10 | GPIO_Pin_11,GPIO_Pin_10);
		delay_ms(1);
	}
}

  const unsigned char WIN_TO_LCD[256] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, //0
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, //1
     ' ',  '!',  '"',  '#',  '$',  '%',  '&', 0x27,  '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/', //2
     '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?', //3
     '@',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O', //4
     'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  '[', '\\',  ']',  '^',  '_', //5
     '`',  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o', //6
     'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, //7
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, //8
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, //9
     ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ', 0xA2,  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ', //A
     ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ', 0xB5,  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ', //B
     'A', 0xA0,  'B', 0xA1, 0xE0,  'E', 0xA3, 0xA4, 0xA5, 0xA6,  'K', 0xA7,  'M',  'H',  'O', 0xA8, //C
     'P',  'C',  'T', 0xA9, 0xAA,  'X', 0xE1, 0xAB, 0xAC, 0xE2, 0xAD, 0xAE,  'b', 0xAF, 0xB0, 0xB1, //D
     'a', 0xB2, 0xB3, 0xB4, 0xE3,  'e', 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD,  'o', 0xBE, //E
     'p',  'c', 0xBF,  'y', 0xE4,  'x', 0xE5, 0xC0, 0xC1, 0xE6, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7  //F
  };
