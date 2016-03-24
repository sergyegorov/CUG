#include <DSP2802x_Device.h>
#include "InOutSignal.h"

void InitInOutSignal()
{
	EALLOW;

	// FRDY1
	GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;   // Enable pullup on GPIO7
   	GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;  // GPIO7 = GPIO7
   	GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;   // GPIO7 = output
	//CSPOUT1
	GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;   // Enable pullup on GPIO6
   	GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;  // GPIO6 = GPIO6
   	GpioCtrlRegs.GPADIR.bit.GPIO6 = 1;   // GPIO6 = output
	//CSPIN
	GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;   // Enable pullup on GPIO4
   	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;  // GPIO4 = GPIO4
   	GpioCtrlRegs.GPADIR.bit.GPIO4 = 0;   // GPIO4 = input

	//FRDY_OFF;
	CSPOUT1_OFF;
	//EDIS;
}
