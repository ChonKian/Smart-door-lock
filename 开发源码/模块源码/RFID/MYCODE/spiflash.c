#include "spiflash.h"

/**************************************
����˵����

ʹ��SPI1
SCK����PB3
MISO����PB4
MOSI����PB5
CS����PB14
**************************************/

#if 0

void Spiflash_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStruct;
	//��2��ʹ��SPIx��IO��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	//��3����ʼ��IO��Ϊ���ù���
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5; //����3 4 5
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14;  				//����14
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;				//���ģʽ
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP; 				//�������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_25MHz;				//�ٶ�
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;					//����

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	//��4���������Ÿ���ӳ�䣺
	//����ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,  GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,  GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,  GPIO_AF_SPI1);
	
	
	//������ʽ0
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;  	//����SPIƵ�� 
	SPI_InitStruct.SPI_CPOL				 = SPI_CPOL_Low; 				//ʱ�Ӽ���Ϊ��
	SPI_InitStruct.SPI_CPHA				 = SPI_CPHA_1Edge;				//��һ���� 
	SPI_InitStruct.SPI_DataSize			 = SPI_DataSize_8b;				//�շ�һ���ֽ�
	SPI_InitStruct.SPI_Direction 		 = SPI_Direction_2Lines_FullDuplex; //ȫ˫��
	SPI_InitStruct.SPI_FirstBit			 = SPI_FirstBit_MSB; 			//�ȷ���λ
	SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;  			//����
	SPI_InitStruct.SPI_CRCPolynomial	 = 7;							//CRCУ��
	SPI_InitStruct.SPI_NSS				 = SPI_NSS_Soft;				//CSʹ���������
	//��5����ʼ��SPIx,����SPIx����ģʽ
	SPI_Init(SPI1, &SPI_InitStruct);
	
	//��6��ʹ��SPIx
	SPI_Cmd(SPI1, ENABLE);
	//��7��SPI��������
	//void SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data);
	//uint16_t SPI_I2S_ReceiveData(SPI_TypeDef* SPIx) ;
	
	//��ʹ��оƬ
	F_CS = 1;
}

u8 Spi1_Send_Recv_Byte(u8 tx_data)
{
	u8 rx_data = 0x00;
	
	//��鷢�ͱ�־λ
	while( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, tx_data);
	
	//�����ձ�־λ
	while( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	rx_data = SPI_I2S_ReceiveData(SPI1) ;

	return rx_data;
}

#else

/**************************************
����˵����

ʹ��SPI1
SCK����PB3
MISO����PB4
MOSI����PB5
CS����PB14
**************************************/

void Spiflash_Init(void)
{
	
	GPIO_InitTypeDef  	GPIO_InitStruct;

	
	//1��ʹ��SPIx��IO��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

	
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_3 | GPIO_Pin_14|GPIO_Pin_5; 	//GPIOB 3 14 5
	GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_OUT;						//����IO�����
	GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_50MHz; 					//�ٶ� 50MHz
	GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP; 						//���츴�����
	GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP; 						//����
    GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	
	GPIO_InitStruct.GPIO_Pin  	= GPIO_Pin_4; 			//����4
	GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_IN;			//����
	GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP; 		//����
    GPIO_Init(GPIOB,&GPIO_InitStruct);		

	//��ʹ��оƬ
	F_CS = 1;
}

u8 Spi1_Send_Recv_Byte(u8 tx_data)
{
	u8 i, rx_data = 0x00;
	
	SCK = 0;
	
	for(i=0; i<8; i++)
	{
		
		//׼������ �ȷ���λ���� 
		if(tx_data &(0x01<<(7-i)))  
		{
			MOSI = 1;//�������1
		}
		else
		{
			MOSI = 0;//�������0
		}
		
	
		delay_us(2);
		
		SCK = 1;	
		
		delay_us(2);
		//����һλ����
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
	
	//ʹ��оƬ
	F_CS = 0;
	
	//��������
	Spi1_Send_Recv_Byte(0x90);
	
	//���͵�ַ
	Spi1_Send_Recv_Byte(0x00);
	Spi1_Send_Recv_Byte(0x00);
	Spi1_Send_Recv_Byte(0x00);
	
	//���������ֽڻ�ȡ������ID
	id |= Spi1_Send_Recv_Byte(0xBB)<<8;
	//���������ֽڻ�ȡ�豸ID
	id |= Spi1_Send_Recv_Byte(0xFF);
	
	return id;
}

