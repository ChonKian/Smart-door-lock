#include "iic.h"

/****************************************
引脚说明
SCL -- PB8
SDA -- PB9

*****************************************/

void Iic_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//打开GPIOB组时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	

	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9|GPIO_Pin_8;		//引脚8
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;				//输出模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_OD;				//开漏输出
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;					//上拉
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;				//速度
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
	
	//空闲状态
	SCL = 1;
	SDA_OUT = 1;
}

//数据引脚模式
void Iic_Sda_Mode(GPIOMode_TypeDef mode)
{

	GPIO_InitTypeDef  GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9;		//引脚8
	GPIO_InitStruct.GPIO_Mode	= mode;							//输出模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_OD;				//开漏输出
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;					//上拉
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;				//速度
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
}



//启动信号
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


//停止信号
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



//发送一位数据 1  0
void Iic_Send_Ack(u8 ack)
{
    Iic_Sda_Mode(GPIO_Mode_OUT);
	
	
	SCL = 0;
	
	//准备数据
	if(ack == 1)
	{
		SDA_OUT = 1; //引脚输出1
	}
	else
	{
		SDA_OUT = 0; //引脚输出0
	}
	
	
	delay_us(5);
	SCL = 1;
	delay_us(5);
	SCL = 0;

}

//发送一个字节(1字节有8位) data:0x87(1 0 0 0 0 1 1 1)
void Iic_Send_Byte(u8 data)
{
	int i;
	
	Iic_Sda_Mode(GPIO_Mode_OUT);	
	
	SCL = 0;
	for(i=0; i<8; i++)
	{
		//准备数据 先发高位数据 
		if(data &(0x01<<(7-i)))  
		{
			SDA_OUT = 1;//引脚输出1
		}
		else
		{
			SDA_OUT = 0;//引脚输出0
		}
		
		
		
		delay_us(5);
		SCL = 1;
		delay_us(5);
		SCL = 0;	
	}
	
}

//接受一位数据
u8 Iic_Recv_Ack(void)
{
	u8 ack = 0;
	
    Iic_Sda_Mode(GPIO_Mode_IN);

	SCL = 0;

	delay_us(5);
	SCL = 1;
	delay_us(5);
	
	if(SDA_IN == 1) //判断引脚电平
	{
		ack = 1;
	}
	if(SDA_IN == 0)//判断引脚电平
	{
		ack = 0;
	}	
	
	SCL = 0;

	
	return ack;

}


//接受一个字节数据
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

//往AT24C02写页数据
void At24c02_Write_Page(u8 addr, u8 *write_buff, u8 len)
{
	u8 ack;
	
	//启动信号
	Iic_Start();
	//STM32发送设备地址，执行写操作
	Iic_Send_Byte(0xA0);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack1 failure\r\n");
		return;
	}
	
	//STM32发送写数据的起始地址
	Iic_Send_Byte(addr);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack2 failure\r\n");
		return;
	}	
	
	while(len--)
	{
		//发送数据
		Iic_Send_Byte(*write_buff);
		ack = Iic_Recv_Ack();
		if(ack == 1)
		{
			printf("ack3 failure\r\n");
			return;
		}	
		//地址自加，指向下一个数据
		write_buff++;
	}
	
	//停止信号
	 Iic_Stop();	
	
	printf("write finish\r\n");
}



//读数据
void At24c02_Read(u8 addr, u8 *read_buff, u8 len)
{
	u8 ack;
	
	//启动信号
	Iic_Start();
	//STM32发送设备地址，执行写操作
	Iic_Send_Byte(0xA0);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack1 failure\r\n");
		return;
	}
	//STM32发送读数据起始地址
	Iic_Send_Byte(addr);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack1 failure\r\n");
		return;
	}	
	
	//启动信号
	Iic_Start();	
	
	//STM32发送设备地址，执行读操作
	Iic_Send_Byte(0xA1);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack1 failure\r\n");
		return;
	}
	
	while(len--) //len = 1
	{
		//接受数据
		*read_buff = Iic_Recv_Byte();
		
		if(len > 0)
			Iic_Send_Ack(0);
		
		read_buff++;
	
	}
	
	Iic_Send_Ack(1);

	//停止信号
	 Iic_Stop();	

}



