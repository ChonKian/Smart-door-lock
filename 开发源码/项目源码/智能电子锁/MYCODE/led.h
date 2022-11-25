#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"

#define LED0_ON		GPIO_ResetBits(GPIOF, GPIO_Pin_9)
#define LED0_OFF	GPIO_SetBits(GPIOF, GPIO_Pin_9)


void Led_Init(void);


#endif
