#include "spiflash.h"

/**************************************
引脚说明：

使用SPI1
SCK连接PB3
MISO连接PB4
MOSI连接PB5
CS连接PB14
**************************************/

#if 0

void Spiflash_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStruct;
	//（2）使能SPIx和IO口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	//（3）初始化IO口为复用功能
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5; //引脚3 4 5
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14;  				//引脚14
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;				//输出模式
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP; 				//输出推挽
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_25MHz;				//速度
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;					//上拉

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	//（4）设置引脚复用映射：
	//引脚映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,  GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,  GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,  GPIO_AF_SPI1);
	
	
	//工作方式0
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;  	//设置SPI频率 
	SPI_InitStruct.SPI_CPOL				 = SPI_CPOL_Low; 				//时钟极性为低
	SPI_InitStruct.SPI_CPHA				 = SPI_CPHA_1Edge;				//第一边沿 
	SPI_InitStruct.SPI_DataSize			 = SPI_DataSize_8b;				//收发一个字节
	SPI_InitStruct.SPI_Direction 		 = SPI_Direction_2Lines_FullDuplex; //全双工
	SPI_InitStruct.SPI_FirstBit			 = SPI_FirstBit_MSB; 			//先发高位
	SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;  			//主机
	SPI_InitStruct.SPI_CRCPolynomial	 = 7;							//CRC校验
	SPI_InitStruct.SPI_NSS				 = SPI_NSS_Soft;				//CS使用软件控制
	//（5）初始化SPIx,设置SPIx工作模式
	SPI_Init(SPI1, &SPI_InitStruct);
	
	//（6）使能SPIx
	SPI_Cmd(SPI1, ENABLE);
	//（7）SPI传输数据
	//void SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data);
	//uint16_t SPI_I2S_ReceiveData(SPI_TypeDef* SPIx) ;
	
	//不使能芯片
	F_CS = 1;
}

u8 Spi1_Send_Recv_Byte(u8 tx_data)
{
	u8 rx_data = 0x00;
	
	//检查发送标志位
	while( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, tx_data);
	
	//检查接收标志位
	while( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	rx_data = SPI_I2S_ReceiveData(SPI1) ;

	return rx_data;
}

#else

/**************************************
引脚说明：

使用SPI1
SCK连接PB3
MISO连接PB4
MOSI连接PB5
CS连接PB14
**************************************/

void Spiflash_Init(void)
{
	
	GPIO_InitTypeDef  	GPIO_InitStruct;

	
	//1、使能SPIx和IO口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

	
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_3 | GPIO_Pin_14|GPIO_Pin_5; 	//GPIOB 3 14 5
	GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_OUT;						//配置IO口输出
	GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_50MHz; 					//速度 50MHz
	GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP; 						//推挽复用输出
	GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP; 						//上拉
    GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	
	GPIO_InitStruct.GPIO_Pin  	= GPIO_Pin_4; 			//引脚4
	GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_IN;			//输入
	GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP; 		//上拉
    GPIO_Init(GPIOB,&GPIO_InitStruct);		

	//不使能芯片
	F_CS = 1;
}

u8 Spi1_Send_Recv_Byte(u8 tx_data)
{
	u8 i, rx_data = 0x00;
	
	SCK = 0;
	
	for(i=0; i<8; i++)
	{
		
		//准备数据 先发高位数据 
		if(tx_data &(0x01<<(7-i)))  
		{
			MOSI = 1;//引脚输出1
		}
		else
		{
			MOSI = 0;//引脚输出0
		}
		
	
		delay_us(2);
		
		SCK = 1;	
		
		delay_us(2);
		//接受一位数据
		if(MISO == 1)
		{
			rx_data |= 0x01<<(7-i);
		}		
		
		SCK = 0;
	}
	


	return rx_data;
}





#endif




u16 Get_W25q128_Id(void)
{
	u16 id = 0x0000;
	
	//使能芯片
	F_CS = 0;
	
	//发送命令
	Spi1_Send_Recv_Byte(0x90);
	
	//发送地址
	Spi1_Send_Recv_Byte(0x00);
	Spi1_Send_Recv_Byte(0x00);
	Spi1_Send_Recv_Byte(0x00);
	
	//发送任意字节获取生产商ID
	id |= Spi1_Send_Recv_Byte(0xBB)<<8;
	//发送任意字节获取设备ID
	id |= Spi1_Send_Recv_Byte(0xFF);
	
	return id;
}

