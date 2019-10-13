/********************************************************************
*	@file 		usart.h
*	@author		Panda
*	@version	V0.0.0
*	@date		2019-09
*********************************************************************/

#include "usart.h"	

//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
//�ض���fgetc����
int fgetc(FILE *f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
	{
	}
	return (int)USART_ReceiveData(USART1);
}

#endif

/*------------------------------ USART1 ------------------------------*/

#if USART1_RX_EN   //���ʹ���˽���
	
u8 USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

/**
*����״̬
*		bit15��		������ɱ�־
*		bit14��		���յ�0x0d
*		bit13~0��	���յ�����Ч�ֽ���Ŀ
*/
u16 USART1_RX_STA=0;       //����״̬���	

#endif	/* USART1_RX_EN */

/**************************************************************
*@brief 	USART1 ��ʼ��
*@param 	bound:USART1 ������
*@retval 	None
***************************************************************/
void USART1_Init(u32 bound)
{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	// ʹ��AFIOʱ��
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if USART1_RX_EN	

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}


void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
//		USART_SendData(USART3,Res);
		if((USART1_RX_STA&0x8000)==0)//����δ���
		{
			if(USART1_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0A)
					USART1_RX_STA=0;//���մ���,���¿�ʼ
				else 
					USART1_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0D)
					USART1_RX_STA|=0x4000;
				else
				{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART1_REC_LEN-1))	//�������ݴ���,���¿�ʼ����	 
						USART1_RX_STA=0; 
				}		 
			}
		}
  } 
 #if SYSTEM_SUPPORT_OS  
	OSIntExit();    	//�˳��ж�
#endif
}


