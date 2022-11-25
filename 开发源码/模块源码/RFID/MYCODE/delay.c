#include "delay.h"

//u32 == unsigned int
u32 my_us = 21;   	 //在21MHZ, 1us计21个数
u32 my_ms = 21000;   //在21MHZ, 1ms计21000个数

//函数声明
void Delay_Init(void)
{
	//Systick定时器时钟源配置 HCLK/8 = 21MHZ
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

//nus范围：0~798915
void delay_us(int nus)
{
	u32 temp = 0x00;
	
	//设置重装值寄存器
	SysTick->LOAD = my_us*nus - 1;
	//设置计数的值为0
	SysTick->VAL = 0x00;
	//启动定时器
	SysTick->CTRL |= (0x01<<0); 
	
	do{
		temp = SysTick->CTRL;
			 //保证定时器是使能的 //判断temp第16位是否为1，为1退出查询。
	}while((temp & (0x01<<0))  && !(temp & (0x01<<16)));

	
	//关闭定时器
	SysTick->CTRL &= ~(0x01<<0);
}

//nms范围：0~798
void delay_ms(int nms)
{
	u32 temp = 0x00;
	
	//设置重装值寄存器
	SysTick->LOAD = my_ms*nms - 1;
	//设置计数的值为0
	SysTick->VAL = 0x00;
	//启动定时器
	SysTick->CTRL |= (0x01<<0); 
	
	do{
		temp = SysTick->CTRL;
			 //保证定时器是使能的 //判断temp第16位是否为1，为1退出查询。
	}while((temp & (0x01<<0))  && !(temp & (0x01<<16)));

	
	//关闭定时器
	SysTick->CTRL &= ~(0x01<<0);
}

void delay_s(int ns)
{
	int i;

	for(i=0; i<ns; i++)
	{
		delay_ms(500);
		delay_ms(500);
	}
}

