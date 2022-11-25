#include "rtc.h"
#include "delay.h"


#define BRK		0x2247

void Rtc_My_Init(void)
{
	RTC_InitTypeDef		RTC_InitStruct;
	RTC_TimeTypeDef		RTC_TimeStruct;
	RTC_DateTypeDef		RTC_DateStruct;
	

	//1��ʹ��PWRʱ�ӣ�
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	//2��ʹ�ܺ󱸼Ĵ�������, ���籣������
	PWR_BackupAccessCmd(ENABLE);
	//3������RTCʱ��Դ��ʹ��RTCʱ�ӣ�
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	//      ���ʹ��LSE��Ҫ��LSE��
	RCC_LSEConfig(RCC_LSE_ON);
	//��ʱ50ms���ȴ�ʱ���ȶ�
	delay_ms(50);


	//���󱸼Ĵ���
	if(RTC_ReadBackupRegister(RTC_BKP_DR0) != BRK)
	{	
	
		RTC_InitStruct.RTC_HourFormat	= RTC_HourFormat_24;  //24Сʱ��
		RTC_InitStruct.RTC_AsynchPrediv = 0x7F;	 //128��Ƶ  �첽ͨ����Ƶ��
		RTC_InitStruct.RTC_SynchPrediv	= 0xFF;	 //256��Ƶ  ͬ��ͨ����Ƶ��
		//4�� ��ʼ��RTC(ͬ��/�첽��Ƶϵ����ʱ�Ӹ�ʽ)��
		RTC_Init(&RTC_InitStruct);
		
		
		RTC_TimeStruct.RTC_H12		= RTC_H12_PM; //����
		RTC_TimeStruct.RTC_Hours	= 15;	//ʱ
		RTC_TimeStruct.RTC_Minutes	= 45;	//��
		RTC_TimeStruct.RTC_Seconds	= 10;	//��
		//5�� ����ʱ�䣺
		RTC_SetTime (RTC_Format_BIN, &RTC_TimeStruct);
		

		RTC_DateStruct.RTC_Year		= 22;	//��
		RTC_DateStruct.RTC_Month	= 10;	//��
		RTC_DateStruct.RTC_Date		= 11;	//��
		RTC_DateStruct.RTC_WeekDay	= 2;	//����
		//6���������ڣ�
		RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);	
		
		//������д��󱸼Ĵ���
		RTC_WriteBackupRegister(RTC_BKP_DR0, BRK);
	
	}
	
}



//����A��ʼ��
void RTC_Alarm_AInit(void)
{
	RTC_AlarmTypeDef	RTC_AlarmStruct;
	RTC_TimeTypeDef 	RTC_AlarmTime; 
	EXTI_InitTypeDef   	EXTI_InitStructure;
	NVIC_InitTypeDef   	NVIC_InitStructure;	
	
	//2���ر����ӣ�
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE); 
	
	//�������ӵ�ʱ��
	RTC_AlarmTime.RTC_H12		= RTC_H12_PM; //����
	RTC_AlarmTime.RTC_Hours		= 15;	//ʱ
	RTC_AlarmTime.RTC_Minutes	= 45;	//��
	RTC_AlarmTime.RTC_Seconds	= 50;	//��
	
	
	RTC_AlarmStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date; //��������������
	RTC_AlarmStruct.RTC_AlarmDateWeekDay    = 11;
	RTC_AlarmStruct.RTC_AlarmMask			= RTC_AlarmMask_None; 	//������λ����ʵ��ʱ����Ӧ����
	RTC_AlarmStruct.RTC_AlarmTime			= RTC_AlarmTime;  		//����ʱ��
	//3���������Ӳ�����
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStruct);
	//4���������ӣ�
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	//5���������������жϣ�     
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);

	//�����жϿ�����
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line17;        	//�ж���0
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;	//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //ѡ�������ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);
	
	//����Ƕ������������
	NVIC_InitStructure.NVIC_IRQChannel 						= RTC_Alarm_IRQn;   //ͨ������  ֻ����stm32f4xx.h�в���
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;         //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;		//ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
}


void RTC_Alarm_IRQHandler(void)
{
	//�жϱ�־λ�Ƿ���λ
	if(EXTI_GetITStatus(EXTI_Line17) == SET)
	{
		//�ж�����A��־�Ƿ���1
		if(RTC_GetFlagStatus(RTC_FLAG_ALRAF) == SET)
		{
			//����
			LED0_ON;
			RTC_ClearFlag(RTC_FLAG_ALRAF);
		}
		
		//����ж���0��־λ
		EXTI_ClearITPendingBit(EXTI_Line17);	
	 }


}

