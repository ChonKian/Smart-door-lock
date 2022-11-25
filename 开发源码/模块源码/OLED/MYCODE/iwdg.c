#include "iwdg.h"


void Iwdg_Init(void)
{
	//1�� ȡ���Ĵ���д������
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//2�����ö������Ź���Ԥ��Ƶϵ����ȷ��ʱ��: 250HZ
	IWDG_SetPrescaler(IWDG_Prescaler_128);
	//3�����ÿ��Ź���װ��ֵ��ȷ�����ʱ��:2S 2S��ι�������� 2S�Ժ�ι��������
	IWDG_SetReload(500);
	
	//4��ʹ�ܿ��Ź�
	IWDG_Enable();

	//Ӧ�ó���ι��:
	IWDG_ReloadCounter();
}