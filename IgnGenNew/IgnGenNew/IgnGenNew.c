/*
 * IgnGen.c
 *
 * Created: 01.07.2014 12:16:42
 *  Author: EAD
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>

#define CHARGE_PIN 1<<PINB2
#define CHARGE_ON 1<<PINB2
#define CHARGE_OFF 0

#define DISCHARGE_PIN 1<<PINB0
#define DISCHARGE_ON 1<<PINB0
#define DISCHARGE_OFF 0

#define OVERCUR_PIN 1<<PINB1

#define EN_PIN 1 << PINB3

#define LED_PIN 1 << PINB4
#define LED_ON 1 << PINB4
#define LED_OFF 0

#define INLINE static inline __attribute__ ((always_inline))

#define delay_us(time) for(int i = 0;i<time;i++) asm volatile("nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n     nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n           nop \n nop \n nop \n nop \n nop \n nop \n")

//#define NON_STOP

int main(void)
{
	PORTB = CHARGE_OFF | DISCHARGE_OFF | LED_OFF;
	
	DDRB = DISCHARGE_PIN | CHARGE_PIN | LED_PIN;
	OSCCAL = 127;
	CLKPR = 0b10000000;
	CLKPR = 0;

	
	/*while(1){
		PORTB = CHARGE_OFF | DISCHARGE_OFF | LED_OFF;
		delay_us(100);
		PORTB = CHARGE_ON | DISCHARGE_OFF | LED_OFF;
		delay_us(100);
	}//*/
	/*PORTB = CHARGE_OFF | DISCHARGE_OFF | LED_ON;

	for(int j = 0;j<1000;j++)	delay_us(3000);

	delay_us(975);
	
	PORTB = CHARGE_ON;
	delay_us(100);
	PORTB = CHARGE_OFF;//*/
	
    while(1)
    {
		#ifndef NON_STOP 
		if((PINB & EN_PIN) == 0)
		#endif
		{
			// turne off charger
			PORTB = CHARGE_OFF | DISCHARGE_OFF | LED_OFF;
			delay_us(10);
			
			// discharge
			PORTB = DISCHARGE_ON | CHARGE_OFF | LED_ON;
			delay_us(20);
			PORTB = CHARGE_OFF | DISCHARGE_OFF | LED_ON;
			delay_us(750);
			
			// delicate charge start
			PORTB = CHARGE_ON | LED_ON | DISCHARGE_OFF;
			delay_us(5);
			PORTB = CHARGE_OFF | LED_ON | DISCHARGE_OFF;
			delay_us(5);
			
			PORTB = CHARGE_ON | LED_ON | DISCHARGE_OFF;
			delay_us(30);
			//unsigned char port = CHARGE_ON | LED_OFF | DISCHARGE_OFF;
			for(char i = 0;i<100;i++){
				if((OVERCUR_PIN & PINB) == 0){
					PORTB = CHARGE_OFF | LED_ON | DISCHARGE_OFF;
					break;
				}
				delay_us(1);
			}
			PORTB = CHARGE_OFF | LED_OFF | DISCHARGE_OFF;
			#ifndef NON_STOP
			while((PINB & EN_PIN) == 0){
				if((OVERCUR_PIN&PINB) == 0){
					PORTB = CHARGE_OFF | LED_OFF | DISCHARGE_OFF;
				}
			}
			#endif
		}
		/*if((OVERCUR_PIN & PINB) == 0){
			PORTB = CHARGE_OFF | LED_OFF | DISCHARGE_OFF;
		}//*/			
    }
}
