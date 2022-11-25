#include "pwm.h"


/*****************************************
����˵����


TIM2_CH3(TIM2 -- APB1 32λ  84MHZ)

*****************************************/


void Pwm_FA2_Init(void)
{
	
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef			TIM_OCInitStruct;
	
	//GPIO�ṹ��
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//��2��ʹ�ܶ�ʱ��14�����IO��ʱ�ӡ�
	//ʹ�ܶ�ʱ��14ʱ�ӣ�
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//ʹ��GPIOFʱ�ӣ�
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_2;		//����9	
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AF;		//����ģʽ
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;	//�������
	GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_25MHz; //�ٶ�
	GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;		//����
	//��3����ʼ��IO��Ϊ���ù��������
	GPIO_Init(GPIOA, &GPIO_InitStruct);	

	//��4��GPIOF9����ӳ�䵽��ʱ��2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2); 
	
	TIM_TimeBaseInitStruct.TIM_Prescaler	= (84-1); 	//84��Ƶ 84MHZ/84 = 1MHZ
	TIM_TimeBaseInitStruct.TIM_Period		= (1000-1);	//ARR   ��1000,��1MHZ��PWM����1ms
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; //��Ƶ����
	//2����ʼ����ʱ��������ARR,PSC��
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode		= TIM_OCMode_PWM1;	//PWMģʽ1
	TIM_OCInitStruct.TIM_OCPolarity	= TIM_OCPolarity_High;//������Ե�ƽ ������ѡ��ߵ�ƽ
	TIM_OCInitStruct.TIM_Pulse 		= 0; //CCR3 = 0;
	TIM_OCInitStruct.TIM_OutputState= TIM_OutputState_Enable; //ͨ��ʹ��
	//��6����ʼ������Ƚϲ��� OC3 -- ͨ��3
	TIM_OC3Init(TIM2, &TIM_OCInitStruct);
	
	//��7��ʹ��Ԥװ�ؼĴ���  OC3 -- ͨ��3	
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	//��8��ʹ���Զ���װ�ص�Ԥװ�ؼĴ�������λ	
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	//��9��ʹ�ܶ�ʱ����
	TIM_Cmd(TIM2, ENABLE);
}