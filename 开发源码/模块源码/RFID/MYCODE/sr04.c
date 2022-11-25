#include "sr04.h"


/**************************************
引脚说明

PA2做为普通输出
PA3做为普通输入

**************************************/

void Sr04_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	
	
	//打开GPIOA组时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//1、能定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
	
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_2;  					//引脚2
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;				//输出模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP; 				//输出推挽
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_25MHz;				//速度
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;					//上拉
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_3;  		//引脚3
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN;		//输入模式
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;		//上拉

	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	TIM_TimeBaseInitStruct.TIM_Prescaler	= 84-1;  				// 84分频 84MHZ/84 = 1MHZ
	TIM_TimeBaseInitStruct.TIM_Period	= 50000-1;					TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up;	// 向上计数
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1;			// 分频因子
	//2、初始化定时器，配置ARR,PSC
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
			
	//5,不使能定时器
	 TIM_Cmd(TIM3, DISABLE);
}


u16 Get_Sr04_Value(void)
{
	u16 temp, distance;
	
	//PA2为低电平
	PAout(2) = 0;
	delay_us(10);
	//PA2为高电平
	PAout(2) = 1;
	delay_us(20);
	//PA2为低电平
	PAout(2) = 0;
	
	//设置定时器的CNT为0  
	TIM3->CNT = 0;
	
	//PA3等待高电平到来，参考按键松开代码 
	while( PAin(3) == 0);

	
	//使能定时器开始计数
	 TIM_Cmd(TIM3, ENABLE);
	
	//PA3等待低电平到来，参考按键松开代码
	while( PAin(3) == 1);
	
	temp =  TIM3->CNT;
	
	//5,不使能定时器
	 TIM_Cmd(TIM3, DISABLE);	
	
	distance = temp/58;
	
	return distance;
}


