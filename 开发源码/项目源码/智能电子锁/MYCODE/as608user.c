#include "as608.h"
#include "as608user.h"
#include "stdlib.h"
#include "stdio.h"
#include "usart.h"
#include "delay.h"
#include "OLED_I2C.h"
#include "key.h"
extern SysPara AS608Para;//ָ��ģ��AS608����
//extern u16 USART2_RX_STA;       //����״̬���	  
//extern u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
extern u16 ValidN;//ģ������Чģ�����
extern u8 rx_buffer[32];
//��ʾȷ���������Ϣ
void ShowErrMessage(u8 ensure)
{
	printf("%s",EnsureMessage(ensure));
	delay_ms(1000);
}
 
//¼ָ�ƹ���
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
				printf("�밴ָ�ƣ�\r\n");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					printf("ָ��������\r\n");
					ensure=PS_GenChar(CharBuffer1);//��������
					if(ensure==0x00)
					{
						i=0;
						processnum=1;//�����ڶ���						
					}
					else 
						printf("��������ʧ�ܣ�\r\n");					
				}
				else
					 printf("ָ�Ʋɼ�ʧ�ܣ�\r\n");
				break;
			
			case 1:
				i++;
				printf("�밴�ٰ�һ��ָ�ƣ�\r\n");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer2);//�ٰ�һ��ָ��
					if(ensure==0x00)
					{
						printf("ָ��������\r\n");
						i=0;
						processnum=2;//����������
					}
					else
					{ printf("��������ʧ�ܣ�\r\n");}	
				}else {printf("ָ�Ʋɼ�ʧ�ܣ�\r\n");}		
				break;
 
			case 2:
				printf("�Ա�����ָ�ƣ�\r\n");
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					processnum=3;//�������Ĳ�
				}
				else 
				{
					printf("�Աȳɹ�,����ָ��һ����\r\n");		
					i=0;
					processnum=0;//���ص�һ��		
				}
				delay_ms(1200);
				break;
 
			case 3:
				printf("����ָ��ģ�壡\r\n");
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					printf("����ָ��ģ��ɹ���\r\n");
					processnum=4;//�������岽
				}else {processnum=0;printf("����ָ��ģ��ʧ�ܣ�\r\n");}
				delay_ms(1200);
				break;
				
			case 4:	
				printf("�����봢��ID,1=< ID <=299,��Enter����\r\n");
				while(ID<=0||ID>299)
				{
					ID=atoi(rx_buffer);
				}
				do
					printf("%d\r\n",AS608Para.PS_max);
				while(!(ID<AS608Para.PS_max));//����ID����С��ָ�����������ֵ
				printf("����ID��λ��:%d\r\n",ID);
				ensure=PS_StoreChar(CharBuffer2,ID);//����ģ��
				if(ensure==0x00) 
				{					
					printf("¼��ָ�Ƴɹ���\r\n");
					PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
					press_FR();
					return ;
				}else {processnum=0;printf("��ȡָ�Ƹ���ʧ�ܣ�\r\n");}	
				printf("�����ɹ�\r\n");
				break;				
		}
		delay_ms(2000);
		if(i==5)//����5��û�а���ָ���˳�
		{
			printf("�������û����ָ\r\n");//����5��û�а���ָ���˳�
			break;	
		}				
	}
}
//¼ָ�ƹ���
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
					OLED_ShowCN(28+(j-16)*16,3,j);//�밴ָ��
				}
				OLED_CLS();
				OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//��������
					if(ensure==0x00)
					{
						i=0;
						processnum=1;//�����ڶ���						
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
					ensure=PS_GenChar(CharBuffer2);//�ٰ�һ��ָ��
					if(ensure==0x00)
					{
						i=0;
						processnum=2;//����������
					}
					else
					{OLED_CLS();OLED_ShowStr(0,4,"ShowErrMessage",2);}	
				}else {OLED_CLS();OLED_ShowStr(0,4,"ShowErrMessage",2);}		
				break;
 
			case 2:
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					processnum=3;//�������Ĳ�
				}
				else 
				{
					OLED_CLS();
					OLED_ShowStr(0,4,"ShowErrMessage",2);		
					i=0;
					processnum=0;//���ص�һ��		
				}
				delay_ms(1200);
				break;
 
			case 3:
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					processnum=4;//�������岽
				}else {processnum=0;OLED_CLS();OLED_ShowStr(0,4,"ShowErrMessage",2);}
				delay_ms(1200);
				break;
				
			case 4:	
				OLED_CLS();
				for(j=86;j<93;j++)
				{
					OLED_ShowCN(20+(j-86)*16,3,j);//�밴ָ��
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
				while(!(ID<AS608Para.PS_max));//����ID����С��ָ�����������ֵ
				sprintf(str,"Location %d",ID);
				OLED_CLS();
				OLED_ShowStr(0,4,str,2);
				OLED_CLS();
				ensure=PS_StoreChar(CharBuffer2,ID);//����ģ��
				if(ensure==0x00) 
				{					
					for(j=68;j<74;j++)
					{
						OLED_ShowCN(20+(j-68)*16,3,j);//¼��ָ�Ƴɹ�
					}
					PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
					press_FR();
					return ;
				}else {processnum=0;OLED_CLS();OLED_ShowStr(0,4,"ShowErrMessage",2);}					
				break;				
		}
		delay_ms(2000);
		if(i==5)//����5��û�а���ָ���˳�
		{
			OLED_CLS();
			for(j=74;j<86;j++)
				{
					OLED_ShowCN(10+(j-74)*16,3,j);//����5��û�а���ָ���˳�
				}
			break;	
		}				
	}
}

 
//ˢָ�ƹ���
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *str;
	ensure=PS_GetImage();
	if(ensure==0x00)//��ȡͼ��ɹ� 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //���������ɹ�
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//�����ɹ�
			{						
				printf("����ָ�Ƴɹ�\r\n");
				str=(char*)malloc(10);//����̫��
				sprintf(str,"��ӦID:%d  �Աȶȷ���:%d",seach.pageID,seach.mathscore);//��ʾƥ��ָ�Ƶ�ID�ͷ���
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
//ˢָ�ƹ���
void press_FR1(void)
{
	u8 i;
	SearchResult seach;
	u8 ensure;
	char *str;
	ensure=PS_GetImage();
	if(ensure==0x00)//��ȡͼ��ɹ� 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //���������ɹ�
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//�����ɹ�
			{						
				OLED_CLS();//����
				for(i=114;i<120;i++)
				{
					OLED_ShowCN(20+(i-114)*16,4,i);//����ָ�Ƴɹ�	
				}
				str=(char*)malloc(10);//����̫��
				sprintf(str,"��ӦID:%d  �Աȶȷ���:%d",seach.pageID,seach.mathscore);//��ʾƥ��ָ�Ƶ�ID�ͷ���
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
 
 
//ɾ��ָ�ƹ���
void Del_FR(void)	/*����ɾ��ָ��ID*/
{
	u8  ensure;
	u16 ID=0;
	printf("��������Ҫɾ����ָ��\r\n");
	while(ID==0)
	{
		ID=atoi(rx_buffer);
	}
	ensure=PS_DeletChar(ID,1);//ɾ������ָ��
	if(ensure==0)
	{
		printf("ɾ��ָ�Ƴɹ�\r\n");		
	}
  else
		ShowErrMessage(ensure);	
	delay_ms(1200);
	PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
 
}