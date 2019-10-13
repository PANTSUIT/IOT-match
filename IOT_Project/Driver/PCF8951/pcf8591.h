/********************************************************************************************************
*
*
*
*
*********************************************************************************************************/
#ifndef _PCF8591_H_
#define _PCF8591_H_

#include "Init.h"
#include "sys.h"
#include "hal_iic.h"


/*******************PCF8591-I2C����*********************/
#define PCF8591_I2C_SDA_PIN_PORT		GPIOA
#define PCF8591_I2C_SDA_PIN_PORT_CLK	RCC_APB2Periph_GPIOA
#define PCF8591_I2C_SDA_PIN				GPIO_Pin_5
#define PCF8591_I2C_SDA_OUT_Init		PCF8591_I2C_SDA_PIN_PORT_CLK,\
										PCF8591_I2C_SDA_PIN_PORT,\
										PCF8591_I2C_SDA_PIN,GPIO_Speed_2MHz,GPIO_Mode_Out_PP
#define PCF8591_I2C_SDA_OUT				PAout(5)
#define PCF8591_I2C_SDA_IN_Init			PCF8591_I2C_SDA_PIN_PORT_CLK,\
										PCF8591_I2C_SDA_PIN_PORT,\
										PCF8591_I2C_SDA_PIN,GPIO_Speed_2MHz,GPIO_Mode_IN_FLOATING
#define PCF8591_I2C_SDA_IN				PAin(5)

#define PCF8591_I2C_SCL_PIN_PORT		GPIOA
#define PCF8591_I2C_SCL_PIN_PORT_CLK	RCC_APB2Periph_GPIOA
#define PCF8591_I2C_SCL_PIN				GPIO_Pin_4
#define PCF8591_I2C_SCL_Init			PCF8591_I2C_SCL_PIN_PORT_CLK,\
										PCF8591_I2C_SCL_PIN_PORT,\
										PCF8591_I2C_SCL_PIN,GPIO_Speed_2MHz,GPIO_Mode_Out_PP
#define PCF8591_I2C_SCL					PAout(4)





#define Address_PCF8591 		0x90 //PCF8591��Ӳ����ַ 1001 a2a1a0 rw
#define Model_PCF8591auto   	0x44 //ģʽ���ĸ�ADC�������� ����DAC��� �����Զ�����
#define Model_PCF8591   		0x40 //ģʽ���ĸ�ADC�������� ����DAC��� 
#define Model_PCF8591S  		0x00 //ģʽ���ĸ�ADC�������� ������DAC���





/************************************************************
*��������uint8_t  PCF8591_Init(uint8_t ch)
*���ܣ���ʼ��PCF8591��IIC���ߣ���ָ��PCF8591��ͨ���͹���ģʽ
*���룺 uint8_t *Data, chͨ����ADC���ݻ�����
*		uint8_t ch��ͨ�����
*��������� HAL_I2C_OK ��return HAL_I2C_FAIL;
*����˵������
************************************************************/

uint8_t PCF8591_Init(uint8_t ch);

/*******************************************************************
*��������uint8_t PCF8591_Write(uint8_t DAC_Date)
*���ܣ�PCF8591��DAC_Date�Ĵ�С������ģת������������Ӧ��С��ѹ��
*				DAC������;
*���룺uint8_t DAC_Date��������
*��������� HAL_I2C_OK ��return HAL_I2C_FAIL;
*����˵������
*******************************************************************/
uint8_t PCF8591_Write(uint8_t DAC_Date);

/**********************************************************
*��������uint8_t PCF8591_Read(uint8_t *a, uint8_t ch, uint8_t n)
*���ܣ���PCF8591�ж�����chͨ����ʼ��n��ADCͨ��������
*���룺 uint8_t *Data, ADC���ݻ�������
*								Data[0]���ͨ��ch��ADC����,
*								Data[1]���ͨ��ch+1��ADC����,
*								......,
*								Data[n-1]���ͨ��ch+n-1��ADC���ݣ�
*				uint8_t ch,��ʼͨ����ţ�
*				uint8_t n,ͨ��������
*��������� HAL_I2C_OK ��return HAL_I2C_FAIL;
*����˵������
**********************************************************/

uint8_t PCF8591_Read(uint8_t *Data, uint8_t ch, uint8_t n);

/**************************************************************
*��������uint8_t PCF8591_ReadChannel(uint8_t *Data, uint8_t ch)
*���ܣ���PCF8591�ж���chͨ����ADC����
*���룺 uint8_t *Data, chͨ����ADC���ݻ�����
*				uint8_t ch��ͨ�����
*��������� HAL_I2C_OK ��return HAL_I2C_FAIL;
*����˵������
***************************************************************/

uint8_t PCF8591_ReadChannel(uint8_t *Data, uint8_t ch);

/************************************************************
*��������uint8_t  PCF8591_ReadChNd(uint8_t *a)
*���ܣ���ָ��ͨ������ADC���ݣ���PCF8591_Init���ʹ��
*���룺 uint8_t *Data ��ADC���ݻ�����
*��������� HAL_I2C_OK ��return HAL_I2C_FAIL;
*����˵������
************************************************************/
uint8_t PCF8591_ReadChNd(uint8_t *a);


#endif //_PCF8591_H_
