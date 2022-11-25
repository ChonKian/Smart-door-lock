#include "dht11.h"


/**********************************
����˵����

PG9 -- DQ(DHT11)
***********************************/

void Dht11_Init(void)
{
	GPIO_InitTypeDef  	GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;				//��9������
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;			//���ģʽ
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;			//�����������ǿ�������������ŵ������������
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;		//���ŵ��ٶ����Ϊ100MHz
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;			//û��ʹ���ڲ���������
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
	
	//��ʪ��ģ�黹û�й�������ô���Ĵ��������Ǹߵ�ƽ
	PGout(9) = 1;
}

//����ģʽ���
void Dht11_Pin_Mode(GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef  	GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;				//��9������
	GPIO_InitStructure.GPIO_Mode  = mode;					//����/���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�����������ǿ�������������ŵ������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//���ŵ��ٶ����Ϊ100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;		//û��ʹ���ڲ���������
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
}

//����DHT11��������0
int  Dht11_Start(void)
{
	u16 t = 0;
	
	//������Ϊ���
	Dht11_Pin_Mode(GPIO_Mode_OUT);
	//�����ź�
	PGout(9) = 1;
	delay_ms(2);
	PGout(9) = 0;
	delay_ms(20);
	PGout(9) = 1;
	delay_us(30);
	
	//������Ϊ���� ��ȡ��Ӧ�ź� 
	Dht11_Pin_Mode(GPIO_Mode_IN);
	
	t = 0;
	//�ȴ��͵�ƽ����
	while(PGin(9) == 1)
	{
		delay_us(10);
		t++;
		//�ȴ�1ms δ�е͵�ƽ���򷵻�
		if(t >= 100)
			return -1;
		
	}
	
	t = 0;
	//�ȴ��ߵ�ƽ������ ���˵͵�ƽ
	while(PGin(9) == 0)
	{
		delay_us(10);
		t++;
		//�ȴ�1ms δ�иߵ�ƽ���򷵻�
		if(t >= 100)
			return -2;
		
	}	
	
	t = 0;
	//�ȴ��͵�ƽ������ ���˸ߵ�ƽ	
	while(PGin(9) == 1)
	{
		delay_us(10);
		t++;
		//�ȴ�1ms δ�е͵�ƽ���򷵻�
		if(t >= 100)
			return -3;
		
	}	
	
	//��������������0
	return 0;
}



//һ���Զ�ȡ��λ���ݺϳ�һ���ֽ�
//�ߵ��ȳ��� ���ݣ�0x87         1(�ȳ�) 0 0 0 0 1 1 1 
uint8_t Dht11_Read_Byte(void)
{
	u16 t = 0;
	u8 i, data = 0x00;  //0 0 0 0 0 0 0 0 0 
	
	for(i=0; i<8; i++)
	{
		//�ȴ��ߵ�ƽ ���˵͵�ƽ 
		t = 0;
		//�ȴ��ߵ�ƽ������ ���˵͵�ƽ
		while(PGin(9) == 0)
		{
			delay_us(1);
			t++;
			//�ȴ�100us û�еȵ����򷵻�
			if(t >= 100)
				return 0;
			
		}	
		//��ʱ40us
		delay_us(40);
	
		
		if(PGin(9) == 1)
		{
			data |= (0x01<<(7-i)); //��Ӧ��λ��1
			t = 0;
			//�ȴ��͵�ƽ������ ���˸ߵ�ƽ
			while(PGin(9) == 1)
			{
				delay_us(1);
				t++;
				//�ȴ�100us û�еȵ����򷵻�
				if(t >= 100)
					return 0;
				
			}			
		}
		
	}

	return data;
}
//�ɹ�����0
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

