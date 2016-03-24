#ifndef LCD_H
#define LCD_H

extern void LCDInit(int line_size);
extern void LCDClear();
extern void LCDMoveTo(char str,char x);
extern void LCDPrintString(const char *str);
extern void LCDPrintInt(int val);
extern void LCDPrintUInt(unsigned int val);
extern void LCDPutChar(char data);
extern void LCDPrintFInt(int val,int deximal);
extern void LCDPrintFloat(float f);
extern void LCDPutByte(char data);

extern void LCDMoveToNoWait(char str,char x);
extern void LCDPutCharNoWait(char data);

extern void LCDLedStatus(char on);

extern int GetKeyCode();
extern int GetKeyPressed();

extern void Beep();

#endif
