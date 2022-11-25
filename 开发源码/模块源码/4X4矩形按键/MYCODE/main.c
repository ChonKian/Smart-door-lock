#include "stm32f4xx.h"
#include "led.h"
#include "key.h"

//����ʱ
void delay(int n)
{
	int i, j;
	
	for(i=0; i<n; i++)
		for(j=0; j<40000; j++);
}




int main(void)
{
	
	unsigned int count = 0;
	
	//��LED��ʼ��
	Led_Init();
	Key_Init();
	
	
	
	while(1)
	{
		//ͨ���жϰ��µ�ʱ������ͬ��Ӳ�����������ֻ��İ���
		//�жϰ����Ƿ���
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{
			//20ms
			delay(20); //��ʱ���жϰ����Ƿ���   �������þ�������
			//�жϰ����Ƿ���
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
			{
				//������ı�������
				count++;
			}
	
		}
		//��1S~2SϢ��1000ms~2000ms
		if(count >= 50 && count <= 100)
		{
			GPIO_ResetBits(GPIOF, GPIO_Pin_9);
		}
		
		//��4S�ػ� 
		if(count >= 200)
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_14);
			count = 0;
		}		
		
		
		//����һ��Ӧ�ó���������� ���С�������
		/*
		//�жϰ����Ƿ���
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{
			delay(15); //��ʱ���жϰ����Ƿ���   �������þ�������
			//�жϰ����Ƿ���
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
			{
				//������ı�������
				//��ѭ�����ȴ������ɿ�
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0);
				
				//�����״̬
				GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
				
			}
	
		}
		*/
	}
	return 0;
}
