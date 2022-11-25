#ifndef __SPIFLASH_H
#define __SPIFLASH_H

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"


/**************************************
引脚说明：

使用SPI1
SCK连接PB3
MISO连接PB4
MOSI连接PB5
CS连接PB14
**************************************/


#define SCK		PBout(3)
#define F_CS  	PBout(14)
#define MOSI	PBout(5)
#define MISO	PBin(4)




//函数声明
void Spiflash_Init(void);


u16 Get_W25q128_Id(void);


#endif
