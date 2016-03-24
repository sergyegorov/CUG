#include "spi.h"
#include "DSP2802x_Device.h"
#include "DSP28x_Project.h"

void SpiInit()
{
	int size = 0x7;
	int mode = 0; // 0 - active 1 - testing

	EALLOW;

   SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;      // SCI-A
   SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;      // SPI-A

	/* Enable internal pull-up for the selected pins */
	// Pull-ups can be enabled or disabled by the user.
	// This will enable the pullups for the specified pins.
	// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up on GPIO16 (SPISIMOA)
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up on GPIO17 (SPISOMIA)
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;   // Enable pull-up on GPIO18 (SPICLKA)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;   // Enable pull-up on GPIO19 (SPISTEA)

	/* Set qualification for selected pins to asynch only */
	// This will select asynch (no qualification) for the selected pins.
	// Comment out other unwanted lines.

    GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input GPIO16 (SPISIMOA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch input GPIO17 (SPISOMIA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input GPIO18 (SPICLKA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // Asynch input GPIO19 (SPISTEA)

	/* Configure SPI-A pins using GPIO regs*/
	// This specifies which of the possible GPIO pins will be SPI functional pins.
	// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1; // Configure GPIO16 as SPISIMOA
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1; // Configure GPIO17 as SPISOMIA
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1; // Configure GPIO18 as SPICLKA
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1; // Configure GPIO19 as SPISTEA


	//spi_fifo_init();	  // Initialize the Spi FIFO
	// Initialize SPI FIFO registers
    SpiaRegs.SPIFFTX.all=0xE040;
    SpiaRegs.SPIFFRX.all=0x2044;
    SpiaRegs.SPIFFCT.all=0x0;

    //spi_init();		  // init SPI
	SpiaRegs.SPICCR.all = 0x0000|size;	             // Reset on, rising edge, 16-bit char bits

	SpiaRegs.SPICTL.all =0x0006;    		     // Enable master mode, normal phase, enable talk, and SPI int disabled.
	SpiaRegs.SPICTL.bit.OVERRUNINTENA = 0;
	SpiaRegs.SPICTL.bit.SPIINTENA = 0;
	SpiaRegs.SPICTL.bit.MASTER_SLAVE = mode;		// 1-default
	SpiaRegs.SPICTL.bit.TALK = 1;
	SpiaRegs.SPICTL.bit.CLK_PHASE = 1;

	SpiaRegs.SPIBRR =0x0078;

    SpiaRegs.SPICCR.all =0x0090|size;		         // Relinquish SPI from Reset
	SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;
	SpiaRegs.SPICCR.bit.SPILBK = mode;				// 1 - default
	SpiaRegs.SPICCR.bit.SPICHAR = size;

    SpiaRegs.SPIPRI.bit.FREE = 1;                // Set so breakpoints don't disturb xmission
}

char SpiHasData()
{
	return SpiaRegs.SPIFFRX.bit.RXFFST;
}

char SpiReadData()
{
	return SpiaRegs.SPIRXBUF & 0xFF;
}

void SpiWriteData(char data)
{
	while(SpiaRegs.SPIFFTX.bit.TXFFST > 3);
	SpiaRegs.SPITXBUF=data<<8;
}
