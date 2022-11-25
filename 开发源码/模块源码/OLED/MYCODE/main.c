#include "stm32f4xx.h"
#include "beep.h"
#include "key.h"
#include "delay.h"
#include "led.h"
#include "tim.h"
#include "pwm.h"
#include "usart.h"
#include "string.h"
#include "sr04.h"
#include "dht11.h"
#include "iwdg.h"
#include "exti.h"
#include "rtc.h"
#include "adc.h"
#include "iic.h"
#include "OLED_I2C.h"



u8 rx_flag = 0;  //rx_flag = 1��ʾ�����������
u8 rx_data = 0;
u8 buffer[32] = {0}, rx_buffer[32] = {0};
u8 count = 0, rx_i = 0;



void USART1_IRQHandler(void)
{
	
	//�жϽ����жϱ�־�Ƿ�Ϊ1
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		//��ձ�־λ
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
		//�������� �ȸ�ֵ�ټ�
		buffer[count++] = USART_ReceiveData(USART1);
		
		//�жϽ��ܵ��ַ��Ƿ�Ϊ ':'
		if(buffer[count-1] == ':')
		{
			//bufferѭ����ֵ��rx_buffer, ������Ҫ��ֵ':'  ���˽�����־
			for(rx_i=0; rx_i<(count-1); rx_i++)
			{
				rx_buffer[rx_i] = buffer[rx_i];
			}
			
			memset(buffer, 0, sizeof(buffer));
			
			//ȷ����һ֡���λ�ô�buffer[0]��ʼ
			count = 0;
			
			rx_flag = 1;
		}
		
	}

}







int main(void)
{
	unsigned char i;
	extern const unsigned char BMP1[];
	
	
	//NVIC����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Delay_Init();
	Led_Init();

	Usart1_Init(115200);

	I2C_Configuration();
	OLED_Init();

	//��ʾ�ַ�
//	OLED_Fill(0x00);//ȫ����
//	OLED_ShowStr(0,4,"Hello Tech",1);				//����8*16�ַ�
//	while(1);
	
	//��ʾ����
//	OLED_Fill(0x00);//ȫ����
//	for(i=5;i<9;i++)
//	{
//		OLED_ShowCN(22+(i-5)*16,0,i);//������ʾ����
//	}
//	while(1);
	
	//��ʾͼƬ
	OLED_Fill(0x00);//ȫ����
	OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ
	while(1);
	
	
	while(1)
	{
	
		OLED_Fill(0xFF);//ȫ������
		delay_s(2);
		OLED_Fill(0x00);//ȫ����
		delay_s(2);
		for(i=0;i<5;i++)
		{
			OLED_ShowCN(22+i*16,0,i);//������ʾ����
		}
		delay_s(2);
		OLED_ShowStr(0,3,"HelTec Automation",1);//����6*8�ַ�
		OLED_ShowStr(0,4,"Hello Tech",2);				//����8*16�ַ�
		delay_s(2);
		OLED_CLS();//����
		OLED_OFF();//����OLED����
		delay_s(2);
		OLED_ON();//����OLED���ߺ���
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ
		delay_s(2);
	
	}

}
