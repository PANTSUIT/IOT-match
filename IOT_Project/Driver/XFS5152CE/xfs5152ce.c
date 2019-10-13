/***********************************************************
*@file		xfs5152ce.c
*@author
*@verdion
*@date		2019-09
*@brief
************************************************************/

#include "xfs5152ce.h"

//ȫ�ֱ�����װ������оƬ�ķ���ֵ
unsigned char xfs_data_buf[80];

//��������оƬ�Ľ���֡
unsigned char xfs_state = 0;
volatile unsigned char xfs_receive_ok = 0;	// ��ɱ�־
unsigned char xfs_protocol_state = 0;
unsigned char xfs_protocol_dat_len = 0;		// ���ݳ���
unsigned char xfs_protocol_dat_flag = 0;	// �������ͱ�־
unsigned char xfs_protocol_dat_cnt = 0;		// �������ݼ���

//���ƽ���ʱ�䣬��ֹ����ͻȻ�жϣ����������ѭ��
unsigned char xfs_timer_flag1 = 0;      //��������ļ�ʱ��־
unsigned char xfs_timer_flag2 = 0;      //�������ݵļ�ʱ��־
unsigned char xfs_timeout_cnt = 0;
unsigned char xfs_time_uplimit = 0;

//�洢�������ݣ����10s����
unsigned char xfs_encode_buf[30000];

//����colloc������Ҫ
void _ttywrch(int ch)
{
    ch = ch;
}

//xfs5152������ö��
typedef enum 
{
	XFS_CMD_STATUS = 0x21,			//��ѯ״̬
	XFS_CMD_SLEEP = 0x88,			//����͹���ģʽ
	XFS_CMD_WAKE_UP = 0xFF,			//�ӵ͹��ķ�������ģʽ
	XFS_CMD_VOICE_START = 0x01,		//��ʼ�����ϳ�
	XFS_CMD_VOICE_STOP = 0x02,		//ֹͣ�����ϳ�
	XFS_CMD_VOICE_PAUSE = 0x03,		//��ͣ�����ϳ�
	XFS_CMD_VOICE_RESTORE = 0x04,	//�ָ������ϳ�
	XFS_CMD_SEND_TO_CACHE = 0x31,	//�����ı���������
	XFS_CMD_PLAY_FROM_CACHE = 0x32,	//�ӻ����������ı�
	XFS_CMD_RECOGNIZE_START = 0x10,	//��ʼ����ʶ��
	XFS_CMD_RECOGNIZE_STOP = 0x1F,	//ֹͣ����ʶ��
    XFS_CMD_ENCODE_START = 0x41,	//��ʼ����
    XFS_CMD_CODE_START = 0x42,		//��ʼ����
    XFS_CMD_ENCODEDATA_SEND = 0x43,	//���ͱ������ݵ�оƬ
    XFS_CMD_EN_CODE_STOP = 0x44		//ֹͣ����/����
}XFS_CommandEnum;

//xfs5152ce�ı���������ö��
typedef enum 
{
	XFS_SAMPRATE_8K = 0,
	XFS_SAMPRATE_16K = 1
}XFS_SampRate;

//xfs5152�����ı����ʵȼ��������ʵȼ�Խ������Խ����
typedef enum 
{
	XFS_BAUD_LEVEL_0 = 0x00,
	XFS_BAUD_LEVEL_1 = 0x01,
	XFS_BAUD_LEVEL_2 = 0x02,
	XFS_BAUD_LEVEL_3 = 0x03,
	XFS_BAUD_LEVEL_4 = 0x04,
	XFS_BAUD_LEVEL_5 = 0x05,
}XFS_BaudRate;

//xfs5152�������������ȼ�
typedef enum 
{
	XFS_VOLUME_LEVEL_0 = 0x00,
	XFS_VOLUME_LEVEL_1 = 0x01,
	XFS_VOLUME_LEVEL_2 = 0x02,
	XFS_VOLUME_LEVEL_3 = 0x03,
	XFS_VOLUME_LEVEL_4 = 0x04,
	XFS_VOLUME_LEVEL_5 = 0x05,
	XFS_VOLUME_LEVEL_6 = 0x06,
	XFS_VOLUME_LEVEL_7 = 0x07,
	XFS_VOLUME_AUTO = 0x08,
}XFS_VolumeLevel;

