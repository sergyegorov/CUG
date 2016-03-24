#ifndef REL_H
#define REL_H

#include "common.h"

#define CMAX 15
#define RMAX 6
#define LMAX 375

extern void RelRefresh();
//extern void RelSendData(unsigned int val);
extern int RelSet(int r,int c,int l,
					unsigned char is_out_enable,unsigned char is_spark_enable,
					unsigned char gas_on,unsigned char gas_hi);
extern int RelSetCode(char r,char c,char l,
					unsigned char is_out_enable,unsigned char is_spark_enable,
					unsigned char gas_on,unsigned char gas_hi);

#endif
