/**********************************************************
*	@file 		NLECloud_TCP.c
*	@author		Panda
*	@version	V0.0.0
*	@date		2019-09
***********************************************************/
#include "NLECLoud_TCP.h"


const char *sensor_apitag[]={"\"sta_door\"","\"sta_lock\"","\"sta_lamp\"","\"sta_buzzer\""};

/*******************************************************************
*@brief 	jsonΪ�ַ������У���json��ʽ�е�Ŀ�����Tag��Ӧ��ֵ�ַ���ת��Ϊ��ֵ
*@param		char *cJson json�ַ���
			char *Tag Ҫ�����Ķ����ǩ
*@retval  ������ֵ���ַ�����ʽ����ʼ��ַ
********************************************************************/
char *GetJsonValue(char *cJson, char *Tag)
{
	char *target = NULL;
	static char temp[10];
	int8_t i=0;
	
	memset(temp, 0x00, sizeof(temp));
	sprintf(temp,"\"%s\":",Tag);
	
	target=strstr((const char *)cJson, (const char *)temp);
	if(target == NULL)
	{
		//printf("���ַ���\r\n");
		return NULL;
	}
	i=strlen((const char *)temp);
	target=target+i;
	memset(temp, 0x00, 128);
	for(i=0; i<10; i++, target++)//��ֵ����10��λΪ�Ƿ�������2^32=4294967296
	{
		if((*target<='9')&&(*target>='0'))
		{
			temp[i]=*target;
		}
		else
		{
			break;
		}
	}
	temp[i+1] = '\0';
	printf("��ֵ=%s\r\n",temp);
	return (char *)temp;
}

// ��ȡjson �����ַ���
char* json_getvalue(char *json_str,char *tag)
{
	char *temp;
	static char target[50]={0};
	int i=0;
	sprintf((char*)target,"\"%s\":",tag);
	temp = strstr(json_str,(char*)target);
	if(temp == NULL)
	{
//		printf("Error: Cannot find the Tag: %s\r\n",tag);
		return 0;
	}
	temp = temp + strlen((char*)target);
	memset(target,0x00,sizeof(target));
	for(;*temp!=',' && *temp!='}';temp++)
	{
		target[i++]= *temp;	 
	} 
//	printf("target: %s\r\n",target); 
	return target;	
}

// ��������ת�� ��string ת����number
double data_type_conversion_S2N(char* str)
{
	char *pstr=str;
	int lenth = strlen(str),num=0,flag=0,d_len=0;
	double value=0;
	
	for(;*pstr!='\0';pstr++)
	{
		if(*pstr == '.' ) 
		{
			flag = 1;
			continue;
		}
		if(flag)
			d_len++;
		num = (*pstr) - '0';
		value *=10;
		value += num;
	}
	if(d_len)
	{
		d_len = pow(10,d_len);
		value /= d_len; 
	}
//	printf("value: %f\r\n",value);
	return value;	 
}

// ȥ���ַ������˵������ַ�
uint8_t remove_both_end(char *str)
{
	uint8_t i=0;
	if(strlen(str)<=2)	// �ַ���̫��
	{	
		printf("The string is too short!\r\n");
		return 1;
	}
	for(i=0;i<strlen(str);i++)
	{
		str[i]=str[i+1];
	}
	str[strlen(str)-1]='\0';
	return 0;
}

uint8_t Data_Report_TCP_Number(uint8_t datatype,char *apitag,char *datetime,uint32_t value,uint8_t msgid)
{
	uint8_t IpSend[128];
	char datas[128];
	memset(IpSend,0x00,sizeof(IpSend));
	memset(datas,0x00,sizeof(datas));
	sprintf(datas,"\"%s\":{\"%s\":\"%d\"}",apitag,datetime,value);
	sprintf((char*)IpSend,"{\"t\":3,\"datatype\":%d,\"datas\":{%s},\"msgid\":%d}",datatype,datas,msgid);
//	printf("IpSend: %s\r\n",IpSend);
	if(ESP8266_IpSend((char*)IpSend,strlen((const char*)IpSend)) < 0 )
	{
		//����ʧ��
		printf("send %d message faild!\r\n",msgid);
		return 1;
	}
	else
	{
		//���ͳɹ�
		for(uint8_t TryGo = 0; TryGo<50; TryGo++)//���ȴ�ʱ��50*10=500ms
		{
			if(strstr((const char *)AT_RX_BUF, (const char *)"\"status\":0") != NULL)//�����Ӧ״̬�Ƿ�Ϊ"�ϱ��ɹ�"
			{
				ClrAtRxBuf();
				return  0;
			}
			delay_ms(10);
		}
	}
	return 1;
}

