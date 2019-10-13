/*****************************************
*@file		app_lib.c
*@author	Panda
*@version	V0.0.0
*@date		2019-10
*@brief		app ʹ�õ�һЩ����
******************************************/
#include "app_lib.h"

/*******************************************************************************
*@breief		������ʼ��
*@param			address: ��ʼ���ݵĵ�ַ
*				pdata:	����
*				len: ���������ݵĳ���
*				sum: ���������ݵĸ���
*@retval		success��
*				failed��0
********************************************************************************/
int32_t app_param_init(uint32_t address,uint8_t *pdata,uint32_t len,uint32_t sum)
{
	uint8_t i=0;
	uint32_t ret = 0;
	for(i=0;i<sum;i++)
		memset(pdata+i,0xFF,len);
	
	bsp_flash_erase(address,len*sum);
	ret = bsp_flash_write(address,pdata,len*sum);
	return ret;
}

/*******************************************************************************
*@breief		д����
*@param			address: д��ĵ�ַ
*				pdata:	д�������
*				len: ���������ݵĳ���
*				sum: ���������ݵĸ���
*@retval		success��
*				failed��0
********************************************************************************/
int32_t app_write_data(uint32_t address,uint8_t *pdata,uint32_t len,uint32_t sum)
{
	int32_t ret = 0;
	bsp_flash_erase(address,len * sum);
	ret = bsp_flash_write(address,pdata,len*sum);
	return ret;
}


/*******************************************************************************
*@breief		������
*@param			address: ��ȡ�ĵ�ַ
*				pdata:	��ȡ������
*				len: ���������ݵĳ���
*				sum: ���������ݵĸ���
*@retval		success��
*				
********************************************************************************/
int32_t app_read_data(uint32_t address,uint8_t *pdata,uint32_t len,uint32_t sum)
{
	int32_t ret=0;
	ret = bsp_flash_read(address,pdata,len*sum);
	return ret;
}

/*******************************************************************************
*@brief		Ѱ������
*@param		addr: ��ʼ��ַ
*			pdata:Ѱ�ҵ�����
*			len:  Ѱ�����ݵĳ���
*			sum:  ���������ݵĸ���
*@retval	�ɹ����������ݵ�ַ
*			ʧ�ܣ����ؿ�
********************************************************************************/
uint32_t *app_find_data(uint32_t address,uint8_t *pdata,uint32_t len,uint32_t sum)
{
	uint32_t *ret=0;
	ret = bsp_flash_find(address,pdata,len,sum);
	return ret;
}

/*******************************************************************************
*@breief		��������
*@param			address: �����ĵ�ַ
*				len: ���������ݵĳ���
*				sum: ���������ݵĸ���
*@retval		success:
*				failed:
********************************************************************************/
int32_t app_erase_data(uint32_t address,uint32_t len,uint32_t sum)
{
	int32_t ret=0;
	ret = bsp_flash_erase(address,len*sum);
	return ret;
}

/*******************************************************************************
*@breief		�������
*@param			address: �洢�����ݵĻ���ַ��ַ
*				pdata:	��ӵ�����
*				len: ���������ݵĳ���
*				*pnum: ��ǰ������
*				sum: ���������ݵĸ���
*@retval		success:
*				failed:
********************************************************************************/
int32_t app_add_data(uint32_t address,uint8_t *pdata,uint8_t *pnew,uint32_t len,uint32_t *pnum,uint32_t sum)
{
	int32_t ret = 0;
	uint32_t *pdest = NULL;
	uint32_t num = *pnum;
	if(num > sum)
		num = 0;
	
	pdest = (uint32_t*)(pdata + num * len);
	memset(pdest,0xFF,len);
	memcpy(pdest,pnew,len);
	num++;
	
	ret = app_erase_data(address,len,sum);
	if(ret != 0)
	{
		DEBUG("Flash erase error");
		ret = -1;
		return ret;
	}
	ret = app_write_data(address,pdata,len,sum);
	if(ret == 0)
	{
		DEBUG("Flash write error!");
		ret = -2;
		return ret;
	}
	*pnum = num;
	return ret; 
}




#define TEST_ADDR 0x08070000

void app_lib_test_dome(void)
{
	uint8_t num = 4;
	
	uint8_t w_buf[10][2]={"ab","cd","ef","gh"};
	uint8_t r_buf[10][2];
	
	
	printf("before: %s\r\n",w_buf[0]);
	app_write_data(0x08070000,w_buf[0],2,10); 
	
	app_read_data(0x08070000,r_buf[0],2,10);
	printf("after: %s\r\n",r_buf[0]);
	
	app_add_data(0x08070000,r_buf[0],(uint8_t*)"cc",2,(uint32_t *)&num,10);
	printf("after: %s\r\n",r_buf[0]);
	printf("num: %d\r\n",num);
//	

}