#ifndef __AS608_H
#define __AS608_H

/* include--------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "delay.h" 	
#include "usart.h"

/* define--------------------------------------------------------*/
#define AS608_Sta   PAin(4)	//��ָ��ģ��״̬����
#define CharBuffer1 0x01
#define CharBuffer2 0x02
#define AS608_DEFAULTADDR   0XFFFFFFFF //оƬĬ�ϵ�ַ

#define MYUSART_SendData USART2_SendData	// ʹ�õĴ��ڷ��ͺ���

/* typedef--------------------------------------------------------*/
typedef enum
{
	CMD = 0x01,
	DATA_ = 0x02,
	DATA_END = 0x08,
	RESPONSE = 0x07
}PackageTag;

typedef enum 
{
	GetImage = 0x01, 	//�Ӵ������϶���ͼ�����ͼ�񻺳���
	GenChar,		 	//����ԭʼͼ������ָ���������� CharBuffer1 �� CharBuffer2
	Match,			 	//��ȷ�ȶ� CharBuffer1 �� CharBuffer2 �е������ļ�
	Search,			 	//�� CharBuffer1 �� CharBuffer2 �е������ļ����������򲿷�ָ�ƿ�
	RegModel,        	//�� CharBuffer1 �� CharBuffer2 �е������ļ��ϲ�����ģ�����CharBuffer2
	StoreChar,       	//�������������е��ļ����浽 flash ָ�ƿ���
	LoadChar,		 	//�� flash ָ�ƿ��ж�ȡһ��ģ�嵽����������
	UpChar,          	//�������������е��ļ��ϴ�����λ��
	DownChar,        	//����λ������һ�������ļ�������������
	UpImage,         	//�ϴ�ԭʼͼ��
	DownImage,       	//����ԭʼͼ��
	DeletChar,       	//ɾ��flashָ�ƿ��е�һ�������ļ�
	Empty,			 	//���flashָ�ƿ�
	WriteReg,        	//дSOCϵͳ�Ĵ���
	ReadSysPara,     	//��ϵͳ��������
	Enroll,          	//ע��ģ��
	Identify,        	//��ָ֤��
	SetPwd,          	//�����豸���ֿ���
	VfyPwd,			 	//��֤���ֿ���	
	GetRandomCode,   	//���������
	SetChipAddr,     	//����оƬ��ַ
	ReadFlashInfoPage,	//��ȡFLASH Information Page����
	Port_Control,    	//ͨѶ�˿�(UART/USB)���ؿ���
	WriteNotepad,    	//д���±�
	ReadNotepad,	 	//�����±�
	BurnCode,		 	//��дƬ��FLASH��PS1802 SOC ��ָ��Ϊ��дƬ��FALSH���룩
	HighSpeedSearch, 	//��������FLASH
	GenBinImage,	 	//���ɶ�ֵ��ָ��ͼ��	 
	ValidTempleteNum,	//����Чģ�����
	UserGPIOConmmand,	//�û�GPIO��������
	ReadIndexTable,	 	//��������            
}AS608_CmdCode;      
                     
typedef struct       
{                    
	u16 pageID;//ָ��ID
	u16 mathscore;//ƥ��÷�
}SearchResult;       
                     
typedef struct       
{                    
	u16 AS608_max;//ָ���������
	u8  AS608_level;//��ȫ�ȼ�
	u32 AS608_addr;
	u8  AS608_size;//ͨѶ���ݰ���С 0:32bytes 1:64bytes 2:128bytes 3:256bytes
	u8  AS608_N;//�����ʻ���N
}SysPara;

/* variable--------------------------------------------------------*/
extern SysPara AS_SysPara;	// ָ��ģ��AS608����
extern u16 ValidN;	//ģ������Чָ�Ƹ���

/* function--------------------------------------------------------*/
void AS608_StaGPIO_Init(void);//��ʼ��PA6��״̬����

void Recive_Data(void); // ??????????

u8 AS608_GetImage(void); //¼��ͼ�� 
 
u8 AS608_GenChar(u8 BufferID);//�������� 

u8 AS608_Match(void);//��ȷ�ȶ���öָ������ 

u8 AS608_Search(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p);//����ָ�� 
 
u8 AS608_RegModel(void);//�ϲ�����������ģ�壩 
 
u8 AS608_StoreChar(u8 BufferID,u16 PageID);//����ģ�� 

uint8_t AS608_LoadChar(uint8_t BufferID,uint16_t PageID);//����ģ��

uint8_t AS608_UpChar(uint8_t BufferID);//�ϴ�������ģ��

uint8_t AS608_DownChar(uint8_t BufferID);//����������ģ��

u8 AS608_DeletChar(u16 PageID,u16 N);//ɾ��ģ�� 

u8 AS608_Empty(void);//���ָ�ƿ� 

u8 AS608_WriteReg(u8 RegNum,u8 DATA);//дϵͳ�Ĵ��� 
 
u8 AS608_ReadSysPara(SysPara *p); //��ϵͳ�������� 

u8 AS608_VfyPwd(void);//��֤�豸���ֿ���
	
u8 AS608_SetChipAddr(u32 addr);  //����ģ���ַ 

u8 AS608_WriteNotepad(u8 NotePageNum,u8 *content);//д���±� 

u8 AS608_ReadNotepad(u8 NotePageNum,u8 *note);//������ 

u8 AS608_HighSpeedSearch(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p);//�������� 
  
u8 AS608_ValidTempleteNum(u16 *ValidN);//����Чģ����� 

u8 AS608_HandShake(u32 *AS608_Addr); //��AS608ģ������

const char *EnsureMessage(u8 ensure);//ȷ���������Ϣ����

#endif

