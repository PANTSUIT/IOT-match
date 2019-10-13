/*********************************************
*@file		2017_A.c
*@author	Panda
*@version	V0.0.0
*@date		2019-10
*@brief		2017����Ŀ
*
**********************************************/
#include "2017_A.h"
#include "temperature.h"

static void DataAnalysis_TCP(char *RxBuf)
{
	char *cmdid = NULL;
	char *data = NULL;
	double value;
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
		if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"lamp_ctrl\"") != NULL)//��/�� ������
		{
			memset(TxetBuf,0x00,128);//��ջ���
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))	//����
			{
				printf("���ƣ�\r\n");
				lamp_ctrl(ON);
				;//...
				;//...
				;//...
//				cmdid = GetJsonValue((char *)RxBuf, (char *)"cmdid");
//				sprintf((char *)TxetBuf,"{\"t\":6,\"cmdid\":%s,\"status\":0,\"data\":1}",cmdid);
////				printf("%s\r\n",TxetBuf);
//				if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
//				{
//					//����ʧ��
//					printf("������Ӧʧ�ܣ�\r\n");
//				}
			}
			else if((strstr((const char *)RxBuf, (const char *)"\"data\":0") != NULL))//����
			{
				printf("�صƣ�\r\n");
				lamp_ctrl(OFF);
				;//...
				;//...
				;//...
//				cmdid = GetJsonValue((char *)RxBuf, (char *)"cmdid");
//				sprintf((char *)TxetBuf,"{\"t\":6,\"cmdid\":%s,\"status\":0,\"data\":0}",cmdid);
////				printf("%s\r\n",TxetBuf);
//				if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
//				{
//					//����ʧ��
//					printf("������Ӧʧ�ܣ�\r\n");
//				}
			}
		}
		else if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"fan_ctrl\"") != NULL)//��/�� ��������
		{
			memset(TxetBuf,0x00,128);//��ջ���
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))//������
			{		
				printf("�����ȣ�\r\n");
				fan_ctrl(ON);
				;//...
				;//...
				;//...
//				cmdid = GetJsonValue((char *)RxBuf, (char *)"cmdid");
//				sprintf((char *)TxetBuf,"{\"t\":6,\"cmdid\":%s,\"status\":0,\"data\":1}",cmdid);
////				printf("%s\r\n",TxetBuf);
//				if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
//				{
//					//����ʧ��
//					printf("������Ӧʧ�ܣ�\r\n");
//				}
			}
			else if((strstr((const char *)RxBuf, (const char *)"\"data\":0") != NULL))//�ط���
			{
				printf("�ط��ȣ�\r\n");
				fan_ctrl(OFF);
				;//...
				;//...
				;//...
//				cmdid = GetJsonValue((char *)RxBuf, (char *)"cmdid");
//				sprintf((char *)TxetBuf,"{\"t\":6,\"cmdid\":%s,\"status\":0,\"data\":0}",cmdid);
////				printf("%s\r\n",TxetBuf);
//				if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
//				{//����ʧ��
//					printf("������Ӧʧ�ܣ�\r\n");
//				}
			}
		}
		else if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"servo_x_ctrl\"") != NULL)// ��� ����
		{
			memset(TxetBuf,0x00,128);//��ջ���	
			;//...
			;//...
			;//...
			data=json_getvalue((char *)RxBuf,(char *)"data");
//			printf("Rx angle: %s\r\n",data);
			remove_both_end(data);
			printf("remove: %s\r\n",data);
			value=data_type_conversion_S2N(data);
			printf("value:%f \r\n",value);
			Servo_Control_X(value);
			
			
//			cmdid = GetJsonValue((char *)RxBuf, (char *)"cmdid");
//			sprintf((char *)TxetBuf,"{\"t\":6,\"cmdid\":%s,\"status\":0,\"data\":%s}",cmdid,data);
////			printf("cmdid: %s\r\n",TxetBuf);
//			if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
//			{
//				//����ʧ��
//				printf("������Ӧʧ�ܣ�\r\n");
//			}
		}
		else if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"servo_y_ctrl\"") != NULL)// ��� ����
		{
			memset(TxetBuf,0x00,128);//��ջ���	
			;//...
			;//...
			;//...
			data=json_getvalue((char *)RxBuf,(char *)"data");
//			printf("Rx angle: %s\r\n",data);
			remove_both_end(data);
//			printf("remove: %s\r\n",data);
			value=data_type_conversion_S2N(data);
//			printf("value:%f",value);
			Servo_Control_Y(value);
			
			
//			cmdid = GetJsonValue((char *)RxBuf, (char *)"cmdid");
//			sprintf((char *)TxetBuf,"{\"t\":6,\"cmdid\":%s,\"status\":0,\"data\":%s}",cmdid,data);
////			printf("cmdid: %s\r\n",TxetBuf);
//			if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
//			{
//				//����ʧ��
//				printf("������Ӧʧ�ܣ�\r\n");
//			}
		}
	}
}

void cloud_task(void)
{
	
}


void _2017_machine_init()
{
	STM_Clock_Init(9);	// ϵͳʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SYSTICK_Init(); 
	
	RTC_Init();
	
	/* RTC ʱ������ */
//	RTC_Set(2019,10,4,22,24,0);
	
	USART1_Init(115200);// ����
	
	UART4_Init(115200); // WIFI
	 
	lcd_init();
	
	TIM3_pwm_init();
	
	ADC_DMA_Init();
	
	PCF8591_Init(1);
	
	fan_init();
	lamp_init();
	buzzer_init();
	
	LF125K_Init();
	Servo_Init();
	
}

void _2017_main_task(void)
{
	int8_t temp=0;
	uint16_t i = 0;
	u8 id[20];
	int32_t temperature = 0;
	float light_lux= 0.0;
	uint32_t cmdid=0;
	
	//���ӷ�����
	ConnectToServer_times();
	
	lcd_clr();
	while(1)
	{

		memset(data_buf,0x00,sizeof(data_buf));
		ESP8266_GetIpData(AT_RX_BUF,(char*)data_buf); 	// �� ESP8266 ��ȡ����
		ClrAtRxBuf();
		DataAnalysis_TCP((char*)data_buf);		// ����	server ���͵�����
		
		
		i++;
		delay_ms(10);
		if(i%500 == 0)
		{
					
			Data_Report_TCP_Number(2,"illumination",(char*)current_time_buf,light_lux,cmdid++);
	
			Data_Report_TCP_Number(2,"temperature",(char*)current_time_buf,temperature,cmdid++);
	
			Data_Report_TCP_Number(2,"lamp_ctrl",(char*)current_time_buf,device_sta.sta_lamp,cmdid++);
	
			Data_Report_TCP_Number(2,"fan_ctrl",(char*)current_time_buf,device_sta.sta_fan,cmdid++);	
	
			Data_Report_TCP_Number(2,"servo_x_ctrl",(char*)current_time_buf, (uint32_t)Servo_Get_X_Angle() ,cmdid++);	
	
		}
		
		light_lux = LDR_Lux(0);	
		temperature =getTemperature();
		
		LCD_PRINT(1,0,lcd_show_buf,"LUX: %.2f",LDR_Lux(0));
		LCD_PRINT(2,0,lcd_show_buf,"Temp: %d",getTemperature());
		if(USART1_RX_STA&0x8000)
		{
//			printf_hex(USART1_RX_BUF,USART1_RX_STA & 0x3FFF);
			NewLand_DataAnalysis_USART(USART1_RX_BUF);
			memset(USART1_RX_BUF,0x00,sizeof(USART1_RX_BUF));
			USART1_RX_STA = 0;
		}
		
	}
}







