#include "exti.h"
#include "delay.h"

/**************************************
引脚说明：
KEY0 连接PA0， 选择下降沿触发
KEY1连接在PE2  选择下降沿触发
KEY2连接在PE3  选择下降沿触发
KEY3连接在PE4  选择下降沿触发
**************************************/




void Exti_PA0_Init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOA clock */
	//GPIOA和GPIOF组时钟初始化
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	/* Enable SYSCFG clock */
	//使能SYSCFG时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PA0 pin as input floating */
	//PA0设置为输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  	//引脚0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上 下拉
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//PE2 3 4设置为输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;  	//引脚2 3 4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上 下拉
	
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Connect EXTI Line0 to PA0 pin */
	//中断线配置（中断线映射）  PA0 -- EXTI0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);	
	/* Configure EXTI Line0 */
	//配置中断控制器
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line0;        	//中断线0
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;	//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //选择下降沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//中断线使能
	EXTI_Init(&EXTI_InitStructure);
	
		//配置中断控制器  PE2 中断线2
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line2;        	//中断线2
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;	//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //选择下降沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//中断线使能
	EXTI_Init(&EXTI_InitStructure);


      //配置中断控制器
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line3;        	//中断线3
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;	//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //选择下降沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//中断线使能
	EXTI_Init(&EXTI_InitStructure);
	
	  //配置中断控制器
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line4;        	//中断线4
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;	//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //选择下降沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//中断线使能
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	//配置嵌套向量控制器
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI0_IRQn;   //通道设置  只能在stm32f4xx.h中查找（在100多行）
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;         //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;		//通道使能
	NVIC_Init(&NVIC_InitStructure);
	
	//配置嵌套向量控制器
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI2_IRQn;   //通道设置  只能在stm32f4xx.h中查找
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;         //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;		//通道使能
	NVIC_Init(&NVIC_InitStructure);

	//配置嵌套向量控制器
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI3_IRQn;   //通道设置  只能在stm32f4xx.h中查找
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;         //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;		//通道使能
	NVIC_Init(&NVIC_InitStructure);
	
	//配置嵌套向量控制器
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI4_IRQn;   //通道设置  只能在stm32f4xx.h中查找
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;         //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;		//通道使能
	NVIC_Init(&NVIC_InitStructure);
}

//粗延时
void delay(int n)
{
	int i, j;
	
	for(i=0; i<n; i++)
		for(j=0; j<30000; j++);
}


/*************************************
a 中断服务函数是不能自定义的，只能是从startup_stm32f40_41xxx.s中查找
b 中断服务函数格式  void  中断服务函数(void)
c 中断服务函数是不需要进行调用的，当条件满足，CPU会自动进入中断服务函数执行
d 中断服务函数可以写在任意文件（自己写的.c文件）当中，但是只能写一次

**************************************/
void EXTI0_IRQHandler(void)
{
	//判断标志位是否置位
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		delay(20);
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{

			
		}
		//清空中断线0标志位
		EXTI_ClearITPendingBit(EXTI_Line0);	
	   }
	
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) == SET)
	{
		
		delay(20);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0)
		{

			//变更灯的状态
			GPIO_ToggleBits(GPIOF, GPIO_Pin_10);
		}	
        EXTI_ClearITPendingBit(EXTI_Line2);				
    }
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) == SET)
	{
		//延时20ms
		delay(20);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0)
		{

			//变更灯的状态
			GPIO_ToggleBits(GPIOE, GPIO_Pin_13);
		}
		EXTI_ClearITPendingBit(EXTI_Line3);	
        }
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET)
	{
		//延时20ms
		delay(20);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0)
		{

			//变更灯的状态
			GPIO_ToggleBits(GPIOE, GPIO_Pin_14);
		}	
		//清空中断线4标志位
		EXTI_ClearITPendingBit(EXTI_Line4);	
	}
   
}