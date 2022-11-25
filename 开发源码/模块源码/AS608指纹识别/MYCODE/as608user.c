#include "as608.h"
#include "as608user.h"
#include "stdlib.h"
#include "stdio.h"
#include "usart.h"
#include "delay.h"

SysPara AS608Para;//指纹模块AS608参数
//extern u16 USART2_RX_STA;       //接收状态标记	  
//extern u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 N;//模块内有效模板个数
 
//显示确认码错误信息
void ShowErrMessage(u8 ensure)
{
	printf("%s",EnsureMessage(ensure));
	delay_ms(1000);
}
 
//录指纹功能
void Add_FR(u16 FR_ID)
{
	u8 i,ensure ,processnum=0;
	u16 ID;
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
				printf("请按指纹\r\n");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//生成特征
					if(ensure==0x00)
					{
						printf("指纹正常\r\n");
						i=0;
						processnum=1;//跳到第二步						
					}
					else 
						ShowErrMessage(ensure);				
				}
				else 
					ShowErrMessage(ensure);						
				break;
			
			case 1:
				i++;
				printf("请按再按一次指纹\r\n");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer2);//生成特征
					if(ensure==0x00)
					{
						printf("指纹正常\r\n");
						i=0;
						processnum=2;//跳到第三步
					}else ShowErrMessage(ensure);	
				}else ShowErrMessage(ensure);		
				break;
 
			case 2:
				printf("对比两次指纹\r\n");
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					printf("对比成功,两次指纹一样\r\n");
					processnum=3;//跳到第四步
				}
				else 
				{
					printf("对比失败，请重新录入指纹\r\n");
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//跳回第一步		
				}
				delay_ms(1200);
				break;
 
			case 3:
				printf("生成指纹模板\r\n");
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					printf("生成指纹模板成功\r\n");
					processnum=4;//跳到第五步
				}else {processnum=0;ShowErrMessage(ensure);}
				delay_ms(1200);
				break;
				
			case 4:	
				printf("储存指纹ID\r\n");
				printf("0=< ID <=299\r\n");
				do
					ID=FR_ID;
				while(!(ID<AS608Para.PS_max));//输入ID必须小于指纹容量的最大值
				printf("%d\t\n",ID);
				ensure=PS_StoreChar(CharBuffer2,ID);//储存模板
				if(ensure==0x00) 
				{					
					printf("录入指纹成功\r\n");
					PS_ValidTempleteNum(&N);//读库指纹个数
					press_FR();
					return ;
				}else {processnum=0;ShowErrMessage(ensure);}					
				break;				
		}
		delay_ms(2000);
		if(i==5)//超过5次没有按手指则退出
		{
			printf("超过5次没有按手指则退出\r\n");
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
 
 
 
//删除指纹功能
void Del_FR(u16 FR_ID)	/*输入删除指纹ID*/
{
	u8  ensure;
	printf("删除指纹\r\n");
	ensure=PS_DeletChar(FR_ID,1);//删除单个指纹
	if(ensure==0)
	{
		printf("删除指纹成功\r\n");		
	}
  else
		ShowErrMessage(ensure);	
	delay_ms(1200);
	PS_ValidTempleteNum(&N);//读库指纹个数
 
}
 