//xfs5152��Ӧ��ʱ
#define XFS_RESPONSE_TIMEOUT 100

// ����ʶ������� 
const char *cmd_word_list[] = 
{
	"���ڳԷ�",     	//0
	"�����޳�",     	//1
	"���ڼ���",     	//2
	"������Ϣ",     	//3
	"ͬ��",         	//4
	"��ͬ��",         	//5
	"��ȥ",           	//6
	"���ڼ���",        	//7 
	"���켸��",        	//8 
	"����Ϣ",          	//9
	"��ʼ��",          	//10
	"�����Ķ�",        	//11 
	"�򿪹㲥",        	//12
	"�ص��㲥",        	//13
	"������",        	//14
	"�ص�����",        	//15
	"����һ��",        	//16
	"�ٶ�һ��",        	//17
	"������",          	//18
	"С����",          	//19
	"������",          	//20
	"��Ԥ����Ϣ",      	//21 
	"������������",    	//22 
	"����Ԥ����Ϣ",    	//23 
	"��ʼ",            	//24
	"ֹͣ",            	//25
	"��ͣ",            	//26
	"������",          	//27 
	"ȷ����ʼ",        	//28
	"ȡ��"             	//29
};

/**************************************************
*@brief		xfs5152�˿ڳ�ʼ��
*@param		None
*@retval	None
*@attenton
***************************************************/
void Xfs5152ce_Init(void)
{
    USART3_Init(115200);
}

/*******************************************************************
*@breef		�ȴ�XFS5152CE��Ӧ
*@param		uint8_t result���ȴ���Ӧ������ 
* 			uint16_t waittime ����ȴ�ʱ�䣬��λms
*@retval	1: success
*			0:
*@attention
*******************************************************************/
unsigned char Xfs_WaitResponse(uint8_t result, uint16_t waittime)
{
    while (--waittime)
	{
		delay_ms(1);
        if(xfs_receive_ok)
        {
            xfs_receive_ok = 0;
            if(result == xfs_state)
            {
                xfs_state = 0;
                return 1;
            }
        }
    }
	return 0;
}

/*********************************************************
*@brief		��������
*@param		cmd:Ҫ���͵�ָ����
*@retval	Xfs5152ce ����Ӧ
**********************************************************/
unsigned char Xfs_SendCmd(uint8_t cmd)
{
	unsigned  char frame_info[8]; //������ı�����
	uint16_t frame_len = 0;
	 
/*****************֡�̶�������Ϣ**************************************/           
	frame_info[frame_len++] = 0xFD ; 	//����֡ͷFD
	frame_info[frame_len++] = 0x00 ; 	//�������������ȵĸ��ֽ�
	frame_info[frame_len++] = 0x01; 	//�������������ȵĵ��ֽ�
	frame_info[frame_len++] = cmd ; 	//���������� 
 
/*******************����֡��Ϣ***************************************/
    
    USART3_SendStrlen(frame_info,frame_len);
	return Xfs_WaitResponse(XFS_ACK_CMD_RIGHT, XFS_RESPONSE_TIMEOUT);
}


/*******************************************************************
*@brief		���XFS5152CE�ĵ�ǰ״̬
*@param		None
*@retval	����XFS5152CE��״̬��
*@attention
*******************************************************************/
unsigned char Xfs_GetStatus(void)
{
	unsigned char rec_buf[8];
	unsigned  char frame_info[8]; //������ı�����
	uint16_t frame_len = 0;
	int timeout = XFS_RESPONSE_TIMEOUT;
	
	memset(rec_buf, 0, sizeof(rec_buf));
	
	Xfs_SendCmd(XFS_CMD_STATUS);		//���������� 
	
	while ((!xfs_receive_ok) && (timeout-- > 0))
		delay_ms(1);
    xfs_receive_ok = 0;
	return xfs_state;
}

/*******************************************************************
*@brief		ʹXFS5152CE����˯��״̬
*@param		None
*@retval	1:
*			0:
*@attention
*******************************************************************/
unsigned char Xfs_Sleep(void)
{
	return Xfs_SendCmd(XFS_CMD_SLEEP);
}

