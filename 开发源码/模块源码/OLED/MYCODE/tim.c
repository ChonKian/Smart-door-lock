#include "tim.h"

/**************************************
定时器属于芯片外设

TIM3挂在APB1总线  定时器频率：84MHZ

TIM3属于16位定时器（计数范围：0~65535）
**************************************/
void Tim3_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	
	//1、能定时器时钟。
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/*
	TIM_TimeBaseInitStruct.TIM_Prescaler	= (84-1);   	//84分频 84MHZ/84 = 1MHZ
	TIM_TimeBaseInitStruct.TIM_Period		= (1000-1);		//计数周期（重载值寄存器）；在1MHZ下，计1000个数，用时1ms
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1;		//分频因子
	//2、初始化定时器，配置ARR,PSC。
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	*/
	TIM_TimeBaseInitStruct.TIM_Prescaler	= (8400-1);   	//8400分频 84MHZ/8400 = 10000HZ
	TIM_TimeBaseInitStruct.TIM_Period		= (10000-1);		//计数周期（重载值寄存器）；在10000HZ下，计10000个数，用时1s
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1;		//分频因子
	//2、初始化定时器，配置ARR,PSC。
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);	
	
	//配置嵌套向量控制器
	NVIC_InitStructure.NVIC_IRQChannel 						= TIM3_IRQn;   //通道设置  只能在stm32f4xx.h中查找
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;         //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;		//通道使能
		//3、启定时器中断，配置NVIC。
	NVIC_Init(&NVIC_InitStructure);	


	
	//4、设置 TIM3_DIER  允许更新中断
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	//5、使能定时器。
	TIM_Cmd(TIM3, ENABLE);
}

//每隔1ms执行中断服务函数
void TIM3_IRQHandler(void)
{

	//判断更新中断标志是否为1
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{

		//变更灯的状态
		GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
	
		//清空中断线0标志位
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	
	}
}	

