
/******************************************************************************
* @file    app/app_pwd.h 
* @author  zhao
* @version V1.0.0
* @date    2018.06.18
* @brief   ����eeprom���� ͷ�ļ�
******************************************************************************/
#ifndef __APP_PWD_H__
#define __APP_PWD_H__
#include "bsp_flash.h"
#include <stdio.h>
#include <stdlib.h>
#include "delay.h"

//MEMSDataTypedef ����
#define ADC_CHANNEL_NUM		5

typedef struct
{
	unsigned short theshold;
	unsigned short static_ADCValue[ADC_CHANNEL_NUM];
}MEMSDataTypedef;


extern enum
{
	PWD_ERROR,
	PWD_SUCCESS,
}PWD_STATUS;

extern enum
{
	EEP_SAVE_FLAG = 0,
	EEP_DIGIT_PWD_OFFSET = 7,
	EEP_DIGIT_PWD_START = 8,
	EEP_ID_CARD_PWD_OFFSET = 71,
	EEP_ID_CARD_PWD_START = 72,
	EEP_MEMS_START = 112,
}EEP_MAP;

/**
 * EEPROM start address in flash
 * As for STM32F103VE (512KB flash), 0x0807F000 is the begining of the last 2 pages.
 * Check the *.map file (in listing folder) to find out the used ROM size.
 */
#define EEP_SAVE_FLAG_ADDRESS    		((uint32_t)0x0807D800)
#define EEP_DIGIT_PWD_OFFSET_ADDRESS   	((uint32_t)0x0807E000)
#define EEP_DIGIT_PWD_START_ADDRESS   	((uint32_t)0x0807E800)
#define EEP_ID_CARD_PWD_OFFSET_ADDRESS  ((uint32_t)0x0807F000)
#define EEP_ID_CARD_PWD_START_ADDRESS   ((uint32_t)0x0807F800)
#define EEP_MEMS_START_ADDRESS    		((uint32_t)0x0807D000)


//Digit password
#define MAX_DIGIT_PWD_NUM 5	 //�ɴ洢���������
#define DIGIT_PWD_LEN 6		 //�������볤��

extern unsigned char DigitPwd_Cache[MAX_DIGIT_PWD_NUM][DIGIT_PWD_LEN];	//�������뻺��
extern unsigned char DigitPwd_Offset;		//���뱣���ڻ����е�λ��
extern unsigned char CurDigitPwd_Cache[DIGIT_PWD_LEN];		//��ǰ���������
extern unsigned char CurDigitPwd_Offset;	//��ǰ�������뵽�ڼ�λ

//LF id card password
#define MAX_ID_CARD_PWD_NUM 5		//�ɴ洢��IDcard�������
#define ID_CARD_PWD_LEN 	8		//ID card���볤��

extern unsigned char IDCardPwd_Cache[MAX_ID_CARD_PWD_NUM][ID_CARD_PWD_LEN];	//ID card���뻺��
extern unsigned char IDCardPwd_Offset;		//���뱣���ڻ����е�λ��
extern unsigned char CurIDCardPwd_Cache[ID_CARD_PWD_LEN];	//��ǰ���������

extern MEMSDataTypedef memsData;

void clear_pwdcache(void);	//����������뻺��

int IsRight_Pwd_Digit(unsigned char pwd[],int len);	//�ж����������Ƿ���ȷ

int IsRight_Pwd_IdCard(unsigned char pwd[],int len); //�ж�ID�������Ƿ���ȷ
	
void param_init(void);

int Save_DigitPwd(unsigned char pwd[],int len);	//���� Digit passsword

unsigned char **Read_DigitPwd(void);// ��ȡ��������

int Save_IDCardPwd(unsigned char pwd[],int len);	//���� IDCard password

int Delete_IDCardPwd(unsigned char pwd[],int len);	//ɾ�� ID Card passsword

unsigned char **Read_IDCardPwd(void);	// �� ID Card passsword

void Save_MemsData(void);	//save memdata

void Read_MemsData(void);	//read memdata


#endif //__APP_PWD_H__

