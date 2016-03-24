#ifndef TUGLAB_H
#define TUGLAB_H

//#define F_CPU 7372800UL //7,3728MHc
//#include <util/delay.h>


void sleep_us(int mks);
void sleep_ms(int mls);

unsigned char EEPROM_read_byte(unsigned int addr);
void EEPROM_write_byte(unsigned int addr,unsigned char val);
unsigned short EEPROM_read_short(unsigned int addr);
void EEPROM_write_short(unsigned int addr,unsigned short val);

#endif
