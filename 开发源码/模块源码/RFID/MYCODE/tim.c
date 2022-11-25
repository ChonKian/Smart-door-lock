#include "tim.h"

/**************************************
��ʱ������оƬ����

TIM3����APB1����  ��ʱ��Ƶ�ʣ�84MHZ

TIM3����16λ��ʱ����������Χ��0~65535��
**************************************/
void Tim3_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	
	//1���ܶ�ʱ��ʱ�ӡ�
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/*
	TIM_TimeBaseInitStruct.TIM_Prescaler	= (84-1);   	//84��Ƶ 84MHZ/84 = 1MHZ
	TIM_TimeBaseInitStruct.TIM_Period		= (1000-1);		//�������ڣ�����ֵ�Ĵ���������1MHZ�£���1000��������ʱ1ms
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1;		//��Ƶ����
	//2����ʼ����ʱ��������ARR,PSC��
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	*/
	TIM_TimeBaseInitStruct.TIM_Prescaler	= (8400-1);   	//8400��Ƶ 84MHZ/8400 = 10000HZ
	TIM_TimeBaseInitStruct.TIM_Period		= (10000-1);		//�������ڣ�����ֵ�Ĵ���������10000HZ�£���10000��������ʱ1s
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1;		//��Ƶ����
	//2����ʼ����ʱ��������ARR,PSC��
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);	
	
	//����Ƕ������������
	NVIC_InitStructure.NVIC_IRQChannel 						= TIM3_IRQn;   //ͨ������  ֻ����stm32f4xx.h�в���
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;         //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;		//ͨ��ʹ��
		//3������ʱ���жϣ�����NVIC��
	NVIC_Init(&NVIC_InitStructure);	


	
	//4������ TIM3_DIER  ��������ж�
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	//5��ʹ�ܶ�ʱ����
	TIM_Cmd(TIM3, ENABLE);
}

//ÿ��1msִ���жϷ�����
void TIM3_IRQHandler(void)
{

	//�жϸ����жϱ�־�Ƿ�Ϊ1
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{

		//����Ƶ�״̬
		GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
	
		//����ж���0��־λ
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	
	}
}	