void USART1_DataAnalysis(void)
{
	u8 temp=0;
	u16 waittime=0;
	if(USART1_RX_STA & 0x8000)
	{
		if(strstr((const char*)USART1_RX_BUF,"unlock")!=NULL)
		{
			lock_ctrl(1);
		}
		else if(strstr((const char*)USART1_RX_BUF,"lock")!=NULL)
		{
			lock_ctrl(0);
		}
		else if(strstr((const char*)USART1_RX_BUF,"light on")!=NULL)
		{
			lamp_ctrl(1);
		}
		else if(strstr((const char*)USART1_RX_BUF,"light off")!=NULL)
		{
			lamp_ctrl(0);
		}
		else if(strstr((const char*)USART1_RX_BUF,"buzzer on")!=NULL)
		{
			buzzer_ctrl(1);
		}else if(strstr((const char*)USART1_RX_BUF,"buzzer off")!=NULL)
		{
			buzzer_ctrl(0);
		}
		else if(strstr((const char*)USART1_RX_BUF,"get id")!=NULL)
		{
			LF125K_Read_1_Page(data_buf);
			data_buf[8]= '\0' ;
			DEBUG("ID Card:%x,%x,%x,%x,%x,%x,%x,%x",data_buf[0],data_buf[1],data_buf[2],data_buf[3],data_buf[4],data_buf[5],data_buf[6],data_buf[7]);
		}
		else if(strstr((const char*)USART1_RX_BUF,"read page 0:0")!=NULL)
		{
			LF125K_Read_0_Page(0,data_buf);
			data_buf[4]='\0';
			DEBUG("addr:00 content:%x,%x,%x,%x",data_buf[0],data_buf[1],data_buf[2],data_buf[3]);
		}
		else if(strstr((const char*)USART1_RX_BUF,"read page 0:1")!=NULL)
		{
			LF125K_Read_0_Page(1,data_buf);
			data_buf[4]='\0';
			DEBUG("addr:01 content:%x,%x,%x,%x",data_buf[0],data_buf[1],data_buf[2],data_buf[3]);
		}
		else if(strstr((const char*)USART1_RX_BUF,"read page 0:2")!=NULL)
		{
			
			LF125K_Read_0_Page(2,data_buf);
			data_buf[4]='\0';
			DEBUG("addr:02 content:%x,%x,%x,%x",data_buf[0],data_buf[1],data_buf[2],data_buf[3]);
		}
		else if(strstr((const char*)USART1_RX_BUF,"read page 0:3")!=NULL)
		{
			LF125K_Read_0_Page(3,data_buf);
			data_buf[4]='\0';
			DEBUG("addr:03 content:%x,%x,%x,%x",data_buf[0],data_buf[1],data_buf[2],data_buf[3]);
		}
		else if(strstr((const char*)USART1_RX_BUF,"read page 0:4")!=NULL)
		{
			LF125K_Read_0_Page(4,data_buf);
			data_buf[4]='\0';
			DEBUG("addr:04 content:%x,%x,%x,%x",data_buf[0],data_buf[1],data_buf[2],data_buf[3]);
		}
		else if(strstr((const char*)USART1_RX_BUF,"read page 0:5")!=NULL)
		{
			LF125K_Read_0_Page(5,data_buf);
			data_buf[4]='\0';
			DEBUG("addr:05 content:%x,%x,%x,%x",data_buf[0],data_buf[1],data_buf[2],data_buf[3]);
		}
		else if(strstr((const char*)USART1_RX_BUF,"read page 0:6")!=NULL)
		{
			LF125K_Read_0_Page(6,data_buf);
			data_buf[4]='\0';
			DEBUG("addr:06 content:%x,%x,%x,%x",data_buf[0],data_buf[1],data_buf[2],data_buf[3]);
		}
		else if(strstr((const char*)USART1_RX_BUF,"read page 0:7")!=NULL)
		{
			LF125K_Read_0_Page(7,data_buf);
			data_buf[4]='\0';
			DEBUG("addr:07 content:%x,%x,%x,%x",data_buf[0],data_buf[1],data_buf[2],data_buf[3]);
		}
		else if(strstr((const char*)USART1_RX_BUF,"swipe lf")!=NULL)
		{
			waittime=1000;
			while(waittime--)
			{
				delay_ms(10);
				temp = LF125K_SwipeCard();
				if(temp == ID_CARD1)
				{
					door_open(500);
					break;
				}
				else if(temp ==ID_CARD3)
				{
					door_open(500);
					break;
				}
			}
			if(waittime==0)
				DEBUG("Swipe Card failed!");		
		}
		else if(strstr((const char*)USART1_RX_BUF,"add fingerprint")!=NULL)
		{
			printf("Add fingerprint\r\n");
			Add_FR();
		}
		else if(strstr((const char*)USART1_RX_BUF,"del fingerprint")!=NULL)
		{
			printf("Del fingerprint!\r\n");
			Del_FR();
		}
		else if(strstr((const char*)USART1_RX_BUF,"press fingerprint")!=NULL)
		{
			printf("Swiping fingerprint... ...\r\n");
			waittime = 1000;
			while(waittime--)
			{
				if(!press_FR())
				{
					printf("Swipe fingerprint Successfully!\r\n");
					break;
				}
			}
			if(waittime == 0)
				printf("Swipe fingerprint Out Time\r\n");
			
		}
		else if(strstr((const char*)USART1_RX_BUF,"up char")!=NULL)
		{
			printf("Uping char... ...\r\n");
			waittime = 1000;
			while(waittime--)
			{
				if(AS608_UpChar(CharBuffer1))
				{
					;
					break;
				}
			}
			if(waittime == 0)
				printf("Swipe fingerprint Out Time\r\n");
			
		}
//		printf("\r\n%s\r\n",USART1_RX_BUF);
		memset(USART1_RX_BUF,0x00,sizeof(USART1_RX_BUF));
		USART1_RX_STA = 0;
	}
}






/*------------------------------ USART2 ------------------------------*/

