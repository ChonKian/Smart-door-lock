#include "key.h"


/**************************************
KEY0连接在PA0
按键未按下，PA0为高电平
按键 按下，PA0为低电平

**************************************/
void Key_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStruct;
	
	//打开GPIOA组时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_0;  		//引脚0
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN;		//输入模式
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;		//上拉

	GPIO_Init(GPIOA, &GPIO_InitStruct);

}
	