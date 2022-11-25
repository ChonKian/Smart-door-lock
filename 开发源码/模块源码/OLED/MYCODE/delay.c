#include "delay.h"

//u32 == unsigned int
u32 my_us = 21;   	 //��21MHZ, 1us��21����
u32 my_ms = 21000;   //��21MHZ, 1ms��21000����

//��������
void Delay_Init(void)
{
	//Systick��ʱ��ʱ��Դ���� HCLK/8 = 21MHZ
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

//nus��Χ��0~798915
void delay_us(int nus)
{
	u32 temp = 0x00;
	
	//������װֵ�Ĵ���
	SysTick->LOAD = my_us*nus - 1;
	//���ü�����ֵΪ0
	SysTick->VAL = 0x00;
	//������ʱ��
	SysTick->CTRL |= (0x01<<0); 
	
	do{
		temp = SysTick->CTRL;
			 //��֤��ʱ����ʹ�ܵ� //�ж�temp��16λ�Ƿ�Ϊ1��Ϊ1�˳���ѯ��
	}while((temp & (0x01<<0))  && !(temp & (0x01<<16)));

	
	//�رն�ʱ��
	SysTick->CTRL &= ~(0x01<<0);
}

//nms��Χ��0~798
void delay_ms(int nms)
{
	u32 temp = 0x00;
	
	//������װֵ�Ĵ���
	SysTick->LOAD = my_ms*nms - 1;
	//���ü�����ֵΪ0
	SysTick->VAL = 0x00;
	//������ʱ��
	SysTick->CTRL |= (0x01<<0); 
	
	do{
		temp = SysTick->CTRL;
			 //��֤��ʱ����ʹ�ܵ� //�ж�temp��16λ�Ƿ�Ϊ1��Ϊ1�˳���ѯ��
	}while((temp & (0x01<<0))  && !(temp & (0x01<<16)));

	
	//�رն�ʱ��
	SysTick->CTRL &= ~(0x01<<0);
}

void delay_s(int ns)
{
	int i;

	for(i=0; i<ns; i++)
	{
		delay_ms(500);
		delay_ms(500);
	}
}

