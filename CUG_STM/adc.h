#ifndef ADC_H
#define ADC_H

extern void initADC();
extern int ADCInput,ADCCur,ADCOut;
extern void updateADC();
extern void analogStart();

#define updateAndStartADC \
	while(!(ADC1->SR & ADC_FLAG_EOC));\
	while(!(ADC2->SR & ADC_FLAG_EOC));\
	while(!(ADC3->SR & ADC_FLAG_EOC));\
	ADCInput = (uint16_t) ADC1->DR;\
	ADCCur = (uint16_t) ADC2->DR;\
	ADCOut = (uint16_t) ADC3->DR;\
	ADC1->SR = 0;\
	ADC2->SR = 0;\
	ADC3->SR = 0;\
	&(ADC1->CR2)|=(uint32_t)ADC_CR2_SWSTART;\
	&(ADC2->CR2)|=(uint32_t)ADC_CR2_SWSTART;\
	&(ADC3->CR2)|=(uint32_t)ADC_CR2_SWSTART;\

#endif