//���ڽ��ջ����� 	
u8 USART2_RX_BUF[USART2_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.

u8 USART2_TX_BUF[USART2_SEND_LEN]; 			  //���ͻ���,���USART2_MAX_SEND_LEN�ֽ�

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
vu16 USART2_RX_STA=0;   	


void USART2_IRQHandler(void)
{
#if	SYSTEM_SUPPORT_OS
	OSIntEnter();
#endif
	uint8_t res;	  
	uint32_t temp;
//	if(!(USART2->CR1 & (0x01<<4)))
//		USART2->CR1 |= (0x01<<4);
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{
		res = USART_ReceiveData(USART2);
//		USART_SendData(USART1,res);
		if(!(USART2_RX_STA & 0x8000))
		{	
//			USART_SendData(USART1,res);
			USART2_RX_BUF[USART2_RX_STA++]=res;
		}
	}
	if(USART_GetITStatus(USART2,USART_IT_IDLE) != RESET)
	{
		temp=USART2->SR;
		temp=USART2->DR;
		USART2_RX_STA |= 0x8000;
	}
		

    
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���յ�����
//	{	 
//		res =USART_ReceiveData(USART2);		 
//		if((USART2_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
//		{ 
//			if(USART2_RX_STA<USART2_RECV_LEN)	//�����Խ�������
//			{
//				TIM_SetCounter(TIM7,0);//���������          				//���������
//				if(USART2_RX_STA==0) 				//ʹ�ܶ�ʱ��7���ж� 
//				{
//					TIM_Cmd(TIM7,ENABLE);//ʹ�ܶ�ʱ��7
//				}
//				USART2_RX_BUF[USART2_RX_STA++]=res;	//��¼���յ���ֵ	 
//			}
//			else 
//			{
//				USART2_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
//			} 
//		}
//	}  	
#if	SYSTEM_SUPPORT_OS
	OSIntExit();
#endif

}   


//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void USART2_Init(u32 bound)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); //����2ʱ��ʹ��

 	USART_DeInit(USART2);  //��λ����2
	//USART2_TX   PA2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA2
   
    //USART2_RX	  PA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA3
	
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
  

	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
	
	//ʹ�ܽ����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�   
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	
	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
//	TIM7_Int_Init(99,7199);		//10ms�ж�
	USART2_RX_STA=0;		//����
//	TIM_Cmd(TIM7,DISABLE);			//�رն�ʱ��7

}

//���ڷ���һ���ֽ�
void USART2_SendData(u8 data)
{
	while((USART2->SR&0X40)==0); 
	USART2->DR = data;
}

//����2,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
void usart2_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
		USART_SendData(USART2,USART2_TX_BUF[j]); 
	} 
}


/*------------------------------ USART3 ------------------------------*/
#if USART3_IT_EN   //���ʹ���˽���	

//u8  USART3_RX_BUF[USART3_RECV_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.
//u16 USART3_RX_STA=0;       //����״̬���	  

void USART3_IRQHandler(void)
{
	uint8_t recv_char;
	uint32_t temp=0;
	
#if	SYSTEM_SUPPORT_OS
	OSIntEnter();
#endif	/* SYSTEM_SUPPORT_OS */

#if USART3_IT_RX_EN	
	if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)  //���յ�����
	{
		recv_char=USART_ReceiveData(USART3);
//		USART_SendData(USART1,recv_char);
//		USART3_RX_BUF[USART3_RX_STA++]=recv_char;
//		if(USART3_RX_STA > USART3_RECV_LEN)
//			USART3_RX_STA = 0;
        Xfs_AckReceive(recv_char);
    }
#endif /* USART3_IT_RX_EN */
#if USART3_IT_IDLE_EN	
	if(USART_GetITStatus(USART3,USART_IT_IDLE) != RESET)
	{
		temp = USART3->SR;
		temp = USART3->DR;
	}
#endif /* USART3_IT_IDLE_EN */	
}
#if	SYSTEM_SUPPORT_OS
	OSIntEnter();
#endif	/* SYSTEM_SUPPORT_OS */

#endif	/* USART3_IT_EN */