/*******************************************************************
*@brief		����XFS5152CE
*@param		None
*@retval	1:
*			0:
*@attention
*			���Ѻ���ȴ�20ms���ٽ��з�ָ�ʹ�乤��
*******************************************************************/
unsigned char Xfs_WakeUp(void)
{
	return Xfs_SendCmd(XFS_CMD_WAKE_UP);
}

/*******************************************************************
*@brief:	��ʼ��������
*@param:	unsigned char *data�� ��Ҫ���ŵ������ı�
*        	unsigned char format�������ı��ı����ʽ
*@retval	-1����������
*         	1�����Կ�ʼ����
*         	0����������ʧ��
*@attentino
*******************************************************************/
signed char Xfs_VoiceBroadcastStart(unsigned char *data, unsigned char format, int wait_finish)
{
   	unsigned  char *frame_info;
	unsigned  int  data_length, frame_offset=0;  
	int ack = 0;
	
	if(data == NULL)
		return -1;
	if(format > XFS_FORMAT_UNICODE)
		return -1;

	//if(wait_finish)
	//	while(xfs_get_status() != XFS_ACK_IDEL)
	//		delay_ms(10);
		
	data_length =strlen((char*)data); 			//��Ҫ�����ı��ĳ���
	frame_info = (unsigned char *)calloc((unsigned int)data_length, sizeof(unsigned char));
	if(frame_info == NULL)
		return -1;

/*****************֡�̶�������Ϣ**************************************/           
	frame_info[frame_offset++] = 0xFD ; 			//����֡ͷFD
	frame_info[frame_offset++] = (data_length>>8) & 0xFF; 			//�������������ȵĸ��ֽ�
	frame_info[frame_offset++] = (data_length & 0xFF) + 2; 		//�������������ȵĵ��ֽ�
	frame_info[frame_offset++] = XFS_CMD_VOICE_START ; 			//���������֣��ϳɲ�������		 		 
	frame_info[frame_offset++] = format;       //�ı������ʽ��GBK 
 
/*******************����֡��Ϣ***************************************/		  
	memcpy(&frame_info[5], data, data_length);
	frame_offset += data_length;
    USART3_SendStrlen(frame_info,frame_offset);
    //while(!F_USART3_RX_FINISH);
	ack = Xfs_WaitResponse(0x41,10);
	if(frame_info)
		free(frame_info);
	
	return ack; 
}

/*******************************************************************
*@brief		ֹͣ��������
*@param		None
*@retval	1��ֹͣ�ɹ�
*			0��ֹͣʧ��
*@attention
*******************************************************************/
unsigned char Xfs_VoiceBroadcastStop(void)
{
	return Xfs_SendCmd(XFS_CMD_VOICE_STOP);
}

/*******************************************************************
*@brief		��ͣ��������
*@param		None
*@retval	1����ͣ�ɹ���
*			0����ͣʧ��
*@attention
*******************************************************************/
unsigned char Xfs_VoiceBroadcastPause(void)
{
	return Xfs_SendCmd(XFS_CMD_VOICE_PAUSE);
}

/*******************************************************************
*@brief		������������
*@param		None
*@retval	1�������ɹ���
*			0������ʧ��
*@attention
*******************************************************************/
unsigned char Xfs_VoiceBroadcastRestore(void)
{
	return Xfs_SendCmd(XFS_CMD_VOICE_RESTORE);
}

