#ifndef REL_H
#define REL_H

extern char IsGasOn;

//#define ARUTIUN
#define SLAVIANSK

#ifdef ARUTIUN
#ifdef REL_GAS_LOW
#error "Select the only target..."
#endif
#define REL_GAS_LOW 	0b00000011 // gas low
#define REL_GAS_MAX 	0b00001111 // gas hi
#define REL_GAS_HI 		0b00001111

#define REL_GAS_MASK    0b00001111
#endif

#ifdef SLAVIANSK
#ifdef REL_GAS_LOW
#error "Select the only target..."
#endif
#define REL_GAS_LOW 	0b00001011 // gas low
#define REL_GAS_MAX 	0b00001111 // gas hi
#define REL_GAS_HI 		0b00001100

#define REL_GAS_MASK    0b00001111
#endif

#define REL_MAIN 		0b01000000 // main key
#define REL_600V 		0b10000000 // 128 600V
#define REL_NONE 		0
#define REL_ALL 		0xFF

//extern unsigned long RelTickCount;

extern void setRelayN(int val);
extern void testRel();

#endif
