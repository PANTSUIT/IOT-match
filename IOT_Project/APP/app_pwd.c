/******************************************************************************
* @file    app/app_pwd.c 
* @author  zhao
* @version V1.0.0
* @date    2018.06.18
* @brief   ����eeprom����
******************************************************************************/
#include "app_pwd.h"
#define __DEBUG__  
#ifdef __DEBUG__  
#define DEBUG(format,...) printf("File: "__FILE__", Line: %05d: "format"\r\n", __LINE__, ##__VA_ARGS__)  
#else  
#define DEBUG(format,...)  
#endif  

// Digital passwd
unsigned char DigitPwd_Cache[MAX_DIGIT_PWD_NUM][DIGIT_PWD_LEN];	//�������뻺��
unsigned char DigitPwd_Offset = 0;		//���뱣���ڻ����е�λ��
unsigned char CurDigitPwd_Cache[DIGIT_PWD_LEN];	//��ǰ���������
unsigned char CurDigitPwd_Offset = 0;	//��ǰ�������뵽�ڼ�λ

//LF ID card password
unsigned char IDCardPwd_Cache[MAX_ID_CARD_PWD_NUM][ID_CARD_PWD_LEN];	//ID card���뻺��
unsigned char IDCardPwd_Offset = 0;	//���뱣���ڻ����е�λ��
unsigned char CurIDCardPwd_Cache[ID_CARD_PWD_LEN];	//��ǰ���������


MEMSDataTypedef memsData = 
{
	.static_ADCValue[0] = 0,
	.static_ADCValue[1] = 0,
	.static_ADCValue[2] = 0,
};


/*********************************************************
*	@brief 	ClearPwdCache ����������뻺��
*	@param 	None
*	@retur	None
*	@note  	1.������ʼ��
*			2.�Ƿ�����ϵ�  ��һ���ϵ� �������� 
**********************************************************/
void clear_pwdcache(void)
{
	CurDigitPwd_Offset = 0;
	memset(CurDigitPwd_Cache, 0, DIGIT_PWD_LEN);  // ��յ�ǰ������������뻺��
	memset(CurIDCardPwd_Cache, 0, ID_CARD_PWD_LEN); // ��� ID Card �����뻺��
}

/**********************************************************
*	@brief  	IsRightPwd_Digit �ж����������Ƿ���ȷ
*	@param  	pwd:���жϵ���������
*				len:��������ĳ���	
*	@retval 	���������Ƿ���ȷ
*		@val	PWD_SUCCESS:����������ȷ
*		@val	PWD_ERROR:�����������	
*	@note  		1.������ʼ��
*               2.�Ƿ�����ϵ�  ��һ���ϵ� �������� 
***********************************************************/
int IsRight_Pwd_Digit(unsigned char pwd[],int len)
{
	int i;
	if(len != DIGIT_PWD_LEN) //�жϳ���
	{
		printf("Digit Password error: Please enter %d-digit password!\r\n",DIGIT_PWD_LEN);
		return PWD_ERROR; // end ���볤�Ȳ�ƥ��
	}
	for(i=0; i < MAX_DIGIT_PWD_NUM; i++)
	{
		if(memcmp(pwd, DigitPwd_Cache[i], DIGIT_PWD_LEN) == 0) // ���Ի���������н��бȶ�
		{
			break;
		}
	}
	if(i < MAX_DIGIT_PWD_NUM)	// �ڻ���� MAX_DIGIT_PWD_NUM �������У��ҵ��˸���������
	{
		return PWD_SUCCESS; // ������������ȷ
	}
	else	// �ڻ���� MAX_DIGIT_PWD_NUM �������У�û���ҵ�����������
	{
		printf("Digit Password error: Cannot find the matching password!\r\n");
		return PWD_ERROR;
	}
}

