#include "as608.h"
#include "as608user.h"
#include "stdlib.h"
#include "stdio.h"
#include "usart.h"
#include "delay.h"
#include "OLED_I2C.h"
#include "key.h"
extern SysPara AS608Para;//指纹模块AS608参数
//extern u16 USART2_RX_STA;       //接收状态标记	  
//extern u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
extern u16 ValidN;//模块内有效模板个数
extern u8 rx_buffer[32];
//显示确认码错误信息
void ShowErrMessage(u8 ensure)
{
	printf("%s",EnsureMessage(ensure));
	delay_ms(1000);
}
 
//录指纹功能
void Add_FR()
{
	u8 i,j,ensure ,processnum=0;
	u16 ID;
	extern const unsigned char BMP1[];
	char str[10]={0};
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
				printf("请按指纹！\r\n");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					printf("指纹正常！\r\n");
					ensure=PS_GenChar(CharBuffer1);//生成特征
					if(ensure==0x00)
					{
						i=0;
						processnum=1;//跳到第二步						
					}
					else 
						printf("生成特征失败！\r\n");					
				}
				else
					 printf("指纹采集失败！\r\n");
				break;
			
			case 1:
				i++;
				printf("请按再按一次指纹！\r\n");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer2);//再按一次指纹
					if(ensure==0x00)
					{
						printf("指纹正常！\r\n");
						i=0;
						processnum=2;//跳到第三步
					}
					else
					{ printf("生成特征失败！\r\n");}	
				}else {printf("指纹采集失败！\r\n");}		
				break;
 
			case 2:
				printf("对比两次指纹！\r\n");
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					processnum=3;//跳到第四步
				}
				else 
				{
					printf("对比成功,两次指纹一样！\r\n");		
					i=0;
					processnum=0;//跳回第一步		
				}
				delay_ms(1200);
				break;
 
			case 3:
				printf("生成指纹模板！\r\n");
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					printf("生成指纹模板成功！\r\n");
					processnum=4;//跳到第五步
				}else {processnum=0;printf("生成指纹模板失败！\r\n");}
				delay_ms(1200);
				break;
				
			case 4:	
				printf("请输入储存ID,1=< ID <=299,按Enter保存\r\n");
				while(ID<=0||ID>299)
				{
					ID=atoi(rx_buffer);
				}
				do
					printf("%d\r\n",AS608Para.PS_max);
				while(!(ID<AS608Para.PS_max));//输入ID必须小于指纹容量的最大值
				printf("储存ID的位置:%d\r\n",ID);
				ensure=PS_StoreChar(CharBuffer2,ID);//储存模板
				if(ensure==0x00) 
				{					
					printf("录入指纹成功！\r\n");
					PS_ValidTempleteNum(&ValidN);//读库指纹个数
					press_FR();
					return ;
				}else {processnum=0;printf("读取指纹个数失败！\r\n");}	
				printf("解锁成功\r\n");
				break;				
		}
		delay_ms(2000);
		if(i==5)//超过5次没有按手指则退出
		{
			printf("超过五次没按手指\r\n");//超过5次没有按手指则退出
			break;	
		}				
	}
}
//录指纹功能
void Add_FR1(void)
{
	u8 i,j,ensure ,processnum=0;
	u16 ID;
	u8 id=0;
	u8 l=0;
	u8 k=0;
	extern const unsigned char BMP1[];
	char str[10]={0};
	char newbuf[10]={0};
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
				OLED_CLS();
				for(j=16;j<20;j++)
				{
					OLED_ShowCN(28+(j-16)*16,3,j);//请按指纹
				}
				OLED_CLS();
				OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//测试BMP位图显示
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//生成特征
					if(ensure==0x00)
					{
						i=0;
						processnum=1;//跳到第二步						
					}
					else {
						OLED_CLS();
						OLED_ShowStr(0,4,"ShowErrMessage",2);}
						//delay_s(1);					
				}
				else{ 
					OLED_CLS();
					OLED_ShowStr(0,4,"ShowErrMessage",2);}	
					//delay_s(1);
				break;
			
			case 1:
				i++;
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer2);//再按一次指纹
					if(ensure==0x00)
					{
						i=0;
						processnum=2;//跳到第三步
					}
					else
					{OLED_CLS();OLED_ShowStr(0,4,"ShowErrMessage",2);}	
				}else {OLED_CLS();OLED_ShowStr(0,4,"ShowErrMessage",2);}		
				break;
 
			case 2:
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					processnum=3;//跳到第四步
				}
				else 
				{
					OLED_CLS();
					OLED_ShowStr(0,4,"ShowErrMessage",2);		
					i=0;
					processnum=0;//跳回第一步		
				}
				delay_ms(1200);
				break;
 
			case 3:
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					processnum=4;//跳到第五步
				}else {processnum=0;OLED_CLS();OLED_ShowStr(0,4,"ShowErrMessage",2);}
				delay_ms(1200);
				break;
				
			case 4:	
				OLED_CLS();
				for(j=86;j<93;j++)
				{
					OLED_ShowCN(20+(j-86)*16,3,j);//请按指纹
				}
				OLED_CLS();
				OLED_ShowStr(0,4,"  0=< ID <=299",2); 
				OLED_CLS();
				while(1)
				{
					id = Key_Scan();
					delay_ms(150);
					OLED_ShowStr(8,3,"{",2);
					OLED_ShowStr(110,3,"}",2);
					if(id==1)
					{
						l=l+8;
						newbuf[k++]='1';
						OLED_ShowStr(k+9+l,3,"*",2);
						printf("%d\r\n",id);
					}
					if(id==2)
					{
						l=l+8;
						newbuf[k++]='2';
						OLED_ShowStr(k+9+l,3,"*",2);
						printf("%d\r\n",id);
					}
					if(id==3)
					{
						l=l+8;
						newbuf[k++]='3';
						OLED_ShowStr(k+9+l,3,"*",2);
						printf("%d\r\n",id);
					}
					if(id==4)
					{
						l=l+8;
						newbuf[k++]='4';
						OLED_ShowStr(k+9+l,3,"*",2);
						printf("%d\r\n",id);
					}
					if(id==5)
					{
						l=l+8;
						newbuf[k++]='5';
						OLED_ShowStr(k+9+l,3,"*",2);
						printf("%d\r\n",id);
					}
					if(id==6)
					{
						l=l+8;
						newbuf[k++]='6';
						OLED_ShowStr(k+9+l,3,"*",2);
						printf("%d\r\n",id);
					}
					if(id==7)
					{
						l=l+8;
						newbuf[k++]='7';
						OLED_ShowStr(k+9+l,3,"*",2);
						printf("%d\r\n",id);
					}
					if(id==8)
					{
						l=l+8;
						newbuf[k++]='8';
						OLED_ShowStr(k+9+l,3,"*",2);
						printf("%d\r\n",id);
					}
					if(id==9)
					{
						l=l+8;
						newbuf[k++]='9';
						OLED_ShowStr(k+9+l,3,"*",2);
						printf("%d\r\n",id);
					}
					if(id==0)
					{
						l=l+8;
						newbuf[k++]='0';
						OLED_ShowStr(k+9+l,3,"*",2);
						printf("%d\r\n",id);
					}
					if(id==10)
					{
						id=atoi(newbuf);
						break;
					}
				}
				do
					ID=id;
				while(!(ID<AS608Para.PS_max));//输入ID必须小于指纹容量的最大值
				sprintf(str,"Location %d",ID);
				OLED_CLS();
				OLED_ShowStr(0,4,str,2);
				OLED_CLS();
				ensure=PS_StoreChar(CharBuffer2,ID);//储存模板
				if(ensure==0x00) 
				{					
					for(j=68;j<74;j++)
					{
						OLED_ShowCN(20+(j-68)*16,3,j);//录入指纹成功
					}
					PS_ValidTempleteNum(&ValidN);//读库指纹个数
					press_FR();
					return ;
				}else {processnum=0;OLED_CLS();OLED_ShowStr(0,4,"ShowErrMessage",2);}					
				break;				
		}
		delay_ms(2000);
		if(i==5)//超过5次没有按手指则退出
		{
			OLED_CLS();
			for(j=74;j<86;j++)
				{
					OLED_ShowCN(10+(j-74)*16,3,j);//超过5次没有按手指则退出
				}
			break;	
		}				
	}
}

 
//刷指纹功能
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *str;
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//搜索成功
			{						
				printf("搜索指纹成功\r\n");
				str=(char*)malloc(10);//不能太大
				sprintf(str,"对应ID:%d  对比度分数:%d",seach.pageID,seach.mathscore);//显示匹配指纹的ID和分数
				printf("%s\r\n",str);
				free(str);
			}
			else 
				ShowErrMessage(ensure);					
	  }
		else
			ShowErrMessage(ensure);
	 delay_ms(1000);
	}
		
}
//刷指纹功能
void press_FR1(void)
{
	u8 i;
	SearchResult seach;
	u8 ensure;
	char *str;
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//搜索成功
			{						
				OLED_CLS();//清屏
				for(i=114;i<120;i++)
				{
					OLED_ShowCN(20+(i-114)*16,4,i);//搜索指纹成功	
				}
				str=(char*)malloc(10);//不能太大
				sprintf(str,"对应ID:%d  对比度分数:%d",seach.pageID,seach.mathscore);//显示匹配指纹的ID和分数
				printf("%s\r\n",str);
				free(str);
			}
			else 
			{OLED_CLS();OLED_ShowStr(0,4,"ShowErrMessage",2);	}			
	  }
		else
		{OLED_CLS();OLED_ShowStr(0,4,"ShowErrMessage",2);}
	 delay_ms(1000);
	}
		
} 
 
 
//删除指纹功能
void Del_FR(void)	/*输入删除指纹ID*/
{
	u8  ensure;
	u16 ID=0;
	printf("请输入你要删除的指纹\r\n");
	while(ID==0)
	{
		ID=atoi(rx_buffer);
	}
	ensure=PS_DeletChar(ID,1);//删除单个指纹
	if(ensure==0)
	{
		printf("删除指纹成功\r\n");		
	}
  else
		ShowErrMessage(ensure);	
	delay_ms(1200);
	PS_ValidTempleteNum(&ValidN);//读库指纹个数
 
}