#include "DSP2802x_Device.h"
/*#include <DSP2802x_SysCtrl.h>
#include <DSP2802x_EPwm_defines.h>
#include <DSP2802x_GlobalPrototypes.h>*/
#include "DSP28x_Project.h"
#include "spkey.h"
#include "adc.h"
#include "DSP28x_Project.h"
#include "inoutsignal.h"
#include "../main/main/spi_command.h"
#include <math.h>

#ifndef Device_cal
#define Device_cal (void   (*)(void))0x3D7C80
#endif

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;


#pragma CODE_SECTION(Kardebalet, "ramfuncs");
#pragma CODE_SECTION(InitFlash, "ramfuncs");
#pragma CODE_SECTION(wait_mk, "ramfuncs");
#pragma CODE_SECTION(TestADC, "ramfuncs");
#pragma CODE_SECTION(Start, "ramfuncs");

int wait_tmp;
void wait_mk(int mks)
{
	int i;
	for(i = 0;i<mks;i++)
	{
		wait_tmp ++;
		wait_tmp ++;
		wait_tmp ++;
		wait_tmp ++;
		wait_tmp ++;
		wait_tmp ++;
		wait_tmp ++;
		wait_tmp ++;
		wait_tmp ++;
		wait_tmp ++;
		wait_tmp ++;
		wait_tmp ++;
		wait_tmp ++;
		wait_tmp ++;
		wait_tmp ++;
	}
}

/*void TestSKeyAndPKey()
{
	PulsSKey(30);
	while(IsKeyPulsFinished() == 0);
	//wait_mk(1);
	PulsSKey(10);
	while(IsKeyPulsFinished() == 0);
	//wait_mk(1);
	PulsSKey(20);
	while(IsKeyPulsFinished() == 0);
	PulsSKeyAgain();
	while(IsKeyPulsFinished() == 0);
	//wait_mk(1);
	TurnOffKeys();

	PulsPKey(30);
	while(IsKeyPulsFinished() == 0);
	//wait_mk(1);
	PulsPKey(10);
	while(IsKeyPulsFinished() == 0);
	//wait_mk(1);
	PulsPKey(20);
	while(IsKeyPulsFinished() == 0);
	PulsPKeyAgain();
	//wait_mk(1);
	TurnOffKeys();
}*/


int temp;


void TestADC()
{
	//int i,j;
    for(;;)
    {
		//FRDY_ON;
		ADC_UOut_Start();
		temp = ADC_UOut_Result;
		//FRDY_OFF;
		ADC_I_Start();
		temp = ADC_I_Result;
    }
}

void InitPll(Uint16 val, Uint16 divsel)
{
	int i;
   volatile Uint16 iVol;

	SysCtrlRegs.PLLSTS.bit.DIVSEL = 1; 
	SysCtrlRegs.PLLSTS.bit.PLLLOCKS = 1;

   // Make sure the PLL is not running in limp mode
   if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0)
   {
      SysCtrlRegs.PLLSTS.bit.MCLKCLR = 1;
      asm("        ESTOP0");     // Uncomment for debugging purposes
   }

   // DIVSEL MUST be 0 before PLLCR can be changed from
   // 0x0000. It is set to 0 by an external reset XRSn
   // This puts us in 1/4
   if (SysCtrlRegs.PLLSTS.bit.DIVSEL != 0)
       SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;

   // Change the PLLCR
   if (SysCtrlRegs.PLLCR.bit.DIV != val)
   {
	  SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
      SysCtrlRegs.PLLCR.bit.DIV = val;
      while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1)
      {
          // Uncomment to service the watchdog
          // ServiceDog();
      }
      SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
    }

    if((divsel == 1)||(divsel == 2))
        SysCtrlRegs.PLLSTS.bit.DIVSEL = divsel;

    if(divsel == 3)
    {
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 2;
		for(i = 0;i<1000;i++);
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 3;
    }
}

void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
       *DestAddr++ = *SourceAddr++;
    return;
}

