#ifndef INOUTSIGNAL_H
#define INOUTSIGNAL_H

#define FRDY_ON GpioDataRegs.GPASET.bit.GPIO7 = 1
#define FRDY_OFF GpioDataRegs.GPACLEAR.bit.GPIO7 = 1

#define CSPOUT1_ON GpioDataRegs.GPASET.bit.GPIO6 = 1
//#define CSPOUT1_OFF GpioDataRegs.GPASET.bit.GPIO6 = 1
#define CSPOUT1_OFF GpioDataRegs.GPACLEAR.bit.GPIO6 = 1
//#define CSPOUT1_ON GpioDataRegs.GPACLEAR.bit.GPIO6 = 1

//#define CSPIN GpioDataRegs.GPADAT.bit.GPIO4

#define SENIN GpioDataRegs.GPADAT.bit.GPIO19

#endif
