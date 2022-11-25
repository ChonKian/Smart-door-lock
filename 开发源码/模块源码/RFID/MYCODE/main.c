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


u8 rx_flag = 0;  //rx_flag = 1表示接受数据完成
u8 rx_data = 0;
u8 buffer[32] = {0}, rx_buffer[32] = {0};
u8 count = 0, rx_i = 0;



void USART1_IRQHandler(void)
{
	
	//判断接收中断标志是否为1
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		//清空标志位
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
		//接受数据 先赋值再加
		buffer[count++] = USART_ReceiveData(USART1);
		
		//判断接受的字符是否为 ':'
		if(buffer[count-1] == ':')
		{
			//buffer循环赋值给rx_buffer, 但不需要赋值':'  过滤结束标志
			for(rx_i=0; rx_i<(count-1); rx_i++)
			{
				rx_buffer[rx_i] = buffer[rx_i];
			}
			
			memset(buffer, 0, sizeof(buffer));
			
			//确保下一帧存放位置从buffer[0]开始
			count = 0;
			
			rx_flag = 1;
		}
		
	}

}

//MFRC522数据区
u8  mfrc552pidbuf[18];
u8  card_pydebuf[2];
u8  card_numberbuf[5];
u8  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};
u8  card_writebuf[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
u8  card_readbuf[18];

//MFRC522测试函数
void MFRC522Test(void)
{
	u8 i,j,status,card_size;
	//
	status=MFRC522_Request(0x52, card_pydebuf);			//寻卡
	//
	if(status==0)		//如果读到卡
	{
		status=MFRC522_Anticoll(card_numberbuf);			//防撞处理			
		card_size=MFRC522_SelectTag(card_numberbuf);	//选卡
		status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//验卡
		status=MFRC522_Write(4, card_writebuf);				//写卡（写卡要小心，特别是各区的块3）
		status=MFRC522_Read(4, card_readbuf);					//读卡
		//MFRC522_Halt();															//使卡进入休眠状态
		//卡类型显示
		printf("卡类型显示:%#X %#X\r\n", card_pydebuf[0], card_pydebuf[1]);
		
		
		printf("卡序列号:");
		//卡序列号显，最后一字节为卡的校验码
		for(i=0;i<5;i++)
		{
			printf("%#X ", card_numberbuf[i]);
		
		}
		printf("\r\n");
		
		
		//卡容量显示，单位为Kbits
		printf("卡容量显示:%dKbits\r\n", card_size);
		

		//读卡状态显示，正常为0
		//读一个块的数据显示
		printf("读一个块的数据显示:");
		for(i=0;i<2;i++)		//分两行显示
		{
			for(j=0;j<9;j++)	//每行显示8个
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
	

	//主函数
	//NVIC分为第二组，抢占优先级范围:0~3, 响应优先级范围:0~3,
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
