#ifndef MENU_H
#define MENU_H

extern char Cv;
extern char Uv;
extern char Iv,Iv1;
extern char Lv;
extern char Rv;
extern char Phv;
extern char Fv;
extern char PreBlowTime;

extern void LoadFromEPROM();

extern char CurValMin,CurValMax,CurValBigStep;
extern char *CurVal;

extern int PrintScr(int col,char is_big_step);
extern void PrintInfoScr();

#endif
