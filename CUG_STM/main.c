/*
 * Старая СТМ и новая силовая.
 * Для АСС
 */
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_pwr.h"

#include "math.h"

#include "ch_lcd.h"
#include "time.h"
#include "run.h"
#include "adc.h"
#include "spi.h"
#include "stm_port.h"
#include "rel.h"

#define ARI_MODE

#ifdef AIR_MODE
#define NO_GAS_CHECK_DEFULT 1
#else
#define NO_GAS_CHECK_DEFULT 0
#endif

#ifndef NULL
#define NULL 0
#endif

//#define ANIMATION_DELAY  150
#ifndef ANIMATION_DELAY	// debug delay
	#define ANIMATION_DELAY  30
#endif

void animation_print_str(char *buf){
	while(*buf != 0){
		LCDPutChar(*buf);
		delay_ms(ANIMATION_DELAY);
		buf ++;
	}
}

void animation(){
	int x;

	LCDLedStatus(0);
	delay_ms(ANIMATION_DELAY);
	LCDLedStatus(1);
	delay_ms(ANIMATION_DELAY);
	LCDLedStatus(0);
	delay_ms(ANIMATION_DELAY);
	LCDLedStatus(1);
	delay_ms(ANIMATION_DELAY);

	LCDClear();

	LCDMoveTo(3,1);
	LCDPrintString("spectrometer.com.ua");

	//LCDMoveTo(1,8);
	//LCDPrintString("ЦУГ");
	//delay_ms(ANIMATION_DELAY*10);
	for(x = 8;x>=0;x--){
		LCDMoveTo(1,x);
		LCDPrintString("ЦУГ ");
		delay_ms(ANIMATION_DELAY);
	}

	LCDMoveTo(0,0);
	LCDPrintString("Ц");
	LCDMoveTo(1,0);
	LCDPrintString("УГ ");
	delay_ms(ANIMATION_DELAY);

	LCDMoveTo(1,0);
	LCDPrintString("У ");
	LCDMoveTo(2,0);
	LCDPrintString("Г");
	delay_ms(ANIMATION_DELAY);

	LCDMoveTo(0,1);
	animation_print_str("фровой");
	LCDMoveTo(1,1);
	animation_print_str("ниверсальный");
	LCDMoveTo(2,1);
	animation_print_str("енератор");
	delay_ms(ANIMATION_DELAY);
	LCDMoveTo(2,13);
	animation_print_str("Ver32.2");
	//LCDMoveTo(3,0);
	//LCDPrintString("                    ");

	delay_ms(ANIMATION_DELAY*30);

	LCDClear();
	LCDLedStatus(0);
}


struct MenuItem{
	const char *Name,*UnitName;
	int Min,Max;
	int DecimalNumber;
	int DefaultStep;
	int DefaultValue;
	int Function;
};

#define MenuItemCountConst 16+2
#define MenuItemFullCountConst 19+2

#define FK_NO 				-1
#define FK_NO_SPARK_CHECK 	0
#define FK_NO_GAS_CHECK 	1
#define FK_NO_PROT_CHECK	2

int MenuItemCount = MenuItemCountConst;
const struct MenuItem MItems[] = {
		{"Стандартные режимы:","",0,0,0,0,FK_NO},
		{" 1.Привязка","",0,0,-1,-1,0,FK_NO},
		{" 2.Искра жесткая","",0,0,-1,-1,0,FK_NO},
		{" 3.Искра мягкая","",0,0,-1,-1,0,FK_NO},
		{" 4.Дуга 4А","",0,0,-1,-1,0,FK_NO},
		{" 5.Дуга 10А","",0,0,-1,-1,0,FK_NO},
		{" 6.Дуга 16А","",0,0,-1,-1,0,FK_NO},
		{"Произвольный режим","",0,0,0,0,FK_NO},
		{"  I1=","A(125 мкс)",0,CUR_MAX,1,20,20,FK_NO},
		{"  I2=","A(250 мкс)",0,CUR_MAX,1,20,0,FK_NO},
		{"  I3=","A(375 мкс)",0,CUR_MAX,1,20,0,FK_NO},
		{"  I4=","A(500 мкс)",0,CUR_MAX,1,20,0,FK_NO},
		{" F =","Гц",25,600,0,25,100,FK_NO},
		{" T =","us",25,600,0,25,600,FK_NO},
		{" Обдув ","t(сек)",0,200,1,10,40,FK_NO},
		{" Обжиг ","сек",0,600,1,10,0,FK_NO},
		{"  I=","A(500 мкс)",0,CUR_MAX,1,20,CUR_MAX,FK_NO},
		{"Готов к работе",(char*)-1,-1,-1,-1,-1,-1,FK_NO},
		{"No Spark Check ","",0,1,0,1,0,FK_NO_SPARK_CHECK},
		{"No GAS Check ","",0,1,0,1,NO_GAS_CHECK_DEFULT,FK_NO_GAS_CHECK},
		{"No PROT Check ","",0,1,0,1,0,FK_NO_PROT_CHECK}
		//{"Связь с компютером","",0,0,-1,0}
};
//*Name,*UnitName;Min,Max;DecimalNumber;DefaultStep;DefaultValue;Function;

