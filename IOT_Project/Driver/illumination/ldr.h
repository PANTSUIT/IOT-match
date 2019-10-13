#ifndef _hal_ldr_h_
#define _hal_ldr_h_

#include "stdint.h"
#include "sys.h"
/********************************************************
*�������ƣ�fp32 LDR_Lux(uint8_t ch)
*���ܣ�����ADC��ͨ����ת��������ߵ�������Lux
*���룺uint8_t ch - ADCͨ����
*��������ߵ������ȣ���λ��Lux��
*����˵���������ADCоƬ��PCF8591,
*					�������裺light dependent resistor => ��д��LDR,
*					Lux:�տ�˹��������λ��,��ƹ����ȵĹ�����������
*					l m2 �����ʱ���ն�.
**********************************************************/
float LDR_Lux(uint8_t ch);		

#endif /*_hal_ldr_h_*/