/*******************************************************************
*@brief		�����ı���������
*@param		data���ı�����
*@param		cache_num (0~15)
*@retval	1���ɹ�
*@retval	0��ʧ��
*@attention	����ռ��ܹ�4K,����16��,ÿ����256�ֽ�
*			�豾����ʼ��������=X(0<=X<=15),�򱾴η��͵��ı����Ȳ��ܴ���(16-X)*256�ֽڣ�������ı�������
*			�û��ڲ��Ż����ı�ǰ���ɶ�η��ͻ���������мǺ������͵��ı����ܲ��ָ��ǻ�ȫ������֮ǰ���ı���
*			���򲻱�֤���ŵ���ȷ��
*******************************************************************/
signed char Xfs_SendTextToCache(unsigned char *data, unsigned char cache_num)
{
	unsigned char *frame_info = NULL; //������ı�����
	unsigned short data_length, frame_offset = 0;  
	unsigned char ack = 0;
	
	if(data == NULL)
		return -1;
	if(cache_num >= 16)
		return -1;
	data_length = strlen((char*)data); 			//��Ҫ�����ı��ĳ���
	if(data_length > (16-cache_num)*256)
		return -1;
	frame_info = (unsigned char *)calloc((unsigned int)data_length, sizeof(unsigned char));
	if(frame_info == NULL)
		return -1;
/*****************֡�̶�������Ϣ**************************************/           
	frame_info[frame_offset++] = 0xFD ; 			//����֡ͷFD
	frame_info[frame_offset++] = (data_length>>8) & 0xFF; 			//�������������ȵĸ��ֽ�
	frame_info[frame_offset++] = (data_length & 0xFF) + 2; 		//�������������ȵĵ��ֽ�
	frame_info[frame_offset++] = XFS_CMD_SEND_TO_CACHE ; 			//����������	 
	frame_info[frame_offset++] = cache_num;       //�ı������ʽ��GBK 
 
/*******************����֡��Ϣ***************************************/		  
	memcpy(&frame_info[frame_offset], data, data_length);
	frame_offset += data_length;
	USART3_SendStrlen(frame_info,frame_offset);//����֡����
	
	ack = Xfs_WaitResponse(XFS_ACK_CMD_RIGHT, XFS_RESPONSE_TIMEOUT);
	
	if(frame_info)
		free(frame_info);
	return ack;

}

/*******************************************************************
*@brief		���Ż�������
*@param		num(1~15)
*			format:0-GB2312,1-GBK,2-BIG,3-UNICODE
*@retval	1���ɹ�
*			0��ʧ��
*@attention
*******************************************************************/
unsigned char Xfs_PalyFromCache(unsigned char num, unsigned char format)
{
	unsigned  char frame_info[8];
	unsigned short frame_len = 0;
	
	if(format > XFS_FORMAT_UNICODE || num > 15)
	return 0;

/*****************֡�̶�������Ϣ**************************************/           
	frame_info[frame_len++] = 0xFD ; 		//����֡ͷFD
	frame_info[frame_len++] = 0x00 ; 		//�������������ȵĸ��ֽ�
	frame_info[frame_len++] = 0x02; 		//�������������ȵĵ��ֽ�
	frame_info[frame_len++] = XFS_CMD_PLAY_FROM_CACHE ; 			//���������� 
 	frame_info[frame_len++] = (num << 4 & 0xF0) | (format & 0x0F); 			//����������λ-�ظ����Ŵ���������λ-�ı������ʽ

/*******************����֡��Ϣ***************************************/
	USART3_SendStrlen(frame_info, frame_len);
    
	return Xfs_WaitResponse(XFS_ACK_CMD_RIGHT, XFS_RESPONSE_TIMEOUT);

}

/*******************************************************************
*@brief		����ʶ��
*@param		result[out]: ʶ����
*			waittime[in]: ��ʱʱ��
*@retval	>0���ɹ�
*			0��ʧ��
*******************************************************************/
unsigned char Xfs_VoiceRecognize(unsigned char *result, signed int waittime)
{
	int err = 1;
	unsigned char n = 0;
	if(result == NULL)
	{
		err = 0;
		return err;
	}
    beep(100);
	printf("recognize start...\r\n");
	while(Xfs_SendCmd(XFS_CMD_RECOGNIZE_START) == 0)
	{
		err = 0;
		printf("Error: XFS_CMD_RECOGNIZE_START,%d\r\n",++n);
		Xfs_SendCmd(XFS_CMD_RECOGNIZE_STOP);
        delay_ms(100);
	}
	
	while(--waittime)
	{
		delay_ms(1);
		if(xfs_receive_ok)
        {
            xfs_receive_ok = 0;
			break;
        }
	}
	
//	while(xfs_receive_ok != 0)
//	{
//		if(waittime--)
//		{
//			
//		}
//	}
//	xfs_receive_ok = 0;
	
	printf("xfs_data_buf:");
	printf_hex(xfs_data_buf,20);
	if(waittime > 0)
	{
		switch (xfs_data_buf[3])
		{
			case 0x01:
				err = 0x01;
				*result = xfs_data_buf[4];
				printf("the result:%s\r\n",cmd_word_list[*result]);
				printf("���������%d\r\n",xfs_data_buf[4]);
				break;
			case 0x02:
				err = 0x02;
				printf("Error: Time out\r\n");
				break;
			case 0x03:
				err = 0x03;
				printf("Error: Refuse\r\n");
				break;
			case 0x04:
				err = 0x04;
				printf("Error: Internal error\r\n");
				break;					
			default:
				err = 0;
				printf("Error: Unknow cmd\r\n");
				break;
		}
	}
	if(waittime == 0)
    {
		err = 0;
        printf("Error: Please speek the words in the time\r\n");
    }
	
	//stop receive
	Xfs_SendCmd(XFS_CMD_RECOGNIZE_STOP);
	return err;
}

