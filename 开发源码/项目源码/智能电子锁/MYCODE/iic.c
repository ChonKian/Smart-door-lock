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

//引脚模式变更--SDA
void Iic_Sda_Mode(GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef  	GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;				//第9号引脚
	GPIO_InitStructure.GPIO_Mode  = mode;					//输入/输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽输出，增强驱动能力，引脚的输出电流更大
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//引脚的速度最大为100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			//没有使用内部上拉电阻
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

//启动信号 由主机发出
void Iic_Start(void)
{
	//数据做输出
	Iic_Sda_Mode(GPIO_Mode_OUT);

	//空闲状态
	SCL = 1;
	SDA_OUT = 1;
	
	delay_us(5);
	
	SDA_OUT = 0;
	delay_us(5);
	SCL = 0;
	

}
//停止信号 由主机发出
void Iic_Stop(void)
{
	//数据做输出
	Iic_Sda_Mode(GPIO_Mode_OUT);
	SCL = 0;
	SDA_OUT = 0;	
	
	delay_us(5);
	
	SCL = 1;
	
	delay_us(5);
	
	SDA_OUT = 1;
	
}

//主机发送一位数据
void Iic_Send_Ack(u8 ack)
{
	//数据做输出
	Iic_Sda_Mode(GPIO_Mode_OUT);
	
	
	SCL = 0;
	
	//准备数据
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

//发送一个字节 主机发出 0x78   0 1 1 1 1 0 0 0 (先发高位)
void Iic_Send_Byte(u8 data)  
{
	u8 i;

	//数据做输出
	Iic_Sda_Mode(GPIO_Mode_OUT);
	
	SCL = 0;
	
	for(i=0; i<8; i++)
	{
		//非0即真  
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


//主机接受一位数据
u8 Iic_Recv_Ack(void)
{
	u8 ack = 0;
	
	//数据做输入
	Iic_Sda_Mode(GPIO_Mode_IN);

	SCL = 0;
	
	delay_us(5);
	
	SCL = 1;
	
	delay_us(5);
	
	//判断引脚，获取数据
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


//主机接受一个字节数据  0x78   0 1 1 1 1 0 0 0 (先发高位)
u8 Iic_Recv_Byte(void)
{
	u8 i, data = 0x00;

	//数据做输入
	Iic_Sda_Mode(GPIO_Mode_IN);
	
	SCL = 0;
	
	for(i=0; i<8; i++)
	{
		//非0即真  

		delay_us(5);
		SCL = 1;
		delay_us(5);
		
		//数据合成
		if(SDA_IN == 1)
		{
			data |= (0x01<<(7-i));
		}
		SCL = 0;
	
	}	
	
	return data;
}

//AT24c02写页操作
void At24c02_Write(u8 addr, u8 *write_buff, u8 len)
{
	u8 ack = 0;
	
	
	//启动信号
	Iic_Start();
	//STM32发送设备地址，执行对AT24C02写操作
	Iic_Send_Byte(0xA0);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\r\n");
		Iic_Stop();
		return;
	}
	
	//STM32发送写数据起始地址
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
		//STM32发送数据（往AT24C02里写数据）
		Iic_Send_Byte(*write_buff);
		ack = Iic_Recv_Ack();
		if(ack == 1)
		{
			printf("ack failure\r\n");
			Iic_Stop();
			return;
		}	

		//地址加1，指向下一个字符
		write_buff++;
	
	}
	
	Iic_Stop();
	
	printf("write finish\r\n");
	
	return;
}

//读数据
void At24c02_Read(u8 addr, u8 *read_buff, u8 len)
{

	u8 ack = 0;
	
	//启动信号
	Iic_Start();

	//STM32发送设备地址，执行对AT24C02写操作
	Iic_Send_Byte(0xA0);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\r\n");
		Iic_Stop();
		return;
	}
	
	
	//STM32发送读数据起始地址
	Iic_Send_Byte(addr);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\r\n");
		Iic_Stop();
		return;
	}	
	
	//启动信号
	Iic_Start();	

	//STM32发送设备地址，执行对AT24C02读操作
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
			//发送应答信号
			Iic_Send_Ack(0);
		}
		
	}
	
	
	Iic_Send_Ack(1);
	
	
	Iic_Stop();
	

}
