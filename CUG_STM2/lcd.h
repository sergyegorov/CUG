#ifndef LCD_H
#define LCD_H

#include "common.h"


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

class LCD{
private:

public:
	static void setData(unsigned char data){
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

protected:
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
		}

private:
	inline void _LCDSendWaitNext()
	{
		delay_us(30);
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


	int CurStr,CurX;
	int LineSize;

public:
#define REPEAT_TIME 200;
#define MIN_REPEAT_TIME 100

	LCD(){}

	void init(){
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

	void setLED(bool status){
		if(!status)
			GPIO_SetBits(LCD_LED_PORT,LCD_LED_PIN);
		else
			GPIO_ResetBits(LCD_LED_PORT,LCD_LED_PIN);
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

	int KeyTime;
	int RepeatTime;
	int Key;
	int GetKeyPressed(){
		int ret = GetKeyState();

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

	void Clear()
	{
		_LCDSendCommand(1); // Reset
	    _LCDSendCommand(2);
	}

	void MoveTo(char str,char x)
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

	void ClearToTheEndOfLine(char byChar = ' '){
		while(CurX < LineSize)
			_LCDSendData(byChar);
	}

	void PrintString(const char *str)
	{
		int i;
	    for(i = 0;i<20 && str[i] != 0 && CurX < LineSize;i++)
	    	_LCDSendData(str[i]);
	}

	void PutChar(char data)
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


	void PrintInt(int val)
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


	void PrintUInt(unsigned int val)
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

	void PrintFInt(int val,int deximal)
	{
		if(deximal == 0)
			return PrintInt(val);
		int rval = 0,dval = 0;

	    if(val < 0)
	    {
	    	PrintString("-");
	        val = -val;
	    }

	    rval = val;
	    int i;
	    for(i = 0;i<deximal;i++)
	        rval /= 10;

	    PrintInt(rval);

	    dval = rval;
	    for(i = 0;i<deximal;i++)
	        dval *= 10;

	    dval = val-dval;
	    if(dval > 0)
	    {
	    	PrintString(".");
	        if(deximal == 2)
	        {
	        	if(dval < 10)
	                PrintString("0");
	        }
	        if(deximal == 3)
	        {
	        	if(dval < 100)
	                PrintString("0");
	            if(dval < 10)
	                PrintString("0");
	        }
	        PrintInt(dval);
	    }
	    else
	    {
	    	PrintString(" ");
	    	for(i = 0;i<deximal;i++)
	    	    PrintString(" ");
	    }
	}
};

extern LCD Lcd;

#endif
