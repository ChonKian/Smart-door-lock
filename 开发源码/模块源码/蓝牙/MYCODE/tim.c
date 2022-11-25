#include "tim.h"

/**************************************
��ʱ��˵��

TIM3 -- APB1

TIM3Ƶ�ʣ�84MHZ

TIM3Ϊ16λ��ʱ����65535��

**************************************/
void Tim3_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef   		NVIC_InitStructure;
	//1���ܶ�ʱ��ʱ�ӡ�
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
//	TIM_TimeBaseInitStruct.TIM_Prescaler	= (84-1); 	//84��Ƶ 84MHZ/84 = 1MHZ
//	TIM_TimeBaseInitStruct.TIM_Period		= (1000-1);	//ARR   ��1000,��1MHZ����ʱ1ms
//	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up;//���ϼ���
//	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; //��Ƶ����
//	//2����ʼ����ʱ��������ARR,PSC��
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

	TIM_TimeBaseInitStruct.TIM_Prescaler	= (8400-1); 		//8400��Ƶ 84MHZ/8400 = 10000HZ
	TIM_TimeBaseInitStruct.TIM_Period		= (10000-1);		//ARR   ��10000,��10000HZ����ʱ1s
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; //��Ƶ����
	//2����ʼ����ʱ��������ARR,PSC��
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	//3������ʱ���жϣ�����NVIC��
	//NVIC����
	NVIC_InitStructure.NVIC_IRQChannel 						= TIM3_IRQn;  //�ж�ͨ������ ֻ��ȥstm32f4xx.h���в���
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x02;			//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE; 		//�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
	
	
	//4������ TIM3_DIER  ��������ж�
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	//5��ʹ�ܶ�ʱ����
	TIM_Cmd(TIM3, ENABLE);

}


void TIM3_IRQHandler (void)
{
	//SET == 1
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{

		GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
		//��ձ�־λ
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	
	}	

}

