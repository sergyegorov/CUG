#ifndef LCD_H
#define LCD_H

void LCDInit();
void LCDClear();
void LCDMoveTo(char str,char x);
void LCDPrintString(char *str);
void LCDPrintInt(int val);
void LCDPrintUInt(unsigned int val);
void LCDPutChar(char data);
void LCDPrintFInt(int val,int deximal);
void LCDPrintFloat(float f);
void LCDPutByte(char data);

void LCDMoveToNoWait(char str,char x);
void LCDPutCharNoWait(char data);

void LCDLedStatus(char on);

#endif
