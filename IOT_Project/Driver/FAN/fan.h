/**********************************************************
*	@file 		fan.h
*	@author		Panda
*	@version	V0.0.0
*	@date		2019-09
*	@brief
***********************************************************/
#ifndef _FAN_H
#define _FAN_H

#define RCC_FAN_PORT	RCC_APB2Periph_GPIOA	// fan ʹ��IO�˿ڵ�ʱ��
#define FAN_PORT		GPIOA				 	// fan ʹ�õĶ˿�
#define FAN_PIN			GPIO_Pin_0			 	// fan ʹ�õ�IO����

#define	FAN_STA		GPIO_ReadInputDataBit(FAN_PORT,FAN_PIN)

#define FAN_ON		GPIO_SetBits(FAN_PORT,FAN_PIN)
#define FAN_OFF		GPIO_ResetBits(FAN_PORT,FAN_PIN)


#include "Init.h"
#include "stm32f10x.h"

void fan_init(void);
void fan_ctrl(bool sta);
bool fan_getsta(void);

#endif


/***************************************END OF FILE**********************************************/

