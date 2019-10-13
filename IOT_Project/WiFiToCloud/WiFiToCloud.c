/******************************************************************************
*	@file		WIFIToCloud.c
*	@author
*	@version
*	@data		2019-09
*	@brief		WIFIģ��ķ�װ����
*
******************************************************************************/


#include "WiFiToCloud.h"

 
/*******************************************************************
*	@brief		���AT���ڽ��ջ�����ؼĴ���
*	@param		None
*	@retval		None
*	@attention	
*******************************************************************/
void ClrAtRxBuf(void)
{
	memset(AT_RX_BUF, 0x00, MAX_AT_RX_LEN);//��ջ���

	AT_RX_STA =0x00000000;
}
/*******************************************************************
*	@brief		ͨ�� USART4 ����ATָ���ַ���
*	@param		uint8_t *string �����͵�ATָ��
*				uint8_t len ����
*	@retval		None
*	@attention
*******************************************************************/
void SendAtCmd(uint8_t *string, uint8_t len)
{
	UART4_SendStrlen(string, len);
	UART4_SendStrlen((uint8_t *)"\r\n", 2);
}

/*******************************************************************
*	@brief		ͨ�� USART4 ���͸��������ַ���
*	@param		uint8_t *string �����͵��ַ���
*				uint8_t len ����
*	@retval		None
*	@attention
*******************************************************************/
void SendStrLen(uint8_t *string, uint8_t len)
{
	UART4_SendStrlen(string, len);
}



/*******************************************************************
*	@function 	int8_t ESP8266_SetStation(void)
*	@brief		ESP8266����Ϊstationģʽ
*	@param 		None
*	@retval
*		@val	return = 0 ,sucess
*		@val	return < 0 ,error
*	@attention
*******************************************************************/
int8_t ESP8266_SetStation(void)
{
	ClrAtRxBuf();//��ջ���
	SendAtCmd((uint8_t *)AT_CWMODE,strlen(AT_CWMODE));
	delay_ms(100);
	if(strstr((const char *)AT_RX_BUF, (const char *)"OK") == NULL)
	{
		return -1;
	}
	ClrAtRxBuf();
	return 0;
}

/*******************************************************************
*	@function	int8_t ESP8266_SetAP(void)
*	@brief		����ESP8266Ҫ���ӵ��ȵ����ƺ�����
*	@param		char *wifi:�ȵ����� 
*				char *pwd:�ȵ�����
*	@retval 
		@val 	return = 0 ,sucess
		@val	return < 0 ,error
*	@attention
*******************************************************************/
int8_t ESP8266_SetAP(char *wifi, char *pwd)
{
	uint8_t AtCwjap[MAX_AT_TX_LEN];
	memset(AtCwjap, 0x00, MAX_AT_TX_LEN);//��ջ���
	ClrAtRxBuf();//��ջ���
	sprintf((char *)AtCwjap,"AT+CWJAP_CUR=\"%s\",\"%s\"",wifi, pwd);
//	printf("%s\r\n",AtCwjap);////////////////////////////////////////////////////////////
	SendAtCmd((uint8_t *)AtCwjap,strlen((const char *)AtCwjap));
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	if(strstr((const char *)AT_RX_BUF, (const char *)"OK") == NULL)
	{
		return -1;
	}
	ClrAtRxBuf();
	return 0;
}

/*******************************************************************
*	@function	int8_t ESP8266_SetStation(void)
*	@brief		ESP8266����TCP����
*	@param 		char *IpAddr-IP��ַ�����磺120.77.58.34
*				uint16_t port-�˿ںţ�ȡֵ0~65535
*	@retval
*		@val	return = 0 ,sucess
*		@val	return < 0 ,error
*	@attention
*******************************************************************/
int8_t ESP8266_IpStart(char *IpAddr, uint16_t port)
{
	uint8_t IpStart[MAX_AT_TX_LEN];
	memset(IpStart, 0x00, MAX_AT_TX_LEN);//��ջ���
	ClrAtRxBuf();//��ջ���
	sprintf((char *)IpStart,"AT+CIPSTART=\"TCP\",\"%s\",%d",IpAddr, port);
	//printf("%s\r\n",IpStart);////////////////////////////////////////////////////////////
	SendAtCmd((uint8_t *)IpStart,strlen((const char *)IpStart));
	delay_ms(1500);
	if(strstr((const char *)AT_RX_BUF, (const char *)"OK") == NULL)
	{
		return -1;
	}
	ClrAtRxBuf();
	return 0;
}

