#ifndef __SPIFLASH_H
#define __SPIFLASH_H

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"


/**************************************
����˵����

ʹ��SPI1
SCK����PB3
MISO����PB4
MOSI����PB5
CS����PB14
**************************************/


#define SCK		PBout(3)
#define F_CS  	PBout(14)
#define MOSI	PBout(5)
#define MISO	PBin(4)




//��������
void Spiflash_Init(void);


u16 Get_W25q128_Id(void);


#endif
