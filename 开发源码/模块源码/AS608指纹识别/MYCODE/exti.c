#include "exti.h"

/*****************************************
引脚说明：

KEY0 连接PA0，选择下降沿触发
*****************************************/



void delay1(int n)
{
	int i, j;
	
	for(i=0; i<n; i++)
		for(j=0; j<40000; j++);
}


void Exti_PA0_Init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOA clock */
	//使能（打开）GPIOA组时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	/* Enable SYSCFG clock */
	//使能SYSCFG组时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PA0 pin as input floating */
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;   	//输入模式
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;	//浮空模式
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0;		//引脚0
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Connect EXTI Line0 to PA0 pin */
	//PA0引脚--EXTI0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	/* Configure EXTI Line0 */
	//配置中断控制器
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line0;			//中断线0
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;	//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //边沿触发 下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//中断线使能	
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	//NVIC配置
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI0_IRQn;  //中断通道设置 只能去stm32f4xx.h当中查找
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;			//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE; 		//中断通道使能
	NVIC_Init(&NVIC_InitStructure);


}

/**************************************

a 中断服务函数是不能自定义的，只能是从startup_stm32f40_41xxx.s中查找
b 中断服务函数格式  void  中断服务函数(void)
c 中断服务函数是不需要进行调用的，当条件满足，CPU会自动进入中断服务函数执行
d 中断服务函数可以写在任意文件（自己写的.c文件）当中，但是只能写一次
e 中断服务函数是执行时间相对较短的程序

***************************************/
void EXTI0_IRQHandler(void)
{
	//SET == 1
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
			//中断当中做相对较长事情，抵触中断再次进来
			delay1(10);

		    if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0))
			{
				//变更灯状态
				GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
			}
		
			//清空中断标志0标志位
			EXTI_ClearITPendingBit(EXTI_Line0);	
	}	
	

}

