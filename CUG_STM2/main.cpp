#include "pin.h"
#include "hr_timer.h"
#include "common.h"
#include "lcd.h"
#include "rel.h"
#include "menu.h"

void systemInit(){
	RCC_ClocksTypeDef RCC_ClockFreq;

	SystemInit();

	RCC_GetClocksFreq(&RCC_ClockFreq);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; // Disable JTAG
}

LCD Lcd;

const int MAX_I = 75;
#define MAXMENU 19
MenuItem *Menu[MAXMENU];
void initMenu(){
	int i = 0;
	Menu[i++] = new MenuExternalStart();
	Menu[i++] = new MenuTextItem("������","���������� ������������ ������");
		//int i0,int i1,int i2,int i3,int f,int preblowTime,int presparkTime,int presparkI
	Menu[i++] = new MenuSetModeItem(" ��������","����� �������� �������",20,0,0,0,200,6,0,0);
	Menu[i++] = new MenuSetModeItem(" ���� 10�","��� 10� 400��",10,10,10,10,400,6,0,0);
	Menu[i++] = new MenuSetModeItem(" ����� �������","������ ������� ���������������� �����",MAX_I,0,0,0,400,6,0,0);
	Menu[i++] = new MenuSetModeItem(" ����� ������","����� �������� �������",MAX_I*2/3,MAX_I/2,MAX_I/3,MAX_I/5,400,6,0,0);
	Menu[i++] = new MenuTextItem("��������� �������","� ������ ������ �������� ��������� �������");
	Menu[i++] = new MenuIntItem(MENU_TYPE_I1," ��� #1","��� ������ 125��� �������","A",1,MAX_I,20,1);
	Menu[i++] = new MenuIntItem(MENU_TYPE_I2," ��� #2","��� ������ 125��� �������","A",0,MAX_I,0,1);
	Menu[i++] = new MenuIntItem(MENU_TYPE_I3," ��� #3","��� ������ 125��� �������","A",0,MAX_I,0,1);
	Menu[i++] = new MenuIntItem(MENU_TYPE_I4," ��� #4","��� ��������� 125��� �������","A",0,MAX_I,0,1);
	Menu[i++] = new MenuIntItem(MENU_TYPE_F," �������","������� ���������� ��������","��",50,MAX_I,50,1);
	Menu[i++] = new MenuTextItem("�������� �������","�������� ������� ����� ������� �������");
	Menu[i++] = new MenuIntItem(MENU_TYPE_PBT," �����","����� �������� ������� �� ������ �������","���",0,20,6,1);
	Menu[i++] = new MenuTextItem("������������","��������������� ����� �������������� ������������");
	Menu[i++] = new MenuIntItem(MENU_TYPE_PST,"   �����","����� ���������������� ������������","���",0,MAX_I,0,1);
	Menu[i++] = new MenuIntItem(MENU_TYPE_PSI,"   ���","��� ���������������� ������������","�",0,MAX_I,0,1);
	Menu[i++] = new MenuInternalStart();
	Menu[i++] = NULL;

	for(int i = 0;i<MAXMENU && Menu[i]!=NULL;i++)
		Menu[i]->init();
};

int MenuIndex = 0;

int main(void)
{
	systemInit();

	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	initHRTimer();
	delay_ms(500);
	Lcd.init();
	Lcd.setLED(true);
	Lcd.Clear();
	Lcd.MoveTo(0,2);
	Lcd.PrintString("��������");
	Lcd.MoveTo(1,2);
	Lcd.PrintString("�������������");
	Lcd.MoveTo(2,2);
	Lcd.PrintString("���������");
	delay_ms(1000);
	Lcd.setLED(false);

	initMenu();
	/*
	for(int i = 0;i<0xFFFF;i+=512){
		new char[512];
		Lcd.MoveTo(3,0);
		Lcd.PrintInt(i);
	}//*/

	int key = -1;
#define DESCRIPTION_TIME_OUT 500
	int descriptionPrintTimeOut = DESCRIPTION_TIME_OUT;
    while(1)
    {
    	switch(key){
    	case 1:
    		Menu[MenuIndex]->onStart();
    		break;
    	case 2:
    		break;
    	case 4:
    		Menu[MenuIndex]->onDec();
    		break;
    	case 8:
    		if(MenuIndex > 0)
    			MenuIndex --;
    		else
    			key = 0;
    		break;
    	case 16:
    		if(Menu[MenuIndex+1] != NULL)
    			MenuIndex ++;
    		break;
    	case 32:
    		Menu[MenuIndex]->onInc();
    		break;
    	}
    	if(key != 0){
    		for(int i = -1;i<=1;i++){
    			Lcd.MoveTo(i+1,0);
    			int index = MenuIndex + i;
    			if(index < 0 || Menu[index] == NULL){
    				Lcd.PrintString("                    ");
    				continue;
    			}
    			Menu[index]->printName(Lcd);
    			if(i != 0)
    				Lcd.ClearToTheEndOfLine(' ');
    			else {
    				Lcd.PrintString("<-");
    				Lcd.ClearToTheEndOfLine('-');
    			}
    		}
    	}
    	else {
    		if(descriptionPrintTimeOut <= 0){
    			Menu[MenuIndex]->printDescription(Lcd);
    			descriptionPrintTimeOut = DESCRIPTION_TIME_OUT;
    		} else {
				descriptionPrintTimeOut --;
			}
		}
    	key = Lcd.GetKeyPressed();
    }
}
