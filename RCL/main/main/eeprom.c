#include "tuglab.h"
#include "lcd.h"
#include "eeprom.h"

void InitEEPROM()
{
	if(EEPROM_read_byte(EEPROM_Inited_addr) != 0xD5)
	{
		//LCDClear();
		//LCDMoveTo(0,0);
		//LCDPrintString("InitEEPROM:");
		EEPROM_write_short(EEPROM_U1_addr,400);
		EEPROM_write_short(EEPROM_I1_addr,500);
		EEPROM_write_byte(EEPROM_F1_addr,1);
		EEPROM_write_byte(EEPROM_C1_addr,1);
		EEPROM_write_byte(EEPROM_L1_addr,0);
		EEPROM_write_byte(EEPROM_T1_addr,50);
		EEPROM_write_byte(EEPROM_R1_addr,0);
		EEPROM_write_byte(EEPROM_S1_addr,60);

		EEPROM_write_short(EEPROM_U2_addr,400);
		EEPROM_write_short(EEPROM_I2_addr,500);
		EEPROM_write_byte(EEPROM_F2_addr,1);
		EEPROM_write_byte(EEPROM_C2_addr,1);
		EEPROM_write_byte(EEPROM_L2_addr,0);
		EEPROM_write_byte(EEPROM_T2_addr,50);
		EEPROM_write_byte(EEPROM_R2_addr,0);
		EEPROM_write_byte(EEPROM_S2_addr,60);

		EEPROM_write_byte(EEPROM_MODE_addr,0);

		/*char is_ok = 1;
		if(EEPROM_read_short(EEPROM_U_addr) != 400)
		{
			is_ok = 0;
			LCDPrintString("U");
		}
		if(EEPROM_read_short(EEPROM_I_addr) != 500)
		{
			is_ok = 0;
			LCDPrintString("I");
		}
		if(EEPROM_read_byte(EEPROM_F_addr) != 1)
		{
			is_ok = 0;
			LCDPrintString("F");
		}
		if(EEPROM_read_byte(EEPROM_C_addr) != 0)
		{
			is_ok = 0;
			LCDPrintString("C");
		}
		if(EEPROM_read_byte(EEPROM_L_addr) != 0)
		{
			is_ok = 0;
			LCDPrintString("L");
		}
		if(EEPROM_read_byte(EEPROM_R_addr) != 0)
		{
			is_ok = 0;
			LCDPrintString("R");
		}
		if(EEPROM_read_byte(EEPROM_S_addr) != 60)
		{
			is_ok = 0;
			LCDPrintString("S");
		}
		if(EEPROM_read_byte(EEPROM_MODE_addr) != 0)
		{
			is_ok = 0;
			LCDPrintString("M");
		}

		if(is_ok)
		{
			LCDPrintString("Done");
			sleep_ms(2000);
		}
		else
		{
			LCDPrintString("Error");
			while(1);
		}*/
		EEPROM_write_byte(EEPROM_Inited_addr,0xD5);
	}
}
