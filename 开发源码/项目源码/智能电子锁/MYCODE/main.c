#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "exti.h"
#include "delay.h"
#include "tim.h"
#include "pwm.h"
#include "usart.h"
#include "string.h"
#include "sr04.h"
#include "dht11.h"
#include "iwdg.h"
#include "adc.h"
#include "iic.h"
#include "OLED_I2C.h"
#include "as608.h"
#include "as608user.h"
#include "malloc.h"
#include "beep.h"
#define usart2_baund  57600//指纹模块默认57600
SysPara AS608Para;//指纹模块AS608参数
u16 ValidN;//模块内有效模板个数
u8** kbd_tbl;
u8 sid = 0;
u8 buffer[32] = {0};
u8 rx_buffer[32] = {0};
u8 count = 0; //0~255   0~127

u8 flag = 0;
u8 data = 0;

u8 res;
//MFRC522数据区
u8  mfrc552pidbuf[18];
u8  card_pydebuf[2];
u8  card_numberbuf[5];
u8  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};
u8  card_writebuf[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
u8  card_readbuf[18];
void USART1_IRQHandler(void)
{

	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{	//清空标志位
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
		//接收数据
		buffer[count++] = USART_ReceiveData(USART1);
		
		//判断是否为结束标识符
		if(buffer[count-1] == ':')
		{
			//将buffer数据存储在rx_buffer，并过滤':'
			for(int i=0; i<(count-1); i++)
			{
				rx_buffer[i] = buffer[i];
			}
			//表示接受一帧数据完成
			flag = 1;
			//保证下一帧数据从buffer[0]开始存储
			count = 0;
			//清空数组
			memset(buffer, 0, sizeof(buffer));
			
		}
		
		
		
	}	


}
void USART3_IRQHandler(void)
{

	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{	//清空标志位
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);	
		//接收数据
		buffer[count++] = USART_ReceiveData(USART3);
		//判断是否为结束标识符
		if(buffer[count-1] == ':')
		{
			//将buffer数据存储在rx_buffer，并过滤':'
			for(int i=0; i<(count-1); i++)
			{
				rx_buffer[i] = buffer[i];
			}
			//表示接受一帧数据完成
			flag = 1;
			//保证下一帧数据从buffer[0]开始存储
			count = 0;
			//清空数组
			memset(buffer, 0, sizeof(buffer));
			
		}	
		
	}

}
void fun(void)
{
	u8 i;
	for(i=0;i<4;i++)
	{
		OLED_ShowCN(30+i*16,0,i);//测试显示中文
	}
	for(i=4;i<8;i++)
	{
		OLED_ShowCN(30+(i-4)*16,2,i);//测试显示中文
	}
	for(i=8;i<12;i++)
	{
		OLED_ShowCN(30+(i-8)*16,4,i);//测试显示中文
	}
	for(i=12;i<16;i++)
	{
		OLED_ShowCN(30+(i-12)*16,6,i);//测试显示中文
	}
}
//MFRC522测试函数
void MFRC522Test(void)
{
	u8 i,j,status,card_size;
	//
	status=MFRC522_Request(0x52, card_pydebuf);			//寻卡
	//
	if(status==0)		//如果读到卡
	{
		GPIO_SetBits(GPIOF, GPIO_Pin_8);
		delay_ms(100);
		GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		status=MFRC522_Anticoll(card_numberbuf);			//防撞处理			
		card_size=MFRC522_SelectTag(card_numberbuf);	//选卡
		status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//验卡
		status=MFRC522_Write(4, card_writebuf);				//写卡（写卡要小心，特别是各区的块3）
		status=MFRC522_Read(4, card_readbuf);					//读卡
		//MFRC522_Halt();															//使卡进入休眠状态
		//卡类型显示
		printf("卡类型显示:%#X %#X\r\n", card_pydebuf[0], card_pydebuf[1]);
		
		
		//卡序列号显示，最后一字节为卡的校验码
		printf("卡序列号:");
		for(i=0;i<5;i++)
		{
			printf("%#X ", card_numberbuf[i]);
		
		}
		printf("\r\n");
		
		
		//卡容量显示，单位为Kbits
		printf("卡容量显示:%dKbits\r\n", card_size);
		
		
		
		//读卡状态显示，正常为0
		printf("读卡状态显示:%d\r\n", status);
		
		
		//读一个块的数据显示
		printf("读一个块的数据显示:");
		for(i=0;i<2;i++)		//分两行显示
		{
			for(j=0;j<9;j++)	//每行显示8个
			{
				printf("%#X ", card_readbuf[j+i*9]);

			}
		}
		printf("\r\n");
		OLED_CLS();//清屏
		for(i=120;i<124;i++)
		{
			OLED_ShowCN(25+(i-120)*16,3,i);//测试显示中文
		}
		delay_s(1);
		OLED_CLS();//清屏
		fun();
		OLED_ShowStr(8,0,"->",2);
		
		
	}	
}
int main(void)
{
	u8 buf[9]={0};
	u8 buf1[9]={0};
	u8 newbuf[10]={0};
	unsigned char i;
	extern const unsigned char BMP1[];
	u16 id=0;
	char *str;
	u8 ensure;
	u8 y=0;
	//设置NVIC分组 第二分组 抢占优先级范围:0~3  响应优先级范围：0~3
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Delay_Init();
	//对LED初始化
	Led_Init();
	Iic_Init();
	Beep_Init();
	Usart1_Init(115200);
	Usart2_Init(usart2_baund);
	//Usart3_Init(9600);
	I2C_Configuration();
	OLED_Init();
	Key_Init();
	MFRC522_Initializtion();
	PS_StaGPIO_Init();
	//显示图片
	/*提醒用户语句*/
	printf("AS608指纹模块串口操作\r\n");
	printf("尝试连接AS608中.....\r\n");
	while(PS_HandShake(&AS608Addr))//与AS608模块握手
	{

		printf("未连接上AS608!\r\n");
		delay_ms(1000);
		printf("再次尝试连接中....\r\n");
		delay_ms(1000);	  
	}
	//连接模块成功，初始化
	printf("连接成功！！\r\n");
	str=mymalloc(30);
	sprintf(str,"波特率:%d   地址:%x",usart2_baund,AS608Addr);//显示波特率
	printf("%s",str);
	delay_ms(100);
	ensure=PS_ValidTempleteNum(&ValidN);//读库指纹个数
	if(ensure!=0x00)
		ShowErrMessage(ensure);//显示确认码错误信息	
	ensure=PS_ReadSysPara(&AS608Para);  //读AS608模块参数 
	if(ensure==0x00)
	{
		mymemset(str,0,50);
		sprintf(str,"RemainNum:%d    Level:%d",AS608Para.PS_max-ValidN,AS608Para.PS_level);//显示剩余指纹数量和安全等级
		printf("%s",str);
	}
	else
		ShowErrMessage(ensure);	
	myfree(str);
	OLED_Fill(0x00);//全屏灭
	delay_s(1);	
	fun();
	OLED_ShowStr(8,0,"->",2);
	u8 j=0,l=0,k=0,r=0;
	int s=0;
	while(1)
	{
		id = Key_Scan();
		delay_ms(150);
		if(id==13&&s==0)
		{
			k++;
			OLED_CLS();//清屏
			for(i=93;i<98;i++)
			{
				OLED_ShowCN(25+(i-93)*16,0,i);//测试显示中文
			}
			id=100;
			while(1)
			{
				id = Key_Scan();
				delay_ms(150);
				OLED_ShowStr(8,3,"{",2);
				OLED_ShowStr(110,3,"}",2);
				if(id==1)
				{
					l=l+8;
					buf[j++]='1';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==2)
				{
					l=l+8;
					buf[j++]='2';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==3)
				{
					l=l+8;
					buf[j++]='3';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==4)
				{
					l=l+8;
					buf[j++]='4';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==5)
				{
					l=l+8;
					buf[j++]='5';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==6)
				{
					l=l+8;
					buf[j++]='6';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==7)
				{
					l=l+8;
					buf[j++]='7';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==8)
				{
					l=l+8;
					buf[j++]='8';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==9)
				{
					l=l+8;
					buf[j++]='9';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==0)
				{
					l=l+8;
					buf[j++]='0';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==10||j==8)
				{
					At24c02_Write(0x00,0, 8);
					delay_ms(10);
					At24c02_Write(0x00, buf, j);
					printf("%s\r\n",buf);
					OLED_CLS();//清屏
					for(i=98;i<104;i++)
					{
						OLED_ShowCN(20+(i-98)*16,2,i);//测试显示中文
					}
					OLED_CLS();//清屏
					l=0;
					j=0;
					fun();
					OLED_ShowStr(8,0,"->",2);
					memset(buf, 0, sizeof(buf));
					break;
				}
			}
		}
		if(id==14)
		{
			s=s+2;
			OLED_ShowStr(8,s,"->",2);
			OLED_ShowStr(8,r,"  ",2);
			r=r+2;
			if(s==6)
			{
				s=-2;
			}
			if(r==8)
			{
				r=0;
			}
		}
		if(id==13&&s==2)
		{
			OLED_CLS();//清屏
			for(i=93;i<98;i++)
			{
				OLED_ShowCN(25+(i-93)*16,0,i);//测试显示中文
			}
			id=100;
			while(1)
			{
				id = Key_Scan();
				delay_ms(150);
				OLED_ShowStr(8,3,"{",2);
				OLED_ShowStr(110,3,"}",2);
				if(id==1)
				{
					l=l+8;
					newbuf[j++]='1';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==2)
				{
					l=l+8;
					newbuf[j++]='2';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==3)
				{
					l=l+8;
					newbuf[j++]='3';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==4)
				{
					l=l+8;
					newbuf[j++]='4';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==5)
				{
					l=l+8;
					newbuf[j++]='5';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==6)
				{
					l=l+8;
					newbuf[j++]='6';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==7)
				{
					l=l+8;
					newbuf[j++]='7';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==8)
				{
					l=l+8;
					newbuf[j++]='8';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==9)
				{
					l=l+8;
					newbuf[j++]='9';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==0)
				{
					l=l+8;
					newbuf[j++]='0';
					OLED_ShowStr(j+9+l,3,"*",2);
					printf("%d\r\n",id);
				}
				if(id==10||j==8)
				{
					delay_ms(10);
					At24c02_Read(0x00,  buf1, j);
					printf("%s\r\n",buf1);
					if(strstr((char *)newbuf,(char *)buf1)!=0)
					{
						OLED_CLS();//清屏
						for(i=104;i<108;i++)
						{
							OLED_ShowCN(25+(i-104)*16,2,i);//测试显示中文
						}
						OLED_CLS();//清屏
						l=0;
						j=0;
						fun();
						OLED_ShowStr(8,2,"->",2);
						id=100;
						memset(newbuf, 0, sizeof(newbuf));
						memset(buf1, 0, sizeof(buf1));
						break;
						
					}
					else
					{
						memset(buf1, 0, sizeof(buf1));
						OLED_CLS();//清屏
						for(i=108;i<114;i++)
						{
							OLED_ShowCN(25+(i-108)*16,0,i);//测试显示中文
						}
						OLED_CLS();//清屏
						fun();
						OLED_ShowStr(8,2,"->",2);
						memset(newbuf, 0, sizeof(newbuf));
						l=0;
						j=0;
						id=100;
					}
					break;
				}
			}
		}
		if(id==13&&s==4)
		{
			Add_FR1();
			OLED_CLS();//清屏
			fun();
			OLED_ShowStr(8,4,"->",2);
		}
		if(id==13&&r==6)
		{
			press_FR1();
			delay_s(1);
			OLED_CLS();//清屏
			fun();
			OLED_ShowStr(8,6,"->",2);
		}
		if(flag == 1)
		{
			
			//录入指纹
			if(strcmp(rx_buffer, "HCL10") == 0)
			{
				memset(rx_buffer, 0, sizeof(rx_buffer));
				Add_FR();
			}
			//删除指纹功能
			if(strcmp(rx_buffer, "HCL11") == 0)
			{
				memset(rx_buffer, 0, sizeof(rx_buffer));
				 Del_FR();
			}
			//刷指纹功能
			if(strcmp(rx_buffer, "HCL12") == 0)
			{
				memset(rx_buffer, 0, sizeof(rx_buffer));
				 press_FR();
			}
			//清除所有指纹功能
			if(strcmp(rx_buffer, "HCL13") == 0)
			{
				memset(rx_buffer, 0, sizeof(rx_buffer));
				 PS_Empty();
			}
			memset(rx_buffer, 0, sizeof(rx_buffer));
			flag = 0;
			printf("ok\r\n");
		}
		if(id==11)
		{
			while(1)
			{
				id = Key_Scan();
				if(id==100)
				{
					MFRC522_Initializtion();	
					MFRC522Test();
				}
				if(id==10)
				{
					break;
				}
			}
		}
		//delay_s(1);
		//OLED_Fill(0xFF);//全屏点亮
		//delay_s(2);
		//OLED_Fill(0x00);//全屏灭
		//delay_s(2);
		//for(i=0;i<5;i++)
		//{
			//OLED_ShowCN(22+i*16,0,i);//测试显示中文
			//OLED_ShowCN(22+i*16,1,i);//测试显示中文
			//OLED_ShowCN(22+i*16,2,i);//测试显示中文
		//}
		//delay_s(2);
		//OLED_ShowStr(0,3,"HelTec Automation",1);//测试6*8字符
		//OLED_ShowStr(0,4,"Hello Tech",2);				//测试8*16字符
		//delay_s(2);
		//OLED_CLS();//清屏
		//OLED_OFF();//测试OLED休眠
		//delay_s(2);
		//OLED_ON();//测试OLED休眠后唤醒
		//OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//测试BMP位图显示
		//delay_s(2);
		
	}
	return 0;
}