u32 ValueFile[MenuItemFullCountConst*2] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
u32 read_memory(int addr){
	return ValueFile[addr];
	//return RTC_ReadBackupRegister (addr);
}

void write_memory(int addr,u32 val){
	//RTC_WriteBackupRegister (addr,val);
	//delay_ms(1);
	ValueFile[addr] = val;
}

#define MenuItemVer			0
#define MenuItemSelector 	1
#define MenuItemValueBase   2

#define MenuItemMode 			7 //6
#define MenuItemIBase			MenuItemMode+1
#define MenuItemF 				MenuItemMode+5
#define MenuItemT 				MenuItemMode+6
#define MenuItemPreBlow 		MenuItemMode+7 //6
#define MenuItemPresparkTime 	MenuItemMode+8 //7
#define MenuItemPresparkI		MenuItemMode+9 //8

char shouldSetup(const char *msg){
	char ret;
	LCDClear();
	LCDMoveTo(0,0);
	LCDPrintString("Установить");
	LCDMoveTo(1,2);
	LCDPrintString(msg);
	LCDPrintString("?");
	LCDMoveTo(2,6);
	LCDPrintString("[желтая - да ]");
	LCDMoveTo(3,6);
	LCDPrintString("[другие - нет]");
	int key = waitForKeyPress();

	LCDClear();
	LCDMoveTo(1,4);
	if(key == 64){
		LCDPrintString("Установлено!");
		ret = 1;
	}else {
		LCDPrintString("Отменено...");
		ret = 0;
	}
	delay_ms(1500);

	return ret;
}

void setupPredefinedMode(int mode){
	mode --;
	switch(mode){
	case 0: //Привязка
		if(shouldSetup("Привязка") == 0)
			return;
		write_memory(MenuItemValueBase+MenuItemIBase,200);
		write_memory(MenuItemValueBase+MenuItemIBase+1,0);
		write_memory(MenuItemValueBase+MenuItemIBase+2,0);
		write_memory(MenuItemValueBase+MenuItemIBase+3,0);
		write_memory(MenuItemValueBase+MenuItemF,200);
		write_memory(MenuItemValueBase+MenuItemT,600);
		write_memory(MenuItemValueBase+MenuItemPreBlow,40);
		write_memory(MenuItemValueBase+MenuItemPresparkTime,0);
		write_memory(MenuItemValueBase+MenuItemPresparkI,CUR_MAX);
		break;
	case 1: //Искра жесткая
		if(shouldSetup("Искра жесткая") == 0)
			return;
		write_memory(MenuItemValueBase+MenuItemIBase,CUR_MAX);
		write_memory(MenuItemValueBase+MenuItemIBase+1,40);
		write_memory(MenuItemValueBase+MenuItemIBase+2,40);
		write_memory(MenuItemValueBase+MenuItemIBase+3,40);
		write_memory(MenuItemValueBase+MenuItemF,400);
		write_memory(MenuItemValueBase+MenuItemT,100);
		write_memory(MenuItemValueBase+MenuItemPreBlow,40);
		write_memory(MenuItemValueBase+MenuItemPresparkTime,0);
		write_memory(MenuItemValueBase+MenuItemPresparkI,CUR_MAX);
		break;
	case 2: //Искра мягкая
		if(shouldSetup("Искра мягкая") == 0)
			return;
		write_memory(MenuItemValueBase+MenuItemIBase,CUR_MAX/2);
		write_memory(MenuItemValueBase+MenuItemIBase+1,160);
		write_memory(MenuItemValueBase+MenuItemIBase+2,40);
		write_memory(MenuItemValueBase+MenuItemIBase+3,40);
		write_memory(MenuItemValueBase+MenuItemF,300);
		write_memory(MenuItemValueBase+MenuItemT,250);
		write_memory(MenuItemValueBase+MenuItemPreBlow,40);
		write_memory(MenuItemValueBase+MenuItemPresparkTime,0);
		write_memory(MenuItemValueBase+MenuItemPresparkI,CUR_MAX);
		break;
	case 3: //Дуга 5А
		if(shouldSetup("Дуга 4А") == 0)
			return;
		write_memory(MenuItemValueBase+MenuItemIBase,40);
		write_memory(MenuItemValueBase+MenuItemIBase+1,40);
		write_memory(MenuItemValueBase+MenuItemIBase+2,40);
		write_memory(MenuItemValueBase+MenuItemIBase+3,40);
		write_memory(MenuItemValueBase+MenuItemF,400);
		write_memory(MenuItemValueBase+MenuItemT,600);
		write_memory(MenuItemValueBase+MenuItemPreBlow,40);
		write_memory(MenuItemValueBase+MenuItemPresparkTime,0);
		write_memory(MenuItemValueBase+MenuItemPresparkI,CUR_MAX);
		break;
	case 4: //Дуга 10А
		if(shouldSetup("Дуга 10А") == 0)
			return;
		write_memory(MenuItemValueBase+MenuItemIBase,100);
		write_memory(MenuItemValueBase+MenuItemIBase+1,100);
		write_memory(MenuItemValueBase+MenuItemIBase+2,100);
		write_memory(MenuItemValueBase+MenuItemIBase+3,100);
		write_memory(MenuItemValueBase+MenuItemF,400);
		write_memory(MenuItemValueBase+MenuItemT,600);
		write_memory(MenuItemValueBase+MenuItemPreBlow,40);
		write_memory(MenuItemValueBase+MenuItemPresparkTime,0);
		write_memory(MenuItemValueBase+MenuItemPresparkI,CUR_MAX);
		break;
	case 5: //Дуга 15А
		if(shouldSetup("Дуга 16А") == 0)
			return;
		write_memory(MenuItemValueBase+MenuItemIBase,160);
		write_memory(MenuItemValueBase+MenuItemIBase+1,160);
		write_memory(MenuItemValueBase+MenuItemIBase+2,160);
		write_memory(MenuItemValueBase+MenuItemIBase+3,160);
		write_memory(MenuItemValueBase+MenuItemF,400);
		write_memory(MenuItemValueBase+MenuItemT,600);
		write_memory(MenuItemValueBase+MenuItemPreBlow,40);
		write_memory(MenuItemValueBase+MenuItemPresparkTime,0);
		write_memory(MenuItemValueBase+MenuItemPresparkI,CUR_MAX);
		break;
	default:
		return;
	}
	Beep();
}