uint8_t Data_Report_TCP_String(uint8_t datatype,char *apitag,char *datetime,uint8_t *pstr,uint8_t msgid)
{
	uint8_t IpSend[128];
	char datas[128];
	memset(IpSend,0x00,sizeof(IpSend));
	memset(datas,0x00,sizeof(datas));
	sprintf(datas,"\"%s\":{\"%s\":\"%s\"}",apitag,datetime,pstr);
	sprintf((char*)IpSend,"{\"t\":3,\"datatype\":%d,\"datas\":{%s},\"msgid\":%d}",datatype,datas,msgid);
	if(ESP8266_IpSend((char*)IpSend,strlen((const char*)IpSend)) < 0 )
	{
		//����ʧ��
		printf("send %d message faild!\r\n",msgid);
		return 1;
	}
	else
	{
		//���ͳɹ�
		for(uint8_t TryGo = 0; TryGo<50; TryGo++)//���ȴ�ʱ��50*10=500ms
		{
			if(strstr((const char *)AT_RX_BUF, (const char *)"\"status\":0") != NULL)//�����Ӧ״̬�Ƿ�Ϊ"�ϱ��ɹ�"
			{
				ClrAtRxBuf();
				return  0;
			}
			delay_ms(10);
		}
	}
	return 1;
}


uint8_t Data_Report_TCP_Float(uint8_t datatype,char *apitag,char *datetime,double value,uint8_t msgid)
{
	uint8_t IpSend[128];
	char datas[128];
	memset(IpSend,0x00,sizeof(IpSend));
	memset(datas,0x00,sizeof(datas));
	sprintf(datas,"\"%s\":{\"%s\":\"%lf\"}",apitag,datetime,value);
	sprintf((char*)IpSend,"{\"t\":3,\"datatype\":%d,\"datas\":{%s},\"msgid\":%d}",datatype,datas,msgid);
	if(ESP8266_IpSend((char*)IpSend,strlen((const char*)IpSend)) < 0 )
	{
		//����ʧ��
		printf("send %d message faild!\r\n",msgid);
		return 1;
	}
	else
	{
		//���ͳɹ�
		for(uint8_t TryGo = 0; TryGo<50; TryGo++)//���ȴ�ʱ��50*10=500ms
		{
			if(strstr((const char *)AT_RX_BUF, (const char *)"\"status\":0") != NULL)//�����Ӧ״̬�Ƿ�Ϊ"�ϱ��ɹ�"
			{
				ClrAtRxBuf();
				return  0;
			}
			delay_ms(10);
		}
	}
	return 1;
}

/***************************************************************************
*@brief		�ϱ��������
*@param		pStruct: �ϱ����ݽṹ��
*			data_num: �ϱ����ݵĸ���
*			msgid: ����id
*@retval	success: 0
*			failed: 1
****************************************************************************/
int8_t Data_Report_TCP(Data_ReportStruct *pStruct,int data_num,int msgid)
{
	uint8_t IpSend[256];
	uint8_t datas[256];
	char temp[50]={0}; 
	uint8_t i=0;
	memset(IpSend,0x00,sizeof(IpSend));
	memset(datas,0x00,sizeof(datas));
	memset(temp,0x00,sizeof(temp));
	
	for(i=0;i<data_num;i++)
	{
		if(pStruct->up_data_type == 1)
			sprintf(temp,"\"%s\":{\"%s\":\"%d\"},",pStruct->apitag , current_time_buf,pStruct->pdata.integer);
		else if(pStruct->up_data_type == 2)
			sprintf(temp,"\"%s\":{\"%s\":\"%lf\"},",pStruct->apitag , current_time_buf,pStruct->pdata.float_num);
		else if(pStruct->up_data_type == 3)
			sprintf(temp,"\"%s\":{\"%s\":\"%s\"},",pStruct->apitag , current_time_buf,pStruct->pdata.string);
		strcat((char*)datas,temp);
		pStruct++;	
	}
	
	//printf("datas: %s\r\n",datas);
	sprintf((char*)IpSend,"{\"t\":3,\"datatype\":2,\"datas\":{%s},\"msgid\":%d}",datas,msgid);
	printf("Ipsend: %s\r\n",IpSend); 
	
	if(ESP8266_IpSend((char*)IpSend,strlen((const char*)IpSend)) < 0 )
	{
		//����ʧ��
		printf("send %d message faild!\r\n",msgid);
		return 1;
	}
	else
	{
		//���ͳɹ�
		for(uint8_t TryGo = 0; TryGo<50; TryGo++)//���ȴ�ʱ��50*10=500ms
		{
			if(strstr((const char *)AT_RX_BUF, (const char *)"\"status\":0") != NULL)//�����Ӧ״̬�Ƿ�Ϊ"�ϱ��ɹ�"
			{
				ClrAtRxBuf();
				return  0;
			}
			delay_ms(10);
		}
	}
	return 1;
}