void InitFlash(void)
{
   EALLOW;
   FlashRegs.FOPT.bit.ENPIPE = 1;
  #if (CPU_FRQ_60MHZ)
   FlashRegs.FBANKWAIT.bit.PAGEWAIT = 2;//Set the Paged Waitstate for the Flash
   FlashRegs.FBANKWAIT.bit.RANDWAIT = 2;//Set the Random Waitstate for the Flash
   FlashRegs.FOTPWAIT.bit.OTPWAIT = 2;//Set the Waitstate for the OTP
  #elif (CPU_FRQ_40MHZ)
   FlashRegs.FBANKWAIT.bit.PAGEWAIT = 1;
   FlashRegs.FBANKWAIT.bit.RANDWAIT = 1;
   FlashRegs.FOTPWAIT.bit.OTPWAIT = 1;
  #endif

   FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;
   FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;
   EDIS;

   asm(" RPT #7 || NOP");//*/
   	/*FlashRegs.FOPT.bit.ENPIPE = 1;

	FlashRegs.FBANKWAIT.bit.PAGEWAIT = 3; //Set the Paged Waitstate for the Flash
	FlashRegs.FBANKWAIT.bit.RANDWAIT = 3; //Set the Random Waitstate for the Flash
	FlashRegs.FOTPWAIT.bit.OTPWAIT = 5; //Set the Waitstate for the OTP

	FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;
	FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;

	asm(" RPT #7 || NOP");//*/
}


void Kardebalet()
{
	EALLOW;
	DINT;
	IER = 0x0000;
   	IFR = 0x0000;

	for(;;)
	{
		//FRDY_ON;
		//*
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");//*/
		//FRDY_OFF;
		//*
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");
		asm("   NOP");//*/
	}
}

//#define CSPIN_Get   GpioDataRegs.GPADAT.bit.GPIO4
void Init(void)
{
	//int i = 0;
    DINT;
    EALLOW;
	//-------------------- disable watchdog
    SysCtrlRegs.WDCR= 0x0068;

	wait_tmp ++;

	SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
	(*Device_cal)();
	SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 0;

	/*SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 0;
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL=0;  // Clk Src = INTOSC1

	///////////////////
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;  // GPIO18 = XCLKOUT
	GpioCtrlRegs.GPADIR.bit.GPIO18 = 1;
   	SysCtrlRegs.LOSPCP.all = 0x0000;

	// XCLKOUT to SYSCLKOUT ratio.  By default XCLKOUT = 1/4 SYSCLKOUT
	SysCtrlRegs.XCLK.bit.XCLKOUTDIV=2; // Set XCLKOUT = SYSCLKOUT/1 */

// XCLKOUT to SYSCLKOUT ratio.  By default XCLKOUT = 1/4 SYSCLKOUT
   SysCtrlRegs.XCLK.bit.XCLKOUTDIV=2; // Set XCLKOUT = SYSCLKOUT/1
	//----------------- init clocks
	// LOSPCP prescale register settings, normally it will be set to default values
   /*GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;  // GPIO18 = XCLKOUT
   SysCtrlRegs.LOSPCP.all = 0x0002;

// XCLKOUT to SYSCLKOUT ratio.  By default XCLKOUT = 1/4 SYSCLKOUT
   SysCtrlRegs.XCLK.bit.XCLKOUTDIV=2; // Set XCLKOUT = SYSCLKOUT/1

// This function is not written to be an example of efficient code.
   SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;       // ADC
   SysCtrlRegs.PCLKCR3.bit.COMP1ENCLK = 1;     // COMP1
   SysCtrlRegs.PCLKCR3.bit.COMP2ENCLK = 1;     // COMP2
   SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1; // CPU Timer-0
   SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1; // CPU Timer-1
   SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 1; // CPU Timer-2
   SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 1;     // eCAP1*/
   /*SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;     // EPWM2
   SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1;     // EPWM3
   SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 1;     // EPWM4
   SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;    // GPIO
   SysCtrlRegs.PCLKCR0.bit.HRPWMENCLK=1;       // HRPWM
   SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;      // I2C*/


	//-------------------- setup maximum speed
	InitPll(8,3);

//  Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
    IFR = 0x0000;

	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
	InitFlash();

	//SetupKeyControl();
	InitADC();
	InitInOutSignal();
	SpiInit();
	//TestSKeyAndPKey();
	//TestADC();
	//Kardebalet();


    EDIS;
}

