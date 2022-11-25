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
	{	//清空标志位
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);	
		//接收数据
		buffer[count++] = USART_ReceiveData(USART3);
		
		//判断是否为结束标识符
		if(buffer[count-1] == ':')
		{
			//将buffer数据存储在rx_buffer，并过滤':'
			for(int i=0; i<(count-1); i++)
			{
				rx_buffer[i] = buffer[i];
			}
			//表示接受一帧数据完成
			flag = 1;
			//保证下一帧数据从buffer[0]开始存储
			count = 0;
			//清空数组
			memset(buffer, 0, sizeof(buffer));
			
		}
		
		
		
	}	


}



int main(void)
{
	
	unsigned int count = 0;
	
	
	//设置NVIC分组 第二分组 抢占优先级范围:0~3  响应优先级范围：0~3
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Delay_Init();
	//对LED初始化
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
			//开灯
			if(strcmp(rx_buffer, "HCL11") == 0)
			{
				LED0_ON;
			}
			//灭灯
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