//APB1Ϊ36Mhz
void USART3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //����2ʱ��ʹ��

 	USART_DeInit(USART3);  //��λ����2
	//USART3_TX   PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 		//PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure); 	//��ʼ��PB10
   
    //USART3_RX	  PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);  	//��ʼ��PB11
	
	USART_DeInit(USART3);
	USART_InitStructure.USART_BaudRate = bound;		//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); 	//��ʼ������3
  
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 
	
#if USART3_IT_RX_EN
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //ʹ�ܽ����ж�
#endif /* USART3_IT_RX_EN */

#if USART3_IT_IDLE_EN
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);	//ʹ�ܿ����ж�  
#endif /* USART3_IT_IDLE_EN */

#if USART3_IT_EN		
	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
#endif /* USART3_IT_EN */
}

/******************************************************************************
* Function Name --> ����3����һ���ֽ�����
* Description   --> nonevoid USART3_SendData(uint16_t Data)
* Input         --> bound��������	
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void USART3_SendData(uint8_t Data)
{
	while((USART3->SR & 0x40) == 0);	//�ȴ��������
	USART3->DR = Data;      	//дDR,����3����������
}


void USART3_SendStrlen(uint8_t *Data, int len)
{
	while(len--)
	{
		USART3_SendData((uint16_t )(*Data));
		Data++;
	}
}
	








/*------------------------------ USART4 ------------------------------*/

/******************************************************************************
* @brief	��ʼ��IO ����4
* @param	bound��������	
* @retval	None
******************************************************************************/
void UART4_Init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
#ifdef UART4_RX_EN	//���ʹ���˽����ж�
	
	UART4->CR1 |= 1 << 8;	//PE�ж�ʹ��
	UART4->CR1 |= 1 << 5;	//���ջ������ǿ��ж�ʹ��
	
	MY_NVIC_Init(0, 1, UART4_IRQn, NVIC_PriorityGroup_2);	//�жϷ���2
#endif

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//ʹ��UART4��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	USART_DeInit(UART4);  //��λ����4
	
	//UART4_TX   PC.10 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��PC10
   
	//UART4_RX	  PC.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);  //��ʼ��PC11

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;	//���ò����ʣ�һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;	//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	
	USART_Init(UART4, &USART_InitStructure); //��ʼ������
	
	USART_ClearITPendingBit(UART4,USART_IT_RXNE);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//���������ж�
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);//���������ж�
	USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ��� 
}

/******************************************************************************
* @brief 	����4����һ���ֽ�����
* @param 	data��Ҫ���͵�����	
* @retval 	None
******************************************************************************/
void UART4_SendData(uint16_t Data)
{
	while((UART4->SR & 0x40) == 0);	//�ȴ��������
	USART_SendData(UART4, Data);
}

/******************************************************************************
* @brief 	����4����һ������
* @param	*Data: Ҫ���͵��ַ���	
* @retval	None
******************************************************************************/
void UART4_SendStr(uint8_t *Data)
{
	while(*Data!='\0')
	{
		UART4_SendData((uint16_t )(*Data));
		Data++;
	}
}

/******************************************************************************
* @brief	����4����һ�����ݣ�ָ�����ݳ���
* @param	*Data:�ַ�����len����	
* @retval	None 
******************************************************************************/
void UART4_SendStrlen(uint8_t *Data, uint8_t len)
{
	while(len--)
	{
		UART4_SendData((uint16_t )(*Data));
		Data++;
	}
}


 #ifdef UART4_RX_EN   //���ʹ���˽���

//����4�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ��� 

