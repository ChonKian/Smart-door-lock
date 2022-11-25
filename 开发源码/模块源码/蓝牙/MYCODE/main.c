#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "exti.h"
#include "delay.h"
#include "tim.h"
#include "pwm.h"
#include "usart.h"
#include "string.h"


u8 buffer[32] = {0};
u8 rx_buffer[32] = {0};
u8 count = 0; //0~255   0~127

u8 flag = 0;
u8 data = 0;

void USART3_IRQHandler(void)
{

	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{	//��ձ�־λ
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);	
		//��������
		buffer[count++] = USART_ReceiveData(USART3);
		
		//�ж��Ƿ�Ϊ������ʶ��
		if(buffer[count-1] == ':')
		{
			//��buffer���ݴ洢��rx_buffer��������':'
			for(int i=0; i<(count-1); i++)
			{
				rx_buffer[i] = buffer[i];
			}
			//��ʾ����һ֡�������
			flag = 1;
			//��֤��һ֡���ݴ�buffer[0]��ʼ�洢
			count = 0;
			//�������
			memset(buffer, 0, sizeof(buffer));
			
		}
		
		
		
	}	


}



int main(void)
{
	
	unsigned int count = 0;
	
	
	//����NVIC���� �ڶ����� ��ռ���ȼ���Χ:0~3  ��Ӧ���ȼ���Χ��0~3
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Delay_Init();
	//��LED��ʼ��
	Led_Init();

	Usart1_Init(115200);
	Usart3_Init(9600);
	printf("initialization\r\n");
	USART_SendData(USART3,'1');
	
	while(1)
	{
		if(flag == 1)
		{
			
			printf("rx_buffer:%s\r\n", rx_buffer);
			//����
			if(strcmp(rx_buffer, "HCL11") == 0)
			{
				LED0_ON;
			}
			//���
			if(strcmp(rx_buffer, "HCL10") == 0)
			{
				LED0_OFF;
			}
				
			
			memset(rx_buffer, 0, sizeof(rx_buffer));
			flag = 0;
			printf("ok\r\n");
		}
		
		
	}
	return 0;
}
