/**********************************************************
*	@file 		Init.c
*	@author		Panda
*	@version	V0.0.0
*	@date		2019-09
*	@brief		�豸��ʼ��
***********************************************************/
#include "Init.h"

/* Includes ------------------------------------------------------------------*/

/* variables -----------------------------------------------------------------*/

char lcd_show_buf[100];

DEV_STATUS device_sta={0};

const char light_status[2][5]={"OFF","ON"};

MENU_STATUS Current_Menu = MENU1;

uint8_t m3_key_sta = 0; 

uint8_t data_buf[800]={0};

/* function ------------------------------------------------------------------*/

/*******************************************************************************
*@brief		M3 ����ģ���ϵİ�����ʼ��
*@param		None
*@retval	None
********************************************************************************/
void M3_key_init(void)
{
	STM32_GPIOx_Init(RCC_KEY1_PORT,KEY1_PORT,KEY1_PIN,GPIO_Speed_50MHz,GPIO_Mode_IPU);
	STM32_GPIOx_Init(RCC_KEY2_PORT,KEY2_PORT,KEY2_PIN,GPIO_Speed_50MHz,GPIO_Mode_IPU);
}

/*******************************************************************************
*@brief		M3 ����ģ���ϵİ���ɨ��
*@param		mode: �������°����򵥴ΰ��°���
*	@arg	0: ���ΰ��°���
*	@arg 	1���������°���
*@retval	�Ƿ��а�������
*	@val	0: û�а�������
*	@val	KEY1_PRESS: KEY1����
*	@val	KEY2_PRESS: KEY2����
********************************************************************************/
uint8_t M3_key_scan(uint8_t mode)
{
	static uint8_t key = 1;
	if(key == 1 && (KEY1 == 0 || KEY2  == 0))
	{
		delay_ms(10);
		key = 0;
		if(KEY1 == 0)
		{
			return KEY1_PRESS;
		}
		else if(KEY2 == 0)		
		{
			return KEY2_PRESS;
		}			
	}
	if(KEY1 == 1 && KEY2 == 1)
		key = 1;
	if(mode)
	{
		key = 1;
	}
	return 0;
}

/*******************************************************************************
*@brief		STM32�豸��ʼ��
*@param		None
*@retval	None
********************************************************************************/
void __Init_Device(void)
{
	STM_Clock_Init(9);	// ϵͳʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SYSTICK_Init();
			
	//USART1 Init  TX:PA9 RX:PA10
	USART1_Init(115200);
	
	//USART2 TX:PA2  RX:PA3
	USART2_Init(115200);
	
	/* TX:PC10;RX:PC11. ע�������Ĭ��״̬Ϊuart4.*/
	UART4_Init(115200);
	
	/* TX: PC12 RX:PD2 */
//	UART5_Init(115200);
	
	if(RTC_Init())
		printf("RTC Init Failed!\r\n");

	
	//as698 ��ʼ��PA4��״̬����
//	AS608_StaGPIO_Init();
//	
	//lcd128*64 Init
	lcd_init();
//	
//	//lock Init
//	lock_init();
//	
//	//Light Init
//	lamp_init();
//	
	//Keyboard Init
	kbd_init();
//	
//	//M3 key init KEY1: PC13  KEY2: PD13
	M3_key_init();
	
	//TIM2 Init ���ڰ���ɨ�� 
	TIM2_Init();
//	
	//LF125K Init
	LF125K_Init();
//	
	//buzzer Init  PA8
	buzzer_init();

	//xfs5152ce Init :usart3 TX: PB10 RX: PB11
	Xfs5152ce_Init();
//	
//	ADC_DMA_Init();
//	
//	NixieTube_Init();
	
	TIM3_Init(2000-1,720-1);
	
}


/*************************************END OF FILE*****************************************/