uint8_t UART4_RX_BUF[UART4_REC_MAX_LEN];	//���ջ���,���UART4_REC_LEN���ֽ�
uint32_t UART4_RX_STA = 0;	
uint8_t  IS_UART4_IN_RX = 0 ;
/******************************************************************************
* @brief	����4�����жϷ������
* @param	None
* @retval	None
*******************************************************************************/
void UART4_IRQHandler(void)
{
#if	SYSTEM_SUPPORT_OS
	OSIntEnter();
#endif	/* SYSTEM_SUPPORT_OS */
	uint32_t temp=0;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //���յ�����
	{
		IS_UART4_IN_RX = 1;
		UART4_RX_BUF[UART4_RX_STA++] = USART_ReceiveData(UART4);	//��ȡ���յ�������
//		USART_SendData(USART1,UART4_RX_BUF[UART4_RX_STA-1]);
		if(UART4_RX_STA>UART4_REC_MAX_LEN) //���������
		{
			UART4_RX_STA = 0 ;
		}
//		printf("%s\r\n",UART4_RX_BUF);
		//end ����δ���   	
	}	//end ���յ�����
	if(USART_GetITStatus(UART4,USART_IT_IDLE)  != RESET)
	{
		temp = UART4->SR;
		temp = UART4->DR;
		IS_UART4_IN_RX = 0;
	}
	
	
	
#if	SYSTEM_SUPPORT_OS
	OSIntExit();
#endif	/* SYSTEM_SUPPORT_OS */	
}

#endif	/* UART4_RX_EN */

//=========================================================


/*------------------------------ UART5 ------------------------------*/

//���ڽ��ջ����� 	
u8 UART5_RX_BUF[UART5_MAX_RECV_LEN]; 				//���ջ���,���UART5_MAX_RECV_LEN���ֽ�.
u8 UART5_TX_BUF[UART5_MAX_SEND_LEN]; 			  //���ͻ���,���UART5_MAX_SEND_LEN�ֽ�

/****************************************************************************************
*	ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
*	���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
*	�κ�����,���ʾ�˴ν������.
*	���յ�������״̬
*	[15]:0,û�н��յ�����;1,���յ���һ������.
*	[14:0]:���յ������ݳ���
*****************************************************************************************/
vu16 UART5_RX_STA=0;   	


void UART5_IRQHandler(void)
{
#if	SYSTEM_SUPPORT_OS
	OSIntEnter();
#endif	/* SYSTEM_SUPPORT_OS */
	u8 res;	      
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		res =USART_ReceiveData(UART5);		 
		if((UART5_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
		{ 
			if(UART5_RX_STA<UART5_MAX_RECV_LEN)	//�����Խ�������
			{
				TIM_SetCounter(TIM7,0);//���������          				//���������
				if(UART5_RX_STA==0) 				//ʹ�ܶ�ʱ��7���ж� 
				{
					TIM_Cmd(TIM7,ENABLE);//ʹ�ܶ�ʱ��7
				}
				UART5_RX_BUF[UART5_RX_STA++]=res;	//��¼���յ���ֵ	 
			}
			else 
			{
				UART5_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
			} 
		}
	}  		
#if	SYSTEM_SUPPORT_OS
	OSIntExit();
#endif	/* SYSTEM_SUPPORT_OS */	
}   

/**********************************************************
*@brief		��ʼ��IO ����2
*@param		bound:������	  
*@retval	None
***********************************************************/
void UART5_Init(u32 bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	// GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE); //����2ʱ��ʹ��

 	USART_DeInit(UART5);  //��λ����2
	//UART5_TX   PC12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��PC12
	
	//UART5_RX	  PD2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOD, &GPIO_InitStructure);  //��ʼ��PA3
	
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  
	USART_Init(UART5, &USART_InitStructure); //��ʼ������2
  

	USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ��� 
	
	//ʹ�ܽ����ж�
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�����ж�   
	
	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
	TIM7_Int_Init(99,7199);		//10ms�ж�
	UART5_RX_STA=0;		//����
	TIM_Cmd(TIM7,DISABLE);			//�رն�ʱ��7

}

//����2,printf ����
//ȷ��һ�η������ݲ�����UART5_MAX_SEND_LEN�ֽ�
void uart5_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)UART5_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)UART5_TX_BUF);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
	  while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
		USART_SendData(UART5,UART5_TX_BUF[j]); 
	} 
}





/********************************************END OF FILE**************************************************/