/***********************************************************
*	@brief  IsRightPwd_IdCard �ж�ID�������Ƿ���ȷ
*	@param  none * 
*	@return	PWD_SUCCESS  
*			PWD_ERROR
*	@note  1.������ʼ��
*	       2.�Ƿ�����ϵ�  ��һ���ϵ� �������� 
************************************************************/
int IsRight_Pwd_IdCard(unsigned char pwd[],int len)
{
	int i;
	Read_IDCardPwd();
	if(len != ID_CARD_PWD_LEN)
	{
		printf("ID Card Password error��Please enter %d-digit password!\r\n",ID_CARD_PWD_LEN);	
		return PWD_ERROR;
	}
	for(i=0; i < MAX_ID_CARD_PWD_NUM; i++)
	{
		if(memcmp(pwd, &IDCardPwd_Cache[i][0], ID_CARD_PWD_LEN) == 0)
		{
			break;
		}
	}
	printf("ID PWD's i=%d\r\n",i);
	if(i < MAX_ID_CARD_PWD_NUM)	// �ڻ���� MAX_DIGIT_PWD_NUM �������У��ҵ��˸�ID Card password
	{	
		return PWD_SUCCESS;
	}
	else
	{
		printf("ID Card Password error: Cannot find the matching password!\r\n");
		return PWD_ERROR;// �ڻ���� MAX_DIGIT_PWD_NUM �������У�û���ҵ���ID Card password
	}
}


/********************************************************
*	@brief  paramInit
*	@param  none
*	@return none
*	@note  	1.������ʼ��
*			2.�Ƿ�����ϵ�  ��һ���ϵ� �������� 
*		   	3.����ǵ�һ���ϵ�0 byte д�� 0xA3
*********************************************************/
void param_init(void)
{
	unsigned char save_flag;
	unsigned short NumByteToRead = 1;
	unsigned short NumByteToWrite;
	int i;
	
    bsp_flash_read(EEP_SAVE_FLAG_ADDRESS, &save_flag, NumByteToRead);

//	DEBUG("%x", save_flag);
	if(save_flag != 0xA5)
	{
		save_flag = 0xA5;

		// 0byte write 0xA3
		NumByteToWrite = 1;
		
		bsp_flash_erase(EEP_SAVE_FLAG_ADDRESS, NumByteToWrite);
		bsp_flash_write(EEP_SAVE_FLAG_ADDRESS, &save_flag, NumByteToWrite);
		
		//digit pwd Init
		for(i=0; i < MAX_DIGIT_PWD_NUM; i++)
		{
			if(i == 0)
			{
				memset(DigitPwd_Cache[i], 0x00, DIGIT_PWD_LEN);//default pwd 000000
			}
			else
			{
				memset(DigitPwd_Cache[i], 0xEE, DIGIT_PWD_LEN);
			}
		}
		
		// Write DigitPwd_Offset
		NumByteToWrite = 1;
		
		bsp_flash_erase(EEP_DIGIT_PWD_OFFSET_ADDRESS, NumByteToWrite);
		bsp_flash_write(EEP_DIGIT_PWD_OFFSET_ADDRESS, &DigitPwd_Offset, NumByteToWrite);
		// Write DigitPwd_Cache
		NumByteToWrite = MAX_DIGIT_PWD_NUM * DIGIT_PWD_LEN;
		
		bsp_flash_erase(EEP_DIGIT_PWD_START_ADDRESS, NumByteToWrite);
		bsp_flash_write(EEP_DIGIT_PWD_START_ADDRESS, (unsigned char *)DigitPwd_Cache[0], NumByteToWrite);
		
		//id card pwd init
		for(i=0; i < MAX_ID_CARD_PWD_NUM; i++)
		{
		  memset(DigitPwd_Cache[i], 0xEE, ID_CARD_PWD_LEN);
		}
		//Write IDCardPwd_Offset
		NumByteToWrite = 1;
		
		bsp_flash_erase(EEP_ID_CARD_PWD_OFFSET_ADDRESS, NumByteToWrite);
		bsp_flash_write(EEP_ID_CARD_PWD_OFFSET_ADDRESS, &IDCardPwd_Offset, NumByteToWrite);
		//Write IDCardPwd_Cache
		NumByteToWrite = MAX_ID_CARD_PWD_NUM * ID_CARD_PWD_LEN;
		
		bsp_flash_erase(EEP_ID_CARD_PWD_START_ADDRESS, NumByteToWrite);
		bsp_flash_write(EEP_ID_CARD_PWD_START_ADDRESS, (unsigned char *)IDCardPwd_Cache[0], NumByteToWrite);
		
		//mems
		memsData.theshold = 50;
		//memset(&memsData.static_ADCValue[0], 0, sizeof(memsData.static_ADCValue));
		
		NumByteToWrite = sizeof(MEMSDataTypedef);
		
		bsp_flash_erase(EEP_MEMS_START_ADDRESS, NumByteToWrite);
		bsp_flash_write(EEP_MEMS_START_ADDRESS, (unsigned char *)&memsData, NumByteToWrite);
	}
}
/*************************************************************************************
*@brief		���� Digit passsword
*@param		pwd: Ҫ����� Digit password
*			len: Digit password �ĳ���
*@retval	0:success
**************************************************************************************/
int Save_DigitPwd(unsigned char pwd[],int len)
{	
	unsigned short data_len = 0;
	
	if(DigitPwd_Offset>= MAX_DIGIT_PWD_NUM)
		DigitPwd_Offset = 0;	
	
	memset(DigitPwd_Cache[DigitPwd_Offset], 0xff, DIGIT_PWD_LEN);
	memcpy(DigitPwd_Cache[DigitPwd_Offset], pwd, len);
	
	DigitPwd_Offset++;
	
	// save DigitPwd_Offset
	data_len = 1;
	
	bsp_flash_erase(EEP_DIGIT_PWD_OFFSET_ADDRESS, data_len);
	bsp_flash_write(EEP_DIGIT_PWD_OFFSET_ADDRESS, &DigitPwd_Offset, data_len);
	
	//save DigitPwd_Cached
	data_len = MAX_DIGIT_PWD_NUM * DIGIT_PWD_LEN;
	bsp_flash_erase(EEP_DIGIT_PWD_START_ADDRESS, data_len);
	bsp_flash_write(EEP_DIGIT_PWD_START_ADDRESS,(unsigned char *)&DigitPwd_Cache[0][0], data_len);
	return 0;
}


