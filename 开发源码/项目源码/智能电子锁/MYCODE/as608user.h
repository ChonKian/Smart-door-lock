#ifndef __AS608USER_H
#define __AS608USER_H
#include <string.h> 	
#include "usart.h"


void Add_FR(); //录入指纹
void Del_FR(void);	//删除指纹
void press_FR(void);//刷指纹
void press_FR1(void);
void Add_FR1(void);
void ShowErrMessage(u8 ensure);




#endif