#include "dht11.h"


/**********************************
引脚说明：

PG9 -- DQ(DHT11)
***********************************/

void Dht11_Init(void)
{
	GPIO_InitTypeDef  	GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;				//第9号引脚
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;			//输出模式
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;			//推挽输出，增强驱动能力，引脚的输出电流更大
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;		//引脚的速度最大为100MHz
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;			//没有使用内部上拉电阻
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
	
	//温湿度模块还没有工作，那么它的触发引脚是高电平
	PGout(9) = 1;
}

//引脚模式变更
void Dht11_Pin_Mode(GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef  	GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;				//第9号引脚
	GPIO_InitStructure.GPIO_Mode  = mode;					//输入/输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽输出，增强驱动能力，引脚的输出电流更大
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//引脚的速度最大为100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;		//没有使用内部上拉电阻
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
}

//启动DHT11正常返回0
int  Dht11_Start(void)
{
	u16 t = 0;
	
	//引脚做为输出
	Dht11_Pin_Mode(GPIO_Mode_OUT);
	//启动信号
	PGout(9) = 1;
	delay_ms(2);
	PGout(9) = 0;
	delay_ms(20);
	PGout(9) = 1;
	delay_us(30);
	
	//引脚做为输入 获取响应信号 
	Dht11_Pin_Mode(GPIO_Mode_IN);
	
	t = 0;
	//等待低电平到来
	while(PGin(9) == 1)
	{
		delay_us(10);
		t++;
		//等待1ms 未有低电平，则返回
		if(t >= 100)
			return -1;
		
	}
	
	t = 0;
	//等待高电平到来， 过滤低电平
	while(PGin(9) == 0)
	{
		delay_us(10);
		t++;
		//等待1ms 未有高电平，则返回
		if(t >= 100)
			return -2;
		
	}	
	
	t = 0;
	//等待低电平到来， 过滤高电平	
	while(PGin(9) == 1)
	{
		delay_us(10);
		t++;
		//等待1ms 未有低电平，则返回
		if(t >= 100)
			return -3;
		
	}	
	
	//启动正常，返回0
	return 0;
}



//一次性读取八位数据合成一个字节
//高低先出， 数据：0x87         1(先出) 0 0 0 0 1 1 1 
uint8_t Dht11_Read_Byte(void)
{
	u16 t = 0;
	u8 i, data = 0x00;  //0 0 0 0 0 0 0 0 0 
	
	for(i=0; i<8; i++)
	{
		//等待高电平 过滤低电平 
		t = 0;
		//等待高电平到来， 过滤低电平
		while(PGin(9) == 0)
		{
			delay_us(1);
			t++;
			//等待100us 没有等到，则返回
			if(t >= 100)
				return 0;
			
		}	
		//延时40us
		delay_us(40);
	
		
		if(PGin(9) == 1)
		{
			data |= (0x01<<(7-i)); //对应的位置1
			t = 0;
			//等待低电平到来， 过滤高电平
			while(PGin(9) == 1)
			{
				delay_us(1);
				t++;
				//等待100us 没有等到，则返回
				if(t >= 100)
					return 0;
				
			}			
		}
		
	}

	return data;
}
//成功返回0
int Dht11_Read(u8 *data)
{
	u8 i;
	
	for(i=0; i<5; i++)
	{
		data[i] = Dht11_Read_Byte();
	}
	
	if(data[4] == data[0]+data[1]+data[2]+data[3])
	{
		return 0;
	}
	else
	{
		return -1;
	}

}