void keyPressWait(){
	while(GetKeyPressed() == 0);
	do{
		delay_ms(10);
	}while(GetKeyPressed() != 0);
}

void calibrateHRTimer(){
	int i;
	while(1){
		for(i = 0;i<25*60;i++){
			resetHRTimer();
			while(getHRTimerValue() < 0xFF00);
		}
		delay(1);
	}
}
void checkDelay(){
	while(1)
		delay_ms(2000);
}

void systemInit(){
	RCC_ClocksTypeDef RCC_ClockFreq;

	SystemInit();

	RCC_GetClocksFreq(&RCC_ClockFreq);

	//initHRTimer();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

	/* Enable ADCx clock so that we can talk to it */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);


	//RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	/* Allow access to RTC */
	//PWR_BackupAccessCmd(ENABLE);

	//GPIO_PinRemapConfig (GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; // Disable JTAG
}

extern void testBooster();

void checkProtection(){
	//int protection = getPin(GPIOC,GPIO_Pin_8);
	//outPin(GPIOC,GPIO_Pin_8,GPIO_Pin_8);
	//GPIO_SetBits(GPIOC,GPIO_Pin_8);
	//int protection = getPin(GPIOC,GPIO_Pin_8);
	//if(protection == 0)
	//	GPIO_SetBits(GPIOC,GPIO_Pin_8);
	SPIINT_ON;
}

