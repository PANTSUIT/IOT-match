/**********************************************************
*	@file 		lamp.c
*	@author		Panda
*	@version	V0.0.0
*	@date		2019-09
*	@brief
***********************************************************/
#include "lamp.h"


/*******************************************
*	@brief	lamp ʹ�õ�GPIO���ų�ʼ��
*	@param	None
*	@retval	None
********************************************/
void lamp_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_LAMP_PORT,ENABLE);

	GPIO_InitStructure.GPIO_Pin = LAMP_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LAMP_PORT, &GPIO_InitStructure);
	
	GPIO_ResetBits(LAMP_PORT,LAMP_PIN);
}

/*******************************************
*	@brief		���� light ����Ŀ��� 
*	@param		sta: light������״̬
*		@arg	1:���Ƶ���
*		@arg 	0:���Ƶ���
*	@retval		None
********************************************/
void lamp_ctrl(bool sta)
{
	if(sta == ON)
	{
		if(device_sta.sta_lamp == OFF)
		{
			GPIO_SetBits(LAMP_PORT,LAMP_PIN);
			device_sta.sta_lamp = ON;
		}
		else
		{	
		}
	}
	else if(sta == OFF)
	{	
		if(device_sta.sta_lamp == ON)
		{
			GPIO_ResetBits(LAMP_PORT,LAMP_PIN);
			device_sta.sta_lamp = OFF;
		}
		else
		{
		}
	}
}

/****************************************************
*@brief		��ȡ�Ƶ�״̬
*@param		None
*@retval	���صƵ�״̬
*	@val	ON:
*	@val 	OFF:
*****************************************************/
bool lamp_getsta(void)
{
	device_sta.sta_lamp = LAMP_STA;
	return device_sta.sta_lamp;
}

/***************************************END OF FILE**********************************************/
