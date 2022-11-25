#include "beep.h"


/**************************************
引脚说明：
BEEP连接在PF8
PF8输出低电平（0），蜂鸣器响；PF8输出高电平（1），蜂鸣器不响；

**************************************/

void Beep_Init(void)
{
	
	GPIO_InitTypeDef   GPIO_InitStruct;
	
	//打开GPIOF组时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_8;  		//引脚8
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;	//输出模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP; 	//输出推挽
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_25MHz;	//速度
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;		//上拉

	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
}
