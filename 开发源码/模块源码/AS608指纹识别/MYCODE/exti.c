#include "exti.h"

/*****************************************
����˵����

KEY0 ����PA0��ѡ���½��ش���
*****************************************/



void delay1(int n)
{
	int i, j;
	
	for(i=0; i<n; i++)
		for(j=0; j<40000; j++);
}


void Exti_PA0_Init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOA clock */
	//ʹ�ܣ��򿪣�GPIOA��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	/* Enable SYSCFG clock */
	//ʹ��SYSCFG��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PA0 pin as input floating */
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;   	//����ģʽ
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;	//����ģʽ
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0;		//����0
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Connect EXTI Line0 to PA0 pin */
	//PA0����--EXTI0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	/* Configure EXTI Line0 */
	//�����жϿ�����
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line0;			//�ж���0
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;	//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //���ش��� �½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//�ж���ʹ��	
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	//NVIC����
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI0_IRQn;  //�ж�ͨ������ ֻ��ȥstm32f4xx.h���в���
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;			//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE; 		//�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);


}

/**************************************

a �жϷ������ǲ����Զ���ģ�ֻ���Ǵ�startup_stm32f40_41xxx.s�в���
b �жϷ�������ʽ  void  �жϷ�����(void)
c �жϷ������ǲ���Ҫ���е��õģ����������㣬CPU���Զ������жϷ�����ִ��
d �жϷ���������д�������ļ����Լ�д��.c�ļ������У�����ֻ��дһ��
e �жϷ�������ִ��ʱ����Խ϶̵ĳ���

***************************************/
void EXTI0_IRQHandler(void)
{
	//SET == 1
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
			//�жϵ�������Խϳ����飬�ִ��ж��ٴν���
			delay1(10);

		    if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0))
			{
				//�����״̬
				GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
			}
		
			//����жϱ�־0��־λ
			EXTI_ClearITPendingBit(EXTI_Line0);	
	}	
	

}

