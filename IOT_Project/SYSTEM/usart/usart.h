/********************************************************************
*	@file 		usart4.h
*	@author		Panda
*	@version	V0.0.0
*	@date		2019-09
*********************************************************************/

#ifndef __USART_H
#define __USART_H

#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	 

#include "sys.h" 
#include "Init.h"
#include "delay.h"
#include "timer.h"

#include "userapp.h"

typedef struct
{
	uint8_t *data_buf;
	uint16_t lenth :15;
	uint16_t finish_flag:1;
}RX_BUF;

typedef struct
{
	uint8_t *data_buf;
	uint16_t lenth :15;
	uint16_t finish_flag:1;
}TX_BUF;


/******************************* USART1 *******************************/

#define USART1_REC_LEN   200  	//�����������ֽ��� 200
#define USART1_RX_EN 	 1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_RX_STA;         		//����״̬���	

//����봮���жϽ��գ��벻Ҫע�����º궨��
void USART1_Init(u32 bound);

void USART1_DataAnalysis(void);

/******************************* USART2 *******************************/

#define USART2_RECV_LEN		600					//�����ջ����ֽ���
#define USART2_SEND_LEN		600					//����ͻ����ֽ���
#define USART2_RX_EN 		1					//0,������;1,����.

extern u8  USART2_RX_BUF[USART2_RECV_LEN]; 		//���ջ���,���USART2_MAX_RECV_LEN�ֽ�
extern u8  USART2_TX_BUF[USART2_SEND_LEN]; 		//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
extern vu16 USART2_RX_STA;   						//��������״̬
extern uint8_t  IS_UART4_IN_RX;

void USART2_Init(u32 bound);		//����2��ʼ�� 
void usart2_printf(char* fmt,...);
void USART2_SendData(u8 data);		//���ڷ���һ���ֽ�

/******************************* USART3 *******************************/

#define USART3_IT_EN	1
#if USART3_IT_EN
	#define USART3_IT_RX_EN		1
	#define USART3_IT_IDLE_EN		1
#endif	/* USART3_IT_EN */

#define USART3_RX_EN 			1		//ʹ�ܣ�1��/��ֹ��0������1����

#define USART3_RECV_LEN		200  	//�����������ֽ��� 200
#define USART3_SNED_LEN 	100

//extern u8  USART3_RX_BUF[USART3_RECV_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
//extern u16 USART3_RX_STA;         		//����״̬���	

void USART3_Init(u32 bound);
void USART3_SendStrlen(uint8_t *Data, int len);
   


/******************************* USART4 *******************************/

/*===========================================================================*/
#ifdef __cplusplus  /* C++֧�� */
	extern "C"{
#endif
/*===========================================================================*/

#define UART4_PRE_PRIORITY	1
#define UART4_SUB_PRIORITY	0	

#define UART4_REC_MAX_LEN	511		//�����������ֽ���
#define UART4_RX_EN		1	//���ڽ���ʹ�ܡ�
							//1���������
							//0����ֹ����


/******************************************************************************
                              �ⲿ���ù��ܺ���
******************************************************************************/

#if UART4_RX_EN	//ʹ���˴��ڽ���
	
	extern uint8_t  UART4_RX_BUF[UART4_REC_MAX_LEN];	//���ջ��壬���UART4_REC_MAX_LEN���ֽڣ�ĩ�ֽ�Ϊ���з� 
	extern uint32_t UART4_RX_STA ;		// bit31:����״̬���,1:UART4������ɣ�0��UART4����δ���
	                                    // bit30:���ڽ������ڽ����б��,1:�����У�0����ͣ�����
	extern uint8_t UART4_RX_TIMEOUT_COUNT;			//���ڽ��ճ�ʱ���������ڳ���3msδ���յ����ݣ���Ϊ��ʱ����λ��ms

#endif

void UART4_Init(uint32_t bound);	//��ʼ��IO ����1
void UART4_SendData(uint16_t Data);
void UART4_SendStr(uint8_t *Data);
void UART4_SendStrlen(uint8_t *Data, uint8_t len);

/*===========================================================================*/
#ifdef __cplusplus  /* C++֧�� */
	}
#endif
/*===========================================================================*/


/******************************* USART5 *******************************/

#define UART5_MAX_RECV_LEN		800					//�����ջ����ֽ���
#define UART5_MAX_SEND_LEN		800					//����ͻ����ֽ���
#define UART5_RX_EN 			1				    //0,������;1,����.

extern u8  UART5_RX_BUF[UART5_MAX_RECV_LEN]; 		//���ջ���,���USART2_MAX_RECV_LEN�ֽ�
extern u8  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 		//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
extern vu16 UART5_RX_STA;   						//��������״̬

void UART5_Init(u32 bound);
void uart5_printf(char* fmt,...);


#endif

/*********************************END OF FILE*********************************/

