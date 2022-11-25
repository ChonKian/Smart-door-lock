#include "sr04.h"

/*****************************************
����˵����

PA2��Ϊ��ͨ���
PA3��Ϊ��ͨ����
*****************************************/


void Sr04_Init(void)
{
	//GPIO�ṹ��
	GPIO_InitTypeDef  GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	
	//��GPIOA��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		//1���ܶ�ʱ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_2;		//����2
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;	//���ģʽ
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;	//�������
	GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_25MHz; //�ٶ�
	GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_3;		//����3
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN;		//����ģʽ
	GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
	TIM_TimeBaseInitStruct.TIM_Prescaler	= 84-1;  				// 84��Ƶ 84MHZ/84 = 1MHZ
	TIM_TimeBaseInitStruct.TIM_Period	= 50000-1;					TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up;	// ���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1;			// ��Ƶ����
	//2����ʼ����ʱ��������ARR,PSC
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
			
	//5,��ʹ�ܶ�ʱ��
	TIM_Cmd(TIM3, DISABLE);

}

u16 Get_Sr04_Value(void)
{
	u16 t = 0;
	
	u16 temp = 0, distance;
	//PA2Ϊ�͵�ƽ
	PAout(2) = 0;
	
	delay_us(10);
	//PA2Ϊ�ߵ�ƽ
	PAout(2) = 1;
	
	delay_us(20);
	//PA2Ϊ�͵�ƽ
	PAout(2) = 0;
	
	//���ö�ʱ����CNTΪ0  
	TIM3->CNT = 0;

	t = 0;
	//PA3�ȴ��ߵ�ƽ�������ο������ɿ����� while( ����ƽ  );
	while(PAin(3) == 0)
	{
		delay_us(1);
		
		t++;
		//5000*1 = 5000us = 5ms �ȴ�5msδ�иߵ�ƽ���򷵻�
		if(t >= 5000)
			return 0;
	}
	
	//ʹ�ܶ�ʱ����ʼ����
	TIM_Cmd(TIM3, ENABLE);
	
	t = 0;
	//PA3�ȴ��͵�ƽ�������ο������ɿ�����
	while(PAin(3) == 1)
	{
		delay_us(1);
		
		t++;
		//20000*1 = 20000us = 20ms �ȴ�20msδ�е͵�ƽ���򷵻�
		if(t >= 20000)
			return 0;
	}
		
	
	
	//��ȡ��ʱ��CNTֵ���Ӷ��õ��ߵ�ƽ����ʱ��    
	temp = TIM3->CNT;

	//�رն�ʱ��
	TIM_Cmd(TIM3, DISABLE);
	
	//ͨ����ʽ�������������������
	distance = temp/58;
	
	return distance;
}

