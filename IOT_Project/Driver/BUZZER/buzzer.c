/**********************************************************
*	@file 		buzzer.c
*	@author		Panda
*	@version	V0.0.0
*	@date		2019-09
*	@brief
***********************************************************/

/* Includes ------------------------------------------------------------------*/

#include "buzzer.h"

/*********************************************
*@brief		buzzer Init
*@param		None
*@retval 	None
**********************************************/
void buzzer_init(void)
{
	TIM1_Init(500-1,72-1);
}

/*********************************************
*@brief		��ȡ������״̬
*@param		None
*@retval 	���ط�������״̬
*	@val	ON:
*	@val 	OFF:
**********************************************/
bool buzzer_getsta(void)
{
	if(TIM1->CR1 & TIM_CR1_CEN)
	{
		device_sta.sta_buzzer = ON;
		return ON;
	}
	else
	{	
		device_sta.sta_buzzer = OFF;
		return OFF;
	}
}

/*********************************************
*@brief		buzzer ctrl
*@param		sta:������ر�buzzer
*	@arg	ON	:����buzzer
*	@arg	OFF	:�ر�buzzer
*@retval 	None
**********************************************/
void buzzer_ctrl(bool sta)
{
	if(sta == true)
	{	
		if(device_sta.sta_buzzer == OFF)
		{
			TIM_Cmd(TIM1,ENABLE);
			device_sta.sta_buzzer = ON;
		}
		else
		{
		}
	}
	else if(sta == false)
	{
		if(device_sta.sta_buzzer == ON)
		{
			TIM_Cmd(TIM1,DISABLE);
			device_sta.sta_buzzer = OFF;
		}
	}
}

/*********************************************
*@brief		buzzer ��һ��
*@param		tms��
*@retval 	None
**********************************************/
void beep(u16 tms)
{
	buzzer_ctrl(ON);
	delay_ms(tms);
	buzzer_ctrl(OFF);
}

/************************ END OF FILE ************************/