int isArgonFlow = 0;
extern unsigned int NoGasTickCounter;
int menu();
int main(void)
{
	int GasPulsDelay = 0;
	int menu_item,i,j;

	NoGasTickCounter = 0;
	//adc_main();
	//RCC_ClocksTypeDef RCC_ClockFreq;

	systemInit();

	//RCC_GetClocksFreq(&RCC_ClockFreq);

	initHRTimer();
	//checkDelay();//------------------------------------------
	//calibrateHRTimer();//------------------------------------

	LCDInit(20);
	LCDClear();

	/*for(i = 0;i<16;i++){
		LCDClear();
		for(j = 0;j<16;j++){
			LCDMoveTo(0,j);
			LCDSymboxl((char)((i<<4)|j));
		}
		j = 0;
		LCDMoveTo(2,2);
		LCDPrintInt((char)((i<<4)|j));
		j = 15;
		LCDMoveTo(3,2);
		LCDPrintInt((char)((i<<4)|j));

		do{
			delay_ms(10);
		}while(GetKeyState() == 0);
		delay_ms(300);
		do{
			delay_ms(10);
		}while(GetKeyState() != 0);
		delay_ms(300);
	}//*/

	run_init();
	animation();

	SpiMasterInit();
	SpiSlaveInit();
	initADC();

	testRel();//=====================================================
	setRelayN(0x0);

	if(GetKeyState() != 0){
		LCDClear();
		LCDMoveTo(1,3);
		LCDPrintString("Режим отладки");
		while(GetKeyState() != 0)
			MenuItemCount = MenuItemFullCountConst;
		delay_ms(1000);
	}

	LCDClear();
	LCDMoveTo(1,3);
	LCDPrintString("Ожидание");
	LCDMoveTo(2,0);
	LCDPrintString("включения питания");
	do{
		analogStart();
		updateADC();
		LCDMoveTo(3,9);
		LCDPrintInt(ADCInput);
		LCDPrintString("       ");
	} while(ADCInput < U_NORMAL_ADC && GetKeyState() == 0);

	do{
		delay_ms(10);
	}while(GetKeyState() != 0);

	LCDClear();
	LCDMoveTo(1,3);
	LCDPrintString("Проверка");
	LCDMoveTo(2,3);
	LCDPrintString("Ключей");

	for(i = 0;i<PINCOUNT;i++){
		do{
			LCDMoveTo(3,0);
			LCDPrintString("Ждем закрытия               ");
			//LCDPrintInt(i+1);
			analogStart();
			updateADC();
		}while(ADCOut >= U_NORMAL_ADC && GetKeyState() == 0);
		do{
			delay_ms(10);
		}while(GetKeyState() != 0);

		LCDMoveTo(3,0);
		LCDPrintString("                            ");
		LCDMoveTo(3,0);
		LCDPrintString("Ключ #");
		LCDPrintInt(i+1);

		setKeyState(i,1);
		delay_ms(10);
		analogStart();
		updateADC();
		if(ADCOut < U_NORMAL_ADC){
			LCDPrintString(" ОБРЫВ!!!!!");
			LCDPrintInt(ADCOut);
			LCDPrintString("       ");
			waitForKeyPress();
		} else {
			LCDPrintString(" - Ok");
			delay_ms(200);
		}
		setKeyState(i,0);
		if(MenuItemCount == MenuItemFullCountConst)
			delay_ms(400);
	}

	//testBooster();//===============================================

	for(i = 0;i<MenuItemCount;i++)
		write_memory(MenuItemValueBase+i,MItems[i].DefaultValue);

	menu_item = -1;

    while(1)
    {
    	checkProtection();
    	menu_item = menu();
    	if(read_memory(MenuItemSelector) == MenuItemCountConst-1)
    	{
#define REL_DELAY_SECOND 10000
#define REL_DELAY REL_DELAY_SECOND*60*5
    		if(IsGasOn != 0)
    			NoGasTickCounter = 0;
    		else{
    			NoGasTickCounter ++;
    			if(NoGasTickCounter > REL_DELAY)
    			{
    				setRelayN(REL_GAS_LOW);
    				GasPulsDelay = REL_DELAY_SECOND*4;
    			}
    		}
    		if(GasPulsDelay > 0){
    			GasPulsDelay --;
    			if(GasPulsDelay == 0)
    				setRelayN(REL_NONE);
    		}
    	}
    }
}

int strlen(const char *buf){
	int l = 0;
	for(;buf[l] != 0 && l < 128;l++)
		;
	return l;
}

int genOn = 0,prespark;
int isGeneratorOn()
{
	int i;
	if(GET_EXT_START == 0){
		genOn = 3;
	} else {
		if(genOn > 0){
			genOn --;
		}
		if(genOn == 0){
			for(i = 0;i<50 && genOn == 0 && GET_EXT_START != 0;i ++)
				delay_ms(10);
			if(GET_EXT_START == 0)
				genOn = 3;
			prespark = 0;
		}
	}
	return genOn;
}

int isCameraNotReady(){
	if(getPin(GPIOA,GPIO_Pin_0))
		return 1;
	if(getPin(GPIOC,GPIO_Pin_15))
		return 2;
	return 0;
}

