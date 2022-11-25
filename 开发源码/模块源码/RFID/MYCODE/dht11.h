#ifndef __DHT11_H_
#define __DHT11_H_
#include "stm32f4xx.h"
#include "delay.h"
#include "sys.h"



void Dht11_Init(void);
void Dht11_Pin_Mode(GPIOMode_TypeDef mode);
int  Dht11_Start(void);
uint8_t Dht11_Read_Byte(void);
int Dht11_Read(u8 *data);

#endif

