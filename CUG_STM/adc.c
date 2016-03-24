#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include "stm32f4xx_adc.h"

GPIO_InitTypeDef  GPIO_InitStructure;

int Delay(__IO uint32_t nCount);

int ConvertedValue = 0; //Converted value readed from ADC

#define TIME_STEP 1000000

void analogStart(){
	int *val1 = &(ADC1->CR2);// |= (uint32_t)ADC_CR2_SWSTART;//ADC_SoftwareStartConv(ADC1);//Start the conversion
	int *val2 = &(ADC2->CR2);
	int *val3 = &(ADC3->CR2);
	int src1 = ADC1->CR2 | (uint32_t)ADC_CR2_SWSTART;
	int src2 = ADC2->CR2 | (uint32_t)ADC_CR2_SWSTART;
	int src3 = ADC3->CR2 | (uint32_t)ADC_CR2_SWSTART;
	*val1 = src1;
	*val2 = src2;
	*val3 = src3;
}

void initADC(){
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
 GPIO_initStructre.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;//The channel 10 is connected to PC0
 GPIO_initStructre.GPIO_Mode = GPIO_Mode_AN; //The PC0 pin is configured in analog mode
 GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
 GPIO_Init(GPIOC,&GPIO_initStructre);//Affecting the port with the initialization structure configuration
 //ADC structure configuration
 ADC_DeInit();

 ADC_CommonInitTypeDef adc_common;
 adc_common.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
 adc_common.ADC_Mode = ADC_AnalogWatchdog_SingleRegEnable;
 adc_common.ADC_Prescaler = ADC_Prescaler_Div2;
 adc_common.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
 ADC_CommonInit(&adc_common);

 ADC_init_structure.ADC_DataAlign = ADC_DataAlign_Right;//data converted will be shifted to right
 ADC_init_structure.ADC_Resolution = ADC_Resolution_10b;//Input voltage is converted into a 12bit number giving a maximum value of 4096
 ADC_init_structure.ADC_ContinuousConvMode = DISABLE;//ENABLE; //the conversion is continuous, the input data is converted more than once
 ADC_init_structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;// conversion is synchronous with TIM1 and CC1 (actually I'm not sure about this one :/)
 ADC_init_structure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//no trigger for conversion
 ADC_init_structure.ADC_NbrOfConversion = 1;//I think this one is clear :p
 ADC_init_structure.ADC_ScanConvMode = DISABLE;//The scan is configured in one channel
 ADC_Init(ADC1,&ADC_init_structure);//Initialize ADC with the previous configuration
 ADC_Init(ADC2,&ADC_init_structure);
 ADC_Init(ADC3,&ADC_init_structure);
 //Enable ADC conversion
 ADC_Cmd(ADC1,ENABLE); //AN1 PC0 ADC1_AIN10 ADC2_AIN10 ADC3_AIN10
 ADC_Cmd(ADC2,ENABLE); //AN2 PC1 ADC1_AIN11 ADC2_AIN11 ADC3_AIN11
 ADC_Cmd(ADC3,ENABLE); //AN3 PC2 ADC1_AIN12 ADC2_AIN12 ADC3_AIN12
 //Select the channel to be read from
 ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_3Cycles);//ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_3Cycles);
 ADC_RegularChannelConfig(ADC2,ADC_Channel_11,1,ADC_SampleTime_3Cycles);//ADC_RegularChannelConfig(ADC2,ADC_Channel_11,1,ADC_SampleTime_3Cycles);
 ADC_RegularChannelConfig(ADC3,ADC_Channel_12,1,ADC_SampleTime_3Cycles);//ADC_RegularChannelConfig(ADC3,ADC_Channel_12,1,ADC_SampleTime_3Cycles);

 analogStart();
}

int ADCInput,ADCCur,ADCOut;
void updateADC(){
	//int ret = 0;
	while(!(ADC1->SR & ADC_FLAG_EOC));
	while(!(ADC2->SR & ADC_FLAG_EOC));
	while(!(ADC3->SR & ADC_FLAG_EOC));

	ADCInput = (uint16_t) ADC1->DR;//ADC_GetConversionValue(ADC1); //Return the converted data
	ADCCur = (uint16_t) ADC2->DR;
	ADCOut = (uint16_t) ADC3->DR;

	ADC1->SR = 0;
	ADC2->SR = 0;
	ADC3->SR = 0;

	//analogStart();
}
