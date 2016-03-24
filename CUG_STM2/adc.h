#ifndef ADC_H
#define ADC_H
#include "common.h"
#include <stm32f10x_rcc.h>
#include "stm32f10x_adc.h"

class ADC{
	ADC_TypeDef* ADCx;
public:
	ADC(){
	}

	/*
	 * ADC *adc1 = new ADC(ADC1);
	 * ADC *adc2 = new ADC(ADC2);
	 */
	void init(ADC_TypeDef* adcx){
		this->ADCx = adcx;

		ADC_InitTypeDef  ADC_InitStructure;
		/* PCLK2 is the APB2 clock */
		/* ADCCLK = PCLK2/6 = 72/6 = 12MHz*/
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);

		/* Enable ADCx clock so that we can talk to it */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
		/* Put everything back to power-on defaults */
		ADC_DeInit(ADCx);

		/* ADCx Configuration ------------------------------------------------------*/
		/* ADCx and ADC2 operate independently */
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		/* Disable the scan conversion so we do one at a time */
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		/* Don't do contimuous conversions - do them on demand */
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
		/* Start conversin by software, not an external trigger */
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		/* Conversions are 12 bit - put them in the lower 12 bits of the result */
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		/* Say how many channels would be used by the sequencer */
		ADC_InitStructure.ADC_NbrOfChannel = 1;

		  /* Now do the setup */
		ADC_Init(ADCx, &ADC_InitStructure);
		  /* Enable ADCx */
		ADC_Cmd(ADCx, ENABLE);

		  /* Enable ADCx reset calibaration register */
		ADC_ResetCalibration(ADCx);
		  /* Check the end of ADCx reset calibration register */
		while(ADC_GetResetCalibrationStatus(ADCx));
		  /* Start ADCx calibaration */
		ADC_StartCalibration(ADCx);
		  /* Check the end of ADCx calibration */
		while(ADC_GetCalibrationStatus(ADCx));
	}

	u16 readADC(u8 channel)
	{
		ADC_RegularChannelConfig(ADCx, channel, 1, ADC_SampleTime_1Cycles5);
		// Start the conversion
		ADC_SoftwareStartConvCmd(ADCx, ENABLE);
		// Wait until conversion completion
		while(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);
		// Get the conversion value
		return ADC_GetConversionValue(ADCx);
	}

	u16 readADC()
	{
		// Start the conversion
		ADC_SoftwareStartConvCmd(ADCx, ENABLE);
		// Wait until conversion completion
		while(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);
		// Get the conversion value
		return ADC_GetConversionValue(ADCx);
	}
};
#endif
