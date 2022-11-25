#ifndef __AS608USER_H
#define __AS608USER_H
#include <string.h> 	
#include "usart.h"


void Add_FR(u16 FR_ID); //录入指纹
void Del_FR(u16 FR_ID);	//删除指纹
void press_FR(void);//刷指纹
void ShowErrMessage(u8 ensure);




#endif