#define SCLF GpioDataRegs.GPADAT.bit.GPIO28

int UUnit,IUnit,I1Unit;
int U = 0,I = 0,I1 = 0;
int C = 0;
int Ph = 0;
#define UOutDivider 259.0F
#define ADCBaseU 3.3F
struct DebugInfo DInfo;
struct DebugInfo DInfoErrors[16];
int DInfoErrorsIndex = 0;
//int FirstStageI = 50;
int UNul,INul;
#define TIME_OUT 1000

const int FirstStageTimeOuts[] = {0,31,42,62,//3
							72,81,95,127,//7
							141,179,200,215,//11
							222,250,274,296};//15

int FirstStageTimeOut = 62;
int FirstStageTimeOutSrc = 0;
#define DOPUSK 20
#define DOPUSK_LOW 6
#define BIG_ERROR 500

unsigned long StatU = 0,StatError = 0;
unsigned long StatCount = 0;

void Ign()
{
	int i;
	CSPOUT1_ON;
	for(i = 0;i<100;i++);
	CSPOUT1_OFF;
}

#define MAX_I_HI_DLT 2400
#define MAX_I_LO_DLT 500
int MAX_I_HI,MAX_I_LO;
int result_u;
int result_in_u;
//int IgnCount = 0;
//int IgnMaxCount = 50;
#define TimeOut 600
void StartI(int time)
{
	int cur;
	if(I1Unit <= 0)
		return;

	PKEY_OFF;
	AdcRegs.ADCSOCFRC1.bit.SOC2 = 1;
	for(;time>0;time--)
	{
		//ADC_I_Start();
		while(AdcRegs.ADCINTFLG.bit.ADCINT3 == 0){}
        AdcRegs.ADCINTFLGCLR.bit.ADCINT3 = 1;
		cur = ADC_I_Result;
		AdcRegs.ADCSOCFRC1.bit.SOC2 = 1;
		if(cur > I1Unit || SCLF == 1)
		{
			if(SCLF == 1)
			{
				SKEY_OFF;
				SKEY_ON;
				DInfo.StepError ++;
			}
			SKEY_OFF;
		}
		else
		{
			SKEY_ON;
			DInfo.StepOk ++;
		}
	}
	SKEY_OFF;
	for(time = 0;time<20;time++)
		ADC_I_Start();
	PKEY_ON;
}

#define DUGA_WAIT 250
void StartDuga()
{
	int i;
	int time = 100;
	int cur;
	PKEY_OFF;
	SKEY_ON;
	//Ign();

	AdcRegs.ADCSOCFRC1.bit.SOC2 = 1;
	for(i = 0;i<DUGA_WAIT;i++)
	{
		//ADC_I_Start();
		//cur = ADC_I_Result;
		while(AdcRegs.ADCINTFLG.bit.ADCINT3 == 0){}
        AdcRegs.ADCINTFLGCLR.bit.ADCINT3 = 1;
		cur = ADC_I_Result;
		AdcRegs.ADCSOCFRC1.bit.SOC2 = 1;
		if(cur > IUnit)
			break;
		if((i&0x1F) == 0)
		{
			CSPOUT1_ON;
		}
		else
		{
			if((i&0xF) == 0)
				CSPOUT1_OFF;
		}
	}

	CSPOUT1_OFF;

	if(i == DUGA_WAIT)
	{
		DInfo.Code |= SPI_ERROR_OverTime;
	}
	else
	{
		StartI(Ph);
	}

	SKEY_OFF;
	PKEY_OFF;
	PKEY_ON;

	SpiWriteData(DInfo.Code);
	FRDY_ON;
}

