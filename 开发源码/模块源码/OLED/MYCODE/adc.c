#include "adc.h"




/**************************************
可调电阻连接在PA5
ADC12_IN5

**************************************/


void Adc_PA5_Init(void)
{
	GPIO_InitTypeDef   		GPIO_InitStruct;
	ADC_CommonInitTypeDef	ADC_CommonInitStruct;
	ADC_InitTypeDef			ADC_InitStruct;
	
	//（2）、开启PA口时钟和ADC1时钟，设置PA5为模拟输入。
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  
	
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_5;  		//引脚5
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AN;		//输入模式
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;		//浮空

	GPIO_Init(GPIOA, &GPIO_InitStruct);   

	
	ADC_CommonInitStruct.ADC_Mode			= ADC_Mode_Independent; //独立模式
	ADC_CommonInitStruct.ADC_Prescaler		= ADC_Prescaler_Div4;	//4分频 21MHZ 分频不能超过36MHZ
	ADC_CommonInitStruct.ADC_DMAAccessMode	= ADC_DMAAccessMode_Disabled; //不使用DMA
	ADC_CommonInitStruct.ADC_TwoSamplingDelay= ADC_TwoSamplingDelay_15Cycles; //两个ADC之前的采样间隔
	//（3）、初始化ADC_CCR寄存器。
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	ADC_InitStruct.ADC_Resolution		= ADC_Resolution_12b;  //12位分辨率ADC 
	ADC_InitStruct.ADC_DataAlign		= ADC_DataAlign_Right; //右对齐
	ADC_InitStruct.ADC_ScanConvMode		= DISABLE;  //不连续采样模式
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;	//单次转换模式
	//ADC_InitStruct.ADC_ExternalTrigConv	 =
	ADC_InitStruct.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;//无边沿
	ADC_InitStruct.ADC_NbrOfConversion = 1; //转换通道数目
	
	
	//（4）、初始化ADC1参数，设置ADC1的工作模式以及规则序列的相关信息。
	ADC_Init(ADC1, &ADC_InitStruct);
	//（5）、使能ADC。
	ADC_Cmd(ADC1, ENABLE);
	//（6）、配置规则通道参数：  ADC_Channel_5:通道5
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_84Cycles);

}

u16 Get_Adc_Value(void)
{
	u16 value = 0;
	
	//开启软件转换：
	ADC_SoftwareStartConv(ADC1);
	//等待转换完成，读取ADC值。
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
	{
	}
	
    value = ADC_GetConversionValue(ADC1);

	return value;
}

