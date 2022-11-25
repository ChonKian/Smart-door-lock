#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "exti.h"
#include "delay.h"
#include "tim.h"
#include "pwm.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include  "as608.h"
#include "as608user.h"
u8 buffer[32] = {0};
u8 rx_buffer[32] = {0};
u8 count = 0, len = 0; //0~255   0~127
u8 flag = 0;
u8 data = 0;
u8 res;
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{	//��ձ�־λ
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
		//��������
		buffer[count++] = USART_ReceiveData(USART1);
		USART_SendData(USART1,buffer[count - 1]); 
		//�ж��Ƿ�Ϊ������ʶ��
		if(buffer[count-1] == ':')
		{
			//��buffer���ݴ洢��rx_buffer��������':'
			for(int i=0; i<(count-1); i++)
			{
				rx_buffer[i] = buffer[i]; 
			}
			
			len = count-1;
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
	//����NVIC���� �ڶ����� ��ռ���ȼ���Χ:0~3  ��Ӧ���ȼ���Χ��0~3
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Delay_Init();
	//��LED��ʼ��
	Led_Init();

	Usart1_Init(115200);
	Usart2_Init(57600);
	PS_StaGPIO_Init();
	while(1)
	{
		Add_FR(res);
		delay_s(1);
	}
}

