#include "math.h"
#include "ldr.h"
#include "pcf8591.h"
#include "stdio.h"
#include "stdint.h"

#define V_REF			3.3     				//�ο���ѹ����λ��V
#define RES_REF  		10000 					//�ο����裬��λ����
#define ResolutionRatio	256						//ADC�ֱ��ʣ�8λ�ֱ��ʣ�2^8=256

/********************************************************
*@brief		: ���������������ADCֵ�������������ֵ����λ������
*@param		: uint16_t ADC_Value��������������ADCֵ��
*@retval	: ��������ֵ����λ������
*@attention	: ADC�ֱ��ʣ�8λ�ֱ��ʣ�2^8=256
**********************************************************/
uint32_t LDR_CalculateResValue(uint16_t ADC_Value)
{
	float Vrt=0.0;
	uint32_t Rrt=0;
	
	Vrt=(V_REF/(ResolutionRatio-1))*ADC_Value;//�����������˵�ѹ
	Rrt=Vrt/((V_REF-Vrt)/RES_REF);//�������裬��ֵ 
#if 0	
	printf("Vrt = %fV\r\n",Vrt);
#endif /*  */
	return (uint32_t)Rrt;
}

//Lux:�տ�˹��������λ��,��ƹ����ȵĹ����������� l m2 �����ʱ���ն�
//�������裺light dependent resistor => ��д��LDR

/********************************************************
*@brief		: ����ADC��ͨ����ת��������ߵ�������Lux
*@param		: uint8_t ch - ADCͨ����
*@retval	: ���ߵ������ȣ���λ��Lux��
*@attention	: �����ADCоƬ��PCF8591,
*					�������裺light dependent resistor => ��д��LDR,
*					Lux:�տ�˹��������λ��,��ƹ����ȵĹ�����������
*					l m2 �����ʱ���ն�.
**********************************************************/
float LDR_Lux(uint8_t ch)				 
{
	uint8_t adcVale;
	uint32_t resTemp;
	float LDR_Lux;

	PCF8591_ReadChannel(&adcVale, ch);

	resTemp=LDR_CalculateResValue(adcVale);//����ֵ����λ��ŷķ

	LDR_Lux = pow(10, ((log10(15) - log10(resTemp/1000.0) + 0.6) / 0.6));//����ĵ���ֵ��λΪǧŷķ
#if 0	
	printf("adcVale = %d\r\n",adcVale);
	printf("Lumen = %f Lux\r\n", LDR_Lux);
	printf("LDR = %d��\r\n",resTemp);
#endif /*  */
	return LDR_Lux;	
}

