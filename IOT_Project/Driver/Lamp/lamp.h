/**********************************************************
*	@file 		lamp.h
*	@author		Panda
*	@version	V0.0.0
*	@date		2019-09
*	@brief
***********************************************************/
#ifndef _LAMP_H
#define _LAMP_H

#define RCC_LAMP_PORT RCC_APB2Periph_GPIOA	 // light ʹ��IO�˿ڵ�ʱ��
#define LAMP_PORT 	   GPIOA				 // light ʹ�õĶ˿�
#define LAMP_PIN 	   GPIO_Pin_1			 // light ʹ�õ�IO����

#define LAMP_STA	GPIO_ReadInputDataBit(LAMP_PORT,LAMP_PIN)

#define LAMP_ON	GPIO_SetBits(LAMP_PORT,LAMP_PIN);
#define LAMP_OFF	GPIO_ResetBits(LAMP_PORT,LAMP_PIN);

#include "Init.h"
#include "stm32f10x.h"

void lamp_init(void);
void lamp_ctrl(bool sta);
bool lamp_getsta(void);

#endif


/***************************************END OF FILE**********************************************/