#define MAX_TIME 300
const char *STime[MAX_TIME] = {"0","1","2","3","4","5","6","7","8","9",
		"10","11","12","13","14","15","16","17","18","19",
		"20","21","22","23","24","25","26","27","28","29",
		"30","31","32","33","34","35","36","37","38","39",
		"40","41","42","43","44","45","46","47","48","49",
		"50","51","52","53","54","55","56","57","58","59",
		"60","61","62","63","64","65","66","67","68","69",
		"70","71","72","73","74","75","76","77","78","79",
		"80","81","82","83","84","85","86","87","88","89",
		"90","91","92","93","94","95","96","97","98","99",
		"100","101","102","103","104","105","106","107","108","109",
		"110","111","112","113","114","115","116","117","118","119",
		"120","121","122","123","124","125","126","127","128","129",
		"130","131","132","133","134","135","136","137","138","139",
		"140","141","142","143","144","145","146","147","148","149",
		"150","151","152","153","154","155","156","157","158","159",
		"160","161","162","163","164","165","166","167","168","169",
		"170","171","172","173","174","175","176","177","178","179",
		"180","181","182","183","184","185","186","187","188","189",
		"190","191","192","193","194","195","196","197","198","199",
		"200","201","202","203","204","205","206","207","208","209",
		"210","211","212","213","214","215","216","217","218","219",
		"220","221","222","223","224","225","226","227","228","229",
		"230","231","232","233","234","235","236","237","238","239",
		"240","241","242","243","244","245","246","247","248","249",
		"250","251","252","253","254","255","256","257","258","259",
		"260","261","262","263","264","265","266","267","268","269",
		"270","271","272","273","274","275","276","277","278","279",
		"280","281","282","283","284","285","286","287","288","289",
		"290","291","292","293","294","295","296","297","298","299"}
