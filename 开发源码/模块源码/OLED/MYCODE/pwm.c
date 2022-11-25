#include "pwm.h"


/****************************************
����˵����

LED0������PF9,�͵�ƽ�������ߵ�ƽ������
TIM14_CH1(TIM14 -- APB1 16λ  84MHZ)

*****************************************/

void Pwm_PF9_Init(void)
{
	GPIO_InitTypeDef   			GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef			TIM_OCInitStruct;
	
	//��2��ʹ�ܶ�ʱ��14�����IO��ʱ�ӡ�
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	
	//��3����ʼ��IO��Ϊ���ù��������

	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_9;  		//����9
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AF;	    //����ģʽ
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP; 	//����
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_25MHz;	//�ٶ�
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;		//����
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	
	
	//��4��GPIOF9����ӳ�䵽��ʱ��14
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); 
	
	
	//����PWM���ڣ�1ms
	TIM_TimeBaseInitStruct.TIM_Prescaler	= (84-1);   	//84��Ƶ 84MHZ/84 = 1MHZ
	TIM_TimeBaseInitStruct.TIM_Period		= (1000-1);		//�������ڣ�����ֵ�Ĵ���������1MHZ�£���1000��������ʱ1ms
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1;		//��Ƶ����
	//2����ʼ����ʱ��������ARR,PSC��
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseInitStruct);
	
	
	TIM_OCInitStruct.TIM_OCMode		= TIM_OCMode_PWM1;  		//PWM1ģʽ
	TIM_OCInitStruct.TIM_OutputState= TIM_OutputState_Enable;	//���ʹ��
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;   	//������ԣ�����ѡ��������ԣ��͵�ƽ
	//��6����ʼ������Ƚϲ�����OC1--ͨ��1
	TIM_OC1Init(TIM14, &TIM_OCInitStruct);
	
	//��7��ʹ��Ԥװ�ؼĴ���  OC1--ͨ��1
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable); 
	
	//��8��ʹ���Զ���װ�ص�Ԥװ�ؼĴ�������λ	
	TIM_ARRPreloadConfig(TIM14,ENABLE);
	
	//��9��ʹ�ܶ�ʱ����
	TIM_Cmd(TIM14, ENABLE);
}



