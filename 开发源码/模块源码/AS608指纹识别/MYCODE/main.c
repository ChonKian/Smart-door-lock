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
	{	//清空标志位
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
		//接收数据
		buffer[count++] = USART_ReceiveData(USART1);
		USART_SendData(USART1,buffer[count - 1]); 
		//判断是否为结束标识符
		if(buffer[count-1] == ':')
		{
			//将buffer数据存储在rx_buffer，并过滤':'
			for(int i=0; i<(count-1); i++)
			{
				rx_buffer[i] = buffer[i]; 
			}
			
			len = count-1;
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
	//设置NVIC分组 第二分组 抢占优先级范围:0~3  响应优先级范围：0~3
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Delay_Init();
	//对LED初始化
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

