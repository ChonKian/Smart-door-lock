#ifndef __IIC_H
#define __IIC_H

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
/*******************************
Òý½ÅËµÃ÷
SCL -- PB8
SDA -- PB9
********************************/


#define SCL  	PBout(8)
#define SDA_IN	PBin(9)
#define SDA_OUT	PBout(9)

void Iic_Init(void);
void At24c02_Write_Page(u8 addr, u8 *write_buff, u8 len);
void At24c02_Read(u8 addr, u8 *read_buff, u8 len);

#endif