void Start()
{
	int time;
	int dlt_u = 10;
	int prev_u;
	int cur;
	int cur_u_target;
	int s,i;
	int fsto_dlt = 0;
	char is_on = 0;

	//IgnCount = IgnMaxCount;

	DInfo.Code = SPI_ERROR_NONE;
	DInfo.U = 0;
	DInfo.UMust = UUnit;
	DInfo.Impuls = 0;
	DInfo.StepError = DInfo.StepOk = 0;
	DInfo.Stage = 0;

	FRDY_OFF;
	PKEY_OFF;

	if(C == 0)
	{
		StartDuga();
		return;
	}

	ADC_UOut_Start();
	prev_u = ADC_UOut_Result;

	cur_u_target = UUnit-DOPUSK*(5-C/3+3);
	//cur_u_target = UUnit-DOPUSK*3;

	ADC_UOut_Start();
	if(ADC_UOut_Result > 1000)
	{
		DInfo.Code = SPI_ERROR_NO_DISCHARGE;
		SKEY_OFF;//SetSKey(0);
		PKEY_ON;
	}
	else
	{
		SKEY_ON;
		for(time = 0;time<TimeOut;time++)
		{
			if(is_on)
				DInfo.Impuls ++;

			ADC_I_Start();
			cur = ADC_I_Result;
			if(cur > MAX_I_HI || SCLF == 1)
			{
				if(SCLF == 1)
				{
					SKEY_OFF;
					SKEY_ON;
					DInfo.StepError ++;
				}
				SKEY_OFF;//SetSKey(0);
				is_on = 0;
			}
			else
			{
				if(cur < MAX_I_LO)
				{
					SKEY_ON;
					DInfo.StepOk ++;
					is_on = 1;
				}
			}

			ADC_UOut_Start();
			if(ADC_UOut_Result > cur_u_target)
				break;
		}

		cur_u_target = UUnit-DOPUSK_LOW;
		SKEY_OFF;
		prev_u = ADC_UOut_Result;
		if(ADC_UOut_Result < cur_u_target && time < TimeOut)
		{
			DInfo.Stage ++;
			while(time < TimeOut && ADC_UOut_Result < cur_u_target)
			{
				time++;
				ADC_UOut_Start();
				if((ADC_UOut_Result-prev_u) < DOPUSK_LOW)
					break;
				prev_u = ADC_UOut_Result;
			}

			if(ADC_UOut_Result < cur_u_target && time < TimeOut)
			{
				DInfo.Stage ++;
				SKEY_ON;
				while(time<TimeOut && ADC_UOut_Result < cur_u_target)
				{
					time++;
					ADC_UOut_Start();
				}
				SKEY_OFF;
			}
		}
	}

	result_u = ADC_UOut_Result;//*/
	
	SKEY_OFF;
	PKEY_ON;
	Ign();

	StatU += result_u;
	if(UUnit > result_u)
		StatError += (UUnit - result_u);
	else
		StatError += (result_u - UUnit);
	StatCount ++;

	DInfo.U = result_u;
	DInfo.Time = time;

	if(time > TIME_OUT)
		DInfo.Code |= SPI_ERROR_OverTime;
	if(result_u < UUnit-DOPUSK)
		DInfo.Code |= SPI_ERROR_ULow;//*/
	if(result_u > UUnit+DOPUSK)
		DInfo.Code |= SPI_ERROR_UHi;

	ADC_UIn_Start();
	result_in_u = ADC_UIn_Result;

	PKEY_OFF;
	PKEY_ON;

	SpiWriteData(DInfo.Code);

	if(DInfo.Code != 0)
	{
		
	}
	FRDY_ON;
}

void CheckNull()
{
	ADC_UOut_Start();
	UNul = ADC_UOut_Result;
	ADC_I_Start();
	INul = ADC_I_Result;
}

void sl(int len)
{
	int i;
	i = 0;
	for(i = 0;i<len;i++);
}

