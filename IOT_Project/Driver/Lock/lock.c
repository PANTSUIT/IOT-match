/**********************************************************
*	@file 		door.c
*	@author		Panda
*	@version	V0.0.0
*	@date		2019-09
*	@brief
***********************************************************/

#include "lock.h"

/*********************************************************
*	@brief		doorʹ�õ� GPIO ���ų�ʼ��
*	@Parma		None
*	@retval		None
**********************************************************/
void lock_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_LOCK_OUT_PORT,ENABLE);

	GPIO_InitStructure.GPIO_Pin = LOCK_OUT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LOCK_OUT_PORT, &GPIO_InitStructure);

}

/**********************************************
*	@brief	�ж����Ƿ���
*	@param	None
*	@retval �ŵ�״̬
***********************************************/
bool lock_getsta(void)
{
	device_sta.sta_lock = LOCK_STA;
	return device_sta.sta_lock;
}

/*******************************************
*	@brief		���� door �Ŀ��� 
*	@param		sta: door�Ŀ���״̬
*		@arg	1:�����ſ�
*		@arg 	0:�����Ź�
*	@retval		None
********************************************/
void lock_ctrl(bool sta)
{
	if(sta == ON)
	{
		if(device_sta.sta_lock == OFF)
		{	
			LOCK_ON;
			device_sta.sta_lock = ON;
			printf("The door opened!\r\n");
		}
		else 
		{
			
		}
	}
	else if(sta == OFF)
	{
		if(device_sta.sta_lock == ON)
		{		
			LOCK_OFF;
			device_sta.sta_lock = OFF;
			printf("The door locked!\r\n");
		}
		else
		{
		}
	}	
}

/*******************************************
*	@brief		door �Ŀ��� 
*	@param		waittime:�ſ���ʱ��
*	@retval		None
********************************************/
void door_open(uint32_t nms)
{
	lock_ctrl(ON);
	delay_ms(nms);
	lock_ctrl(OFF);
	
}
/*************************************END OF FILE*****************************************/


