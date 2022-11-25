#include "tim.h"

/**************************************
��ʱ��˵��

TIM2 -- APB1

TIM2Ƶ�ʣ�84MHZ

TIM2Ϊ32λ��ʱ��

**************************************/
void Tim2_Init(u16 arr,u16 psc)
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef   		 NVIC_InitStructure;
	//1���ܶ�ʱ��ʱ�ӡ�
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitStruct.TIM_Prescaler	= arr; 	////��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStruct.TIM_Period		= psc;	//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; //��Ƶ����
	//2����ʼ����ʱ��������ARR,PSC��
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	//3������ʱ���жϣ�����NVIC��
	//NVIC����
	NVIC_InitStructure.NVIC_IRQChannel 						= TIM2_IRQn;  //�ж�ͨ������ ֻ��ȥstm32f4xx.h���в���
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;			//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE; 		//�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
	
	
	//4������ TIM2_DIER  ��������ж�
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	//5��ʹ�ܶ�ʱ����
	TIM_Cmd(TIM2, ENABLE);
}


void TIM2_IRQHandler (void)
{
	//SET == 1
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{

		GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
		//��ձ�־λ
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	
	}	

}

