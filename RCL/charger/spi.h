#ifndef SPI_H
#define SPI_H

extern void SpiInit();
extern char SpiHasData();
extern char SpiReadData();
extern void SpiWriteData(char data);

#endif
