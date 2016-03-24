#ifndef STAT_DATA_H
#define STAT_DATA_H

struct STAT
{
  long EverData;
  long EverDataCount;
  long EverDataDlt;
  long ChargeTime;
  
  unsigned char AlgoStep;
  
  unsigned char Puls[3];
  unsigned char PulsCount[3];
  unsigned char TunningPuls;
   
  unsigned char FatalErrors;
  
  unsigned char Debug1;
  unsigned char Debug2;
  unsigned char LowData;
  unsigned char HiData;
  unsigned char DebugCount;
  
  unsigned char CRC;
};

#define STAT_FatalErrors_KZbit 1
#define STAT_FatalErrors_C0bit 2

#endif
