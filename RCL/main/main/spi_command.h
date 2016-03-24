#ifndef SPI_COMMAND_H
#define SPI_COMMAND_H

#include "..\..\charger\stat_data.h"

#define SPI_COMMAND_HELLO 0x5A
#define SPI_COMMAND_HREPLY 0xA5

#define SPI_NOP					0
#define SPI_SET_I				0x1
#define SPI_SET_U				0x2
#define SPI_ERROR				0x3
#define SPI_ULo					0x4
#define SPI_UHi					0x5
#define SPI_DEBUG_INFO_START	0x6
#define SPI_DEBUG_INFO_STEP_LO	0x7
#define SPI_DEBUG_INFO_STEP_HI	0x8
#define SPI_C_INFO				0x9
#define SPI_READ_U				0xA
#define SPI_READ_STAT			0xB
#define SPI_COMMAND_START_NEW   0xC
#define SPI_START				0xD
#define SPI_SET_Ph				0xE
#define SPI_SET_I1				0xF

struct DebugInfo
{
	int Code;
	int U;
	int UMust;
	int Stage;
	int Impuls;
	int Time;
	int StepOk;
	int StepError;
};
#define DebugInfoSizeInWord 8


// ERRORS
#define SPI_ERROR_NONE 			0
#define SPI_ERROR_ULow 			1
#define SPI_ERROR_UHi  			2
#define SPI_ERROR_OverTime 		4
#define SPI_ERROR_NO_DISCHARGE  8
//#define SPI_ERROR_KZ  			16
//#define SPI_ERROR_NO_SPARK		32

#endif
