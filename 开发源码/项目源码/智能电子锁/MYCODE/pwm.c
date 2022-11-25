#include "pwm.h"


/*****************************************
引脚说明：


TIM2_CH3(TIM2 -- APB1 32位  84MHZ)

*****************************************/


void Pwm_FA2_Init(void)
{
	
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef			TIM_OCInitStruct;
	
	//GPIO结构体
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//（2）使能定时器14和相关IO口时钟。
	//使能定时器14时钟：
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//使能GPIOF时钟：
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_2;		//引脚9	
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AF;		//复用模式
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;	//输出推挽
	GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_25MHz; //速度
	GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;		//上拉
	//（3）初始化IO口为复用功能输出。
	GPIO_Init(GPIOA, &GPIO_InitStruct);	

	//（4）GPIOF9复用映射到定时器2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2); 
	
	TIM_TimeBaseInitStruct.TIM_Prescaler	= (84-1); 	//84分频 84MHZ/84 = 1MHZ
	TIM_TimeBaseInitStruct.TIM_Period		= (1000-1);	//ARR   计1000,在1MHZ，PWM周期1ms
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; //分频因子
	//2、初始化定时器，配置ARR,PSC。
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode		= TIM_OCMode_PWM1;	//PWM模式1
	TIM_OCInitStruct.TIM_OCPolarity	= TIM_OCPolarity_High;//输出极性电平 ，这里选择高电平
	TIM_OCInitStruct.TIM_Pulse 		= 0; //CCR3 = 0;
	TIM_OCInitStruct.TIM_OutputState= TIM_OutputState_Enable; //通道使能
	//（6）初始化输出比较参数 OC3 -- 通道3
	TIM_OC3Init(TIM2, &TIM_OCInitStruct);
	
	//（7）使能预装载寄存器  OC3 -- 通道3	
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	//（8）使能自动重装载的预装载寄存器允许位	
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	//（9）使能定时器。
	TIM_Cmd(TIM2, ENABLE);
}