#include "run.h"
#include "time.h"
#include "stm_port.h"
#include "adc.h"
#include <stdio.h>

GPIO_InitTypeDef  GPIO_InitStructure;

#define GATE_PIN1 GPIO_Pin_3
#define GATE_PIN2 GPIO_Pin_2
#define GATE_PIN3 GPIO_Pin_4
#define GATE_PIN4 GPIO_Pin_5
#define GATE_PIN5 GPIO_Pin_13
#define GATE_PIN6 GPIO_Pin_6

#define GATE_PIN7 GPIO_Pin_10
#define GATE_PIN8 GPIO_Pin_11

struct PINInfo{
	GPIO_TypeDef* GPIO;
	int pin;
	int current10;

};

void init(struct PINInfo *inf){
	GPIO_InitStructure.GPIO_Pin = inf->pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(inf->GPIO, &GPIO_InitStructure);

	GPIO_ResetBits(inf->GPIO,inf->pin);
}

inline void set(struct PINInfo *inf){
	GPIO_SetBits(inf->GPIO,inf->pin);
}

inline void clr(struct PINInfo *inf){
	GPIO_ResetBits(inf->GPIO,inf->pin);
}

//[4][1][2][0][3][5][7][6]
// hi curent key must to be first... low curent last...
struct PINInfo PINs[PINCOUNT] = {
		{GPIOE,GATE_PIN1,70},
		{GPIOE,GATE_PIN2,70},
		{GPIOE,GATE_PIN3,70},
		{GPIOE,GATE_PIN4,70},
		{GPIOC,GATE_PIN5,70},
		{GPIOE,GATE_PIN6,70},
		{GPIOE,GATE_PIN8,35},
		{GPIOE,GATE_PIN7,18}
};

void setKeyState(int pin,char state){
	if(state == 0)
		clr(PINs+pin);
	else
		set(PINs+pin);
}

#define PRECHARGE_PIN GPIO_Pin_13
#define PRECHARGE_ON outPin(GPIOD,PRECHARGE_PIN,PRECHARGE_PIN)                //#define PRECHARGE_OFF outPin(GPIOD,PRECHARGE_PIN,PRECHARGE_PIN)
#define PRECHARGE_OFF outPin(GPIOD,PRECHARGE_PIN,0)                           //#define PRECHARGE_ON outPin(GPIOD,PRECHARGE_PIN,0)
#define PRECHARGE_RDY GPIO_Pin_12
#define GET_PRECHARGE_DONE getPin(GPIOD,PRECHARGE_RDY) == 0

char checkPrecharge(){
	if(GET_PRECHARGE_DONE)
		PRECHARGE_OFF;
	else
		return 0;
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

#define SPINI_PIN GPIO_Pin_15
#define SPINI_OFF outPin(GPIOD,GPIO_Pin_15,GPIO_Pin_15)                        //#define SPINI_ON outPin(GPIOD,GPIO_Pin_15,GPIO_Pin_15)
#define SPINI_ON outPin(GPIOD,GPIO_Pin_15,0)                                 //#define SPINI_OFF outPin(GPIOD,GPIO_Pin_15,0)

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
				set(PINs+i);
			delay_ms(BLINK_DELAY);
			while(GetKeyCode() == 0);
			while(GetKeyCode() != 0);
			clr(PINs+i);
		}
	} while(1);
	do{
		set(PINs);
		delay_ms(BLINK_DELAY);
		set(PINs+1);
		delay_ms(BLINK_DELAY);
		set(PINs+2);
		delay_ms(BLINK_DELAY);
		set(PINs+3);
		delay_ms(BLINK_DELAY);
		set(PINs+4);
		delay_ms(BLINK_DELAY);//*/
		set(PINs+5);
		delay_ms(BLINK_DELAY);

		clr(PINs);
		delay_ms(BLINK_DELAY);
		clr(PINs+1);
		delay_ms(BLINK_DELAY);
		clr(PINs+2);
		delay_ms(BLINK_DELAY);
		clr(PINs+3);
		delay_ms(BLINK_DELAY);
		clr(PINs+4);
		delay_ms(BLINK_DELAY);//*/
		clr(PINs+5);
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
	GPIO_InitStructure.GPIO_Pin = SPINI_PIN | PRECHARGE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	initInputPin(GPIOD,PRECHARGE_RDY);

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
			set(PINs+i);
		} else {
			clr(PINs+i);
		}
	}
}

