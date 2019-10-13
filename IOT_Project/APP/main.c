/* ==================================================================
#     FileName: main.c
#      History:
================================================================== */
/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* Library includes. */
#include "stm32f10x.h"
#include "stm32f10x_it.h"
/* user files */
#include "Init.h"
#include "delay.h"

#include "LCD12864.h"
#include "lock.h"
#include "lamp.h"
#include "buzzer.h"

#include "userapp.h"
#include "WiFiToCloud.h"
#include "NLECloud_TCP.h"

#include "bsp_flash.h"

#include "2017_A.h"
#include "queuing_machine.h"

void SendHead(void);
void SendAddr(void);
void SendFlag(u8 flag);
void SendLength(int length);
void Sendcmd(u8 cmd);
void SendCheck(u16 check);
u8 *Recive_response(u16 waittime);



int main(void)
{
	uint8_t err;
	
	int8_t temp=0;

	uint16_t i=0,len=0;
	uint8_t ensure;
	uint8_t *data;
	
	uint8_t page_0_buf[8][4]={0};
	uint8_t page_1_buf[8]={0};
	uint8_t w_buf[4];
	
	uint8_t send_buf[20]="abcdefgh";
	uint8_t recv_buf[20]={0};
	
	uint8_t w=16;
	uint8_t r=0;
	
	unsigned char voice[20];
	
	u16 value=0;
	float vol;
	
	
	// 2017 ����Ŀ
//	_2017_machine_init();
//	_2017_main_task();
	
	
	// �Ŷӻ�
//	void Queue_machine_init(void);
//	void Queue_main_task(void);
	Queue_machine_init();
	Queue_main_task();
//	
	__Init_Device();
	SI4332A_Init();
	SI4332B_Init();
	
	printf("Init Device Completed!\r\n");
	
	//���ӷ�����
//	ConnectToServer_times();
	
	lcd_clr();
	
	//ָ��ģ�� 	
//	printf("��AS608ģ������....\r\n");	
//	while(AS608_VfyPwd())//��AS608ģ������
//	{
//		delay_ms(400);
//		printf("δ��⵽ģ��!!!\r\n");
//		delay_ms(800);
//		printf("��������ģ��...\r\n");		  
//	}
//	printf("ͨѶ�ɹ�!!!\r\n");
//	ensure = AS608_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
//	if(ensure != 0x00)
//		ShowErrMessage(ensure);//��ʾȷ���������Ϣ	
//	ensure = AS608_ReadSysPara(&AS_SysPara);  //������ 
//	if(ensure==0x00)
//	{
//		printf("AS608_N:%d,BAUD:%d \r\n",AS_SysPara.AS608_N,9600*(AS_SysPara.AS608_N));
//		printf("AS608_Addr= %#x,AS608_size=%d\r\n",AS_SysPara.AS608_addr,AS_SysPara.AS608_size);
//		printf("������:%d,�Աȵȼ�: %d\r\n",AS_SysPara.AS608_max-ValidN,AS_SysPara.AS608_level);
//	}
//	else
//		ShowErrMessage(ensure);	
//	delay_ms(500);
//	userApp();
	

//	demo_flash();
//	STM32_Flash_Capacity(data_buf);
//	printf("%s\r\n",data_buf);
	
//	printf("w=%d\r\n",w);
//	printf("r=%d\r\n",r);
//	
//	bsp_flash_erase(0x8000000 + 0x70000,1);
//	bsp_flash_write(0x8000000 + 0x70000,&w,1);
//	bsp_flash_read(0x8000000 + 0x70000,&r,1);

//	demo_flash();
	int16_t site=1;
	char * d;
	double a;
	while(1)
	{
		
//		memset((char*)data_buf,0x00,sizeof(data_buf));
//		ESP8266_GetIpData(AT_RX_BUF,(char *)data_buf);
//		ClrAtRxBuf();
//		d=json_getvalue((char *)data_buf,(char *)"data");
//		printf("return d: %s\r\n",d);
//		remove_both_end(d);
//		printf("remove d: %s\r\n",d);
//		a=data_type_conversion_S2N(d);
//		printf("return a: %f\r\n",a);
////		delay_ms(10);
//		i++;
//		if(i%30==0)
//			ESP8266_IpSend((char *)PING_REQ, strlen((const char *)PING_REQ));
//		delay_ms(100);
		
		
		
//		STM32_Flash_Capacity(data_buf);
//		printf("%s\r\n",data_buf);
		
//		printf("w=%d\r\n",w);
//		printf("r=%d\r\n",r);
		
//		m3_key_sta=M3_key_scan(1);
//		if(m3_key_sta == KEY2_PRESS)
//		{
//			site+=1;

//		}
//		else if(m3_key_sta == KEY1_PRESS)
//		{
//			site-=1;

//		}		
//		
//		Servo_Control_X(site);
//		Servo_Control_Y(site);
//		
//		i++;
//		if(i % 50000 == 0)
//		{
////			printf("site=%d\r\n",site);
////			printf("O1:%d\r\n",TIM_GetCapture1(TIM3));
////			printf("O2:%d\r\n\r\n",TIM_GetCapture2(TIM3));
//			a = (float)pwm_drive_lower("0");
//			printf("1: %f\r\n",a);
//			a = pwm_drive_lower("45");
//			printf("2: %f\r\n",a);
//			a = pwm_drive_lower("90");
//			printf("3: %f\r\n",a);
//			a = pwm_drive_lower("135");
//			printf("4: %f\r\n",a);
//			a = pwm_drive_lower("180");
//			printf("5: %f\r\n",a);

//		}
//		memset(recv_buf,0x00,sizeof(recv_buf));
//		SI4332A_Send_Msg(send_buf,8,0);
//		delay_ms(200);
//		SI4332B_Read_Msg(recv_buf,8,2000);
//	
//		
//		printf("send buf: %s\r\n",send_buf);
//		printf("recv buf: %s\r\n",recv_buf);
//		delay_ms(2000);
//		
//		NixieTube_Show(0,2);
//		NixieTube_Show(2,2);
//		NixieTube_Show(5,2);
//		NixieTube_Show(7,2);
//		NixieTube_Show(6,2);

	}	
}	

