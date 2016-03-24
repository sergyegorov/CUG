#ifndef MENU_H
#define MENU_H

#include "lcd.h"

#define MENU_TYPE_NONE 		0
#define MENU_TYPE_TEXT 		1
#define MENU_TYPE_ACTION 	2
#define MENU_TYPE_I1 		3
#define MENU_TYPE_I2 		4
#define MENU_TYPE_I3 		5
#define MENU_TYPE_I4 		6
#define MENU_TYPE_F  		7
#define MENU_TYPE_PBT 		8
#define MENU_TYPE_PST		9
#define MENU_TYPE_PSI		10

class MenuItem{
protected:
	const char *Name;
	const char *Description;
	int MenuType;
	int DescriptionScrollPosition;
	MenuItem(int menuType,const char *name,const char *description){
		Name = name;
		Description = description;
		MenuType = menuType;
		DescriptionScrollPosition = 20;
	}
public:
	void init(){}
	virtual bool isVisible(){return false;}
	const char *getTypeDescription();
	int getType(){return MenuType;}
	virtual void onInc(){}
	virtual void onDec(){}
	virtual void onStart(){}

	virtual void printName(LCD &lcd){
		lcd.PrintString(Name);
	}

	void printDescription(LCD &lcd){
		lcd.MoveTo(3,0);
		if(DescriptionScrollPosition > 0){
			for(int i = 0;i<DescriptionScrollPosition;i++)
				lcd.PrintString("_");
			lcd.PrintString(Description);
			DescriptionScrollPosition --;
		} else {
			int pos = -DescriptionScrollPosition;
			lcd.PrintString(Description+pos);
			lcd.ClearToTheEndOfLine('_');
			if(Description[pos] == 0)
				DescriptionScrollPosition = 20;
			else
				DescriptionScrollPosition --;
		}
	}
};

class MenuTextItem : public MenuItem{
public:
	MenuTextItem(const char *name,const char *description) : MenuItem(MENU_TYPE_TEXT,name,description){}
	bool isVisible(){return true;}
};

class MenuSetModeItem : public MenuItem{
private:
	int I0,I1,I2,I3,F,PreblowTime,PresparkTime,PresparkI;
public:
	MenuSetModeItem(const char *name,const char *description,
			int i0,int i1,int i2,int i3,
			int f,int preblowTime,
			int presparkTime,int presparkI) :
				MenuItem(MENU_TYPE_ACTION,name,description){
				I0 = i0;
				I1 = i1;
				I2 = i2;
				I3 = i3;
				F = f;
				PreblowTime = preblowTime;
				PresparkTime = presparkTime;
				PresparkI = presparkI;
	}
	bool isVisible(){return true;}
};

class MenuIntItem : public MenuItem{
private:
	int Min;
	int Max;
	int Val;
	int Step;
	const char *UnitName;
public:
	MenuIntItem(int type,const char *name,const char *description,const char *unitName,int min,int max,int val,int step) :
		MenuItem(type,name,description){
		Min = min;
		Max = max;
		Val = val;
		Step = step;
		UnitName = unitName;
	}
	bool isVisible(){return true;}
	void printName(LCD &lcd){
		MenuItem::printName(lcd);
		lcd.PrintString(" ");
		lcd.PrintInt(Val);
		lcd.PrintString(" ");
		lcd.PrintString(UnitName);
	}
	virtual void onInc(){
		Val += Step;
		if(Val > Max)
			Val = Max;
	}
	virtual void onDec(){
		Val -= Step;
		if(Val < Min)
			Val = Min;
	}
};


class MenuExternalStart : public MenuItem{
public:
	MenuExternalStart() : MenuItem(MENU_TYPE_NONE,"Внешний пуск","Разрешон внешний пуск и управление генератором"){
	}
	void startedExternaly();

	void onStart(){
		startedExternaly();
	}
};

class MenuInternalStart : public MenuItem{
public:
	MenuInternalStart() : MenuItem(MENU_TYPE_NONE,"Внутренний пуск","Разрешон внутренний пуск"){
	}
	void startedManualy();

	void onStart(){
		startedManualy();
	}
};

#endif
