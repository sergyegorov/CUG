#include <DSP2802x_Device.h>
//#include <DSP2802x_SysCtrl.h>
#include <DSP2802x_EPwm_defines.h>
#include "spkey.h"

/*inline void CheckSKey()
{
	int i = 0;

	SLIM = 1;
	for(;i<100;i++)
		;
	SLIM = 0;

	while(SCLF == 1)
	{
		SKEY = 0;
		for(;i<100;i++)
			;
		SKEY = 1;
		for(;i<100;i++)
			;
	}
	SLIM = 0;
}

inline void CheckPKey()
{
	int i = 0;

	PLIM = 1;
	while(PCLF == 1)
	{
		PKEY = 0;
		for(;i<100;i++)
			;
		PKEY = 1;
		for(;i<100;i++)
			;
	}
	PLIM = 0;
}*/

void SetupKeyControl()
{
	EALLOW;
	// SKEY
	//SKEY = 1;//
	GpioDataRegs.GPADAT.bit.GPIO0 = 1;
   	GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;   // GPIO0 = output
	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;   // Enable pullup on GPIO0
   	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;  // GPIO0 = GPIO0

	// PKEY
	//PKEY = 1;//
	GpioDataRegs.GPADAT.bit.GPIO1 = 0;
   	GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;   // GPIO1 = output
	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;   // Enable pullup on GPIO1
   	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;  // GPIO1 = GPIO1

	// SLIM1
	SLIM = 0;
	GpioDataRegs.GPADAT.bit.GPIO3 = 0;
   	GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;   // GPIO3 = output
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;   // Enable pullup on GPIO1
   	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;  // GPIO3 = GPIO3

	// PLIM1
	PLIM = 0;
	GpioDataRegs.GPADAT.bit.GPIO2 = 0;
   	GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;   // GPIO2 = output
	GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;   // Enable pullup on GPIO1
   	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;  // GPIO2 = GPIO2

	// SCLF1
	GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;   // Enable pullup on GPIO1
   	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;  // GPIO28 = GPIO28
   	GpioCtrlRegs.GPADIR.bit.GPIO28 = 0;   // GPIO28 = input

	// PCLF1
	GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;   // Enable pullup on GPIO1
   	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;  // GPIO12 = GPIO12
   	GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;   // GPIO12 = input

	EDIS;

	//CheckPKey();
	//CheckSKey();
}

//#pragma CODE_SECTION(PulsSKey, "ramfuncs");

/*char PulsSKey(Uint32 val)
{
	int i,j;
	char ret;
	//CheckSKey();
	SKEY = 0;//GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;
	//for(i = 0;i<val && SCLF == 1;i++)
	for(i = 0;i<val;i++)
		for(j = 0;j<4;j++)
		;
	if(SCLF == 1)
		ret = OVER_CUR;	
	else
		ret = UNDER_CUR;
	SKEY = 1;//GpioDataRegs.GPASET.bit.GPIO0 = 1;
	return ret;
}

void PulsSMax(int time_out)
{
    int i;
	SKEY = 0;//GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;
	for(i = 0;i<time_out && SCLF != 1;i++)
	;     
}


char PulsPKey(Uint32 val)
{
	int i,j;
	char ret;
	//CheckSKey();
	PKEY = 0;//GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;
	for(i = 0;i<val;i++)
		//for(j = 0;j<4;j++)
			;
	if(PCLF == 1)
		ret = OVER_CUR;	
	else
		ret = UNDER_CUR;
	PKEY = 1;//GpioDataRegs.GPASET.bit.GPIO0 = 1;
	return ret;
}*/

/*void SetPKey(char val)
{
	if(val == 1)
		PKEY = 0;
	else
		PKEY = 1;
	//PKEY = val;
}

void SetSKey(char val)
{
	if(val == 1)
		SKEY = 0;
	else
		SKEY = 1;
	//SKEY = val;
}//*/