void kardebalet()
{
	/*int i;
	while(1)
	{
		SKEY_ON;//GpioDataRegs.GPASET.bit.GPIO0 = 1;
		PKEY_ON;//GpioDataRegs.GPASET.bit.GPIO1 = 1;
		sl();
		SKEY_OFF;//GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;
		PKEY_OFF;//GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;
		sl();
	}//*/
	PKEY_OFF;
	SKEY_ON;
	sl(5);
	SKEY_OFF;
	sl(10);

	SKEY_ON;
	sl(20);
	SKEY_OFF;
	sl(10);

	SKEY_ON;
	sl(100);
	SKEY_OFF;
	sl(50);

	SKEY_ON;
	sl(1000);
	SKEY_OFF;
	while(1);
}


void main()
{
	int tmp = 0,i,c;
	int ign_count = 0;
	unsigned long stat = 0;
	int src_in_u;
	//char data = 0;                                                                                                                                                                                                        
	int idata = 0;
	int ever_count = 0;
	long ever_u = 0,ever_i = 0;
	//int ss = 300;
	//int s = sizeof(int);
	int *pointer = (int*)&DInfo;

	CSPOUT1_OFF;

	SKEY_OFF;//SetSKey(0);
	PKEY_OFF;//SetPKey(0);

	GpioDataRegs.GPADAT.bit.GPIO0 = 1;
	//char *pointer1 = (char*)&(DInfo.U);
	//int pointer_size = sizeof(DInfo);
	SKEY_OFF;//SetSKey(0);
	PKEY_OFF;//SetPKey(0);

	Init();

	CSPOUT1_OFF;
	/*while(1)
	{
		CSPOUT1_ON;
		for(i = 0;i<100;i++);
		CSPOUT1_OFF;
		for(i = 0;i<32000;i++);
	}*/
	//SetSKey(0);
	//SetPKey(0);

	//FRDY_OFF;
	CSPOUT1_OFF;//FF;

	SetupKeyControl();
	//SetSKey(0);
	//SetPKey(1);

	for(i = 0;i<1000;i++)
		for(tmp = 0;tmp<1000;tmp++)
			CheckNull();
	
	//while(CSPIN == 0)
	//	;

	/*do
	{
		while(SpiHasData() == 0);
	}	while(SpiReadData() != SPI_COMMAND_HELLO);

	SpiWriteData(SPI_COMMAND_HREPLY);//*/

	/*while(1)
	{
		for(i = 0;i<10000;i++)
		FRDY_ON;
		for(i = 0;i<10000;i++)
		FRDY_OFF;
	}*/

	//kardebalet();

	//while(CSPIN == 0)
	//	;

	for(i = 0;i<16;i++)
	{
		CheckNull();
		ever_u += UNul;
		ever_i += INul;
		ever_count ++;
	}

	UNul = ever_u / ever_count;
	INul = ever_i / ever_count;

	ADC_UIn_Start();
	src_in_u = ADC_UIn_Result;

	while(1)
	{
		SKEY_OFF;//SetSKey(0);
		PKEY_ON;//SetPKey(1);
		FRDY_ON;

		/*if(IgnCount > 0)
		{
			ign_count ++;
			if(ign_count >= 100)
			{
				ADC_UOut_Start();
				if(ADC_UOut_Result > 1000)
				{
					Ign();
					IgnCount --;
				}
				else
				{
					//StartI(100);
					IgnCount = 0;
				}
				ign_count = 0;
			}
		}//*/

		if(SpiHasData() != 0)
		{
			switch(SpiReadData())
			{
				case SPI_COMMAND_START_NEW:
					StatU = 0;
					StatError = 0;
					StatCount = 0;
					ADC_UIn_Start();
					src_in_u = ADC_UIn_Result;
					ADC_UIn_Start();
					src_in_u += ADC_UIn_Result;
					src_in_u /= 2;
					break;
				case SPI_COMMAND_HELLO:
					SpiWriteData(SPI_COMMAND_HREPLY);
					break;
				case SPI_SET_U:
					while(SpiHasData() == 0);
					U = SpiReadData();
					SpiWriteData(U+1);
					UUnit = (int)(UNul + ((350+U)/UOutDivider) * 0xFFF / ADCBaseU);
					break;
				case SPI_SET_I:
					while(SpiHasData() == 0);
					I = SpiReadData();
					SpiWriteData(I+1);
					IUnit = (int)(INul + 3.5F/30 *(I/10.0F)*0xFFF/ADCBaseU);
					break;
				case SPI_SET_I1:
					while(SpiHasData() == 0);
					I1 = SpiReadData();
					SpiWriteData(I1+1);
					I1Unit = (int)(INul + 3.5F/30 *(I1/10.0F)*0xFFF/ADCBaseU);
					break;
				case SPI_SET_Ph:
					while(SpiHasData() == 0);
					Ph = SpiReadData();
					SpiWriteData(Ph+1);
					break;
				case SPI_ERROR:
					SpiWriteData(DInfo.Code);
					break;
				case SPI_ULo:
					SpiWriteData((DInfo.U&0xFF));
					break;
				case SPI_UHi:
					SpiWriteData(((DInfo.U>>8)&0xFF));
					break;
				case SPI_DEBUG_INFO_START:
					pointer = (int*)&DInfo;
					break;
				case SPI_DEBUG_INFO_STEP_LO:
				    idata = *pointer;
					SpiWriteData(idata&0xFF);
					break;
				case SPI_DEBUG_INFO_STEP_HI:
					SpiWriteData((idata>>8)&0xFF);
					pointer++;
					break;
				case SPI_C_INFO:
					while(SpiHasData() == 0);
					C = SpiReadData();
					c = C;
					SpiWriteData(C+1);
					FirstStageTimeOut = FirstStageTimeOuts[C];
					FirstStageTimeOutSrc = FirstStageTimeOut;
					MAX_I_LO = INul + MAX_I_LO_DLT;
					MAX_I_HI = INul + MAX_I_HI_DLT;
					break;
				case SPI_READ_U:
					ADC_UOut_Start();
					
					SpiWriteData((result_u>>8)&0xFF);
					SpiWriteData(result_u&0xFF);

					SpiWriteData((result_in_u>>8)&0xFF);
					SpiWriteData(result_in_u&0xFF);

					SpiWriteData((src_in_u>>8)&0xFF);
					SpiWriteData(src_in_u&0xFF);

					break;
				case SPI_READ_STAT:
					//UUnit = UNul + ((350+U)/UOutDivider) * 0xFFF / ADCBaseU;
					//UUnit - UNul = ((350+U)/UOutDivider) * 0xFFF / ADCBaseU;
					//(UUnit - UNul)*ADCBaseU = ((350+U)/UOutDivider) * 0xFFF;
					//(UUnit - UNul)*ADCBaseU/0xFFF = (350+U)/UOutDivider;
					//((UUnit - UNul)*ADCBaseU/0xFFF)*UOutDivider = 350+U;
					//((UUnit - UNul)*ADCBaseU/0xFFF)*UOutDivider-350 = U;
					//((StatU - UNul)*ADCBaseU/0xFFF)*UOutDivider;
					stat = StatU / StatCount;//((StatU - UNul)*ADCBaseU/0xFFF)*UOutDivider / StatCount;
					SpiWriteData(stat&0xFF);
					SpiWriteData((stat >> 8)&0xFF);
					stat = StatError / StatCount;//((StatError - UNul)*ADCBaseU/0xFFF)*UOutDivider / StatCount;
					SpiWriteData(stat&0xFF);
					SpiWriteData((stat >> 8)&0xFF);
					break;
				case SPI_START:
					/*if(IgnCount > 0)
					{
						IgnMaxCount -= IgnCount;
						IgnMaxCount -= 2;
					}
					if(IgnMaxCount <= 10)
						IgnMaxCount = 10;
					if(IgnMaxCount > 100)
						IgnMaxCount = 10;*/
					Start();
					break;
			}	
		}
	}
}
