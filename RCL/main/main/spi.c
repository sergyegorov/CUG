#include <avr/io.h>
#include "spi.h"
#include "spi_command.h"

void SPI_Init(void)
{
	// Enable SPI, Master, set clock rate fck/128 */
	// 01010011 = 0x53
	char tmp = SPSR;
	//SPCR = 0x53;
	SPCR = 0 |
		//(1<<SPIE) |		// interupt enable
		(1<<SPE) |		// spi enable
		//(1<<DORD) |		// data order
		(1<<MSTR) |		// master/slave

		(1<<CPOL) |	// clock incative hi
		(1<<CPHA) |

		(1<<SPR1) |
		(1<<SPR0) |
		0
	;
	SPSR = 0 |
		//(1<<SPI2X)	| // double speed bit
		0
		;
	tmp = SPSR;
	DDRB |= (1<<PB6);
	DDRE |= (1<<PE7);
}


int SelectedSPIChanel = 3;
void SetupChanel(char chanel)
{
	/*PORTB &= (0xFF^(1<<PB0)) & (0xFF^(1<<PB6));// 0xDF;
	PORTE &= 0x7F;
	if(chanel == 0)
		PORTE |= 0x80;
	if(chanel == 1)
		PORTB |= (1<<PB0) | (1<<PB6);*/
	PORTB |= (1<<PB0);
	PORTB |= (1<<PB6);
	if(chanel == 0)
	{
		PORTB &= (0xFF^(1<<PB0));
		PORTB &= (0xFF^(1<<PB6));
	}
}

char SPI_Transmit(char chanel,char cData)
{
	SetupChanel(chanel);
	// Start transmission 
	SPDR = cData;
	while(!(SPSR & (1<<SPIF)))
		;
	SetupChanel(0x80);
	return SPDR ^ 0xFF;
}


