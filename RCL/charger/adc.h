#ifndef ADC_H	
#define ADC_H

//ifb
#define ADC_I_Start() AdcRegs.ADCSOCFRC1.bit.SOC2 = 1;\
                     while(AdcRegs.ADCINTFLG.bit.ADCINT3 == 0){}\
                     AdcRegs.ADCINTFLGCLR.bit.ADCINT3 = 1;
#define ADC_I_Result AdcResult.ADCRESULT2

//usfb
#define ADC_UIn_Start() AdcRegs.ADCSOCFRC1.bit.SOC1 = 1;\
                     while(AdcRegs.ADCINTFLG.bit.ADCINT2 == 0){}\
                     AdcRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;
#define ADC_UIn_Result AdcResult.ADCRESULT1

//ufb
#define ADC_UOut_Start() AdcRegs.ADCSOCFRC1.bit.SOC0 = 1;\
                     while(AdcRegs.ADCINTFLG.bit.ADCINT1 == 0){}\
                     AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
#define ADC_UOut_Result AdcResult.ADCRESULT0

extern void InitAdc();

#endif
