/********************************************************
*@file		usart_protocol.h
*@author 	panda
*@version 	V0.0.0
*@date		2019-10
*@brief		��������֡��ʽͷ�ļ�
*********************************************************/

#ifndef	_USART_PROTOCOL_H
#define _USART_PROTOCOL_H


#include "sys.h"
#include "Init.h"

#define FRAME_HEADER	0xBB
#define FRAME_END		0x7E

#define RESPONSE_FRAME_HEX_OUTPUT_EN  0


typedef enum
{
	RRAME_CMD,			// ����֡
	FRAME_RESPONSE		// ��Ӧ֡
}FRAME_TYPE;



typedef enum
{
	GET_ILLUMINATION = 0x02,	// ��ȡ��������
	SERVO_CTRL = 0x08,			// ���ƶ���Ƕ�����
	INIT_LF_CARD = 0x09,		// ��ʼ��LF������
	READ_LF_CARD_NUM = 0x0A,	// ��ȡLF������
	READ_LF_CARD = 0x0B,		// ��LF�� 
	WRITE_LF_CARD = 0x0C		// дLF��
}FRAME_CMD_CODE;


void respond_get_Illumination(void);
void respond_steeringengine(uint8_t site,uint16_t buf);
void respond_lf125k_init(void);
void respond_read_lf125k_ID(void);
void respond_read_lf125k_data(uint8_t block);
void respond_write_lf125k_data(unsigned char *buf); 





#endif	/* _USART_PROTOCOL_H */