#define DEBUGP(c) // {LCDMoveTo(3,15); LCDPutChar(c);}
void mainCycle(int by_key){
	float spark_time_s = 0.0006;
	LCDLedStatus(1);
	delay_ms(5);
	int protection = getPin(GPIOD,GPIO_Pin_14);
	if(protection != 0 && FLAG_NO_PROT_CHECK == 0){
		LCDClear();
		LCDMoveTo(1,0);
		LCDPrintString("Защита");
		LCDMoveTo(2,0);
		LCDPrintString("Штатива");
		Beep();
		delay_ms(200);
		LCDLedStatus(0);
		return;
	}
	// ecxidental start protection...
	if(!by_key){
		int i;
		for(i = 0;i<1000;i++){
			if(GET_EXT_START == 1){
				LCDLedStatus(0);
				return;
			}
		}
	}
	if(read_memory(MenuItemSelector) != MenuItemCountConst-1){
		LCDClear();
		LCDMoveTo(1,3);
		LCDPrintString("К пуску");
		LCDMoveTo(2,3);
		LCDPrintString("не готов");
		delay_ms(500);
		return;
	}

	initInputPin(GPIOA,GPIO_Pin_10);
	initInputPin(GPIOD,GPIO_Pin_14);
	int key = GetKeyState();
	int i;//,c_lim;
	do{
		delay_ms(100);
		key = GetKeyState();
	}while(key != 0);

	NoSparkCount = 0;
	NoCurrentCount = 0;
	SparkCount = 0;

	int f = read_memory(MenuItemValueBase+MenuItemF);//5);
	if(f == 0){
		Beep();
		LCDPrintString("Не задана частота");
		delay_ms(1000);
		return;
	}

	spark_time_s = read_memory(MenuItemValueBase+MenuItemT)*0.000001;
	if(f == 0){
		Beep();
		LCDPrintString("Не задано время");
		delay_ms(1000);
		return;
	}

	int mla[4];
	int mla_len = 0;
	for(i = 0;i<=3;i++){
		int cur = read_memory(MenuItemValueBase+i+MenuItemIBase);
		if(cur == 0){
			//mla_len = i;
			break;
		} else {
			mla_len ++;
		}
	}

	if(mla_len == 0){
		setPrecharge(0);
		Beep();
		Beep();
		LCDPrintString("Не заданы токи");
		delay_ms(1000);
		return;
	}

	for(i = 0;i<mla_len;i++){
		mla[i] = read_memory(MenuItemValueBase+i+MenuItemIBase);//1);
	}

	int prespark_i = read_memory(MenuItemValueBase+MenuItemPresparkI);//8);
	int mla_presp[4] = {prespark_i,prespark_i,prespark_i,prespark_i};

	LCDClear();
	LCDMoveTo(1,3);
	LCDPrintString("Продувка...");
	int preBlow = read_memory(MenuItemValueBase+MenuItemPreBlow);// 6);
	LCDPrintFInt(preBlow,1);LCDPrintString("сек.");
	setRelayN(REL_ALL);
	while(preBlow > 0){
		delay_ms(99);
		preBlow -= 1;
		key = GetKeyState();
		if(key != 0){
			setPrecharge(0);
			LCDClear();
			LCDMoveTo(1,3);
			LCDPrintString("СТОП");
			setRelayN(REL_NONE);
			delay_ms(400);
			setRelayN(REL_ALL);
			delay_ms(400);
			setRelayN(REL_NONE);
			while(GetKeyState() != 0)
				;
			while(by_key == 0 && isGeneratorOn())
				;
			return;
		}
		LCDMoveTo(2,8);
		LCDPrintFInt(preBlow,1);
		LCDPrintString("   ");
	}

	prespark = read_memory(MenuItemValueBase+MenuItemPresparkTime);//*60;

	LCDClear();
	LCDMoveTo(2,12);
	LCDPrintString("РАЗРЯД!");

	setPrecharge(1);
	//int prev = getHRTimerValue();//SysTickCounter;
	int t_step_real = HRES_TICKS_IN_SEC/f;
	//t_step_real /= 2;
	int t_protection_zone = HRES_TICKS_IN_SEC / 1000;//t_step - HRES_TICKS_IN_SEC / 700;
	//int next = prev + t_step;
	int spark_count = 0;

	int t_step;
	//if(prespark > 0)
	//	t_step = HRES_TICKS_IN_SEC/MAX_F;
	//else
		t_step = t_step_real;

	LCDMoveTo(0,0);
	if(prespark > 0){
		LCDPrintInt(prespark/10);
		LCDPrintString("X");
		LCDPrintInt(mla_presp[0]/10);
	}
	LCDPrintString(" A[");
	for(i = 0;i<mla_len;i++){
		LCDPrintInt(mla[i]/10);
		if(i < mla_len-1)
			LCDPrintString(" ");
	}
	LCDPrintString("]");
	LCDMoveTo(1,0);
	LCDPrintInt(f);
	LCDPrintString("Hz ");
	LCDPrintInt(spark_time_s*1000000);
	LCDPrintString("us");

	prespark = prespark * f / 10;

	LCDMoveTo(3,0);

	int opened = isCameraNotReady();//getPin(GPIOA,GPIO_Pin_10);
	if(opened != 0 && FLAG_NO_GAS_CHECK == 0){
		opened = 1;
		LCDClear();
		LCDMoveTo(1,3);
		LCDPrintString("ОТКРЫТ ШТАТИВ!");
		setRelayN(REL_NONE);
	}
	else{
		opened = 0;
		//delay_ms(200);
		setRelayN(REL_GAS_LOW | REL_MAIN | REL_600V);
		delay_ms(200);
	}

	resetHRTimer();
	int error_count = 0;
	int count = 0;
	int count_error = 0;
	char exit_by_error = 0;
	int no_spark = 0;
	int mode = 0;
	//for(c_lim = 0;c_lim < 1000;c_lim++){
	//while(by_key == 1 || (by_key == 0 && GET_EXT_START == 0)){

	int led_blink_counter = 0;
	while(by_key == 1 || (by_key == 0 && isGeneratorOn())){
		//checkPrecharge();
		DEBUGP('S');
		if(opened == 1){
			key = GetKeyState();
			if(key != 0){
				LCDClear();
				LCDMoveTo(1,3);
				LCDPrintString("СТОП");
				while(GetKeyState() != 0)
					;
				break;
			}
			continue;
		}
		DEBUGP('P');
		if(getHRTimerValue() >= t_step*0.8)
			setPrecharge(1);
		DEBUGP('C');
		if(getHRTimerValue() >= t_step){
			int protection = getPin(GPIOD,GPIO_Pin_14);
			if(protection != 0 && FLAG_NO_PROT_CHECK == 0){
				LCDClear();
				LCDMoveTo(1,0);
				LCDPrintString("Защита");
				LCDMoveTo(2,0);
				LCDPrintString("Штатива");
				Beep();
				delay_ms(1000);
				break;
			}

			resetHRTimer();
#define SPARK_NO_CONTROL 50
			//next = SysTickCounter + t_step;
			int sparks;
			DEBUGP('S');
			if(prespark > 0){
				sparks = spark(mla_presp,4,t_protection_zone,spark_time_s);
				prespark --;
				if(mode != 1){
					LCDMoveTo(3,0);
					LCDPutChar('P');
					mode = 1;
				}

			} else {
				sparks = spark(mla,mla_len,t_protection_zone,spark_time_s);
				t_step = t_step_real;
				if(mode != 2){
					LCDMoveTo(3,0);
					LCDPutChar('P');
					mode = 2;
				}
				/*LCDMoveTo(3,0);
				if(count%f == 0){
					LCDPutChar('S');
					LCDPrintInt(count/f);
				}*/
			}
			if(count%f == 0 && count%f < MAX_TIME){
				LCDMoveTo(3,1);
				LCDPrintString(STime[count%f]);
			}
			DEBUGP('E');
			if(sparks <= 0)
				no_spark ++;
			//LCDMoveTo(2,0);
			//LCDPrintInt(no_spark);
			int spark_time = getHRTimerValue();
			if((sparks < 1 || spark_time > t_step)
					&& count > SPARK_NO_CONTROL && FLAG_NO_SPARK_CHECK == 0){
				count_error ++;
				count ++;
				if(error_count > 50){
					setRelayN(REL_NONE);
					LCDClear();
					LCDMoveTo(1,0);
					LCDPrintString("Ошибка(");
					LCDPrintInt(count);
					LCDPrintString("ign)");
					LCDMoveTo(2,0);
					if(spark_time > t_step){
						LCDPrintString("Overtime");
					} else {
						if(sparks >= 0)
							LCDPrintString("I достигнут ");LCDPrintInt(sparks);LCDPrintString(" раз");
						LCDMoveTo(3,2);
						LCDPrintString(ErrorTxt);
					}
					Beep();
					setPrecharge(0);
					exit_by_error = 1;
					waitForKeyPress();
					break;
				} else {
					if(count > SPARK_NO_CONTROL)
						error_count +=10;
				}
				//setPrecharge(1);
			}
			else
			{
				//setPrecharge(1);
				led_blink_counter ++;
				if(led_blink_counter & 128)
					LCDLedStatus(1);
				else
					LCDLedStatus(0);
				count ++;
				key = GetKeyState();
				if(key != 0){
					LCDPrintString("СТОП");
					while(GetKeyState() != 0)
						delay_ms(10);
					break;
				}
				error_count /= 2;
			}
			spark_count ++;
			delay_us(50);
		}
		DEBUGP('!');
	}
	LCDLedStatus(0);
	sparkForDischarge();
	setRelayN(REL_NONE);
	setPrecharge(0);
	delay_ms(200);
	setRelayN(REL_ALL);
	delay_ms(200);
	setRelayN(REL_NONE);
	if(exit_by_error != 0)
		delay_ms(200);
	LCDLedStatus(0);
	LCDClear();
	LCDMoveTo(0,0);
	LCDPrintString("Stat:");
	LCDPrintInt(count);
	LCDPrintString("/");
	LCDPrintInt(count_error);
	LCDMoveTo(1,1);
	LCDPrintFInt(count_error*10000/count,2);
	LCDPrintString("%");
	delay_ms(5000);
	NoGasTickCounter = 0;
}

