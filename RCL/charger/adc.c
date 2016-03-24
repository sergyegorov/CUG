#include "adc.h"
#include <DSP2802x_Device.h>

#ifndef Device_cal
#define Device_cal (void   (*)(void))0x3D7C80
#endif

void InitADC()
{
	int16 i;

	EALLOW;

	AdcRegs.ADCCTL1.bit.RESET = 1;

	for(i = 0;i<2;i++){}

    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1; // Enable ADC peripheral clock
    (*Device_cal)();
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 0; // Return ADC clock to original state

    SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 0;
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL=0;  // Clk Src = INTOSC1
    SysCtrlRegs.CLKCTL.bit.XCLKINOFF=1;     // Turn off XCLKIN
    SysCtrlRegs.CLKCTL.bit.XTALOSCOFF=1;    // Turn off XTALOSC
    SysCtrlRegs.CLKCTL.bit.INTOSC2OFF=1;    // Turn off INTOSC2

    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;       // ADC

    AdcRegs.ADCCTL1.bit.ADCBGPWD  = 1;      // Power ADC BG
    AdcRegs.ADCCTL1.bit.ADCREFPWD = 1;      // Power reference
    AdcRegs.ADCCTL1.bit.ADCPWDN   = 1;      // Power ADC
    AdcRegs.ADCCTL1.bit.ADCENABLE = 1;      // Enable ADC
    AdcRegs.ADCCTL1.bit.ADCREFSEL = 0;      // Select interal BG

	for(i = 0;i<100;i++){}

    AdcRegs.ADCSOC0CTL.bit.CHSEL  = 2; //Set SOC0 channel select to ADCINA2
    AdcRegs.ADCSOC1CTL.bit.CHSEL  = 4; //Set SOC1 channel select to ADCINA4
    AdcRegs.ADCSOC2CTL.bit.CHSEL  = 6; //Set SOC2 channel select to ADCINA6
	AdcRegs.ADCSOC3CTL.bit.CHSEL  = 2;	// Set ADCINA6 and ADCINA2

    AdcRegs.ADCSOC0CTL.bit.ACQPS  = 6; //Set SOC0 acquisition period to 7 ADCCLK
	AdcRegs.ADCSOC1CTL.bit.ACQPS  = 6; //Set SOC1 acquisition period to 7 ADCCLK
	AdcRegs.ADCSOC2CTL.bit.ACQPS  = 6; //Set SOC2 acquisition period to 7 ADCCLK
	AdcRegs.ADCSOC3CTL.bit.ACQPS  = 6; //Set SOC2 acquisition period to 7 ADCCLK

	AdcRegs.ADCSOC0CTL.bit.TRIGSEL = 0;
	AdcRegs.ADCSOC1CTL.bit.TRIGSEL = 0;
	AdcRegs.ADCSOC2CTL.bit.TRIGSEL = 0;
	AdcRegs.ADCSOC3CTL.bit.TRIGSEL = 0;


    AdcRegs.INTSEL1N2.bit.INT1SEL = 0; //Connect ADCINT1 to SOC0
    AdcRegs.INTSEL1N2.bit.INT1E  =  1; //Enable ADCINT1
	AdcRegs.INTSEL1N2.bit.INT1CONT = 0;

    AdcRegs.INTSEL1N2.bit.INT2SEL = 1; //Connect ADCINT1 to SOC1
    AdcRegs.INTSEL1N2.bit.INT2E  =  1; //Enable ADCINT1
	AdcRegs.INTSEL1N2.bit.INT2CONT = 0;

    AdcRegs.INTSEL3N4.bit.INT3SEL = 2; //Connect ADCINT1 to SOC2
    AdcRegs.INTSEL3N4.bit.INT3E  =  1; //Enable ADCINT1
	AdcRegs.INTSEL3N4.bit.INT3CONT = 0;

	AdcRegs.INTSEL3N4.bit.INT4SEL = 3; //Connect ADCINT1 to SOC3
    AdcRegs.INTSEL3N4.bit.INT4E  =  1; //Enable ADCINT1
	AdcRegs.INTSEL3N4.bit.INT4CONT = 0;

    // Set the flash OTP wait-states to minimum. This is important
    // for the performance of the temperature conversion function.
    FlashRegs.FOTPWAIT.bit.OTPWAIT = 1;
}


