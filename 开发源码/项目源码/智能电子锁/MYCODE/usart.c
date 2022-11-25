#include "usart.h"
#include "stdio.h"

#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
int _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc����   printf ��һ����
int fputc(int ch, FILE *f)
{ 	
	USART_SendData(USART1,ch);  //ͨ�����ڷ�������
	//�ȴ����ݷ������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);      
	return ch;
}



u8 i;
//���ڽ��ջ����� 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 			  //���ͻ���,���USART2_MAX_SEND_LEN�ֽ�

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
vu16 USART2_RX_STA=0;   	
                 
void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���յ�����
	{	 
 
	res =USART_ReceiveData(USART2);		
	if((USART2_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
	{ 		
		if(USART2_RX_STA<USART2_MAX_RECV_LEN)		//�����Խ�������
		{
			TIM_SetCounter(TIM2,0);//���������        				 
			if(USART2_RX_STA==0)		
				TIM_Cmd(TIM2, ENABLE);  //ʹ�ܶ�ʱ��2
			USART2_RX_BUF[USART2_RX_STA++]=res;		//��¼���յ���ֵ	 
		}else 
		{
			USART2_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
		} 
	}  	
 }										 
} 
/*********************************

PA9  ---- USART1_TX(���Ͷ�)
PA10 ---- USART1_RX(���ն�)

**********************************/

void Usart1_Init(int myBaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	//ʹ��GPIOAʱ��
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);

	/* Enable USART clock */
	//ʹ��USART1ʱ��
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//����ӳ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//���ù���
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;	//�ٶ�
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;    
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;	//����9
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//����10
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate 		= myBaudRate;			//������
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;	//�ֳ�
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;		//ֹͣλ
	USART_InitStructure.USART_Parity 		= USART_Parity_No;		//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ȫ˫��
	USART_Init(USART1, &USART_InitStructure);


	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel 						= USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//4���������ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	

	/* Enable USART */
	USART_Cmd(USART1, ENABLE);


}
/**************************************
����˵��

PA2  ---- USART2_TX(���Ͷ�)
PA3 ---- USART2_RX(���ն�)

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
	//����ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,  GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2);
	
	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3; //����2 3
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
	USART_InitStructure.USART_BaudRate 		= bound;			//������
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;	//�ֳ� 8λ
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;		//1λֹͣλ
	USART_InitStructure.USART_Parity 		= USART_Parity_No;		//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //����Ϊȫ˫��ģʽ
	USART_Init(USART2, &USART_InitStructure);
	

	
	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel 					 = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 				 = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//����Ϊ�����жϣ���ʾ�����ݹ�����CPUҪ�жϽ��н��գ�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
	
	/* Enable USART */
	//ʹ�ܴ���
	USART_Cmd(USART2, ENABLE);
	
	Tim2_Init(99,7199);		//10ms�ж�
	USART2_RX_STA=0;		//����
	TIM_Cmd(TIM2,DISABLE);			//�رն�ʱ��4


}
/*********************************

PB10  ---- USART3_TX(���Ͷ�)
PB11  ---- USART3_RX(���ն�)

**********************************/

void Usart3_Init(int myBaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	//ʹ��GPIOBʱ��
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);

	/* Enable USART clock */
	//ʹ��USART3ʱ��
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//����ӳ��
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//���ù���
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;	//�ٶ�
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;    
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;	//����10
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//����11
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate 		= myBaudRate;			//������
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;	//�ֳ�
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;		//ֹͣλ
	USART_InitStructure.USART_Parity 		= USART_Parity_No;		//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ȫ˫��
	USART_Init(USART3, &USART_InitStructure);


	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel 						= USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//4���������ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	

	/* Enable USART */
	USART_Cmd(USART3, ENABLE);
}