void NLECloud_DataAnalysis_TCP(char *RxBuf)
{
	char *cmdid = NULL;
	uint8_t TxetBuf[128];
	if(strstr((const char *)RxBuf, (const char *)PING_REQ) != NULL)//��������
	{
		if(ESP8266_IpSend((char *)PING_RSP, strlen((const char *)PING_RSP)) < 0)//��Ӧ����
		{
			//����ʧ��
			printf("����������ʧ�ܣ�\r\n");
		}
		else
		{
			printf("��������\r\n");
		}
	}
	else if(strstr((const char *)RxBuf, (const char *)"\"t\":5") != NULL)//��������
	{
		if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"door\"") != NULL)//��/��������
		{
			memset(TxetBuf,0x00,128);//��ջ���
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))	//����
			{
				printf("���ţ�\r\n");
				door_open(500);
				;//...
				;//...
				;//...
				cmdid = GetJsonValue((char *)RxBuf, (char *)"cmdid");
				sprintf((char *)TxetBuf,"{\"t\":6,\"cmdid\":%s,\"status\":0,\"data\":1}",cmdid);
				printf("%s\r\n",TxetBuf);
				if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
				{
					//����ʧ��
					printf("������Ӧʧ�ܣ�\r\n");
				}
			}
			else if((strstr((const char *)RxBuf, (const char *)"\"data\":0") != NULL))//����
			{
				printf("���ţ�\r\n");
			
				;//...
				;//...
				;//...
				cmdid = GetJsonValue((char *)RxBuf, (char *)"cmdid");
				sprintf((char *)TxetBuf,"{\"t\":6,\"cmdid\":%s,\"status\":0,\"data\":0}",cmdid);
				printf("%s\r\n",TxetBuf);
				if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
				{
					//����ʧ��
					printf("������Ӧʧ�ܣ�\r\n");
				}
			}
		}
		else if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"lock\"") != NULL)//����/��������
		{
			memset(TxetBuf,0x00,128);//��ջ���
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))//����
			{		
				printf("������\r\n");
				lock_ctrl(1);
				;//...
				;//...
				;//...
				cmdid = GetJsonValue((char *)RxBuf, (char *)"cmdid");
				sprintf((char *)TxetBuf,"{\"t\":6,\"cmdid\":%s,\"status\":0,\"data\":1}",cmdid);
				printf("%s\r\n",TxetBuf);
				if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
				{//����ʧ��
					printf("������Ӧʧ�ܣ�\r\n");
				}
			}
			else if((strstr((const char *)RxBuf, (const char *)"\"data\":0") != NULL))//����
			{
				printf("������\r\n");
				lock_ctrl(0);
				;//...
				;//...
				;//...
				cmdid = GetJsonValue((char *)RxBuf, (char *)"cmdid");
				sprintf((char *)TxetBuf,"{\"t\":6,\"cmdid\":%s,\"status\":0,\"data\":0}",cmdid);
//				printf("%s\r\n",TxetBuf);
				if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
				{//����ʧ��
					printf("������Ӧʧ�ܣ�\r\n");
				}
			}
		}
		else if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"light\"") != NULL)///����/�ص�����
		{
			memset(TxetBuf,0x00,128);//��ջ���
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))//����
			{
				printf("���ƣ�\r\n");
				lamp_ctrl(ON);
				;//...
				;//...
				;//...
				cmdid = GetJsonValue((char *)RxBuf, (char *)"cmdid");
				sprintf((char *)TxetBuf,"{\"t\":6,\"cmdid\":%s,\"status\":0,\"data\":1}",cmdid);
//				printf("%s\r\n",TxetBuf);
				if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
				{
					//����ʧ��
					printf("������Ӧʧ�ܣ�\r\n");
				}
			}
			else if((strstr((const char *)RxBuf, (const char *)"\"data\":0") != NULL))//�ص�
			{
				printf("�صƣ�\r\n");
				lamp_ctrl(OFF);
				;//...
				;//...
				;//...
				cmdid = GetJsonValue((char *)RxBuf, (char *)"cmdid");
				sprintf((char *)TxetBuf,"{\"t\":6,\"cmdid\":%s,\"status\":0,\"data\":0}",cmdid);
//				printf("%s\r\n",TxetBuf);
				if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
				{
					//����ʧ��
					printf("������Ӧʧ�ܣ�\r\n");
				}
			}
		}
		else if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"buzzer\"") != NULL)///��/�ط���������
		{
			memset(TxetBuf,0x00,128);//��ջ���
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))//��������
			{
				printf("����������\r\n");
				buzzer_ctrl(1);
				;//...
				;//...
				;//...
				cmdid = GetJsonValue((char *)RxBuf, (char *)"cmdid");
				sprintf((char *)TxetBuf,"{\"t\":6,\"cmdid\":%s,\"status\":0,\"data\":1}",cmdid);
//				printf("%s\r\n",TxetBuf);
				if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
				{
					//����ʧ��
					printf("������Ӧʧ�ܣ�\r\n");
				}
			}
			else if((strstr((const char *)RxBuf, (const char *)"\"data\":0") != NULL))//�ط�����
			{
				printf("�ط�������\r\n");
				buzzer_ctrl(0);
				;//...
				;//...
				;//...
				cmdid = GetJsonValue((char *)RxBuf, (char *)"cmdid");
				sprintf((char *)TxetBuf,"{\"t\":6,\"cmdid\":%s,\"status\":0,\"data\":0}",cmdid);
//				printf("%s\r\n",TxetBuf);
				if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
				{
					//����ʧ��
					printf("������Ӧʧ�ܣ�\r\n");
				}
			}
		}
	}
}