/*******************************************************************
*@brief		��ʼ��������
*@param		samp: ������
*			baud: ������
*           volum������
*@retval	>0���ɹ�
*			0��ʧ��
*			<0����������
*******************************************************************/
signed char Xfs_VoiceEncodeStart(unsigned char samp,unsigned char baud,unsigned char volum)
{
    unsigned  char frame_info[7];
	unsigned short frame_len = 0;
    unsigned int data_cnt = 0;
    unsigned char datlen = 0;
    if(samp > 1)
        return -1;
    if(baud > 5)
        return -1;
    if(volum > 8)
        return -1;
/*****************֡�̶�������Ϣ**************************************/           
	frame_info[frame_len++] = 0xFD ; 			//����֡ͷFD
	frame_info[frame_len++] = 0x00 ; 			//�������������ȵĸ��ֽ�
	frame_info[frame_len++] = 0x04; 		//�������������ȵĵ��ֽ�
    frame_info[frame_len++] = XFS_CMD_ENCODE_START ;      //����
    
 	frame_info[frame_len++] = samp;             //����1:���ò�����
    frame_info[frame_len++] = baud;             //����2:���ñ�����
    frame_info[frame_len++] = volum;            //����3:��������

/*******************����֡��Ϣ***************************************/
	USART3_SendStrlen(frame_info, frame_len);
    if(Xfs_WaitResponse(XFS_ACK_CMD_RIGHT, XFS_RESPONSE_TIMEOUT))
    {
        
        for(data_cnt = 0; data_cnt < 29000;data_cnt+=datlen)
        {
            while(!xfs_receive_ok);
            xfs_receive_ok = 0;
            if(xfs_data_buf[0] == 0xFC && xfs_data_buf[3] == 0x20){
                datlen = (((int)xfs_data_buf[1] << 8) | (xfs_data_buf[2])) + 4;
                memcpy(&xfs_encode_buf[data_cnt],xfs_data_buf,datlen);
            }
        }
        Xfs_VoiceCODECStop();
        if(!(xfs_data_buf[0] == 0xFC && xfs_data_buf[3] == 0x22))
            return 0;
        
        xfs_timer_flag1 = 0;
        xfs_timer_flag2 = 0;
        xfs_timeout_cnt = 0;
        xfs_protocol_state = 0;
        xfs_protocol_dat_len = 0;
        xfs_protocol_dat_flag = 0;
        delay_ms(1000);
        printf("Code Start...\r\n");
        Xfs_VoiceDecodeStart(0,3,7,xfs_encode_buf);
        return 1;
    }
	else
        return 0;
        
}