/***************************************************************************
*	@brief  Read_DigitPwd ��ȡ��������
*	@param  none
*	@note  	1.������ʼ��
*	       	2.�����ڴ� calloc(128,sizeof(char))
*	       	3.��ȡ��ǰ����λ��
*	       	4.DigitPwd_Cache����дFF
*	       	5.��digitPwd����д�� debug_buff  ��ӡ ����  strcat �����ַ���
*	       	6.free �ͷ��ڴ�
*	@return:(unsigned char **)DigitPwd ָ���ַ
****************************************************************************/
unsigned char **Read_DigitPwd(void)
{	
	int i,j;
	unsigned short data_len = 0;
	char *debug_buf = calloc(128, sizeof(char));
	char temp_buf[8];
	// read DigitPwd_Offset
	data_len = 1;
	
	bsp_flash_read(EEP_DIGIT_PWD_OFFSET_ADDRESS, &DigitPwd_Offset, data_len);
	//erase DigitPwd_Cache
	memset(DigitPwd_Cache[0], 0xff, MAX_DIGIT_PWD_NUM * DIGIT_PWD_LEN);
	// read DigitPwd_Cache
	data_len = MAX_DIGIT_PWD_NUM * DIGIT_PWD_LEN;
	
	bsp_flash_read(EEP_DIGIT_PWD_START_ADDRESS, DigitPwd_Cache[0], data_len);

	for(i=0; i < MAX_DIGIT_PWD_NUM; i++)
	{
		for(j=0; j < DIGIT_PWD_LEN; j++)
		{
			sprintf(temp_buf, "%x ", DigitPwd_Cache[i][j]);
			strcat(debug_buf, temp_buf);
		}
		strcat(debug_buf, "\r\n");
	}
	DEBUG("DigitPwd_Cache: %s", debug_buf);
	delay_ms(5);
	free(debug_buf);
	return (unsigned char **)DigitPwd_Cache;
}

/***************************************************
*@breif		���� IDCard password
*@param		pwd:Ҫ����� IDCard password
*			len:Ҫ����� IDCard password �ĳ���
*@retval 	0: success
****************************************************/
int Save_IDCardPwd(unsigned char pwd[],int len)
{	
	unsigned short data_len = 0;
	//static unsigned char pwd_offset = 0;
	if(IDCardPwd_Offset>= MAX_ID_CARD_PWD_NUM)
		IDCardPwd_Offset = 0;
	
	memset(IDCardPwd_Cache[IDCardPwd_Offset], 0xff, ID_CARD_PWD_LEN);
	memcpy(IDCardPwd_Cache[IDCardPwd_Offset], pwd, len);
	IDCardPwd_Offset++;
	
	//save IDCardPwd_Offset
	data_len = 1;
	bsp_flash_erase(EEP_ID_CARD_PWD_OFFSET_ADDRESS, data_len);
	bsp_flash_write(EEP_ID_CARD_PWD_OFFSET_ADDRESS,&IDCardPwd_Offset, data_len);
	
	//save IDCardPwd_Cache
	data_len = MAX_ID_CARD_PWD_NUM * ID_CARD_PWD_LEN;
	bsp_flash_erase(EEP_ID_CARD_PWD_START_ADDRESS, data_len);
	bsp_flash_write(EEP_ID_CARD_PWD_START_ADDRESS,(unsigned char *)&IDCardPwd_Cache[0][0], data_len);
	return 0;
}


