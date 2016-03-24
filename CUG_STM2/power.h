#ifndef POWER_H
#define POWER_H
#include "common.h"
#include "adc.h"

#define MAX_CURRENT 54

#define KEY1_PORT GPIOC
#define KEY1_PIN GPIO_Pin_4
#define KEY2_PORT GPIOB
#define KEY2_PIN GPIO_Pin_1
#define KEY3_PORT GPIOA
#define KEY3_PIN GPIO_Pin_7
#define KEY4_PORT GPIOB
#define KEY4_PIN GPIO_Pin_0
#define KEY5_PORT GPIOA
#define KEY5_PIN GPIO_Pin_6
#define KEY6_PORT GPIOC
#define KEY6_PIN GPIO_Pin_5
#define KEY7_PORT GPIOA
#define KEY7_PIN GPIO_Pin_4
#define KEY8_PORT GPIOA
#define KEY8_PIN GPIO_Pin_5

#define BOST_EN_PORT GPIOC
#define BOST_EN_PIN GPIO_Pin_1
#define BOST_RDY_PORT GPIOC
#define BOST_RDY_PIN GPIO_Pin_2

#define ADC_UIN 	ADC_Channel_1
#define ADC_I   	ADC_Channel_2
#define ADC_UOUT 	ADC_Channel_3

class PowerBoard{
private:
	void initGPIO(){
		initGPIOBitOut(KEY1_PORT,KEY1_PIN);
		initGPIOBitOut(KEY2_PORT,KEY2_PIN);
		initGPIOBitOut(KEY3_PORT,KEY3_PIN);
		initGPIOBitOut(KEY4_PORT,KEY4_PIN);
		initGPIOBitOut(KEY5_PORT,KEY5_PIN);
		initGPIOBitOut(KEY6_PORT,KEY6_PIN);
		initGPIOBitOut(KEY7_PORT,KEY7_PIN);
		initGPIOBitOut(KEY8_PORT,KEY8_PIN);

		initGPIOBitOut(BOST_EN_PORT,BOST_EN_PIN);
	}
	static ADC Adc1;
	static ADC Adc2;
public:
	PowerBoard(){
		initGPIO();
		Adc1.init(ADC1);
		Adc2.init(ADC2);
		Adc2.readADC(ADC_I);
	}

	void setCurrent(int val){
		int currents[] = {8,8,8,8,4,2,8,8};
		//int index = 0;
		if(val >= currents[0]){
			GPIO_ResetBits(KEY1_PORT,KEY1_PIN);
			val -= currents[0];
		} else	GPIO_SetBits(KEY1_PORT,KEY1_PIN);
		if(val >= currents[1]){
			GPIO_ResetBits(KEY2_PORT,KEY2_PIN);
			val -= currents[1];
		} else	GPIO_SetBits(KEY2_PORT,KEY2_PIN);
		if(val >= currents[2]){
			GPIO_ResetBits(KEY3_PORT,KEY3_PIN);
			val -= currents[2];
		} else	GPIO_SetBits(KEY3_PORT,KEY3_PIN);
		if(val >= currents[3]){
			GPIO_ResetBits(KEY4_PORT,KEY4_PIN);
			val -= currents[3];
		} else	GPIO_SetBits(KEY4_PORT,KEY4_PIN);
		if(val >= currents[6]){
			GPIO_SetBits(KEY7_PORT,KEY7_PIN);
			val -= currents[6];
		} else	GPIO_ResetBits(KEY7_PORT,KEY7_PIN);
		if(val >= currents[7]){
			GPIO_SetBits(KEY8_PORT,KEY8_PIN);
			val -= currents[7];
		} else	GPIO_ResetBits(KEY8_PORT,KEY8_PIN);
		if(val >= currents[4]){
			GPIO_ResetBits(KEY5_PORT,KEY5_PIN);
			val -= currents[4];
		} else	GPIO_SetBits(KEY5_PORT,KEY5_PIN);
		if(val >= currents[5]){
			GPIO_ResetBits(KEY6_PORT,KEY6_PIN);
			val -= currents[5];
		} else	GPIO_SetBits(KEY6_PORT,KEY6_PIN);
	}

	int getOutputU(){
		return Adc1.readADC(ADC_UOUT);
	}

	int getInputU(){
		return Adc1.readADC(ADC_UIN);
	}

	int getI(){
		return Adc2.readADC(ADC_I);
	}

	void setBuster(bool flag){
		if(flag)
			GPIO_SetBits(BOST_EN_PORT,BOST_EN_PIN);
		else
			GPIO_ResetBits(BOST_EN_PORT,BOST_EN_PIN);
	}

	bool isBusterRedy(){
		return GPIO_ReadInputDataBit(BOST_RDY_PORT, BOST_RDY_PIN) == 0;
	}
};

extern PowerBoard *PBoard;

#endif