void TurnOffKeys()
{
	SKEY_OFF;
	PKEY_OFF;
	//SKEY = 1;
	//PKEY = 1;
}

/*void SetupKeyControl()
{
   SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;     // EPWM1
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;      // Enable TBCLK within the EPWM


   	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;    // Disable pull-up on GPIO1 (EPWM1B)

    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B
   
      // Setup TBCLK
   EPwm1Regs.TBPRD = 0xFFFF;           // Set timer period 801 TBCLKs
   EPwm1Regs.TBPHS.half.TBPHS = 0x0000;           // Phase is 0

   // Set Compare values
   //EPwm1Regs.CMPA.half.CMPA = 10;//EPWM1_MIN_CMPA;     // Set compare A value
   EPwm1Regs.CMPCTL.bit.SHDWBFULL = CC_IMMEDIATE;
   EPwm1Regs.CMPCTL.bit.SHDWAFULL = CC_IMMEDIATE;
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_IMMEDIATE;
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;

   EPwm1Regs.CMPA.half.CMPA = 1;//EPWM1_MIN_CMPA;     // Set compare A value
   EPwm1Regs.CMPB = 100;               // Set Compare B value

   // Setup counter mode
   EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;   //EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
   EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
   EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;
   //EPwm1Regs.TBCTL.bit.SYNCOSEL = 2;	//----------

   // Setup shadowing
   EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;//CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_IMMEDIATE;//CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;  // Load on Zero
   EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   // Set actions
   EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;

   EPwm1Regs.AQCTLA.bit.CAU = AQ_SET; // in working mode must to be AQ_CLEAR
   EPwm1Regs.AQCTLA.bit.CBU = AQ_SET;

   EPwm1Regs.AQCTLA.bit.CBD = AQ_SET;
   EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;
   EPwm1Regs.AQCTLA.bit.PRD = AQ_SET;


   EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;

   EPwm1Regs.AQCTLB.bit.CAU = AQ_SET; // in working mode must to be AQ_CLEAR
   EPwm1Regs.AQCTLB.bit.CBU = AQ_SET;

   EPwm1Regs.AQCTLB.bit.CBD = AQ_SET;
   EPwm1Regs.AQCTLB.bit.CAD = AQ_SET;
   EPwm1Regs.AQCTLB.bit.PRD = AQ_SET;

   EPwm1Regs.TBCTR = 0x0;   //EPwm1Regs.TBCTR = 0x0000;                      // Clear counter
}

void PulsSKey(Uint32 val)
{
    EPwm1Regs.CMPB = val+1;               // Set Compare B value
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;             // Set PWM1A on event A, up count
	EPwm1Regs.TBCTR = 0x0;
	//EPwm1Regs.TBCTL.bit.SWFSYNC = 1;
}

void PulsSKeyAgain()
{
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;             // Set PWM1A on event A, up count
	EPwm1Regs.TBCTR = 0x0;
}

void PulsPKey(Uint32 val)
{
    EPwm1Regs.CMPB = val+1;               // Set Compare B value
    EPwm1Regs.AQCTLB.bit.CAU = AQ_CLEAR;             // Set PWM1A on event A, up count
	EPwm1Regs.TBCTR = 0x0;
}

void PulsPKeyAgain()
{
    EPwm1Regs.AQCTLB.bit.CAU = AQ_CLEAR;             // Set PWM1A on event A, up count
	EPwm1Regs.TBCTR = 0x0;
}

int IsKeyPulsFinished()
{
	int ret;
	if(EPwm1Regs.TBCTR > EPwm1Regs.CMPB)
	{
		ret = 1;
	    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;             // Set PWM1A on event A, up count
    	EPwm1Regs.AQCTLB.bit.CAU = AQ_SET;             // Set PWM1A on event A, up count
	}
	else
		ret = 0;
	return ret;
}

void TurnOffKeys()
{
    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;             // Set PWM1A on event A, up count
   	EPwm1Regs.AQCTLB.bit.CAU = AQ_SET;             // Set PWM1A on event A, up count
}*/