/*************************************************************************************
*@brief		ɾ�� ID Card passsword
*@param		pwd: Ҫ����� ID Card passsword
*			len: ID Card passsword �ĳ���
*@retval	0:success
**************************************************************************************/
int Delete_IDCardPwd(unsigned char pwd[],int len)
{	
	int i;
	unsigned short data_len;
	if(len != ID_CARD_PWD_LEN)
	{
		return PWD_ERROR;
	}
	for(i=0; i < MAX_ID_CARD_PWD_NUM; i++)
	{
		if(memcmp(pwd, IDCardPwd_Cache[i], ID_CARD_PWD_LEN) == 0)
		{
			// �� IDCardPwd_Cache ���ҵ��� pwd,ɾ������
			memset(IDCardPwd_Cache[i], 0xff, ID_CARD_PWD_LEN);
			break;
		}
	}
	if(i < MAX_ID_CARD_PWD_NUM) // 
	{
		// ����� IDCardPwd_Cache ���ҵ��� pwd,�����޸�
		data_len = MAX_ID_CARD_PWD_NUM * ID_CARD_PWD_LEN;
		
		bsp_flash_erase(EEP_ID_CARD_PWD_START_ADDRESS, data_len);
	    bsp_flash_write(EEP_ID_CARD_PWD_START_ADDRESS,(unsigned char *)&IDCardPwd_Cache[0][0], data_len);
	}
	return 0;
}

/*************************************************************************************
*@brief		�� ID Card passsword
*@param		None
*@retval	(unsigned char **)IDCardPwd_Cache -> ָ���ַ
**************************************************************************************/
unsigned char **Read_IDCardPwd(void)
{	
	unsigned short data_len = 0;
	// read IDCardPwd_Offset
	data_len = 1;
	
	bsp_flash_read(EEP_ID_CARD_PWD_OFFSET_ADDRESS, &IDCardPwd_Offset, data_len);
	// erase IDCardPwd_Cache
	memset(IDCardPwd_Cache[0], 0xff, MAX_ID_CARD_PWD_NUM * ID_CARD_PWD_LEN);
	// read IDCardPwd_Cache
	data_len = MAX_ID_CARD_PWD_NUM * ID_CARD_PWD_LEN;
	
	bsp_flash_read(EEP_ID_CARD_PWD_START_ADDRESS, IDCardPwd_Cache[0], data_len);
	return (unsigned char **)IDCardPwd_Cache;
}
/*************************************************************************************
*@brief		save memdata
*@param		None
*@retval	None
**************************************************************************************/
void Save_MemsData(void)
{
	unsigned short NumByteToWrite;	// д���ֽ���
	
	NumByteToWrite = sizeof(MEMSDataTypedef);
	//����
	bsp_flash_erase(EEP_MEMS_START_ADDRESS, NumByteToWrite);
    //д������
	bsp_flash_write(EEP_MEMS_START_ADDRESS,(unsigned char *)&memsData, NumByteToWrite);
	
	DEBUG("MEMS theshold %d, ADC Val:%d %d %d",memsData.theshold, memsData.static_ADCValue[0], memsData.static_ADCValue[1], memsData.static_ADCValue[2]);
}
/*************************************************************************************
*@brief		read memdata
*@param		None
*@retval	None
**************************************************************************************/
void Read_MemsData(void)
{
	unsigned short data_len;
	data_len = sizeof(MEMSDataTypedef);
	bsp_flash_read(EEP_MEMS_START_ADDRESS, (unsigned char *)&memsData, data_len);
	DEBUG("MEMS theshold %d, ADC Val:%d %d %d",memsData.theshold, memsData.static_ADCValue[0], memsData.static_ADCValue[1], memsData.static_ADCValue[2]);
}


