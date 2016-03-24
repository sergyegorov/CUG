#ifndef RUN_H
#define RUN_H

extern void run_test_pins();

#define U_NORMAL_ADC 900
#define PINCOUNT 8
extern void setKeyState(int pin,char state);

#define READY_PIN GPIO_Pin_8
#define READY_ON outPin(GPIOD,READY_PIN,READY_PIN)
#define READY_OFF outPin(GPIOD,READY_PIN,0)

extern char FLAG_NO_SPARK_CHECK;
extern char FLAG_NO_GAS_CHECK;
extern char FLAG_NO_PROT_CHECK;

extern void run_init();
extern int spark(int cur_mla[],int len,int time_out_sys_ticks_val);

#define MAX_F 400
#define CUR_MAX 380+160

#define REL_GAS1 1 // 1 gas 1
#define REL_GAS2 2 // 2 gas 2
// 4
// 8
// 16 main relay
// 32 main relay
// 64 main relay
#define REL_MAIN 64+32+16
#define REL_600V 128 // 128 600V
#define REL_NONE 0
#define REL_ALL 0xFF
extern void setRelayN(int val);

extern int getKeyMask(int cur);
extern void sparkForDischarge();
extern char checkPrecharge();
extern void setPrecharge(char isOn);

extern int NoSparkCount,NoCurrentCount;
extern int SparkCount,IgnCount;
extern char *ErrorTxt;
#endif
