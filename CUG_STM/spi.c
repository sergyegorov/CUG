#include "spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"

void SpiMasterInit(){
	/*	GPIO_InitTypeDef gpio;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);  // тактирование порта
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);  // тактирование SPI1
	    GPIO_StructInit(&gpio);
	    gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10;
	    gpio.GPIO_Mode = GPIO_Mode_AF;
	    gpio.GPIO_Speed = GPIO_Speed_50MHz;
	    gpio.GPIO_OType = GPIO_OType_PP;
	    gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
	    GPIO_Init(GPIOA,&gpio);

	    GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
	    GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
	    GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
	    SPI_I2S_DeInit(SPI1);
	    SPI_InitTypeDef spi1;
	    SPI_StructInit(&spi1);
	    spi1.SPI_Mode = SPI_Mode_Master;
	    spi1.SPI_DataSize = SPI_DataSize_8b;
	    spi1.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // Предделитель SCK
	    spi1.SPI_NSS = SPI_NSS_Soft;
	    SPI_Init(SPI1,&spi1);
	    SPI_Cmd(SPI1,ENABLE);*/
}

void SpiSlaveInit(){
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*	GPIO_InitTypeDef GPIO_InitStructure;

	// Поскольку сигнал NSS контролируется программно, установим его в единицу
	// Если сбросить его в ноль, то наш SPI модуль подумает, что
	// у нас мультимастерная топология и его лишили полномочий мастера.
	SPI_NSSInternalSoftwareConfig(SPI2, SPI_NSSInternalSoft_Set);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB,&gpio);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);
	SPI_I2S_DeInit(SPI2);
	SPI_InitTypeDef spi2;
	SPI_StructInit(&spi2);
	spi2.SPI_Mode = SPI_Mode_Slave;
	spi2.SPI_DataSize = SPI_DataSize_8b;
	spi2.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // Предделитель SCK
	spi2.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI2,&spi2);
	SPI_Cmd(SPI2,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;// NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);*/
}

/*uint16_t SpiMasterSend(uint16_t val){
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1,val);
	return SPI_I2S_ReceiveData(SPI1);
}

uint16_t SpiSlaveReply(uint16_t val){
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);  // ждём пока данные появтся
	SPI_I2S_SendData(SPI2,val);
	return SPI_I2S_ReceiveData(SPI2);
}

uint16_t SpiSlaveReceived(){
	return SPI_I2S_ReceiveData(SPI2);
}
*/
