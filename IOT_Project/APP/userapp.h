#ifndef _USERAPP_H
#define _USERAPP_H


#include "Init.h"
#include "lamp.h"
#include "lock.h"	
#include "NLECloud_TCP.h"

void userApp(void);	
	
void ShowErrMessage(u8 ensure);
	
uint8_t M3_GetNum(void);
	
void Add_FR(void);//¼ָ��
	
uint8_t press_FR(void);//ˢָ��

void Del_FR(void);//ɾ��ָ��

void Upload_Char(u8 *data);

#endif // _USERAPP_H