/*******************************************************************
*@brief		�������봫��
*@param		*dat��������ͷ�ı�������
*@retval	>0���ɹ�
*			0��ʧ��
*******************************************************************/
signed char Xfs_VoiceCodeTransmit(u8 *dat)
{
    u8 datlen = 0;
    unsigned int cnt = 0;
    dat[cnt] = 0xFD;
    dat[cnt+3] = 0x43;
    datlen = dat[cnt + 1] << 8 | dat[cnt + 2] + 4;
    USART3_SendStrlen(&dat[cnt],datlen);
    for(cnt = datlen;cnt < 29000;cnt += datlen){
    
        while(xfs_receive_ok == 0)
        {
            
        }
        xfs_receive_ok = 0;
        if(xfs_data_buf[0] == 0xFC && xfs_data_buf[3] == 0x23)
        {  
            if(!Xfs_WaitResponse(XFS_ACK_CMD_RIGHT, XFS_RESPONSE_TIMEOUT))
            {
                Xfs_VoiceCODECStop();
                return 0;
            }

            while(!(dat[cnt] == 0xFC && dat[cnt+3] == 0x20))
            {
                cnt ++;
                if(cnt > 29000)
                {
                    Xfs_VoiceCODECStop();
                    return 0;
                }
            }
            dat[cnt] = 0xFD;
            dat[cnt+3] = 0x43;
            datlen = dat[cnt + 1] << 8 | dat[cnt + 2] + 4;
            USART3_SendStrlen(&dat[cnt],datlen);
        }
    }
	return 1; 
}

/*******************************************************************
*@brief		��ʼ�������룬��ϱ��봫�亯��ʹ��
*@param		samp: ������
*           baud: ������
*           volum������
*           *data����������
*@retval	>0���ɹ�
*			0��ʧ��
*			<0����������
*******************************************************************/
signed char Xfs_VoiceDecodeStart(unsigned char samp,unsigned char baud,unsigned char volum,u8 *data)
{
    unsigned  char frame_info[7];
	unsigned short frame_len = 0;

    if(samp > 1)
        return -1;
    if(baud > 5)
        return -1;
    if(volum > 8)
        return -1;
/*****************֡�̶�������Ϣ**************************************/           
	frame_info[frame_len++] = 0xFD ; 			//����֡ͷFD
	frame_info[frame_len++] = 0x00 ; 			//�������������ȵĸ��ֽ�
	frame_info[frame_len++] = 0x04; 		//�������������ȵĵ��ֽ�
    frame_info[frame_len++] = 0x42 ;      //����
    
 	frame_info[frame_len++] = samp;             //����1:���ò�����
    frame_info[frame_len++] = baud;             //����2:���ñ�����
    frame_info[frame_len++] = volum;            //����3:��������

/*******************����֡��Ϣ***************************************/
	USART3_SendStrlen(frame_info, frame_len);
    if(!Xfs_WaitResponse(XFS_ACK_CMD_RIGHT, XFS_RESPONSE_TIMEOUT))
    {
        Xfs_VoiceCODECStop();
        return 0;
    }
    memset(xfs_data_buf,0x00,80);
    delay_ms(20);
    if(Xfs_VoiceCodeTransmit(data))
    {
        Xfs_VoiceCODECStop();
        printf("Code End...\r\n");
        return 1;
    }
    else{
        printf("Code End...\r\n");
        return 0;
    }
}

/*******************************************************************
*@brief		ֹͣ�����
*@param		None
*@retval	>0���ɹ�
*			0��ʧ��
*@attention
*******************************************************************/
unsigned char Xfs_VoiceCODECStop(void)
{
    unsigned  char frame_info[8]; //������ı�����
	uint16_t frame_len = 0;
	 
/*****************֡�̶�������Ϣ**************************************/           
	frame_info[frame_len++] = 0xFD ; 			//����֡ͷFD
	frame_info[frame_len++] = 0x00 ; 			//�������������ȵĸ��ֽ�
	frame_info[frame_len++] = 0x01; 		//�������������ȵĵ��ֽ�
	frame_info[frame_len++] = XFS_CMD_EN_CODE_STOP ; 			//���������� 
 
/*******************����֡��Ϣ***************************************/
    
    USART3_SendStrlen(frame_info,frame_len);
    if(Xfs_WaitResponse(XFS_ACK_CMD_RIGHT, XFS_RESPONSE_TIMEOUT))
        return 1;
    else
        return 0;
}