/*******************************************************************
*	@function	int8_t ESP8266_IpSend(char *IpBuf, uint8_t len)
*	@brief		ESP8266��������
*	@param		char *IpBuf-IP����
*				uint8_t len-���ݳ���
*	@retval
*		@val	return = 0 ,sucess
*		@val	return < 0 ,error
*	@attention
*******************************************************************/
int8_t ESP8266_IpSend(char *IpBuf, uint8_t len)
{
	uint8_t TryGo = 0;
	int8_t error = 0;
	uint8_t IpSend[MAX_AT_TX_LEN];
	memset(IpSend, 0x00, MAX_AT_TX_LEN);//��ջ���
	ClrAtRxBuf();//��ջ���
	sprintf((char *)IpSend,"AT+CIPSEND=%d",len);
//	printf("%s\r\n",IpSend);////////////////////////////////////////////////////////////
	SendAtCmd((uint8_t *)IpSend,strlen((const char *)IpSend));
	delay_ms(3);
//	printf("nprintf = %s\r\n",AT_RX_BUF);
	if(strstr((const char *)AT_RX_BUF, (const char *)"OK") == NULL)
	{
		return -1;
	}
	ClrAtRxBuf();//��ջ���
	SendStrLen((uint8_t *)IpBuf, len);
//	printf("%s\r\n",IpBuf);////////////////////////////////////////////////////////////
	for(TryGo = 0; TryGo<60; TryGo++)//���ȴ�ʱ��100*60=6000ms
	{
		if(strstr((const char *)AT_RX_BUF, (const char *)"SEND OK") == NULL)
		{
			error = -2;
		}
		else
		{
			error = 0;
			break;
		}
		delay_ms(100);
	}
//	ClrAtRxBuf();
	return error;
}

/*******************************************************************
*	@function	int8_t ConnectToServer(void)
*	@brief		���ӵ�������
*	@param 		None
*	@retval
*		@val	return = 0 ,sucess
*		@val	return < 0 ,error
*	@attention
*******************************************************************/
int8_t ConnectToServer(char *DeviceID, char *SecretKey)
{
	uint8_t TryGo = 0;
	int8_t error = 0;
	uint8_t TxetBuf[MAX_AT_TX_LEN];
	memset(TxetBuf,0x00,MAX_AT_TX_LEN);//��ջ���
	for(TryGo = 0; TryGo<3; TryGo++)
	{
		if(ESP8266_SetStation() == 0)//����WiFiͨѶģ�鹤��ģʽ
		{
			error = 0;
			break;
		}
		else
		{
			error = -1;
		}
	}
	if(error < 0)
	{
		return error;
	}
	for(TryGo = 0; TryGo<3; TryGo++)
	{
		if(ESP8266_SetAP((char *)WIFI_AP, (char *)WIFI_PWD) == 0)//�����ȵ����ƺ�����
		{
			error = 0;
			break;
		}
		else
		{
			error = -2;
		}
	}
	if(error < 0)
	{
		return error;
	}
	for(TryGo = 0; TryGo<3; TryGo++)
	{
		if(ESP8266_IpStart((char *)SERVER_IP,SERVER_PORT) == 0)//���ӷ�����IP��ַ���˿ڣ�120.77.58.34,8600
		{
			error = 0;
			break;
		}
		else
		{
			error = -3;
		}
	}
	if(error < 0)
	{
		return error;
	}
	
	sprintf((char *)TxetBuf,"{\"t\":1,\"device\":\"%s\",\"key\":\"%s\",\"ver\":\"v0.0.0.0\"}",DeviceID,SecretKey);
	//printf("%s\r\n",TxetBuf);////////////////////////////////////////////////////////////
	if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
	{//����ʧ��
		error=-4;
	}
	else
	{//���ͳɹ�
		for(TryGo = 0; TryGo<50; TryGo++)//���ȴ�ʱ��50*10=500ms
		{
			if(strstr((const char *)AT_RX_BUF, (const char *)"\"status\":0") == NULL)//�����Ӧ״̬�Ƿ�Ϊ���ֳɹ�
			{
				error = -5;
			}
			else
			{
				error = 0;
				break;
			}
			delay_ms(10);
		}
	}
	return error;
}

/*******************************************************************
*	@function	int8_t ESP8266_SendSensor(char *SensorTag, char *TimeStr, uint8_t sensor)
*	@brief 		���ʹ������ݵ�������
*	@param		char *SensorTag	�����ǩ����
*				uint8_t sensor	������ֵ
*				char *TimeStr	�ɼ�����ʱ��ʱ�䣬����yyyy-mm-dd hh:mm:ss��ʽ
*	@retval
*	   	@val	return = 0 ,sucess
*		@val	return < 0 ,error
*	@attention
*******************************************************************/
int8_t ESP8266_SendSensor(char *SensorTag, char *TimeStr, uint8_t sensor)
{
	uint8_t TryGo = 0;
	int8_t error = 0;
	uint8_t TxetBuf[MAX_AT_TX_LEN];
	memset(TxetBuf,0x00,MAX_AT_TX_LEN);//��ջ���
	sprintf((char *)TxetBuf,"{\"t\":3,\"datatype\":2,\"datas\":{\"%s\":{\"%s\":%d}},\"msgid\":001}",SensorTag, TimeStr, sensor);
	//printf("%s\r\n",TxetBuf);////////////////////////////////////////////////////////////
	if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
	{//����ʧ��
		error=-1;
	}
	else
	{//���ͳɹ�
		for(TryGo = 0; TryGo<50; TryGo++)//���ȴ�ʱ��50*10=500ms
		{
			if(strstr((const char *)AT_RX_BUF, (const char *)"\"status\":0") == NULL)//�����Ӧ״̬�Ƿ�Ϊ"�ϱ��ɹ�"
			{
				error = -1;
			}
			else
			{
				error = 0;
				break;
			}
			delay_ms(10);
		}
		ClrAtRxBuf();
	}
	return error;
}

