#include "run.h"
#include "time.h"
#include "stm_port.h"
#include "adc.h"
#include "common.h"
#include <stdio.h>

GPIO_InitTypeDef  GPIO_InitStructure;

#define GATE_PIN1_PORT 	GPIOC
#define GATE_PIN1 		GPIO_Pin_4
#define GATE_PIN2_PORT 	GPIOB
#define GATE_PIN2 		GPIO_Pin_1
#define GATE_PIN3_PORT 	GPIOA
#define GATE_PIN3 		GPIO_Pin_7
#define GATE_PIN4_PORT 	GPIOB
#define GATE_PIN4 		GPIO_Pin_0
#define GATE_PIN5_PORT 	GPIOA
#define GATE_PIN5 		GPIO_Pin_6
#define GATE_PIN6_PORT 	GPIOC
#define GATE_PIN6 		GPIO_Pin_5
#define GATE_PIN7_PORT 	GPIOA
#define GATE_PIN7 		GPIO_Pin_4
#define GATE_PIN8_PORT 	GPIOA
#define GATE_PIN8 		GPIO_Pin_5

struct PINInfo{
	GPIO_TypeDef* GPIO;
	int pin;
	int current10;
	char isInverted;
};

inline void on(struct PINInfo *inf){
	if(inf->isInverted)
		GPIO_SetBits(inf->GPIO,inf->pin);
	else
		GPIO_ResetBits(inf->GPIO,inf->pin);
}

inline void off(struct PINInfo *inf){
	if(inf->isInverted)
		GPIO_ResetBits(inf->GPIO,inf->pin);
	else
		GPIO_SetBits(inf->GPIO,inf->pin);
}

void init(struct PINInfo *inf){
	GPIO_InitStructure.GPIO_Pin = inf->pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(inf->GPIO, &GPIO_InitStructure);

	off(inf);
}

// hi curent key must to be first... low curent last...
struct PINInfo PINs[PINCOUNT] = {
		{GATE_PIN7_PORT,GATE_PIN7,80,true}, // 8A  1
		{GATE_PIN8_PORT,GATE_PIN8,80,true}, // 8A  2
 		{GATE_PIN5_PORT,GATE_PIN5,80,false}, // 8A 3
		{GATE_PIN6_PORT,GATE_PIN6,80,false}, // 8A  4
		{GATE_PIN3_PORT,GATE_PIN3,80,false}, // 8A 5
		{GATE_PIN4_PORT,GATE_PIN4,80,false}, // 8A 6
		{GATE_PIN1_PORT,GATE_PIN1,40,false}, // 4A 7
		{GATE_PIN2_PORT,GATE_PIN2,20,false} // 2A 8
};

void setKeyState(int pin,char state){
	if(state == 0)
		off(PINs+pin);
	else
		on(PINs+pin);
}

#define PRECHARGE_PIN GPIO_Pin_1
#define PRECHARGE_PORT GPIOC
#define PRECHARGE_ON outPin(PRECHARGE_PORT,PRECHARGE_PIN,PRECHARGE_PIN)                //#define PRECHARGE_OFF outPin(GPIOD,PRECHARGE_PIN,PRECHARGE_PIN)
#define PRECHARGE_OFF outPin(PRECHARGE_PORT,PRECHARGE_PIN,0)                           //#define PRECHARGE_ON outPin(GPIOD,PRECHARGE_PIN,0)

//#define PRECHARGE_PIN GPIO_Pin_1
//#define PRECHARGE_PORT GPIOC
//#define PRECHARGE_ON outPin(PRECHARGE_PORT,PRECHARGE_PIN,PRECHARGE_PIN)                //#define PRECHARGE_OFF outPin(GPIOD,PRECHARGE_PIN,PRECHARGE_PIN)
//#define PRECHARGE_OFF outPin(PRECHARGE_PORT,PRECHARGE_PIN,0)                           //#define PRECHARGE_ON outPin(GPIOD,PRECHARGE_PIN,0)

char checkPrecharge(){
	/*if(GET_PRECHARGE_DONE)
		PRECHARGE_OFF;
	else
		return 0;*/
	return 1;
}

setPrecharge(char isOn){
	if(isOn)
		PRECHARGE_ON;
	else
		PRECHARGE_OFF;
	/*LCDClear();
	LCDMoveTo(1,1);
	LCDPrintString("PRECHARGE OFF");
	delay_ms(500);//*/
}

