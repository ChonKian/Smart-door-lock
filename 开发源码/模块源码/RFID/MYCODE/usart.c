#include "usart.h"
#include "stdio.h"

#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
int _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数   printf 是一个宏
int fputc(int ch, FILE *f)
{ 	
	USART_SendData(USART1,ch);  //通过串口发送数据
	//等待数据发送完毕
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);      
	return ch;
}











/**************************************
引脚说明

PA9  ---- USART1_TX(发送端)
PA10 ---- USART1_RX(接收端)

**************************************/
void Usart1_Init(int MyBaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* Connect USART pins to AF7 */
	//引脚映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,  GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10, GPIO_AF_USART1);
	
	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; //引脚9 10
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
	USART_InitStructure.USART_BaudRate 		= MyBaudRate;			//波特率
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;	//字长 8位
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;		//1位停止位
	USART_InitStructure.USART_Parity 		= USART_Parity_No;		//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件控制流
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //设置为全双工模式
	USART_Init(USART1, &USART_InitStructure);
	

	
	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel 					 = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 				 = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//配置为接收中断（表示有数据过来，CPU要中断进行接收）
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
	
	/* Enable USART */
	//使能串口
	USART_Cmd(USART1, ENABLE);

}
