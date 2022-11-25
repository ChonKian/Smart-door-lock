#include "iic.h"

/****************************************
����˵��
SCL -- PB8
SDA -- PB9

*****************************************/

void Iic_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//��GPIOB��ʱ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	

	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9|GPIO_Pin_8;		//����8
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;				//���ģʽ
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_OD;				//��©���
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;					//����
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;				//�ٶ�
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
	
	//����״̬
	SCL = 1;
	SDA_OUT = 1;
}

//��������ģʽ
void Iic_Sda_Mode(GPIOMode_TypeDef mode)
{

	GPIO_InitTypeDef  GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9;		//����8
	GPIO_InitStruct.GPIO_Mode	= mode;							//���ģʽ
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_OD;				//��©���
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;					//����
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;				//�ٶ�
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
}



//�����ź�
void Iic_Start(void)
{
	
	Iic_Sda_Mode(GPIO_Mode_OUT);
	
	SCL = 1;
	SDA_OUT = 1;
	
	delay_us(5);
	
	SDA_OUT = 0;
	delay_us(5);
	SCL = 0;
	
}


//ֹͣ�ź�
void Iic_Stop(void)
{
    Iic_Sda_Mode(GPIO_Mode_OUT);

    SCL = 0;
    SDA_OUT = 0;

    delay_us(5);

    SCL = 1;
    delay_us(5);
    SDA_OUT = 1;
}



//����һλ���� 1  0
void Iic_Send_Ack(u8 ack)
{
    Iic_Sda_Mode(GPIO_Mode_OUT);
	
	
	SCL = 0;
	
	//׼������
	if(ack == 1)
	{
		SDA_OUT = 1; //�������1
	}
	else
	{
		SDA_OUT = 0; //�������0
	}
	
	
	delay_us(5);
	SCL = 1;
	delay_us(5);
	SCL = 0;

}

//����һ���ֽ�(1�ֽ���8λ) data:0x87(1 0 0 0 0 1 1 1)
void Iic_Send_Byte(u8 data)
{
	int i;
	
	Iic_Sda_Mode(GPIO_Mode_OUT);	
	
	SCL = 0;
	for(i=0; i<8; i++)
	{
		//׼������ �ȷ���λ���� 
		if(data &(0x01<<(7-i)))  
		{
			SDA_OUT = 1;//�������1
		}
		else
		{
			SDA_OUT = 0;//�������0
		}
		
		
		
		delay_us(5);
		SCL = 1;
		delay_us(5);
		SCL = 0;	
	}
	
}

//����һλ����
u8 Iic_Recv_Ack(void)
{
	u8 ack = 0;
	
    Iic_Sda_Mode(GPIO_Mode_IN);

	SCL = 0;

	delay_us(5);
	SCL = 1;
	delay_us(5);
	
	if(SDA_IN == 1) //�ж����ŵ�ƽ
	{
		ack = 1;
	}
	if(SDA_IN == 0)//�ж����ŵ�ƽ
	{
		ack = 0;
	}	
	
	SCL = 0;

	
	return ack;

}


//����һ���ֽ�����
u8 Iic_Recv_Byte(void)
{
	u8 data = 0x00;
	int i;
    Iic_Sda_Mode(GPIO_Mode_IN);	

	SCL = 0;
	for(i=0; i<8; i++)
	{

		delay_us(5);
		SCL = 1;
		delay_us(5);
		
		if(SDA_IN == 1)
		{
			data |= 0x01<<(7-i);
		}
	
		SCL = 0;	
	}
	
	
	return data;
}

//��AT24C02дҳ����
void At24c02_Write_Page(u8 addr, u8 *write_buff, u8 len)
{
	u8 ack;
	
	//�����ź�
	Iic_Start();
	//STM32�����豸��ַ��ִ��д����
	Iic_Send_Byte(0xA0);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack1 failure\r\n");
		return;
	}
	
	//STM32����д���ݵ���ʼ��ַ
	Iic_Send_Byte(addr);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack2 failure\r\n");
		return;
	}	
	
	while(len--)
	{
		//��������
		Iic_Send_Byte(*write_buff);
		ack = Iic_Recv_Ack();
		if(ack == 1)
		{
			printf("ack3 failure\r\n");
			return;
		}	
		//��ַ�Լӣ�ָ����һ������
		write_buff++;
	}
	
	//ֹͣ�ź�
	 Iic_Stop();	
	
	printf("write finish\r\n");
}



//������
void At24c02_Read(u8 addr, u8 *read_buff, u8 len)
{
	u8 ack;
	
	//�����ź�
	Iic_Start();
	//STM32�����豸��ַ��ִ��д����
	Iic_Send_Byte(0xA0);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack1 failure\r\n");
		return;
	}
	//STM32���Ͷ�������ʼ��ַ
	Iic_Send_Byte(addr);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack1 failure\r\n");
		return;
	}	
	
	//�����ź�
	Iic_Start();	
	
	//STM32�����豸��ַ��ִ�ж�����
	Iic_Send_Byte(0xA1);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack1 failure\r\n");
		return;
	}
	
	while(len--) //len = 1
	{
		//��������
		*read_buff = Iic_Recv_Byte();
		
		if(len > 0)
			Iic_Send_Ack(0);
		
		read_buff++;
	
	}
	
	Iic_Send_Ack(1);

	//ֹͣ�ź�
	 Iic_Stop();	

}