#define SPINI_PIN GPIO_Pin_0
#define SPINI_PORT GPIOC
#define SPINI_OFF outPin(SPINI_PORT,SPINI_PIN,SPINI_PIN)                        //#define SPINI_ON outPin(GPIOD,GPIO_Pin_15,GPIO_Pin_15)
#define SPINI_ON outPin(SPINI_PORT,SPINI_PIN,0)                                 //#define SPINI_OFF outPin(GPIOD,GPIO_Pin_15,0)

#define BLINK_DELAY 500
void run_test_pins(){
	int i;
	while(1){
		PRECHARGE_ON;
		delay_ms(200);
		PRECHARGE_OFF;
		delay_ms(200);
	}
	do{
		for(i = 3;i<PINCOUNT;i++){
			if(i > 3)
				on(PINs+i);
			delay_ms(BLINK_DELAY);
			while(GetKeyReleased() == 0);
			while(GetKeyReleased() != 0);
			off(PINs+i);
		}
	} while(1);
	do{
		on(PINs);
		delay_ms(BLINK_DELAY);
		on(PINs+1);
		delay_ms(BLINK_DELAY);
		on(PINs+2);
		delay_ms(BLINK_DELAY);
		on(PINs+3);
		delay_ms(BLINK_DELAY);
		on(PINs+4);
		delay_ms(BLINK_DELAY);//*/
		on(PINs+5);
		delay_ms(BLINK_DELAY);

		off(PINs);
		delay_ms(BLINK_DELAY);
		off(PINs+1);
		delay_ms(BLINK_DELAY);
		off(PINs+2);
		delay_ms(BLINK_DELAY);
		off(PINs+3);
		delay_ms(BLINK_DELAY);
		off(PINs+4);
		delay_ms(BLINK_DELAY);//*/
		off(PINs+5);
		delay_ms(BLINK_DELAY);
	}while(1);
	/*while(1){
		GATE3_ON;
		GATE3A_ON;
		GATE4_ON;
		GATE4A_ON;
		delay_ms(1);
		GATE3_OFF;
		GATE3A_OFF;
		GATE4_OFF;
		GATE4A_OFF;
		delay_ms(1);
	}//*/
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	while(1){
		outPin(GPIOE,GPIO_Pin_2,GPIO_Pin_2);
		outPin(GPIOE,GPIO_Pin_3,GPIO_Pin_3);
		delay_ms(1);
		outPin(GPIOE,GPIO_Pin_2,0);
		outPin(GPIOE,GPIO_Pin_3,0);
		delay_ms(1);
	}//*/
}

void run_init(){
	int i;
	for(i = 0;i<PINCOUNT;i++)
		init(PINs+i);
	/*GPIO_InitStructure.GPIO_Pin = GATE_PIN1 | GATE_PIN2 | GATE_PIN3 | GATE_PIN4 | GATE_PIN6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);*/

	SPINI_ON;
	PRECHARGE_OFF;
	initGPIOBitOut(SPINI_PORT,SPINI_PIN);
	initGPIOBitOut(PRECHARGE_PORT,PRECHARGE_PIN);

	//initInputPin(GPIOD,PRECHARGE_RDY);

	READY_ON;
	initOutputPin(GPIOD,READY_PIN);
}

/*void spark_correct(int cur_mla[],int len){
	int cycle;
	for(cycle = 0;cycle<len;cycle++)
		cur_mla[cycle] = (int)(112.97+cur_mla[cycle]/66.2516791549);
}//*/

int getKeyMask(int cur){
	int ret = 0;
	int i;
	int mask = 1;
	for(i = 0;i<PINCOUNT;i++){
		if(cur >= PINs[i].current10){
			ret |= mask;
			cur -= PINs[i].current10;
		}
		mask = mask << 1;
	}
	if(cur == 0)
		return ret;
	return -1;
}

void setCurrent(int cur){
	//int ret = 0;
	int i;
	for(i = 0;i<PINCOUNT;i++){
		if(cur >= PINs[i].current10){
			cur -= PINs[i].current10;
			on(PINs+i);
		} else {
			off(PINs+i);
		}
	}
}

void sparkForDischarge(){
	PRECHARGE_OFF;
	SPINI_OFF;
	delay_us(1000);
	SPINI_ON;
}

void testBooster(){
	while(1){
		PRECHARGE_ON;
		delay_ms(100);
		PRECHARGE_OFF;
		delay_ms(1000);
	}
}

