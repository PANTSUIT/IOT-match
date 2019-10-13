/************************************************************************
*@file		timer.c
*@author	Panda
*@version 	V0.0.0
*@date		2019-09
*@brief		��ʱ����ʼ��
*************************************************************************/

#include "timer.h"

/*-------------------------- TIM1 ----------------------------------*/

/*********************************************************************
*@brief		TIM1 Init
*@param		arr: ��װ��ֵ
*			pre: Ԥ��Ƶϵ��
*@retval	None
*@attention  CH1 ������
**********************************************************************/
void TIM1_Init(uint16_t arr,uint16_t pre)
{
	GPIO_InitTypeDef GPIO_InitStructure;	// GPIO init struct
	NVIC_InitTypeDef NVIC_InitStructure;	// NVIC config struct
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	//TIM_TimeBase init struct
	TIM_OCInitTypeDef TIM_OCInitStructure;	// TIM OC  init Struct
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); // ʹ�� GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	// ʹ�� AFIO ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);	//ʹ��TIM1��ʱ��
	
	// TIM1 CH1 GPIO ��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;         //ѡ������8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //���Ƶ�����50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
#if TIM1_IT_EN	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
#endif	/* TIM1_IT_EN */
	
	TIM_DeInit(TIM1);	//���½�Timer����Ϊȱʡֵ

	TIM_InternalClockConfig(TIM1);//�����ڲ�ʱ�Ӹ�TIM1�ṩʱ��Դ

	TIM_TimeBaseStructure.TIM_Prescaler = pre;	//����Ԥ��Ƶϵ��
	TIM_TimeBaseStructure.TIM_Period = arr;		//������װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���ü�����ģʽΪ���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);	//������Ӧ�õ�TIM1��
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 	//Specifies the TIM mode
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCNPolarity_Low;	//Specifies the complementary output polarity.
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_Low;	//Specifies the output polarity
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;	//Specifies the TIM Output Compare pin state during Idle state
	TIM_OCInitStructure.TIM_OCNIdleState =TIM_OCNIdleState_Reset;	//Specifies the TIM Output Compare pin state during Idle state
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//Specifies the TIM Output Compare state
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;	//Specifies the TIM complementary Output Compare state
	TIM_OCInitStructure.TIM_Pulse = 50; 

	// CH1 Config
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); 
	
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);	//�������жϱ�־
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);//��ֹARRԤװ�ػ�����
	
#if TIM1_IT_UPDATE_EN	
	TIM_ITConfig(TIM1,TIM_IT_Update ,ENABLE);//����TIM1���ж�
#endif /* TIM1_IT_UPDATE_EN */
#if TIM1_IT_TRIGGER_EN
	TIM_ITConfig(TIM1,TIM_IT_Trigger,ENABLE);//����TIM1���ж�
#endif	/* TIM1_IT_TRIGGER_EN */
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	//Enables the TIM peripheral Main Outputs. 
	
	TIM_Cmd(TIM1,DISABLE);	//������ʱ��1
	
}


#if TIM1_IT_EN
void TIM1_UP_IRQHandler(void)
{
	static int32_t count = 0;
	
   //����Ƿ�����������¼�
   if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
   {
      count++;
      //���TIM1���жϴ�����λ
      TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);
	}
 }
#endif	/* TIM1_IT_EN */



/*-------------------------- TIM2 ----------------------------------*/

/***********************************************************
*@brief		TIM2 CH2 Init
*@param		None
*@retval	None
************************************************************/
static void TIM2_CH2_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;         //ѡ������5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //���Ƶ�����50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�������������

	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/***********************************************************
*@brief		TIM2 NVIC config
*@param		None
*@retval	None
************************************************************/
static void TIM2_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

/***********************************************************
*@brief		TIM2 Time Base config
*@param		arr: װ��ֵ
*			pre: Ԥ��Ƶϵ��
*@retval	None
************************************************************/
static void TIM2_TimeBase_Config(uint16_t arr,uint16_t pre)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);// ʹ��TIM2��ʱ��

	TIM_DeInit(TIM2);	//���½�Timer����Ϊȱʡֵ

	TIM_TimeBaseStructure.TIM_Prescaler = pre;
	TIM_TimeBaseStructure.TIM_Period = arr;	//���ü��������С��ÿ��1000�����Ͳ���һ�������¼�
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ü�����ģʽΪ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);	//������Ӧ�õ�TIM2��

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	//�������жϱ�־
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);	//��ֹARRԤװ�ػ�����

#if TIM2_IT_UPDATE_EN
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//����TIM2���ж�
#endif /* TIM2_IT_UPDATE_EN */
	
}

/***********************************************************
*@brief		TIM2 OC2 config, OCmode: PWM1
*@param		None
*@retval	None
************************************************************/
static void TIM2_OC2_Config(void)
{
	TIM_OCInitTypeDef	TIM_OCInitStructure;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	// 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
	TIM_SetCompare2(TIM2,500);

}

/***********************************************************
*@brief		TIM2 Init
*@param		None
*@retval	None
************************************************************/
void TIM2_Init(void)
{
	// GPIO config
#if TIM2_OC2_EN	
	TIM2_CH2_GPIO_Config();	
#endif	/* TIM2_OC2_EN*/
	
	//interrupt init
#if TIM2_IT_EN
	TIM2_NVIC_Configuration();
#endif	/* TIM2_IT_EN */
	
	//tim2 timer init
	TIM2_TimeBase_Config(1000-1,72-1);

#if TIM2_OC2_EN	
	//TI2 OC2 config
	TIM2_OC2_Config();
#endif 	/* TIM2_OC2_EN */
	//������ʱ��2
	TIM_Cmd(TIM2,ENABLE);
}

#if TIM2_IT_EN

void TIM2_IRQHandler(void)
{
	static int32_t count = 0;
	
   //����Ƿ�����������¼�
   if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	// û1ms ����һ���ж�
   {
		count++;
		//���TIM2���жϴ�����λ
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
		//do keboard scaning
		if(count % 10 == 0)
		{	
			kbd_scan();
			NixieTube_Refresh();
		}
		if(count % 1000 == 0)
		{
			lock_getsta();
			lamp_getsta();
			buzzer_getsta();
		}
	}
 }
#endif /* TIM2_IT_EN */
 
/* TIM3--------------------------------------------------------------------*/


/*****************************************************************************************
*@brief		TIM3 Init
*@param		arr: װ��ֵ
*			pre: Ԥ��Ƶϵ��
*@retval	None
******************************************************************************************/
void TIM3_Init(u16 arr,u16 pre)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer3������ӳ��  
	
	//���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOC7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO
 
	//��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = pre; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel-1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	
	//��ʼ��TIM3 Channel-2 PWMģʽ	 
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	
	//��ʼ��TIM3 Channel-3 PWMģʽ	 
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	
	//��ʼ��TIM3 Channel-4 PWMģʽ	 
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
}

 
 

/********************************* TIM7 ******************************************/

 //��ʱ��7�жϷ������		    
void TIM7_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
//		USART2_RX_STA|=1<<15;
		UART5_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIM7�����жϱ�־    
		TIM_Cmd(TIM7, DISABLE);  //�ر�TIM7 
	}	    
}
 
//ͨ�ö�ʱ��7�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ42M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz 
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM7_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM7ʱ��ʹ��    
	
	//��ʱ��TIM7��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; 	//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM7�ж�,��������ж�
	
	TIM_Cmd(TIM7,ENABLE);//������ʱ��7
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}



