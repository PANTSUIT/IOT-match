/******************************************************************************
*	@file		WIFIToCloud.h
*	@author
*	@version
*	@date		2019-09
*	@brief		
*
******************************************************************************/

#ifndef _WIFITOCLOUD_H
#define _WIFITOCLOUD_H

/* includes------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdint.h>

#include "Init.h"
#include "delay.h"

#include "CloudReference.h"

/* defines------------------------------------------------------------------------*/
//�궨��
#define AT_CWMODE	"AT+CWMODE_CUR=1" //����Ϊ��station��ģʽ

#define PING_REQ "$#AT#\r"		//��������
#define PING_RSP "$OK##\r"		//������Ӧ

#define MAX_AT_RX_LEN	255			//�����ճ���
#define MAX_AT_TX_LEN	128			//����ͳ���
#define AT_RX_BUF 		UART4_RX_BUF		//AT���ڽ��ջ��棬����һ������
#define AT_RX_STA		UART4_RX_STA	



/* function------------------------------------------------------------------------*/
/*******************************************************************
*������void ClrAtRxBuf(void)
*���ܣ����AT���ڽ��ջ�����ؼĴ���
*���룺��
*�������
*����˵������
*******************************************************************/
void ClrAtRxBuf(void);

/*******************************************************************
*������void SendAtCmd(uint8_t *string)
*���ܣ�����ATָ���ַ���
*���룺
		uint8_t *string �����͵�ATָ��
		uint8_t len ����
*�������
*����˵������
*******************************************************************/
void SendAtCmd(uint8_t *string, uint8_t len);

/*******************************************************************
*������void SendStrLen(uint8_t *string, uint8_t len)
*���ܣ������ַ�����ATָ���
*���룺
		uint8_t *string �����͵��ַ���
		uint8_t len ����

*�������
*����˵������
*******************************************************************/
void SendStrLen(uint8_t *string, uint8_t len);


/*******************************************************************
*������int8_t ESP8266_SetStation(void)
*���ܣ�ESP8266����Ϊstationģʽ
*���룺��
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_SetStation(void);

/*******************************************************************
*������int8_t ESP8266_SetAP(void)
*���ܣ�����ESP8266Ҫ���ӵ��ȵ����ƺ�����
*���룺char *wifi-�ȵ����� char *pwd-�ȵ�����
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_SetAP(char *wifi, char *pwd);

/*******************************************************************
*������int8_t ESP8266_SetStation(void)
*���ܣ�ESP8266����TCP����
*���룺
	char *IpAddr-IP��ַ�����磺120.77.58.34
	uint16_t port-�˿ںţ�ȡֵ0~65535
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_IpStart(char *IpAddr, uint16_t port);

/*******************************************************************
*������int8_t ESP8266_IpSend(char *IpBuf, uint8_t len)
*���ܣ�ESP8266��������
*���룺
		char *IpBuf-IP����
		uint8_t len-���ݳ���
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_IpSend(char *IpBuf, uint8_t len);

/*******************************************************************
*������int8_t ConnectToServer(void)
*���ܣ����ӵ�������
*���룺��
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ConnectToServer(char *DeviceID, char *SecretKey);

/*******************************************************************
*������int8_t ESP8266_SendSensor(char *SensorTag, char *TimeStr, uint8_t sensor)
*���ܣ����ʹ������ݵ�������
*���룺
	char *SensorTag, �����ǩ����
	uint8_t sensor, ������ֵ
	char *TimeStr,�ɼ�����ʱ��ʱ�䣬����yyyy-mm-dd hh:mm:ss��ʽ
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_SendSensor(char *SensorTag, char *TimeStr, uint8_t sensor);

/*******************************************************************
*������uint8_t ESP8266_GetIpData(uint8_t *AtRxBuf, char *GetIpData)
*���ܣ���������������
*���룺
		uint8_t *AtRxBuf ��ԭʼAT���ڻ���
		char *GetIpData ����ȡ������Itcp/ip����
*����������յ���IP���ݳ���
*����˵����
	AT+CIPSEND=76
	{"t":3,"datatype":2,"datas":{"alarm":{"2018-06-19 18:15:02":0}},"msgid":001}
	+IPD,29:{"msgid":1,"status":0,"t":4}
*******************************************************************/
uint8_t ESP8266_GetIpData(uint8_t *AtRxBuf, char *GetIpData);
	
/*******************************************************************
*������void ESP8266_DataAnalysisProcess(char *RxBuf)
*���ܣ���������������
*���룺char *RxBuf �������·�����
*�����
*����˵����
*******************************************************************/
void ESP8266_DataAnalysisProcess(char *RxBuf);

int8_t ESP8266_SendPing(void);

void ConnectToServer_times(void);

#endif /* _WIFITOCLOUD_H */

/******************************************* END OF FILE ********************************************/


