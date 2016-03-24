#ifndef SPKEY_H
#define SPKEY_H

#define UNDER_CUR 0
#define OVER_CUR 1

#define SLIM GpioDataRegs.GPADAT.bit.GPIO3
#define PLIM GpioDataRegs.GPADAT.bit.GPIO2

#define SCLF GpioDataRegs.GPADAT.bit.GPIO28
#define PCLF GpioDataRegs.GPADAT.bit.GPIO12

extern void SetupKeyControl();
//extern char PulsSKey(Uint32 val);
//extern char PulsPKey(Uint32 val);
//extern void PulsSKeyAgain();
//extern void PulsPKey(Uint32 val);
//extern void PulsPKeyAgain();
//extern int IsKeyPulsFinished();
extern void TurnOffKeys();

//extern void SetPKey(char val);
//#define SetPKey(t) ( PKEY = (!t)&1 )
//extern void PulsSMax(int time_out);
//extern void SetSKey(char val);
//#define SetSKey(t) ( SKEY = (!t)&1 )

#define SKEY_OFF GpioDataRegs.GPASET.bit.GPIO0 = 1
#define SKEY_ON GpioDataRegs.GPACLEAR.bit.GPIO0 = 1

#define USE_PKEY

#ifdef USE_PKEY
	#define PKEY_OFF GpioDataRegs.GPASET.bit.GPIO1 = 1
	#define PKEY_ON GpioDataRegs.GPACLEAR.bit.GPIO1 = 1
#else
	#define PKEY_OFF GpioDataRegs.GPASET.bit.GPIO1 = 1
	#define PKEY_ON GpioDataRegs.GPASET.bit.GPIO1 = 1
#endif

#endif
