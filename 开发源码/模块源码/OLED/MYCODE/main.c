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







int main(void)
{
	unsigned char i;
	extern const unsigned char BMP1[];
	
	
	//NVIC分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Delay_Init();
	Led_Init();

	Usart1_Init(115200);

	I2C_Configuration();
	OLED_Init();

	//显示字符
//	OLED_Fill(0x00);//全屏灭
//	OLED_ShowStr(0,4,"Hello Tech",1);				//测试8*16字符
//	while(1);
	
	//显示汉字
//	OLED_Fill(0x00);//全屏灭
//	for(i=5;i<9;i++)
//	{
//		OLED_ShowCN(22+(i-5)*16,0,i);//测试显示中文
//	}
//	while(1);
	
	//显示图片
	OLED_Fill(0x00);//全屏灭
	OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//测试BMP位图显示
	while(1);
	
	
	while(1)
	{
	
		OLED_Fill(0xFF);//全屏点亮
		delay_s(2);
		OLED_Fill(0x00);//全屏灭
		delay_s(2);
		for(i=0;i<5;i++)
		{
			OLED_ShowCN(22+i*16,0,i);//测试显示中文
		}
		delay_s(2);
		OLED_ShowStr(0,3,"HelTec Automation",1);//测试6*8字符
		OLED_ShowStr(0,4,"Hello Tech",2);				//测试8*16字符
		delay_s(2);
		OLED_CLS();//清屏
		OLED_OFF();//测试OLED休眠
		delay_s(2);
		OLED_ON();//测试OLED休眠后唤醒
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//测试BMP位图显示
		delay_s(2);
	
	}

}