void NewLand_DataAnalysis_USART(unsigned char* buf)
{
	unsigned char i, d=0, data_type, data_commad, buf_temp[15],lf_buf[8];
	signed data_PL;
	long data_int = 0;
	u8 data_checksum = 0, tx_count = 0;
	
	data_int = 0;
	if(buf[0] != 0xBB || buf[0] == '\0')
		return;
	data_type = buf[1];
	data_checksum += data_type;
	
	data_commad = buf[2];
	data_checksum += data_commad;
	
	data_PL = buf[3];
	data_checksum += data_PL;
	
	for(i = 4; (i-4) < data_PL; i++)
	{
		data_checksum += buf[i];
		data_int |= buf[i];				//�����ݲ��ֱ���������buf[4]�ǵ�һ�������ֽ�
		data_int <<= 8;
		d++;
	}
//	lcd_clr();
//	LcdPrintf(1,2,"data_PL:%d",d);
	d=0;
	d = (data_int>>8)&0xFF;
//	LcdPrintf(1,3,"data_PL:%d",d);	
	
//	if(data_PL != 0x00)
//	{
//		data_checksum += buf[i];
//		i++;
//		//data_f = (float)(data_int + (buf[i]/0.01));
//	}
	
	if(data_checksum != buf[i])
	{
		printf("����У��error! %02X",data_checksum);
		return;
	}
	if(buf[++i] != 0x7E)
	{
		printf("����֡βerror! %d",buf[i]);
		return;	
	}
	
	switch(data_type)
	{
		case 0x00:
			switch(data_commad)
			{
				case 0x02:	/*��ȡ������Ӧ*/
					respond_get_Illumination();
					break;
				case 0x08:	/*�������*/
					respond_steeringengine(buf[4],buf[5]);
					break;
				case 0x09:	/*��ʼ����Ƶ��*/
					respond_lf125k_init();
					break;
				case 0x0A:	/*����Ƶ������*/
					respond_read_lf125k_ID();
					break;
				case 0x0B:	/*����Ƶ������(��0ҳ)*/
					respond_read_lf125k_data(buf[5]);
					break;
				case 0x0C:	/*д��Ƶ������(��0ҳ)*/
					respond_write_lf125k_data(buf); 
					break;
				
				
				default:
					break;
			}
			
			break;
		default:break;
	}
}

/***************************** END OF FILE *********************************/

