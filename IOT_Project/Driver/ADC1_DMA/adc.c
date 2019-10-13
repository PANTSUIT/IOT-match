/**********************************************************
*	@file 		adc.c
*	@author		Panda
*	@version	V0.0.0
*	@date		2019-09
*	@brief		
***********************************************************/

#include "adc.h"

u16 ADCRawData[ADC_CHANNEL_NUMS];


/*******************************************************************************
 * @Function		: ADC_Calibrate
 * @Description		: ADC�ڲ�У׼
 * @Arguments   	: ADCx��ADCͨ��
 * @Returns     	: ��
 * @Other       	: ���¶�ADC�ڲ�У׼
 ******************************************************************************/
static void ADC_Calibrate(ADC_TypeDef *ADCx) 
{
	// Enable ADC reset calibration register
	ADC_ResetCalibration(ADCx);
	// Check the end of ADC reset calibration register
	while(ADC_GetResetCalibrationStatus(ADCx)){}
	// Start ADC calibration
	ADC_StartCalibration(ADCx);
	// Check the end of ADC calibration
	while(ADC_GetCalibrationStatus(ADCx)){}
}

/*******************************************************************************
 * @Function		: ADC_DMA_Init
 * @Description		: ADCת��DMA�����ʼ������
 * @Arguments   	: ��
 * @Returns     	: ��
 * @Other       	: ����DMA����
 ******************************************************************************/
void ADC_DMA_Init(void) 
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	DMA_InitTypeDef 	DMA_InitStructure;
	ADC_InitTypeDef 	ADC_InitStructure;
//	NVIC_InitTypeDef 	NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);								//DMA1ʱ�ӳ�ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);																				//DMA1��ͨ��1
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);	//ADC1_DR_Address;	 //ADC��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCRawData[0];	//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					//����Ϊ����Դ
	DMA_InitStructure.DMA_BufferSize =ADC_CHANNEL_NUMS;					//������DMAҪ����������ܴ�С��
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  			//�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;		//�������ݵ�λ������16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;		//�洢�����ݵ�λ������16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;					//ѭ��ģʽ��������ɺ��Զ�װ��DMAҪ����������ܴ�С��
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;			//���ȼ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;					//��ֹ�ڴ浽�ڴ�Ĵ���
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);											


	/* DMA channel1 interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;				//�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;			//��ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//�ж�ʹ��
//	NVIC_Init(&NVIC_InitStructure);   
//	
//	DMA_ClearITPendingBit(DMA1_IT_TC1);								//����жϱ�־λ
//	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);  					//����DMA������ɺ�����ж�

	DMA_Cmd(DMA1_Channel1, ENABLE);																		/* Enable DMA channel1 */
	
	/* ADC1 configuration */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						//����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 						//ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;						//��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		//��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 					//�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = ADC_CHANNEL_NUMS;					//Ҫת����ͨ����
	ADC_Init(ADC1, &ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 																/*����ADCʱ�ӣ�ΪPCLK2��6��Ƶ����12MHz*/
	
	/*����ADC1��ͨ��1-5Ϊ239.5���������ڣ�����Ϊ1 */
	ADC_TempSensorVrefintCmd(ENABLE);																	//�����¶ȴ������� Vref ͨ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, ADC1_CH10_RANK1, ADC_SampleTime_239Cycles5 );	//ADC-CH1 SENSE_A
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, ADC1_CH11_RANK2, ADC_SampleTime_239Cycles5 );	//ADC-CH1 SENSE_A
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, ADC1_CH12_RANK3, ADC_SampleTime_239Cycles5 );	//ADC-CH2 SENSE_B
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, ADC1_CH13_RANK4, ADC_SampleTime_239Cycles5 );	//ADC-CH3 SENSE_C

	ADC_DMACmd(ADC1, ENABLE);		// Enable ADC1 DMA
	ADC_Cmd(ADC1, ENABLE);			// Enable ADC1
	
	ADC_Calibrate(ADC1);			//��λУ׼�Ĵ���  
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);				// ����û�в����ⲿ����������ʹ���������ADCת��
}




/*******************************************************************************
 * @Function		: DMA1_Channel1_IRQHandler
 * @Description		: DMA1 ͨ��1 ADCת���жϺ���
 * @Arguments   	: ��
 * @Returns     	: ��
 * @Other       	: 
 ******************************************************************************/
void DMA1_Channel1_IRQHandler(void) 
{
	uint8_t i=0;
	if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
	{
		printf("%f V\r\n",ADCRawData[i] * 3.3/4096);
		delay_ms(50);
		DMA_ClearITPendingBit(DMA1_IT_TC1);
	}
}





/***********************************END OF FILE*********************************/