char *ErrorTxt;
char ErrorTxtBuf[1024];
//#define MAX_STEP_TIME 1019
//#define I_DEBUG
#ifdef I_DEBUG
int CurTmp[1024];
#endif
int NoSparkCount,NoCurrentCount;
int SparkCount,IgnCount;
int spark(int cur_mla[],int len,int time_out_sys_ticks_val,float spark_time_s){
	int cycle;
	//const int step_t = (int)(HRES_TICKS_IN_SEC * 0.0005)/4;
	//const int step_t = (int)(HRES_TICKS_IN_SEC * 0.0006)/4;
	int step_t = (int)(HRES_TICKS_IN_SEC * spark_time_s)/4;
#define ITRACK_PROTECTION_ZONE_SIZE 20
	int minCur = 0,maxCur,itrack = ITRACK_PROTECTION_ZONE_SIZE;
	int i;
	//initInputPin(GPIOD,PRECHARGE_RDY);

	PRECHARGE_ON;
	delay_us(300);

	for(i = 0;i<16;i++){
		analogStart();
		updateADC();
		minCur += ADCCur;
	}
	minCur /= 15;
	//minCur += 10;

	SparkCount ++;

	//int i;
	//int us_from = ADCInput,ut_from = ADCOut;
	int i_from = ADCCur;

	//int ign_pin = 5;
	setCurrent(cur_mla[0]);//setCurrent(CUR_MAX);//cur_mla[0]);
	analogStart();
	updateADC();
	if(ADCOut < U_NORMAL_ADC && ADCInput > U_NORMAL_ADC){
		setCurrent(0);
		sprintf(ErrorTxtBuf," « ‚ ÿ“¿“»¬≈!");
		delay_ms(500);
		return 0;
	}
	//int phase = 0;
	int i_target = i_from + 150;

	PRECHARGE_OFF;
	SPINI_OFF; //------ Ignition start ----------------------
	delay_us(30);
	SPINI_ON;

	int ign_tick;
	analogStart();
	//GATE_ALL_ON;
	for(ign_tick = 0;ign_tick < 2;ign_tick ++)
	{
		updateADC();
		if(ADCCur > i_target)
			break;
		analogStart();
	}
	//LCDMoveTo(0,15);
	//LCDPutChar('I');
	//LCDPrintInt(ign_tick);
	SPINI_ON;
	//GATES_OFF;

	/*if(ADCCur < i_target)
	{
		setCurrent(0);
		//ErrorTxt = "No spark";
		sprintf(ErrorTxtBuf,"NoSpark I=%d",ADCCur);
		ErrorTxt = ErrorTxtBuf;
		NoSparkCount ++;
		PRECHARGE_ON;
		return -1;
	}//*/
	SPINI_ON;

	//setCurrent(cur_mla[0]);

	int done = 1;
	int next_spark = getHRTimerValue();
	//resetHRTimer();
//#define CYCLE_DELAY 5
	for(cycle = 0;cycle<len;cycle ++){
		if(cur_mla[cycle] != 0)
			setCurrent(cur_mla[cycle]);
		else
			break;
		next_spark += step_t;
		do{
			/*analogStart();
			updateADC();
			if(ADCCur < i_target){
				sprintf(ErrorTxtBuf,"NoCurFound!!");
				done --;
				break;
			}//*/
		}while(getHRTimerValue() < next_spark);
	}

	setCurrent(0);

	//maxCur = 0;
	minCur += 5;
	/*while(itrack > 0){
		analogStart();
		updateADC();
		if(ADCCur <= minCur)
			itrack --;
		else
			itrack = ITRACK_PROTECTION_ZONE_SIZE;
		if(itrack == 100){
			LCDMoveTo(2,0);
			LCDPrintString(" ");
			LCDPrintInt(ADCCur);
		    LCDPrintString(">");
		    LCDPrintInt(minCur);
		    LCDPrintString("    ");
		    LCDPrintInt(itrack);
		    LCDPrintString("tick ");
		    //delay_ms(100);
		}
		//else
		//	maxCur ++;
	}//*/
	time_out_sys_ticks_val -= time_out_sys_ticks_val/10;
	while(getHRTimerValue() < time_out_sys_ticks_val && ADCCur > minCur){
		analogStart();
		updateADC();
	}

	/*if(ADCCur > minCur){
		LCDMoveTo(2,0);
		LCDPrintString(" ");
		LCDPrintInt(ADCCur);
		LCDPrintString(">");
		LCDPrintInt(minCur);
		LCDPrintString("    ");
	}*/

	if(done <= 0)
		NoCurrentCount ++;

	//delay_us(300);

	//PRECHARGE_ON;

	//initInputPin(GPIOD,PRECHARGE_RDY);

	return done;
}


