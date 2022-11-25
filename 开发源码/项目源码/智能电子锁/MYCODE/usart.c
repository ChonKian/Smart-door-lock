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



u8 i;
//串口接收缓存区 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 			  //发送缓冲,最大USART2_MAX_SEND_LEN字节

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
vu16 USART2_RX_STA=0;   	
                 
void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
	{	 
 
	res =USART_ReceiveData(USART2);		
	if((USART2_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
	{ 		
		if(USART2_RX_STA<USART2_MAX_RECV_LEN)		//还可以接收数据
		{
			TIM_SetCounter(TIM2,0);//计数器清空        				 
			if(USART2_RX_STA==0)		
				TIM_Cmd(TIM2, ENABLE);  //使能定时器2
			USART2_RX_BUF[USART2_RX_STA++]=res;		//记录接收到的值	 
		}else 
		{
			USART2_RX_STA|=1<<15;					//强制标记接收完成
		} 
	}  	
 }										 
} 
/*********************************

PA9  ---- USART1_TX(发送端)
PA10 ---- USART1_RX(接收端)

**********************************/

void Usart1_Init(int myBaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	//使能GPIOA时钟
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);

	/* Enable USART clock */
	//使能USART1时钟
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//引脚映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//复用功能
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;	//速度
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;    
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;	//引脚9
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//引脚10
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate 		= myBaudRate;			//波特率
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;	//字长
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;		//停止位
	USART_InitStructure.USART_Parity 		= USART_Parity_No;		//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件控制流
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//全双工
	USART_Init(USART1, &USART_InitStructure);


	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel 						= USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//4、检测接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	

	/* Enable USART */
	USART_Cmd(USART1, ENABLE);


}
/**************************************
引脚说明

PA2  ---- USART2_TX(发送端)
PA3 ---- USART2_RX(接收端)

**************************************/
void Usart2_Init(u32 bound)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/* Enable USART clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	/* Connect USART pins to AF7 */
	//引脚映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,  GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2);
	
	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3; //引脚2 3
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
	USART_InitStructure.USART_BaudRate 		= bound;			//波特率
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;	//字长 8位
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;		//1位停止位
	USART_InitStructure.USART_Parity 		= USART_Parity_No;		//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件控制流
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //设置为全双工模式
	USART_Init(USART2, &USART_InitStructure);
	

	
	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel 					 = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 				 = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//配置为接收中断（表示有数据过来，CPU要中断进行接收）
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
	
	/* Enable USART */
	//使能串口
	USART_Cmd(USART2, ENABLE);
	
	Tim2_Init(99,7199);		//10ms中断
	USART2_RX_STA=0;		//清零
	TIM_Cmd(TIM2,DISABLE);			//关闭定时器4


}
/*********************************

PB10  ---- USART3_TX(发送端)
PB11  ---- USART3_RX(接收端)

**********************************/

void Usart3_Init(int myBaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	//使能GPIOB时钟
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);

	/* Enable USART clock */
	//使能USART3时钟
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//引脚映射
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//复用功能
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;	//速度
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;    
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;	//引脚10
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//引脚11
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate 		= myBaudRate;			//波特率
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;	//字长
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;		//停止位
	USART_InitStructure.USART_Parity 		= USART_Parity_No;		//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件控制流
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//全双工
	USART_Init(USART3, &USART_InitStructure);


	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel 						= USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//4、检测接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	

	/* Enable USART */
	USART_Cmd(USART3, ENABLE);
}