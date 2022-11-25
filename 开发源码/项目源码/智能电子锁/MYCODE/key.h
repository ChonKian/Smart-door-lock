#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx.h"
#include "sys.h"

void Key_Init(void);
u16 Key_Scan(void);
#endif
