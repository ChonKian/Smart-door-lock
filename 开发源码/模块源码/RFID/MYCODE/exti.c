#include "exti.h"
#include "delay.h"

/**************************************
����˵����
KEY0 ����PA0�� ѡ���½��ش���
KEY1������PE2  ѡ���½��ش���
KEY2������PE3  ѡ���½��ش���
KEY3������PE4  ѡ���½��ش���
**************************************/




void Exti_PA0_Init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOA clock */
	//GPIOA��GPIOF��ʱ�ӳ�ʼ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	/* Enable SYSCFG clock */
	//ʹ��SYSCFGʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PA0 pin as input floating */
	//PA0����Ϊ����
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  	//����0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//���� ����
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//PE2 3 4����Ϊ����
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;  	//����2 3 4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//���� ����
	
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Connect EXTI Line0 to PA0 pin */
	//�ж������ã��ж���ӳ�䣩  PA0 -- EXTI0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);	
	/* Configure EXTI Line0 */
	//�����жϿ�����
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line0;        	//�ж���0
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;	//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //ѡ���½��ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);
	
		//�����жϿ�����  PE2 �ж���2
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line2;        	//�ж���2
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;	//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //ѡ���½��ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);


      //�����жϿ�����
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line3;        	//�ж���3
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;	//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //ѡ���½��ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);
	
	  //�����жϿ�����
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line4;        	//�ж���4
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;	//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //ѡ���½��ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	//����Ƕ������������
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI0_IRQn;   //ͨ������  ֻ����stm32f4xx.h�в��ң���100���У�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;         //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;		//ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
	
	//����Ƕ������������
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI2_IRQn;   //ͨ������  ֻ����stm32f4xx.h�в���
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;         //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;		//ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);

	//����Ƕ������������
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI3_IRQn;   //ͨ������  ֻ����stm32f4xx.h�в���
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;         //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;		//ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
	
	//����Ƕ������������
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI4_IRQn;   //ͨ������  ֻ����stm32f4xx.h�в���
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;         //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;		//ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
}

//����ʱ
void delay(int n)
{
	int i, j;
	
	for(i=0; i<n; i++)
		for(j=0; j<30000; j++);
}


/*************************************
a �жϷ������ǲ����Զ���ģ�ֻ���Ǵ�startup_stm32f40_41xxx.s�в���
b �жϷ�������ʽ  void  �жϷ�����(void)
c �жϷ������ǲ���Ҫ���е��õģ����������㣬CPU���Զ������жϷ�����ִ��
d �жϷ���������д�������ļ����Լ�д��.c�ļ������У�����ֻ��дһ��

**************************************/
void EXTI0_IRQHandler(void)
{
	//�жϱ�־λ�Ƿ���λ
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		delay(20);
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{

			
		}
		//����ж���0��־λ
		EXTI_ClearITPendingBit(EXTI_Line0);	
	   }
	
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) == SET)
	{
		
		delay(20);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0)
		{

			//����Ƶ�״̬
			GPIO_ToggleBits(GPIOF, GPIO_Pin_10);
		}	
        EXTI_ClearITPendingBit(EXTI_Line2);				
    }
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) == SET)
	{
		//��ʱ20ms
		delay(20);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0)
		{

			//����Ƶ�״̬
			GPIO_ToggleBits(GPIOE, GPIO_Pin_13);
		}
		EXTI_ClearITPendingBit(EXTI_Line3);	
        }
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET)
	{
		//��ʱ20ms
		delay(20);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0)
		{

			//����Ƶ�״̬
			GPIO_ToggleBits(GPIOE, GPIO_Pin_14);
		}	
		//����ж���4��־λ
		EXTI_ClearITPendingBit(EXTI_Line4);	
	}
   
}