/*******************************************************************
*	@function	uint8_t ESP8266_GetIpData(uint8_t *AtRxBuf, char *GetIpData)
*	@brief		��������������
*	@param		uint8_t *AtRxBuf ��ԭʼAT���ڻ���
*		 		char *GetIpData ����ȡ������Itcp/ip����
*	@retval 	�����յ���IP���ݳ���
*	@attention
*		AT+CIPSEND=76
*		{"t":3,"datatype":2,"datas":{"alarm":{"2018-06-19 18:15:02":0}},"msgid":001}
*		+IPD,29:{"msgid":1,"status":0,"t":4}
*******************************************************************/
uint8_t ESP8266_GetIpData(uint8_t *AtRxBuf, char *GetIpData)
{
	char *Point = NULL;
	uint8_t len = 0;
	
	Point = strstr((const char *)AtRxBuf, (const char *)"+IPD,");
	if(Point != NULL)//���ģ���յ�TCP/IP���ݰ���
	{
		len=(Point[5]-'0')*10+Point[6]-'0';//����IP���ݳ��Ȳ��ᳬ��100���ʿ��԰�����λ���͡�ʮλ�������յ���IP���ݳ���
		Point = strstr((const char *)AtRxBuf, (const char *)":")+1;
		memcpy(GetIpData, Point, len);
		GetIpData[len]='\0';
		printf("�յ�IP���ݣ�%s\r\n",GetIpData);	
		printf("�յ�IP���ݳ���=%d\r\n",len);
	}
	
	return (len);
}


int8_t ESP8266_SendPing(void)
{
	int8_t error=0;
	uint8_t TryGo = 0;
	if(ESP8266_IpSend((char *)PING_REQ, strlen((const char *)PING_REQ)< 0))
	{
		//����ʧ��
		error=-1;
	}
	else
	{
		//���ͳɹ�
		for(TryGo = 0; TryGo<50; TryGo++)//���ȴ�ʱ��50*10=500ms
		{
			if(strstr((const char *)AT_RX_BUF, (const char *)PING_RSP) == NULL)//�����Ӧ״̬�Ƿ�Ϊ"�ϱ��ɹ�"
			{
				error = -1;
			}
			else
			{
				error = 0;
				break;
			}
			delay_ms(10);
		}
		ClrAtRxBuf();
	}
	return error;
}




void ConnectToServer_times(void)
{
	int8_t temp=0;
	//���ӷ�����
	for(u8 TryCount=0; TryCount<3; TryCount++)
	{
		LCD_PRINT(0,0,lcd_show_buf,"Connect to sever");
		LCD_PRINT(1,0,lcd_show_buf,"Try %d times",TryCount);
		LCD_PRINT(2,0,lcd_show_buf,"... ...");
		temp=ConnectToServer((char *)MY_DEVICE_ID, (char *)MA_SECRET_KEY);
		if(temp != 0)
		{
			printf("Connect To Server ERROR=%d\r\n",temp);
		}
		else
		{
			printf("Connection successful!\r\n");
			device_sta.is_connect_to_server = ON;
			break; // end Server connected successfully
		}
	}
	if(temp!=0)
	{
		printf("Connection failed!\r\n");
		device_sta.is_connect_to_server = OFF;
	}
	lcd_clr();
}
	


/*******************************************************************
*	@function		void ESP8266_DataAnalysisProcess(char *RxBuf)
*	@brief			��������������
*	@param			char *RxBuf �������·�����
*	@retval			None
*	@attention		�û������ڴ˻����ϸ������չ�ú���������ֻ�Ǹ��򵥵�DEMO
*******************************************************************/
void ESP8266_DataAnalysisProcess(char *RxBuf)
{
	if(strstr((const char *)RxBuf, (const char *)PING_REQ) != NULL)//��������
	{
		ESP8266_IpSend((char *)PING_RSP, strlen((const char *)PING_RSP));//��Ӧ����
	}
	else if(strstr((const char *)RxBuf, (const char *)CMD_REQ_STR) != NULL)//��������
	{
		if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"ctrl\"") != NULL)//��������
		{
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))//����
			{
				ESP8266_IpSend((char *)PING_RSP, strlen((const char *)PING_RSP));//��Ӧ����
			}
		}
		else if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"defense\"") != NULL)//����/��������
		{
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))//����
			{
				printf("������\r\n");
				;//...
				;//...
				;//...
			}
			else if((strstr((const char *)RxBuf, (const char *)"\"data\":0") != NULL))//����
			{
				printf("������\r\n");
				;//...
				;//...
				;//...
			}
		}
	}
}

/******************************************* END OF FILE ********************************************/

