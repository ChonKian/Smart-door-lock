#include "rtc.h"
#include "delay.h"


#define BRK		0x2247

void Rtc_My_Init(void)
{
	RTC_InitTypeDef		RTC_InitStruct;
	RTC_TimeTypeDef		RTC_TimeStruct;
	RTC_DateTypeDef		RTC_DateStruct;
	

	//1、使能PWR时钟：
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	//2、使能后备寄存器访问, 掉电保存数据
	PWR_BackupAccessCmd(ENABLE);
	//3、配置RTC时钟源，使能RTC时钟：
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	//      如果使用LSE，要打开LSE：
	RCC_LSEConfig(RCC_LSE_ON);
	//延时50ms，等待时钟稳定
	delay_ms(50);


	//读后备寄存器
	if(RTC_ReadBackupRegister(RTC_BKP_DR0) != BRK)
	{	
	
		RTC_InitStruct.RTC_HourFormat	= RTC_HourFormat_24;  //24小时制
		RTC_InitStruct.RTC_AsynchPrediv = 0x7F;	 //128分频  异步通道分频器
		RTC_InitStruct.RTC_SynchPrediv	= 0xFF;	 //256分频  同步通道分频器
		//4、 初始化RTC(同步/异步分频系数和时钟格式)：
		RTC_Init(&RTC_InitStruct);
		
		
		RTC_TimeStruct.RTC_H12		= RTC_H12_PM; //下午
		RTC_TimeStruct.RTC_Hours	= 15;	//时
		RTC_TimeStruct.RTC_Minutes	= 45;	//分
		RTC_TimeStruct.RTC_Seconds	= 10;	//秒
		//5、 设置时间：
		RTC_SetTime (RTC_Format_BIN, &RTC_TimeStruct);
		

		RTC_DateStruct.RTC_Year		= 22;	//年
		RTC_DateStruct.RTC_Month	= 10;	//月
		RTC_DateStruct.RTC_Date		= 11;	//日
		RTC_DateStruct.RTC_WeekDay	= 2;	//星期
		//6、设置日期：
		RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);	
		
		//将数据写入后备寄存器
		RTC_WriteBackupRegister(RTC_BKP_DR0, BRK);
	
	}
	
}



//闹钟A初始化
void RTC_Alarm_AInit(void)
{
	RTC_AlarmTypeDef	RTC_AlarmStruct;
	RTC_TimeTypeDef 	RTC_AlarmTime; 
	EXTI_InitTypeDef   	EXTI_InitStructure;
	NVIC_InitTypeDef   	NVIC_InitStructure;	
	
	//2、关闭闹钟：
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE); 
	
	//设置闹钟的时间
	RTC_AlarmTime.RTC_H12		= RTC_H12_PM; //下午
	RTC_AlarmTime.RTC_Hours		= 15;	//时
	RTC_AlarmTime.RTC_Minutes	= 45;	//分
	RTC_AlarmTime.RTC_Seconds	= 50;	//秒
	
	
	RTC_AlarmStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date; //按日期设置闹钟
	RTC_AlarmStruct.RTC_AlarmDateWeekDay    = 11;
	RTC_AlarmStruct.RTC_AlarmMask			= RTC_AlarmMask_None; 	//无掩码位，按实际时间响应闹钟
	RTC_AlarmStruct.RTC_AlarmTime			= RTC_AlarmTime;  		//设置时间
	//3、配置闹钟参数：
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStruct);
	//4、开启闹钟：
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	//5、开启配置闹钟中断：     
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);

	//配置中断控制器
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line17;        	//中断线0
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;	//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //选择上升沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//中断线使能
	EXTI_Init(&EXTI_InitStructure);
	
	//配置嵌套向量控制器
	NVIC_InitStructure.NVIC_IRQChannel 						= RTC_Alarm_IRQn;   //通道设置  只能在stm32f4xx.h中查找
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;         //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;			//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;		//通道使能
	NVIC_Init(&NVIC_InitStructure);	
}


void RTC_Alarm_IRQHandler(void)
{
	//判断标志位是否置位
	if(EXTI_GetITStatus(EXTI_Line17) == SET)
	{
		//判断闹钟A标志是否置1
		if(RTC_GetFlagStatus(RTC_FLAG_ALRAF) == SET)
		{
			//亮灯
			LED0_ON;
			RTC_ClearFlag(RTC_FLAG_ALRAF);
		}
		
		//清空中断线0标志位
		EXTI_ClearITPendingBit(EXTI_Line17);	
	 }


}

