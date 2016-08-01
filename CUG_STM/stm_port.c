#include "stm_port.h"

void initOutputPin(GPIO_TypeDef*  port,int pin){
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(port, &GPIO_InitStructure);
}

void initInputPin(GPIO_TypeDef*  port,int pin){
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(port, &GPIO_InitStructure);
}

void outPin(GPIO_TypeDef* port,int mask,int val){
	GPIO_SetBits(port, mask & val);
	val = 0xFFFF ^ val;
	GPIO_ResetBits(port, mask & val);
}

int getPin(GPIO_TypeDef*  port,int mask){
	return GPIO_ReadInputDataBit(port,mask);
}

/*void initADC(){

	ADC_InitTypeDef ADC_init_structure; //Structure for adc confguration
	GPIO_InitTypeDef GPIO_initStructre; //Structure for analog input pin
	//Clock configuration
	//RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_ClocksTypeDef clk;
	RCC_GetClocksFreq(&clk);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//The ADC1 is connected the APB2 peripheral bus thus we will use its clock source
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);//The ADC1 is connected the APB2 peripheral bus thus we will use its clock source
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);//The ADC1 is connected the APB2 peripheral bus thus we will use its clock source

	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);//Clock for the ADC port!! Do not forget about this one ;)

	//Analog pin configuration
	GPIO_initStructre.GPIO_Pin = GPIO_Pin_5;//The channel 10 is connected to PC0
	GPIO_initStructre.GPIO_Mode = GPIO_Mode_AN; //The PC0 pin is configured in analog mode
	GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
	GPIO_Init(GPIOC,&GPIO_initStructre);//Affecting the port with the initialization structure configuration
	//----------------------------------------
	GPIO_initStructre.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;//The channel 10 is connected to PC0
	GPIO_initStructre.GPIO_Mode = GPIO_Mode_AN; //The PC0 pin is configured in analog mode
	GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
	GPIO_Init(GPIOB,&GPIO_initStructre);//Affecting the port with the initialization structure configuration

	//ADC structure configuration
	ADC_DeInit();

	ADC_CommonInitTypeDef adc_common;
	adc_common.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	adc_common.ADC_Mode = ADC_AnalogWatchdog_SingleRegEnable;
	adc_common.ADC_Prescaler = ADC_Prescaler_Div2;
	adc_common.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&adc_common);

	ADC_init_structure.ADC_DataAlign = ADC_DataAlign_Right;//data converted will be shifted to right
	ADC_init_structure.ADC_Resolution = ADC_Resolution_12b;//Input voltage is converted into a 12bit number giving a maximum value of 4096
	ADC_init_structure.ADC_ContinuousConvMode = ENABLE; //the conversion is continuous, the input data is converted more than once
	ADC_init_structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;// conversion is synchronous with TIM1 and CC1 (actually I'm not sure about this one :/)
	ADC_init_structure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//no trigger for conversion
	ADC_init_structure.ADC_NbrOfConversion = 1;//I think this one is clear :p
	ADC_init_structure.ADC_ScanConvMode = DISABLE;//The scan is configured in one channel
	ADC_Init(ADC1,&ADC_init_structure);//Initialize ADC with the previous configuration
	ADC_Init(ADC2,&ADC_init_structure);
	ADC_Init(ADC3,&ADC_init_structure);
	//Enable ADC conversion
	ADC_Cmd(ADC1,ENABLE);
	ADC_Cmd(ADC2,ENABLE);
	ADC_Cmd(ADC3,ENABLE);
	//Select the channel to be read from
	//ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC2,ADC_Channel_2,1,ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC3,ADC_Channel_3,1,ADC_SampleTime_3Cycles);

	assert_param(IS_ADC_ALL_PERIPH(ADC1));
	assert_param(IS_ADC_ALL_PERIPH(ADC2));
	assert_param(IS_ADC_ALL_PERIPH(ADC3));
	ADC1->CR2 |= (uint32_t)ADC_CR2_SWSTART;
	ADC2->CR2 |= (uint32_t)ADC_CR2_SWSTART;
	ADC3->CR2 |= (uint32_t)ADC_CR2_SWSTART;
}*/

