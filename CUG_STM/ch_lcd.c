#include "ch_lcd.h"
#include "stm_port.h"
#include "time.h"

#define RUS_CHARSET

#ifndef bool
typedef uint8_t bool;
#endif

#define LCD_LED_PIN GPIO_Pin_15
#define LCD_LED_PORT GPIOA

#define LCD_KEY_PIN GPIO_Pin_14
#define LCD_KEY_PORT GPIOC

#define IS_KEY_ON GPIO_ReadInputDataBit(LCD_KEY_PORT, LCD_KEY_PIN)

#define LCD_RS_PIN GPIO_Pin_10
#define LCD_RS_PORT GPIOC
#define LCD_E_PIN GPIO_Pin_11
#define LCD_E_PORT GPIOC

#define LCD_DB0_PIN GPIO_Pin_12
#define LCD_DB0_PORT GPIOC
#define LCD_DB1_PIN GPIO_Pin_2
#define LCD_DB1_PORT GPIOD
#define LCD_DB2_PIN GPIO_Pin_3
#define LCD_DB2_PORT GPIOB
#define LCD_DB3_PIN GPIO_Pin_4
#define LCD_DB3_PORT GPIOB
#define LCD_DB4_PIN GPIO_Pin_5
#define LCD_DB4_PORT GPIOB
#define LCD_DB5_PIN GPIO_Pin_6
#define LCD_DB5_PORT GPIOB
#define LCD_DB6_PIN GPIO_Pin_7
#define LCD_DB6_PORT GPIOB
#define LCD_DB7_PIN GPIO_Pin_13
#define LCD_DB7_PORT GPIOC

#define RS_On GPIO_SetBits(LCD_RS_PORT,LCD_RS_PIN)
#define RS_Off GPIO_ResetBits(LCD_RS_PORT,LCD_RS_PIN)
#define EN_On GPIO_SetBits(LCD_E_PORT,LCD_E_PIN)
#define EN_Off GPIO_ResetBits(LCD_E_PORT,LCD_E_PIN)

extern const unsigned char WIN_TO_LCD[];
extern const char LCDLineAdr[];

#define KEY_START 	1
#define KEY_STOP	2
#define KEY_LEFT	4
#define KEY_UP		8
#define KEY_DOWN	16
#define KEY_RIGHT	32
#define KEY_ALT1	64
#define KEY_ALT2	128

int CurStr,CurX;
int LineSize;
int KeyTime;
int RepeatTime;
int Key;

const char LCDLineAdr[4] = {0,64,20,84};

#define SPEAKER_PORT GPIOB
#define SPEAKER_PIN GPIO_Pin_2

