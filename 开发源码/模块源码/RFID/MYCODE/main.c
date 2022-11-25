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
#include "flash.h"
#include "spiflash.h"
#include "MFRC522.h" 


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

//MFRC522������
u8  mfrc552pidbuf[18];
u8  card_pydebuf[2];
u8  card_numberbuf[5];
u8  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};
u8  card_writebuf[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
u8  card_readbuf[18];

//MFRC522���Ժ���
void MFRC522Test(void)
{
	u8 i,j,status,card_size;
	//
	status=MFRC522_Request(0x52, card_pydebuf);			//Ѱ��
	//
	if(status==0)		//���������
	{
		status=MFRC522_Anticoll(card_numberbuf);			//��ײ����			
		card_size=MFRC522_SelectTag(card_numberbuf);	//ѡ��
		status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//�鿨
		status=MFRC522_Write(4, card_writebuf);				//д����д��ҪС�ģ��ر��Ǹ����Ŀ�3��
		status=MFRC522_Read(4, card_readbuf);					//����
		//MFRC522_Halt();															//ʹ����������״̬
		//��������ʾ
		printf("��������ʾ:%#X %#X\r\n", card_pydebuf[0], card_pydebuf[1]);
		
		
		printf("�����к�:");
		//�����к��ԣ����һ�ֽ�Ϊ����У����
		for(i=0;i<5;i++)
		{
			printf("%#X ", card_numberbuf[i]);
		
		}
		printf("\r\n");
		
		
		//��������ʾ����λΪKbits
		printf("��������ʾ:%dKbits\r\n", card_size);
		

		//����״̬��ʾ������Ϊ0
		//��һ�����������ʾ
		printf("��һ�����������ʾ:");
		for(i=0;i<2;i++)		//��������ʾ
		{
			for(j=0;j<9;j++)	//ÿ����ʾ8��
			{
				printf("%#X ", card_readbuf[j+i*9]);

			}
		}
		printf("\r\n");

	}	
}



int main(void)
{
	u16 id;
	

	//������
	//NVIC��Ϊ�ڶ��飬��ռ���ȼ���Χ:0~3, ��Ӧ���ȼ���Χ:0~3,
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	Delay_Init();
	
	Usart1_Init(115200);
	printf("RFID test\n");
	MFRC522_Initializtion();
	
	
	while(1)
	{
	
		MFRC522_Initializtion();	
		MFRC522Test();
		delay_s(1);
	
	}

}
