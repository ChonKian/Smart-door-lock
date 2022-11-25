#include "key.h"
#include "delay.h"
#include "usart.h"
#include "beep.h"
u16 only;
/**************************
引脚说明
PD6 PD7 PC6 PC8为推挽输出
PC11 PE5 PE6 PG9为下拉输入
**************************/

void Key_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//GPIOD组时钟

	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6;    			//引脚 6
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT; 			//输出模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP; 			//推挽输出
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;			//输出速度
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     		//上拉
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_7;    			//引脚 7
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT; 			//输出模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP; 			//推挽输出
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;			//输出速度
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     		//上拉
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6|GPIO_Pin_8;    //引脚6 8
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT; 			//输出模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP; 			//推挽输出
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;			//输出速度
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     		//上拉
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	//GPIOC组时钟
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_11;    			//引脚11
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN; 			//输入模式
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;			//输出速度
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     		//上拉
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	//GPIOE组时钟
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_5;    			//引脚5
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN; 			//输入模式
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;			//输出速度
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     		//上拉
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	//GPIOE组时钟
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6;    			//引脚6
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN; 			//输入模式
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;			//输出速度
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     		//上拉
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);	//GPIOG组时钟
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9;    			//引脚9
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN; 			//输入模式
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;			//输出速度
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     		//上拉
	GPIO_Init(GPIOG, &GPIO_InitStruct);
}

/*********************************************************************
*函数说明： 按键扫描
*返回值  :  按键值
*参数    ：  void
**********************************************************************/
u16 Key_Scan(void)
{
	u16 key_val=100;	//初始化获取值可以自行设置 不设置为0是因为键盘有0
	//--------------------------------------第一排按键
	PDout(6) = 0;	PDout(7) = 1;	PCout(6) = 1;	PCout(8) = 1;
	if(PCin(11) == 0)
	{	delay_us(100);
		if(PCin(11) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);
			key_val = 1;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}		
	}
		
	if(PEin(5) == 0)
	{	
		delay_us(100);
		if(PEin(5) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);
			key_val = 4;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	if(PEin(6) == 0)
	{	
		delay_us(100);
		if(PEin(6) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);
			key_val = 7;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	if(PGin(9) == 0)
	{
		delay_us(100);
		if(PGin(9) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);
			key_val = 15;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	//--------------------------------------第二排按键
	PDout(6) = 1;	PDout(7) = 0;	PCout(6) = 1;	PCout(8) = 1;
		
	if(PCin(11) == 0)
	{	delay_us(100);
		if(PCin(11) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 2;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
		
	if(PEin(5) == 0)
	{	
		delay_us(100);
		if(PEin(5) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);
			key_val = 5;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	if(PEin(6) == 0)
	{	
		delay_us(100);
		if(PEin(6) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 8;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	if(PGin(9) == 0)
	{
		delay_us(100);
		if(PGin(9) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 0;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	
	//--------------------------------------第三排按键
	PDout(6) = 1;	PDout(7) = 1;	PCout(6) = 0;	PCout(8) = 1;
		
	if(PCin(11) == 0)
	{	
		delay_us(100);
		if(PCin(11) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 3;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
		
	if(PEin(5) == 0)
	{	
		delay_us(100);
		if(PEin(5) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 6;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	if(PEin(6) == 0)
	{	
		delay_us(100);
		if(PEin(6) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 9;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	if(PGin(9) == 0)
	{
		delay_us(100);
		if(PGin(9) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 14;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	
	//--------------------------------------第四排按键
	PDout(6) = 1;	PDout(7) = 1;	PCout(6) = 1;	PCout(8) = 0;

	if(PCin(11) == 0)
	{	
		delay_us(100);
		if(PCin(11) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 10;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
		
	if(PEin(5) == 0)
	{	
		delay_us(100);
		if(PEin(5) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 11;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	if(PEin(6) == 0)
	{	
		delay_us(100);
		if(PEin(6) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 12;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	if(PGin(9) == 0)
	{
		delay_us(100);
		if(PGin(9) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 13;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}	
	return key_val;
}