//extern void delay(int val);
void _LCDSendCommand(char command);

 void setData(unsigned char data){
		if(data&1)	GPIO_SetBits(LCD_DB0_PORT,LCD_DB0_PIN);
		else		GPIO_ResetBits(LCD_DB0_PORT,LCD_DB0_PIN);
		if(data&2)	GPIO_SetBits(LCD_DB1_PORT,LCD_DB1_PIN);
		else		GPIO_ResetBits(LCD_DB1_PORT,LCD_DB1_PIN);
		if(data&4)	GPIO_SetBits(LCD_DB2_PORT,LCD_DB2_PIN);
		else		GPIO_ResetBits(LCD_DB2_PORT,LCD_DB2_PIN);
		if(data&8)	GPIO_SetBits(LCD_DB3_PORT,LCD_DB3_PIN);
		else		GPIO_ResetBits(LCD_DB3_PORT,LCD_DB3_PIN);
		if(data&16)	GPIO_SetBits(LCD_DB4_PORT,LCD_DB4_PIN);
		else		GPIO_ResetBits(LCD_DB4_PORT,LCD_DB4_PIN);
		if(data&32)	GPIO_SetBits(LCD_DB5_PORT,LCD_DB5_PIN);
		else		GPIO_ResetBits(LCD_DB5_PORT,LCD_DB5_PIN);
		if(data&64)	GPIO_SetBits(LCD_DB6_PORT,LCD_DB6_PIN);
		else		GPIO_ResetBits(LCD_DB6_PORT,LCD_DB6_PIN);
		if(data&128)GPIO_SetBits(LCD_DB7_PORT,LCD_DB7_PIN);
		else		GPIO_ResetBits(LCD_DB7_PORT,LCD_DB7_PIN);
	}


	void initGPIO(){
			initGPIOBitOut(LCD_LED_PORT,LCD_LED_PIN);
			initGPIOBitOut(LCD_RS_PORT,LCD_RS_PIN);
			initGPIOBitOut(LCD_E_PORT,LCD_E_PIN);
			initGPIOBitOut(LCD_DB0_PORT,LCD_DB0_PIN);
			initGPIOBitOut(LCD_DB1_PORT,LCD_DB1_PIN);
			initGPIOBitOut(LCD_DB2_PORT,LCD_DB2_PIN);
			initGPIOBitOut(LCD_DB3_PORT,LCD_DB3_PIN);
			initGPIOBitOut(LCD_DB4_PORT,LCD_DB4_PIN);
			initGPIOBitOut(LCD_DB5_PORT,LCD_DB5_PIN);
			initGPIOBitOut(LCD_DB6_PORT,LCD_DB6_PIN);
			initGPIOBitOut(LCD_DB7_PORT,LCD_DB7_PIN);

			GPIO_ResetBits(SPEAKER_PORT,SPEAKER_PIN);
			initGPIOBitOut(SPEAKER_PORT,SPEAKER_PIN);
		}

	void makeBeep(int time_ms){
		int t;
			for(t = 0;t<time_ms;t++){
				GPIO_SetBits(SPEAKER_PORT,SPEAKER_PIN);//outPin(SPEAKER_PORT,SPEAKER_PIN,SPEAKER_PIN);
				delay_us(500);
				GPIO_ResetBits(SPEAKER_PORT,SPEAKER_PIN);//outPin(SPEAKER_PORT,SPEAKER_PIN,0);
				delay_us(500);
			}
		}

	void Beep(){
		makeBeep(100);
	}

		void makeHello(int time_ms){
			makeBeep(time_ms);
		}

		void makeAsk(int time_ms){
			int dl = 1000,t;
			for(t = 0;t<time_ms;t++){
				GPIO_SetBits(SPEAKER_PORT,SPEAKER_PIN);//outPin(SPEAKER_PORT,SPEAKER_PIN,SPEAKER_PIN);
				delay_us(dl);
				GPIO_ResetBits(SPEAKER_PORT,SPEAKER_PIN);//outPin(SPEAKER_PORT,SPEAKER_PIN,0);
				delay_us(dl);
				dl --;
			}
		}

	inline void _LCDSendWaitNext()
	{
		delay_us(35);
	}

	inline void _LCDMiniWait()
	{
		delay_us(10);
	}

	void _LCDSendCommand(char command)
	{
	    RS_Off;//PORTD &= 0xBF;          // RS = 0
	    _LCDMiniWait();
	    setData(command);//outPin(GPIOD,0xFF,command);//PORTC = command;        // Setup command
	    _LCDMiniWait();
	    EN_On;//PORTD |= 0x80;          // Enable = 1
	    _LCDMiniWait();
	    EN_Off;//PORTD &= 0x7F;          // Enable = 0
	    _LCDMiniWait();
	    setData(0xFF);//outPin(GPIOD,0xFF,0xFF);//PORTC = 0xFF;
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
	    setData(WIN_TO_LCD[(unsigned int)data]);//outPin(GPIOD,0xFF,WIN_TO_LCD[(unsigned int)data]);//PORTC = WIN_TO_LCD[data];
	    _LCDMiniWait();
	    EN_On;//PORTD |= 0x80;          // Enable = 1
	    _LCDMiniWait();
	    EN_Off;//PORTD &= 0x7F;          // Enable = 0
	    _LCDMiniWait();
	    setData(0xFF);//outPin(GPIOD,0xFF,0xFF);//PORTC = 0xFF;
	    _LCDSendWaitNext();
	    CurX ++;
	}

	void LCDSymboxl(char data)
	{
		if(CurX >= LineSize)
			return;
	    RS_On;//PORTD |= 0x40;          // RS = 1
	    _LCDMiniWait();
	    setData(data);//outPin(GPIOD,0xFF,WIN_TO_LCD[(unsigned int)data]);//PORTC = WIN_TO_LCD[data];
	    _LCDMiniWait();
	    EN_On;//PORTD |= 0x80;          // Enable = 1
	    _LCDMiniWait();
	    EN_Off;//PORTD &= 0x7F;          // Enable = 0
	    _LCDMiniWait();
	    setData(0xFF);//outPin(GPIOD,0xFF,0xFF);//PORTC = 0xFF;
	    _LCDSendWaitNext();
	    CurX ++;
	}

#define REPEAT_TIME 800;
#define MIN_REPEAT_TIME 300