char FLAG_NO_GAS_CHECK = NO_GAS_CHECK_DEFULT;
char FLAG_NO_SPARK_CHECK = 0;
char FLAG_NO_PROT_CHECK = 0;
void menuFunctions(int function, int dir){
	switch(function){
	case FK_NO_SPARK_CHECK:
		if(dir > 0)
			FLAG_NO_SPARK_CHECK = 1;
		else
			FLAG_NO_SPARK_CHECK = 0;
		break;
	case FK_NO_GAS_CHECK:
		if(dir > 0)
			FLAG_NO_GAS_CHECK = 1;
		else
			FLAG_NO_GAS_CHECK = 0;
		break;
	case FK_NO_PROT_CHECK:
		if(dir > 0)
			FLAG_NO_PROT_CHECK = 1;
		else
			FLAG_NO_PROT_CHECK = 0;
		break;
	default:
		return;
	}
}

char IsFirstTime = 1;
int menu(){
	int menu_item_index = read_memory(MenuItemSelector);
	int loop_menu_item;
	int val = read_memory(MenuItemValueBase+menu_item_index);
	int key;

	LCDLedStatus(0);

	if(GET_EXT_START == 0){
		mainCycle(0);
		key = 0xFFFF;
	}
	else
		key = GetKeyPressed();

	switch(key){
	case 1:
		mainCycle(1);
		break;
	case 8:
		if(menu_item_index > 0){
			menu_item_index --;
			write_memory(MenuItemSelector,menu_item_index);
			val = read_memory(MenuItemValueBase+menu_item_index);
		}
		break;
	case 16:
		if(menu_item_index < MenuItemCount-1){
			menu_item_index ++;
			write_memory(MenuItemSelector,menu_item_index);
			val = read_memory(MenuItemValueBase+menu_item_index);
		}
		break;
	case 4:
		menuFunctions(MItems[menu_item_index].Function,-1);
		if(MItems[menu_item_index].DecimalNumber < 0){
			if(MItems[menu_item_index].Min >= 0)
				setupPredefinedMode(menu_item_index);
			break;
		}
		if(MItems[menu_item_index].DefaultStep > 0){
			val -= MItems[menu_item_index].DefaultStep;
		} else {
			int key_val = getKeyMask(val);
			int new_key_val;
			do{
				val -= 20;
				new_key_val = getKeyMask(val);
			}while(key_val == new_key_val && val > MItems[menu_item_index].Min && new_key_val >= 0);
		}
		if(val < MItems[menu_item_index].Min)
			val = MItems[menu_item_index].Min;
		write_memory(MenuItemValueBase+menu_item_index,val);
		break;
	case 32:
		while(GetKeyState() != 0)
			delay_ms(10);
		menuFunctions(MItems[menu_item_index].Function,1);
		if(MItems[menu_item_index].DecimalNumber < 0){
			if(MItems[menu_item_index].Min >= 0)
				setupPredefinedMode(menu_item_index);
			break;
		}
		if(MItems[menu_item_index].DefaultStep > 0){
			val += MItems[menu_item_index].DefaultStep;
		} else {
			int key_val = getKeyMask(val);
			int new_key_val;
			do{
				val +=20;
				new_key_val = getKeyMask(val);
			}while(key_val == new_key_val && new_key_val > 0 && val < MItems[menu_item_index].Max);
		}
		if(val > MItems[menu_item_index].Max)
			val = MItems[menu_item_index].Max;
		write_memory(MenuItemValueBase+menu_item_index,val);
		break;
	case 64:
		if(menu_item_index > 0){
			val = read_memory(MenuItemValueBase+menu_item_index-1);
			write_memory(MenuItemValueBase+menu_item_index,val);
		}
		break;
	case 128:
		/*if(menu_item_index < MenuItemCount){
			val = read_memory(MenuItemValueBase+menu_item_index+1);
			write_memory(MenuItemValueBase+menu_item_index,val);
		}*/
		//setRelay(128+64);
		if(isArgonFlow == 0){
			setRelayN(REL_GAS_MAX);
			delay_ms(500);
			initInputPin(GPIOA,GPIO_Pin_10);
			int opened = 0;
			int cycled = 0;
			while(GetKeyState() != 0){
				delay_ms(100);
				cycled ++;
				opened = isCameraNotReady();
				if(opened == 1){
					LCDClear();
					LCDMoveTo(1,0);
					LCDPrintString("ШТАТИВ НЕ ГЕРМЕТИЧЕН!!!");
				} else {
					if(opened == 2){
						LCDClear();
						LCDMoveTo(1,3);
						LCDPrintString("ЗАЩИТА ШТАТИВА!!!");
					} else {
						LCDClear();
						LCDMoveTo(1,1);
						LCDPrintString("Штатив готов");
					}
				}
				if(cycled > 50){
					LCDMoveTo(2,5);
					LCDPrintString("Режим залипания");
					isArgonFlow = 1;
				}
			}
			if(isArgonFlow == 0){
				setRelayN(REL_NONE);
				delay_ms(200);
				setRelayN(REL_GAS_MAX);
				delay_ms(200);
				setRelayN(REL_NONE);
			}
		} else {
			setRelayN(REL_NONE);
			delay_ms(200);
			setRelayN(REL_GAS_MAX);
			delay_ms(200);
			setRelayN(REL_NONE);
			while(GetKeyState() != 0)
					delay_ms(100);
			isArgonFlow = 0;
		}
		break;
	default:
		if(IsFirstTime == 0)
			return menu_item_index;
		else{
			IsFirstTime = 0;
			write_memory(MenuItemSelector,0);
			menu_item_index = 0;
		}
	case 0xFFFF:
		break;
	}

	for(loop_menu_item = menu_item_index-1;
			loop_menu_item <= menu_item_index+1;
			loop_menu_item++){
		//char str_buffer[64];
		int y = loop_menu_item - menu_item_index + 1;
		LCDMoveTo(y,2);
		if(loop_menu_item < 0 || loop_menu_item >= MenuItemCount){//MItems[loop_menu_item].Name == NULL){
			LCDPrintString("                    ");
			continue;
		}

		LCDPrintString(MItems[loop_menu_item].Name);
		if(MItems[loop_menu_item].DecimalNumber >= 0)
			LCDPrintFInt(read_memory(MenuItemValueBase+loop_menu_item),MItems[loop_menu_item].DecimalNumber);
		LCDPrintString(MItems[loop_menu_item].UnitName);

		LCDPrintString("                    ");
	}

	LCDMoveTo(0,0);
	if(menu_item_index > 0)
		LCDPrintString("^ ");
	else
		LCDPrintString("  ");

	LCDMoveTo(1,0);
	if(MItems[menu_item_index].DefaultStep > 0)
	{
		if(val > MItems[menu_item_index].Min)
			LCDPrintString("<");
		else
			LCDPrintString(" ");
		if(val < MItems[menu_item_index].Max)
			LCDPrintString(">");
		else
			LCDPrintString(" ");
	}
	else{
		if(MItems[menu_item_index].DefaultStep < 0)
			LCDPrintString("s-");
		else
			LCDPrintString("  ");
	}

	LCDMoveTo(2,0);
	if(menu_item_index < MenuItemCount-1)
		LCDPrintString("v ");
	else
		LCDPrintString("  ");

	LCDMoveTo(3,0);
	LCDPrintString(" spectrometer.com.ua    ");

	return menu_item_index;
}
