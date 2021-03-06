#ifndef spi_h
#define spi_h

#include "stm32f10x.h"

#define SPIINT_ON GPIO_SetBits(GPIOC,GPIO_Pin_6)
#define SPIINT_OFF GPIO_ReSetBits(GPIOC,GPIO_Pin_6)

#define GET_EXT_START getPin(GPIOB,GPIO_Pin_12)

extern void SpiMasterInit();
extern void SpiSlaveInit();

extern uint16_t SpiMasterSend(uint16_t val);
extern uint16_t SpiSlaveReply(uint16_t reply);
extern uint16_t SpiSlaveReceived();
#endif