void sparkForDischarge(){
	PRECHARGE_OFF;
	SPINI_OFF;
	delay_us(1000);
	SPINI_ON;
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
int spark(int cur_mla[],int len,int time_out_sys_ticks_val){
	int cycle;
	//const int step_t = (int)(HRES_TICKS_IN_SEC * 0.0005)/4;
	const int step_t = (int)(HRES_TICKS_IN_SEC * 0.0006)/4;

	//initInputPin(GPIOD,PRECHARGE_RDY);

	analogStart();
	updateADC();

	SparkCount ++;

	//int i;
	//int us_from = ADCInput,ut_from = ADCOut;
	int i_from = ADCCur;

	//int ign_pin = 5;
	setCurrent(CUR_MAX);//cur_mla[0]);
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

	int ign_tick;
	analogStart();
	//GATE_ALL_ON;
	for(ign_tick = 0;ign_tick < 200;ign_tick ++)
	{
		updateADC();
		if(ADCCur > i_target)
			break;
		analogStart();
	}
	//GATES_OFF;

	if(ADCCur < i_target){
		SPINI_ON;
		setCurrent(0);
		//ErrorTxt = "No spark";
		sprintf(ErrorTxtBuf,"NoSpark I=%d",ADCCur);
		ErrorTxt = ErrorTxtBuf;
		NoSparkCount ++;
		PRECHARGE_ON;
		return -1;
	}//*/
	SPINI_ON;

	setCurrent(cur_mla[0]);

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

	if(done <= 0)
		NoCurrentCount ++;

	delay_us(300);

	PRECHARGE_ON;

	//initInputPin(GPIOD,PRECHARGE_RDY);

	return done;
}

#define RCSTR_PIN GPIO_Pin_8
#define RCDAT_PIN GPIO_Pin_7
#define RCCLK_PIN GPIO_Pin_6

#define RCSTR_OFF outPin(GPIOC,RCSTR_PIN,RCSTR_PIN)    //#define RCSTR_ON outPin(GPIOC,RCSTR_PIN,RCSTR_PIN)
#define RCSTR_ON outPin(GPIOC,RCSTR_PIN,0)           //#define RCSTR_OFF outPin(GPIOC,RCSTR_PIN,0)
#define RCDAT_OFF outPin(GPIOC,RCDAT_PIN,RCDAT_PIN)    //#define RCDAT_ON outPin(GPIOC,RCDAT_PIN,RCDAT_PIN)
#define RCDAT_ON outPin(GPIOC,RCDAT_PIN,0)           //#define RCDAT_OFF outPin(GPIOC,RCDAT_PIN,0)
#define RCCLK_OFF outPin(GPIOC,RCCLK_PIN,RCCLK_PIN)    //#define RCCLK_ON outPin(GPIOC,RCCLK_PIN,RCCLK_PIN)
#define RCCLK_ON outPin(GPIOC,RCCLK_PIN,0)           //#define RCCLK_OFF outPin(GPIOC,RCCLK_PIN,0)

void setRelayN(int val){
	int i;

	RCSTR_ON;
	RCDAT_ON;
	RCCLK_ON;

	GPIO_InitStructure.GPIO_Pin = RCSTR_PIN | RCDAT_PIN| RCCLK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	for(i = 0;i<8;i++){
		if(val&1)
			RCDAT_OFF;
		else
			RCDAT_ON;
		delay_ms(2);
		RCCLK_OFF;
		delay_ms(2);
		RCCLK_ON;
		val >>= 1;
	}

	RCSTR_OFF;
	delay_ms(2);

	RCSTR_ON;
	RCDAT_ON;
	RCCLK_ON;
}

