#ifndef __XFS5152CE_H
#define __XFS5152CE_H

#include "usart.h"
#include "string.h"
#include "stdlib.h"
#include "delay.h"


/* Includes ------------------------------------------------------------------*/
/* Public typedef ------------------------------------------------------------*/
/* ���ı����ʽ */
typedef enum 
{
	XFS_FORMAT_GB2312 = 0x00,
	XFS_FORMAT_GBK = 0x01,
	XFS_FORMAT_BIG5 = 0x02,
	XFS_FORMAT_UNICODE = 0x03,
}xfsEnodingFormatEnum;

/* ��Ӧ״̬ */
typedef enum 
{
	XFS_ACK_CMD_RIGHT = 0x41,
	XFS_ACK_CMD_ERROR = 0x45,
	XFS_ACK_INIT_SUCCESS = 0x4A,
	XFS_ACK_BUSY = 0x4E,
	XFS_ACK_IDEL = 0x4F,
}xfsAckStatusEnum;


extern const char *cmd_word_list[]; 



// xfs5152�˿ڳ�ʼ��
void Xfs5152ce_Init(void);	

// ��ʼ��������
signed char Xfs_VoiceBroadcastStart(unsigned char *data, unsigned char format, int wait_finish);	

//���Ż�������
unsigned char Xfs_PalyFromCache(unsigned char num, unsigned char format);	

//�����ı���������
signed char Xfs_SendTextToCache(unsigned char *data, unsigned char cache_num);	

//����ʶ��
unsigned char Xfs_VoiceRecognize(unsigned char *result, signed int waittime);

//������������
unsigned char Xfs_VoiceBroadcastRestore(void);	

//��ͣ��������
unsigned char Xfs_VoiceBroadcastPause(void);	

//ֹͣ��������
unsigned char Xfs_VoiceBroadcastStop(void);	

//����XFS5152CE
unsigned char Xfs_WakeUp(void);	

//ʹXFS5152CE����˯��״̬
unsigned char Xfs_Sleep(void);	

//���XFS5152CE�ĵ�ǰ״̬
unsigned char Xfs_GetStatus(void);	

//ֹͣ�����
unsigned char Xfs_VoiceCODECStop(void);	

//��ʼ��������
signed char Xfs_VoiceEncodeStart(unsigned char samp,unsigned char baud,unsigned char volum);

//��ʼ�������룬��ϱ��봫�亯��ʹ��
signed char Xfs_VoiceDecodeStart(unsigned char samp,unsigned char baud,unsigned char volum,unsigned char *data);

//���XFS5152CE�����ݽ���
void Xfs_AckReceive(unsigned char dat);	

//��ֹ�����жϵ���ʱ����
void Xfs_ReceiveTimeLimt(void); 


#endif



