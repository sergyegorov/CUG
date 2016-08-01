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

extern int waitForKeyPress();
extern int GetKeyState();
extern int GetKeyPressed();
#define WAIT_FOR_NO_KEY while(GetKeyState() != 0) delay_ms(10);

extern void LCDSymboxl(char data);

extern void Beep();

#endif
