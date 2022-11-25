#include "stm32f4xx.h"
#include "led.h"
#include "key.h"

//粗延时
void delay(int n)
{
	int i, j;
	
	for(i=0; i<n; i++)
		for(j=0; j<40000; j++);
}




int main(void)
{
	
	unsigned int count = 0;
	
	//对LED初始化
	Led_Init();
	Key_Init();
	
	
	
	while(1)
	{
		//通过判断按下的时间做不同的硬件动作，如手机的按键
		//判断按键是否按下
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{
			//20ms
			delay(20); //延时再判断按键是否按下   这里作用就是消抖
			//判断按键是否按下
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
			{
				//按键真的被按下了
				count++;
			}
	
		}
		//在1S~2S息屏1000ms~2000ms
		if(count >= 50 && count <= 100)
		{
			GPIO_ResetBits(GPIOF, GPIO_Pin_9);
		}
		
		//在4S关机 
		if(count >= 200)
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_14);
			count = 0;
		}		
		
		
		//这是一种应用场景，如电梯 银行。。。。
		/*
		//判断按键是否按下
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{
			delay(15); //延时再判断按键是否按下   这里作用就是消抖
			//判断按键是否按下
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
			{
				//按键真的被按下了
				//死循环，等待按键松开
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0);
				
				//变更灯状态
				GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
				
			}
	
		}
		*/
	}
	return 0;
}
