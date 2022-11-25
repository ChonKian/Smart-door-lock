#include "adc.h"


/*****************************************
����˵����

�ɵ�����������PA5
ADC12_IN5

*****************************************/

void Adc_PA5_Init(void)
{
	//GPIO�ṹ��
	GPIO_InitTypeDef  		GPIO_InitStruct;
	ADC_CommonInitTypeDef	ADC_CommonInitStruct;
	ADC_InitTypeDef			ADC_InitStruct;
	
	
	//����PA��ʱ�Ӻ�ADC1ʱ�ӣ�����PA5Ϊģ�����롣
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_5;		//����5
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AN;		//ģ��ģʽ
	GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_NOPULL;	//����
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	

	ADC_CommonInitStruct.ADC_Mode				= ADC_Mode_Independent; //����ģʽ ADC1 ADC2 ADC3����ʹ��
	ADC_CommonInitStruct.ADC_Prescaler			= ADC_Prescaler_Div4;	//4��Ƶ ADCƵ�ʣ�84/4 = 21MHZ ����ܳ���36MHZ
	ADC_CommonInitStruct.ADC_TwoSamplingDelay   = ADC_TwoSamplingDelay_15Cycles; //����ADC֮��ɼ����
	ADC_CommonInitStruct.ADC_DMAAccessMode      = ADC_DMAAccessMode_Disabled;	 //��ʹ��DMA
	//��ʼ��ADC_CCR�Ĵ�����
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	
	ADC_InitStruct.ADC_Resolution			= ADC_Resolution_12b;  	//12λ�ֱ���
	ADC_InitStruct.ADC_ScanConvMode			= DISABLE;				//��ֹɨ��ģʽ
	ADC_InitStruct.ADC_ContinuousConvMode	= DISABLE;				//������ת��
	ADC_InitStruct.ADC_DataAlign			= ADC_DataAlign_Right;	//�����Ҷ���
	//ADC_InitStruct.ADC_ExternalTrigConv		= 
	ADC_InitStruct.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_None; //û�б��ش��� ��ʹ���������
	ADC_InitStruct.ADC_NbrOfConversion		= 1; //ת��ͨ����Ŀ
	
	//��ʼ��ADC1����������ADC1�Ĺ���ģʽ�Լ��������е������Ϣ��
	ADC_Init(ADC1, &ADC_InitStruct);
	
	
	//ʹ��ADC��
	ADC_Cmd(ADC1, ENABLE);
	//���ù���ͨ��������   ADC_SampleTime_15Cycles �������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_15Cycles);
}

u16 Get_Adc_Value(void)
{
	u16 temp;
	
	//�������ת��
	ADC_SoftwareStartConv(ADC1);

	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) ==  RESET  );
	
	
	temp = ADC_GetConversionValue(ADC1);
	
	return temp;
}

