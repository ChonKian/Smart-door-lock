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

//����ģʽ���--SDA
void Iic_Sda_Mode(GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef  	GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;				//��9������
	GPIO_InitStructure.GPIO_Mode  = mode;					//����/���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�����������ǿ�������������ŵ������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//���ŵ��ٶ����Ϊ100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			//û��ʹ���ڲ���������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

//�����ź� ����������
void Iic_Start(void)
{
	//���������
	Iic_Sda_Mode(GPIO_Mode_OUT);

	//����״̬
	SCL = 1;
	SDA_OUT = 1;
	
	delay_us(5);
	
	SDA_OUT = 0;
	delay_us(5);
	SCL = 0;
	

}
//ֹͣ�ź� ����������
void Iic_Stop(void)
{
	//���������
	Iic_Sda_Mode(GPIO_Mode_OUT);
	SCL = 0;
	SDA_OUT = 0;	
	
	delay_us(5);
	
	SCL = 1;
	
	delay_us(5);
	
	SDA_OUT = 1;
	
}

//��������һλ����
void Iic_Send_Ack(u8 ack)
{
	//���������
	Iic_Sda_Mode(GPIO_Mode_OUT);
	
	
	SCL = 0;
	
	//׼������
	if(ack == 1)
	{
		SDA_OUT = 1;
	}
	else
	{
		SDA_OUT = 0;
	}	
	delay_us(5);
	
	SCL = 1;
	
	delay_us(5);
	
	SCL = 0;
	

}

//����һ���ֽ� �������� 0x78   0 1 1 1 1 0 0 0 (�ȷ���λ)
void Iic_Send_Byte(u8 data)  
{
	u8 i;

	//���������
	Iic_Sda_Mode(GPIO_Mode_OUT);
	
	SCL = 0;
	
	for(i=0; i<8; i++)
	{
		//��0����  
		if( data & (0x01<< (7-i)))
		{
			SDA_OUT = 1;
		}
		else
		{
			SDA_OUT = 0;
		}
		
		delay_us(5);
		SCL = 1;
		delay_us(5);
		SCL = 0;
	
	}
	
}


//��������һλ����
u8 Iic_Recv_Ack(void)
{
	u8 ack = 0;
	
	//����������
	Iic_Sda_Mode(GPIO_Mode_IN);

	SCL = 0;
	
	delay_us(5);
	
	SCL = 1;
	
	delay_us(5);
	
	//�ж����ţ���ȡ����
	if(SDA_IN == 1)
	{
		ack = 1;
	}
	else
	{
		ack = 0;
	}
	
	SCL = 0;	
	
	
	return ack;
	
}


//��������һ���ֽ�����  0x78   0 1 1 1 1 0 0 0 (�ȷ���λ)
u8 Iic_Recv_Byte(void)
{
	u8 i, data = 0x00;

	//����������
	Iic_Sda_Mode(GPIO_Mode_IN);
	
	SCL = 0;
	
	for(i=0; i<8; i++)
	{
		//��0����  

		delay_us(5);
		SCL = 1;
		delay_us(5);
		
		//���ݺϳ�
		if(SDA_IN == 1)
		{
			data |= (0x01<<(7-i));
		}
		SCL = 0;
	
	}	
	
	return data;
}

//AT24c02дҳ����
void At24c02_Write(u8 addr, u8 *write_buff, u8 len)
{
	u8 ack = 0;
	
	
	//�����ź�
	Iic_Start();
	//STM32�����豸��ַ��ִ�ж�AT24C02д����
	Iic_Send_Byte(0xA0);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\r\n");
		Iic_Stop();
		return;
	}
	
	//STM32����д������ʼ��ַ
	Iic_Send_Byte(addr);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\r\n");
		Iic_Stop();
		return;
	}
	
	while(len--)
	{
		//STM32�������ݣ���AT24C02��д���ݣ�
		Iic_Send_Byte(*write_buff);
		ack = Iic_Recv_Ack();
		if(ack == 1)
		{
			printf("ack failure\r\n");
			Iic_Stop();
			return;
		}	

		//��ַ��1��ָ����һ���ַ�
		write_buff++;
	
	}
	
	Iic_Stop();
	
	printf("write finish\r\n");
	
	return;
}

//������
void At24c02_Read(u8 addr, u8 *read_buff, u8 len)
{

	u8 ack = 0;
	
	//�����ź�
	Iic_Start();

	//STM32�����豸��ַ��ִ�ж�AT24C02д����
	Iic_Send_Byte(0xA0);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\r\n");
		Iic_Stop();
		return;
	}
	
	
	//STM32���Ͷ�������ʼ��ַ
	Iic_Send_Byte(addr);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\r\n");
		Iic_Stop();
		return;
	}	
	
	//�����ź�
	Iic_Start();	

	//STM32�����豸��ַ��ִ�ж�AT24C02������
	Iic_Send_Byte(0xA1);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\r\n");
		Iic_Stop();
		return;
	}	
	
	while(len--) //len = 4
	{
		*read_buff =  Iic_Recv_Byte();
		read_buff++;
		
		if(len > 0)
		{
			//����Ӧ���ź�
			Iic_Send_Ack(0);
		}
		
	}
	
	
	Iic_Send_Ack(1);
	
	
	Iic_Stop();
	

}
