#include "key.h"
#include "delay.h"
#include "usart.h"
#include "beep.h"
u16 only;
/**************************
����˵��
PD6 PD7 PC6 PC8Ϊ�������
PC11 PE5 PE6 PG9Ϊ��������
**************************/

void Key_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//GPIOD��ʱ��

	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6;    			//���� 6
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT; 			//���ģʽ
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP; 			//�������
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;			//����ٶ�
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     		//����
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_7;    			//���� 7
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT; 			//���ģʽ
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP; 			//�������
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;			//����ٶ�
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     		//����
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6|GPIO_Pin_8;    //����6 8
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT; 			//���ģʽ
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP; 			//�������
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;			//����ٶ�
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     		//����
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	//GPIOC��ʱ��
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_11;    			//����11
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN; 			//����ģʽ
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;			//����ٶ�
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     		//����
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	//GPIOE��ʱ��
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_5;    			//����5
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN; 			//����ģʽ
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;			//����ٶ�
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     		//����
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	//GPIOE��ʱ��
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6;    			//����6
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN; 			//����ģʽ
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;			//����ٶ�
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     		//����
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);	//GPIOG��ʱ��
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9;    			//����9
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN; 			//����ģʽ
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;			//����ٶ�
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     		//����
	GPIO_Init(GPIOG, &GPIO_InitStruct);
}

/*********************************************************************
*����˵���� ����ɨ��
*����ֵ  :  ����ֵ
*����    ��  void
**********************************************************************/
u16 Key_Scan(void)
{
	u16 key_val=100;	//��ʼ����ȡֵ������������ ������Ϊ0����Ϊ������0
	//--------------------------------------��һ�Ű���
	PDout(6) = 0;	PDout(7) = 1;	PCout(6) = 1;	PCout(8) = 1;
	if(PCin(11) == 0)
	{	delay_us(100);
		if(PCin(11) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);
			key_val = 1;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}		
	}
		
	if(PEin(5) == 0)
	{	
		delay_us(100);
		if(PEin(5) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);
			key_val = 4;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	if(PEin(6) == 0)
	{	
		delay_us(100);
		if(PEin(6) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);
			key_val = 7;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	if(PGin(9) == 0)
	{
		delay_us(100);
		if(PGin(9) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);
			key_val = 15;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	//--------------------------------------�ڶ��Ű���
	PDout(6) = 1;	PDout(7) = 0;	PCout(6) = 1;	PCout(8) = 1;
		
	if(PCin(11) == 0)
	{	delay_us(100);
		if(PCin(11) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 2;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
		
	if(PEin(5) == 0)
	{	
		delay_us(100);
		if(PEin(5) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);
			key_val = 5;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	if(PEin(6) == 0)
	{	
		delay_us(100);
		if(PEin(6) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 8;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	if(PGin(9) == 0)
	{
		delay_us(100);
		if(PGin(9) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 0;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	
	//--------------------------------------�����Ű���
	PDout(6) = 1;	PDout(7) = 1;	PCout(6) = 0;	PCout(8) = 1;
		
	if(PCin(11) == 0)
	{	
		delay_us(100);
		if(PCin(11) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 3;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
		
	if(PEin(5) == 0)
	{	
		delay_us(100);
		if(PEin(5) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 6;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	if(PEin(6) == 0)
	{	
		delay_us(100);
		if(PEin(6) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 9;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	if(PGin(9) == 0)
	{
		delay_us(100);
		if(PGin(9) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 14;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	
	//--------------------------------------�����Ű���
	PDout(6) = 1;	PDout(7) = 1;	PCout(6) = 1;	PCout(8) = 0;

	if(PCin(11) == 0)
	{	
		delay_us(100);
		if(PCin(11) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 10;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
		
	if(PEin(5) == 0)
	{	
		delay_us(100);
		if(PEin(5) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 11;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	if(PEin(6) == 0)
	{	
		delay_us(100);
		if(PEin(6) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 12;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}
	if(PGin(9) == 0)
	{
		delay_us(100);
		if(PGin(9) == 0)
		{
			GPIO_SetBits(GPIOF, GPIO_Pin_8);	
			key_val = 13;
			delay_ms(100);
			GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		}
	}	
	return key_val;
}
