#include "flash.h"



static uint32_t GetSector(uint32_t Address)
{
	uint32_t sector = 0;

	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	{
	  sector = FLASH_Sector_0;  
	}
	else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	{
	  sector = FLASH_Sector_1;  
	}
	else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	{
	  sector = FLASH_Sector_2;  
	}
	else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	{
	  sector = FLASH_Sector_3;  
	}
	else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	{
	  sector = FLASH_Sector_4;  
	}
	else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	{
	  sector = FLASH_Sector_5;  
	}
	else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	{
	  sector = FLASH_Sector_6;  
	}
	else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
	{
	  sector = FLASH_Sector_7;  
	}

	return sector;
}






//��������
void Flash_write(u32 addr, u8 *write_buff, u32 len)
{

	u32 FLASH_USER_START_ADDR, FLASH_USER_END_ADDR;
	u32 uwStartSector, uwEndSector, uwSectorCounter, uwAddress;
	
	/* Unlock the Flash *********************************************************/
	/* Enable the flash control register access */
	//FLASH����
	FLASH_Unlock();
	  
	/* Erase the user Flash area ************************************************/
	/* area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */

	/* Clear pending flags (if any) */  
	//��ձ�־λ
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
					FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

	//���㿪ʼ��ַ
	FLASH_USER_START_ADDR = addr;
	//���������ַ
	FLASH_USER_END_ADDR = FLASH_USER_START_ADDR+len;
	
	/* Get the number of the start and end sectors */
	//��ȡ��ʼ�����������
	uwStartSector = GetSector(FLASH_USER_START_ADDR);
	uwEndSector = GetSector(FLASH_USER_END_ADDR);

	/* Strat the erase operation */
	//��ʼ��д������ֵ��uwSectorCounter
	uwSectorCounter = uwStartSector;
	
	//ѭ����������
	while (uwSectorCounter <= uwEndSector) 
	{
	  /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
		 be done by word */ 
	  if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE)
	  { 
		/* Error occurred while sector erase. 
		   User can add here some code to deal with this error  */
			printf("erase failure\r\n");
			return ;
	  }
	  /* jump to the next sector */

	 uwSectorCounter += 8;
	  
	}

	/* Program the user Flash area word by word ********************************/
	/* area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */
	//��ʼ��ַ��ֵ��uwAddress
	uwAddress = FLASH_USER_START_ADDR;
	
	//ѭ��д����
	while (uwAddress < FLASH_USER_END_ADDR)
	{
		//���ֽ�д��FLASH����
	  if (FLASH_ProgramByte(uwAddress, *write_buff) == FLASH_COMPLETE)
	  {
		  //FLASH��ַ��1
		uwAddress = uwAddress + 1;
		  //���ݵ�ַ�ռ��1
		  write_buff++;
	  }
	  else
	  { 
			printf("erase failure\r\n");
			return ;
	  }
	}

	/* Lock the Flash to disable the flash control register access (recommended
	   to protect the FLASH memory against possible unwanted operation) */
	
	//FLASH����
	FLASH_Lock(); 

}

void Flash_Read(u32 addr, u8 *read_buff, u32 len)
{
	
	u32 FLASH_USER_START_ADDR, FLASH_USER_END_ADDR, uwAddress;
	
	
	//���㿪ʼ��ַ
	FLASH_USER_START_ADDR = addr;
	//���������ַ
	FLASH_USER_END_ADDR = FLASH_USER_START_ADDR+len;	
	
	 uwAddress = FLASH_USER_START_ADDR;
	 
	 
	//ѭ��������
	 while (uwAddress < FLASH_USER_END_ADDR)
	 {
	   *read_buff = *(__IO uint8_t*)uwAddress;


		  //FLASH��ַ��1
		uwAddress = uwAddress + 1;
		  //���ݵ�ַ�ռ��1
		  read_buff++;
	 }  



}