void LCDInit(int line_size){
		LineSize = 20;
		CurStr = 0;
		CurX = 0;
		KeyTime = 0;
		RepeatTime = REPEAT_TIME;
		Key = 0;
		initGPIO();
		RS_Off;
		RS_On;
		setData(0xFF);
		RS_Off;
		RS_On;
		_LCDSendCommand(0x38); // 8-bit,double string,5x8
	    delay_us(1000);//for(i = 0;i<1000 MULT;i++);
	    _LCDSendCommand(0b1100); // Display on
	    delay_us(1000);//for(i = 0;i<1000 MULT;i++);
	    _LCDSendCommand(1); // Reset
	    _LCDSendCommand(0x6); // Increment cursor to 1
	}

void LCDLedStatus(char status){
	if(!status)
		GPIO_SetBits(LCD_LED_PORT,LCD_LED_PIN);
	else
		GPIO_ResetBits(LCD_LED_PORT,LCD_LED_PIN);
}

int waitForKeyPress(){
	int ret = 0;
	do{
		ret = GetKeyState();
	}while(ret == 0);
	delay_ms(50);
	while(GetKeyState() != 0);
	delay_ms(50);
	return ret;
}

int GetKeyState(){
	int ret = 0;
	int key = 1;
	int i;
	EN_Off;
	setData(0xFF);//outPin(GPIOD,0xFF,0xFF);
	delay_us(100);
	if(IS_KEY_ON == 0)
		return ret;
	for(i = 0;i<8;i++){
		setData(key);//outPin(GPIOD,0xFF,key);
		delay_us(100);
		if(IS_KEY_ON)
			ret |= key;
		key <<= 1;
	}
	return ret;
}

int PrevKey = 0;
int KeyCycle = 0;
int GetKeyPressed(){
	int key = GetKeyState();
	if(PrevKey == key){
		KeyCycle ++;
		PrevKey = key;
	} else {
		KeyCycle = 0;
		PrevKey = key;
		return key;
	}
	if(KeyCycle > 500){
		KeyCycle -= 200;
		return key;
	}
	return 0;
}

void LCDClear()
{
	_LCDSendCommand(1); // Reset
    _LCDSendCommand(2);
}

void LCDMoveTo(char str,char x)
{
    char adr = LCDLineAdr[(unsigned int)str]+x;
    _LCDSendCommand(0x80|(adr&0x7F));
    CurStr = str;
    CurX = x;
}

void MoveToNoWait(char str,char x)
{
    char adr = (char)(LCDLineAdr[(unsigned int)str]+x);
    adr = 0x80|(adr&0x7F);
    RS_Off;//PORTD &= 0xBF;          // RS = 0
    setData(adr);//outPin(GPIOD,0xFF,adr);//PORTC = adr;    // Setup command
    EN_On;//PORTD |= 0x80;          // Enable = 1
    EN_On;//PORTD |= 0x80;          // Enable = 1
    EN_Off;//PORTD &= 0x7F;          // Enable = 0
    setData(0xFF);//outPin(GPIOD,0xFF,0xFF);//PORTC = 0xFF;
}

void ClearToTheEndOfLine(char byChar){
	while(CurX < LineSize)
		_LCDSendData(byChar);
}

/*void ClearToTheEndOfLine(){
	ClearToTheEndOfLine(' ');
}*/

void LCDPrintString(const char *str)
{
	int i;
    for(i = 0;i<20 && str[i] != 0 && CurX < LineSize;i++)
    	_LCDSendData(str[i]);
}

void LCDPutChar(char data)
{
	_LCDSendData(data);
}

void PutLCDChar(char data)
{
	RS_On;//PORTD |= 0x40;          // RS = 1
	setData(data);//outPin(GPIOD,0xFF,data);//PORTC = data;
    EN_On;//PORTD |= 0x80;          // Enable = 1
    EN_Off;//PORTD &= 0x7F;          // Enable = 0
    setData(0xFF);//outPin(GPIOD,0xFF,0xFF);//PORTC = 0xFF;
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
	    	LCDPrintString(" ");
	    	for(i = 0;i<deximal;i++)
	    	    LCDPrintString(" ");
	    }
	}

#ifdef RUS_CHARSET
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
  };// Rus */
#else
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
	     'A', 'B',  'V', 'G', 'D',  'E', 'G', 'Z', 'I', 'J',  'K', 'L',  'M',  'N',  'O', 'P', //C
	     'R',  'S',  'T', 'U', 'F',  'H', 'C', '4', 'W', 'W', '\'', '"',  '\'', 'E', 'U', 'A', //D
	     'a', 'b', 'v', 'g', 'd', 'e', 'g', 'z', 'e', 'j', 'k', 'l', 'm', 'n',  'o', 'p', //E
	     'r',  's', 't',  'u', 'f',  'h', 'c', '4', 'w', 'w', '\'', '"', '\'', 'e', 'u', 'a'  //F
	  };// Rus */
#endif