/*******************************************************************
*@brief 	���XFS5152CE�����ݽ���
*@param		None
*@retval	None
*******************************************************************/
void Xfs_AckReceive(uint8_t dat)
{
	if(dat == 0xFC && xfs_protocol_dat_flag == 0)
	{
		xfs_timeout_cnt = 0;
		xfs_protocol_dat_cnt = 0;	// �������ݼ���
		xfs_data_buf[xfs_protocol_dat_cnt++] = dat;
		xfs_timer_flag1 = 1;
		xfs_protocol_dat_flag =  1;		
		xfs_receive_ok = 0;
		xfs_protocol_state = 0;
		xfs_protocol_dat_len = 0;	// ���ݳ���
    }
	else if(xfs_protocol_dat_flag == 0)
	{
        switch(dat)
        {
            case XFS_ACK_INIT_SUCCESS:
            case XFS_ACK_CMD_RIGHT:
            case XFS_ACK_CMD_ERROR:
            case XFS_ACK_BUSY:
            case XFS_ACK_IDEL:
                xfs_state = dat;
                xfs_receive_ok = 1;
            break;
        }
    }
	else if(xfs_protocol_dat_flag == 1)
    {
        if(xfs_protocol_dat_flag == 1)
        {
            switch(xfs_protocol_state)
            {
                case 0:
                    xfs_protocol_dat_len = (u16)dat << 8;
                    xfs_data_buf[xfs_protocol_dat_cnt++] = dat;
                    xfs_protocol_state = 1;
                    break;
                case 1:
                    xfs_protocol_dat_len |= dat;
                    if(xfs_protocol_dat_len == 1)
                        xfs_protocol_dat_len = 0;
                    xfs_data_buf[xfs_protocol_dat_cnt++] = dat;
                    xfs_protocol_state = 2;
                    if(xfs_protocol_dat_len > 42)
                        printf("datas is too long,%d\r\n",xfs_protocol_dat_len);
                    break;
                case 2:
                    if(xfs_protocol_dat_cnt <=  xfs_protocol_dat_len + 3)
                    {
                        xfs_data_buf[xfs_protocol_dat_cnt++] = dat;
                    }
                    if(xfs_protocol_dat_len > 2)
                        xfs_timer_flag2 = 1;
                    else{
                        xfs_timer_flag1 = 0;
                    }
                    break;
                default:
                    break;
            }
            if((xfs_protocol_state == 2)&& (xfs_protocol_dat_cnt == xfs_protocol_dat_len + 4))
            {
                xfs_receive_ok = 1;
                xfs_timer_flag1 = 0;
                xfs_timer_flag2 = 0;
                xfs_timeout_cnt = 0;
                xfs_protocol_state = 0;
                xfs_protocol_dat_len = 0;
                xfs_protocol_dat_flag = 0;
            }
        }
    }
}


/*******************************************************************
*@brief		��ֹ�����жϵ���ʱ����
*@param		None
*@retval	None
*******************************************************************/
void Xfs_ReceiveTimeLimt(void)
{
    if(xfs_timer_flag1 || xfs_timer_flag2)//���ڽ��մ�������
    {
        if(xfs_timer_flag1)
            xfs_time_uplimit = 10;
        if(xfs_timer_flag2)
            xfs_time_uplimit = 30;
        xfs_timeout_cnt++;//���ڳ�ʱ����
        if(xfs_timeout_cnt > xfs_time_uplimit)//���ݽ��ռ������0.3ms
        {
			//���ڽ�����ɻ����
            xfs_timer_flag1 = xfs_timer_flag2 = 0;
            xfs_timeout_cnt = 0;
            xfs_receive_ok = 1;
            xfs_protocol_dat_flag = 0;
        }
    }   
}




void xfs_recognize_test(void)
{  
	uint8_t result=30;
	if(Xfs_VoiceRecognize(&result, 5000) != 1)
	{
		DEBUG("recognition failed!");
		Xfs_VoiceBroadcastStart((unsigned char *)"����ʶ��ʧ��", XFS_FORMAT_GBK, 1);
		while(Xfs_GetStatus() != XFS_ACK_IDEL);
		return;
	}
	
	printf("get :%s\r\n",cmd_word_list[result]);
	Xfs_VoiceBroadcastStart((unsigned char *)cmd_word_list[result], XFS_FORMAT_GBK, 1);
	while(Xfs_GetStatus() != XFS_ACK_IDEL);
	

}




