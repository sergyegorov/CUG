#include "spi.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"

void SpiMasterInit(){
	/*GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	// ������������ ������ SPI1 � ����� �
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// ����������� ���� SPI1 ��� ������ � ������ �������������� �������
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//��������� ��������� � ����������� SPI ������
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //������ �������
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // �������� �� 8 ���
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // ���������� �
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // ���� ��������� �������
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; // ��������� ���������� ������� NSS ����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // ������������ SCK
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // ������ ������������ ������� ���
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // ����� - ������
	SPI_Init(SPI1, &SPI_InitStructure); //����������� SPI1
	SPI_Cmd(SPI1, ENABLE); // �������� ������ SPI1....

	// ��������� ������ NSS �������������� ����������, ��������� ��� � �������
	// ���� �������� ��� � ����, �� ��� SPI ������ ��������, ���
	// � ��� ��������������� ��������� � ��� ������ ���������� �������.
	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);//*/
	GPIO_InitTypeDef gpio;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);  // ������������ �����
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);  // ������������ SPI1
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
	    spi1.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // ������������ SCK
	    spi1.SPI_NSS = SPI_NSS_Soft;
	    SPI_Init(SPI1,&spi1);
	    SPI_Cmd(SPI1,ENABLE);
}

void SpiSlaveInit(){
	GPIO_InitTypeDef GPIO_InitStructure;
	/*SPI_InitTypeDef SPI_InitStructure;

	// ������������ ������ SPI1 � ����� �
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	// ����������� ���� SPI1 ��� ������ � ������ �������������� �������
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//��������� ��������� � ����������� SPI ������
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //������ �������
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // �������� �� 8 ���
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // ���������� �
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // ���� ��������� �������
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; // ��������� ���������� ������� NSS ����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // ������������ SCK
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // ������ ������������ ������� ���
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	SPI_Init(SPI2, &SPI_InitStructure); //����������� SPI2
	SPI_Cmd(SPI2, ENABLE); // �������� ������ SPI2....

	// ��������� ������ NSS �������������� ����������, ��������� ��� � �������
	// ���� �������� ��� � ����, �� ��� SPI ������ ��������, ���
	// � ��� ��������������� ��������� � ��� ������ ���������� �������.
	SPI_NSSInternalSoftwareConfig(SPI2, SPI_NSSInternalSoft_Set);//*/
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
	spi2.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // ������������ SCK
	spi2.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI2,&spi2);
	SPI_Cmd(SPI2,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;// NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint16_t SpiMasterSend(uint16_t val){
	/*while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET) //���������� �����?
		; // ������ ������ �� ������
	SPI_I2S_SendData(SPI1, val); //�������� ���� 0x93 ����� SPI1*/
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1,val);
	return SPI_I2S_ReceiveData(SPI1);
}

uint16_t SpiSlaveReply(uint16_t val){
	/*while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET) //���������� �����?
		; // ������ ������ �� ������
	SPI_I2S_SendData(SPI2, val); //�������� ���� 0x93 ����� SPI1*/
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);  // ��� ���� ������ �������
	SPI_I2S_SendData(SPI2,val);
	return SPI_I2S_ReceiveData(SPI2);
}

uint16_t SpiSlaveReceived(){
	return SPI_I2S_ReceiveData(SPI2);
}

