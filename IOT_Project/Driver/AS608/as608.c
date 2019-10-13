


#include "as608.h"

const uint8_t HeadPackage[]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x00};
//Ӧ�����ͷ
const uint8_t HeadResponse[]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x07,0x00};
//���ݰ���ͷ
const uint8_t HeadData[]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x02,0x00};
//��������ͷ
const uint8_t HeadDataEnd[]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x08,0x00};

SysPara AS_SysPara;	//ָ��ģ��AS608����

u16 ValidN=0; // //ģ������Чָ�Ƹ���

//��ʼ��PA6Ϊ��������		    
//��������Ӧ״̬(������Ӧʱ����ߵ�ƽ�ź�)
void AS608_StaGPIO_Init(void)
{   
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	//��ʼ����״̬����GPIOA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//��������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO	
}

////���ڷ���һ���ֽ�
//static void MYUSART_SendData(u8 data)
//{
//	while((USART2->SR&0X40)==0); 
//	USART2->DR = data;
//}

//���Ͱ�ͷ
void SendHead(void)
{
	MYUSART_SendData(0xEF);
	MYUSART_SendData(0x01);
}
//���͵�ַ
void SendAddr(void)
{
	MYUSART_SendData((u8)(AS608_DEFAULTADDR>>24));
	MYUSART_SendData((u8)(AS608_DEFAULTADDR>>16));
	MYUSART_SendData((u8)(AS608_DEFAULTADDR>>8));
	MYUSART_SendData((u8)(AS608_DEFAULTADDR));
}
//���Ͱ���ʶ
void SendFlag(u8 flag)
{
	MYUSART_SendData(flag);
}
//���Ͱ�����
void SendLength(int length)
{
	MYUSART_SendData(length>>8);
	MYUSART_SendData(length);
}
//����ָ����
void Sendcmd(u8 cmd)
{
	MYUSART_SendData(cmd);
}
//����У���
void SendCheck(u16 check)
{
	MYUSART_SendData(check>>8);
	MYUSART_SendData(check);
}

//�ж��жϽ��յ�������û��Ӧ���
//waittimeΪ�ȴ��жϽ������ݵ�ʱ�䣨��λ1ms��
//����ֵ�����ݰ��׵�ַ
u8 *Recive_response(u16 waittime)
{
	char *data;
	uint16_t len;
	USART2_RX_STA=0;
	while(--waittime)
	{
		delay_ms(1);
		if(USART2_RX_STA&0X8000)//���յ�һ������
		{
			len = USART2_RX_STA & 0x7FFF;
			printf_hex(USART2_RX_BUF,len);
			data=strstr((const char*)USART2_RX_BUF,(const char*)HeadResponse);
			if(data)
				return (u8*)data;	
		}
	}
	return 0;
}

//�������ݰ�
void Recive_Data(void)
{
	uint8_t *temp;
	USART2_RX_STA=0;
	while(1)
	{
		delay_ms(1);
//		USART2_RX_STA=0;
		temp = (uint8_t *)strstr((const char*)USART2_RX_BUF,(const char*)HeadData);
		if(strstr((const char*)USART2_RX_BUF,(const char*)HeadDataEnd))
		{
			break;			
		}
	}
	printf("Data:\r\n");
	printf("%s\r\n",temp);
}

//¼��ͼ�� AS608_GetImage
//����:̽����ָ��̽�⵽��¼��ָ��ͼ�����ImageBuffer�� 
//ģ�鷵��ȷ����
u8 AS608_GetImage(void)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(GetImage);
	temp =  0x01+0x03+GetImage;
	SendCheck(temp);
	data=Recive_response(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}

//�������� AS608_GenChar
//����:��ImageBuffer�е�ԭʼͼ������ָ�������ļ�����CharBuffer1��CharBuffer2			 
//����:BufferID --> charBuffer1:0x01	charBuffer1:0x02												
//ģ�鷵��ȷ����
u8 AS608_GenChar(u8 BufferID)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x04);
	Sendcmd(GenChar);
	MYUSART_SendData(BufferID);
	temp = 0x01+0x04+GenChar+BufferID;
	SendCheck(temp);
	data=Recive_response(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}

//��ȷ�ȶ���öָ������ AS608_Match
//����:��ȷ�ȶ�CharBuffer1 ��CharBuffer2 �е������ļ� 
//ģ�鷵��ȷ����
u8 AS608_Match(void)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(Match);
	temp = 0x01+0x03+Match;
	SendCheck(temp);
	data=Recive_response(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//����ָ�� AS608_Search
//����:��CharBuffer1��CharBuffer2�е������ļ����������򲿷�ָ�ƿ�.�����������򷵻�ҳ�롣			
//����:  BufferID @ref CharBuffer1	CharBuffer2
//˵��:  ģ�鷵��ȷ���֣�ҳ�루����ָ��ģ�壩
u8 AS608_Search(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x08);
	Sendcmd(Search);	//ָ����
	MYUSART_SendData(BufferID);
	MYUSART_SendData(StartPage>>8);
	MYUSART_SendData(StartPage);
	MYUSART_SendData(PageNum>>8);
	MYUSART_SendData(PageNum);
	temp = 0x01+0x08+Search+BufferID
	+(StartPage>>8)+(u8)StartPage
	+(PageNum>>8)+(u8)PageNum;
	SendCheck(temp);
	data=Recive_response(2000);
	if(data)
	{
		ensure = data[9];
		p->pageID   =(data[10]<<8)+data[11];
		p->mathscore=(data[12]<<8)+data[13];	
	}
	else
		ensure = 0xff;
	return ensure;	
}
//�ϲ�����������ģ�壩AS608_RegModel
//����:��CharBuffer1��CharBuffer2�е������ļ��ϲ����� ģ��,�������CharBuffer1��CharBuffer2	
//˵��:  ģ�鷵��ȷ����
u8 AS608_RegModel(void)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(RegModel);
	temp = 0x01+0x03+RegModel;
	SendCheck(temp);
	data=Recive_response(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;		
}
//����ģ�� AS608_StoreChar
//����:�� CharBuffer1 �� CharBuffer2 �е�ģ���ļ��浽 PageID ��flash���ݿ�λ�á�			
//����:  BufferID @ref charBuffer1:0x01	charBuffer1:0x02
//       PageID��ָ�ƿ�λ�úţ�
//˵��:  ģ�鷵��ȷ����
u8 AS608_StoreChar(u8 BufferID,u16 PageID)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x06);
	Sendcmd(StoreChar);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(PageID>>8);
	MYUSART_SendData(PageID);
	temp = 0x01+0x06+StoreChar+BufferID
	+(PageID>>8)+(u8)PageID;
	SendCheck(temp);
	data=Recive_response(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;	
}
//����˵���� �� flash ���ݿ���ָ�� ID �ŵ�ָ��ģ����뵽ģ�建���� CharBuffer1 �� CharBuffer2
//��������� BufferID(��������)��PageID(ָ�ƿ�ģ���)
// ���ز����� ȷ����
uint8_t AS608_LoadChar(uint8_t BufferID,uint16_t PageID)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x06);
	Sendcmd(LoadChar);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(PageID>>8);
	MYUSART_SendData(PageID);
	temp = 0x01+0x06+LoadChar+BufferID
	+(PageID>>8)+(u8)PageID;
	SendCheck(temp);
	data=Recive_response(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;	
}

//�ϴ�������ģ�� PS_UpChar
//����˵���� �������������е������ļ��ϴ�����λ��
//��������� BufferID(��������)
//���ز����� ȷ����
//Ӧ��֮���ͺ������ݰ�
uint8_t AS608_UpChar(uint8_t BufferID)
{
	u16 temp;
	u8 ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x04);
	Sendcmd(UpChar);
	MYUSART_SendData(BufferID);
	temp = 0x01+0x04+UpChar+BufferID;
	SendCheck(temp);
	data = Recive_response(2000);
	if(data)
		ensure = data[9];
	else
		ensure=0xff;
	
	if(ensure==0)
	{
		
	}
	return ensure;	
}


//����������ģ�� PS_DownChar
//����˵���� ��λ�����������ļ���ģ���һ������������
//��������� BufferID(��������)
//���ز����� ȷ����
uint8_t AS608_DownChar(uint8_t BufferID)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x04);
	Sendcmd(DownChar);
	MYUSART_SendData(BufferID);
	temp = 0x01+0x04+DownChar+BufferID;
	SendCheck(temp);
	data=Recive_response(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;	
}

//�ϴ�ͼ�� PS_UpImage
//����˵���� ��ͼ�񻺳����е������ϴ�����λ��
//��������� none
//���ز����� ȷ����
uint8_t AS608_UpImage(void)
{
	u8  ensure;
	
	return ensure;	
}

//����ͼ�� PS_DownImage
// ����˵���� ��λ������ͼ�����ݸ�ģ��
// ��������� none
// ���ز����� ȷ����
uint8_t AS608_DownImage(void)
{
	return 0;
}


//ɾ��ģ�� AS608_DeletChar
//����:  ɾ��flash���ݿ���ָ��ID�ſ�ʼ��N��ָ��ģ��
//����:  PageID(ָ�ƿ�ģ���)��Nɾ����ģ�������
//˵��:  ģ�鷵��ȷ����
u8 AS608_DeletChar(u16 PageID,u16 N)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x07);
	Sendcmd(DeletChar);
	MYUSART_SendData(PageID>>8);
	MYUSART_SendData(PageID);
	MYUSART_SendData(N>>8);
	MYUSART_SendData(N);
	temp = 0x01+0x07+DeletChar
	+(PageID>>8)+(u8)PageID
	+(N>>8)+(u8)N;
	SendCheck(temp);
	data=Recive_response(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//���ָ�ƿ� AS608_Empty
//����:  ɾ��flash���ݿ�������ָ��ģ��
//����:  ��
//˵��:  ģ�鷵��ȷ����
u8 AS608_Empty(void)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(Empty);
	temp = 0x01+0x03+Empty;
	SendCheck(temp);
	data=Recive_response(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
/*********************************************************
*@brief		дϵͳ�Ĵ��� AS608_WriteReg
*	   		дģ��Ĵ���
*@param		RegNum:�Ĵ������4\5\6
*	@arg	4: �����ʿ��ƼĴ���
*	@arg	5: �ȶ���ֵ�Ĵ���
*	@arg	6: ����С�Ĵ���
*@param		DATA: д�������
*	@arg	RegNum == 4��9600�ı���
*	@arg	RegNum == 5, 1:level1,2:level2,**,5:level5
*	@arg	RegNum == 6, 0:32bytes,1:64bytes,2:128bytes,3:256bytes
*@return	ģ�鷵��ȷ����
**********************************************************/
u8 AS608_WriteReg(u8 RegNum,u8 DATA)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x05);
	Sendcmd(WriteReg);
	MYUSART_SendData(RegNum);
	MYUSART_SendData(DATA);
	temp = RegNum+DATA+0x01+0x05+WriteReg;
	SendCheck(temp);
	data=Recive_response(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	if(ensure==0)
		printf("\r\n���ò����ɹ���");
	else
		printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}

/*****************************************************
*@brief		��ϵͳ�������� AS608_ReadSysPara
*			��ȡģ��Ļ��������������ʣ�����С��)
*@parm  	SysPara *p:������ϵͳ��������
*@return	ģ�鷵��ȷ���� + ����������16bytes��
******************************************************/
u8 AS608_ReadSysPara(SysPara *param)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(ReadSysPara);
	temp = 0x01+0x03+ReadSysPara;
	SendCheck(temp);
	data=Recive_response(1000);
	if(data)
	{
		ensure = data[9];
		param->AS608_max = (data[14]<<8)+data[15];
		param->AS608_level = data[17];
		param->AS608_addr=(data[18]<<24)+(data[19]<<16)+(data[20]<<8)+data[21];
		param->AS608_size = data[23];
		param->AS608_N = data[25];
	}		
	else
		ensure=0xff;
	if(ensure==0x00)
	{
		printf("ģ�����ָ������=%d\r\n",param->AS608_max);
		printf("�Աȵȼ�=%d\r\n",param->AS608_level);
		printf("��ַ=%#x\r\n",param->AS608_addr);
		printf("������=%d\r\n",param->AS608_N*9600);
	}
	else 
			printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}

//PS_VfyPwd
//ָ����룺13H
//���ܣ���֤�豸���ֿ���
u8 AS608_VfyPwd(void)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x07);
	Sendcmd(VfyPwd);
	MYUSART_SendData(0x00);
	MYUSART_SendData(0x00);
	MYUSART_SendData(0x00);
	MYUSART_SendData(0x00);
	temp = 0x01+0x07+VfyPwd;
	SendCheck(temp);
	data=Recive_response(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;	
	if(ensure==0x00)
		printf("\r\n���ֿ�����֤�ɹ���");
	else
		printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}

//����ģ���ַ AS608_SetAddr
//����:  ����ģ���ַ
//����:  AS608_addr
//˵��:  ģ�鷵��ȷ����
u8 AS608_SetChipAddr(u32 AS608_addr)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x07);
	Sendcmd(SetChipAddr);
	MYUSART_SendData(AS608_addr>>24);
	MYUSART_SendData(AS608_addr>>16);
	MYUSART_SendData(AS608_addr>>8);
	MYUSART_SendData(AS608_addr);
	temp = 0x01+0x07+SetChipAddr
	+(u8)(AS608_addr>>24)+(u8)(AS608_addr>>16)
	+(u8)(AS608_addr>>8) +(u8)AS608_addr;				
	SendCheck(temp);
	AS_SysPara.AS608_addr =AS608_addr;//������ָ�������ַ
	data=Recive_response(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;	
		AS_SysPara.AS608_addr = AS608_addr;
	if(ensure==0x00)
		printf("\r\n���õ�ַ�ɹ���");
	else
		printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}


//���ܣ� ģ���ڲ�Ϊ�û�������256bytes��FLASH�ռ����ڴ��û����±�,
//	�ü��±��߼��ϱ��ֳ� 16 ��ҳ��
//����:  NotePageNum(0~15),Byte32(Ҫд�����ݣ�32���ֽ�)
//˵��:  ģ�鷵��ȷ����
u8 AS608_WriteNotepad(u8 NotePageNum,u8 *Byte32)
{
	u16 temp;
	u8  ensure,i;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(36);
	Sendcmd(WriteNotepad);
	MYUSART_SendData(NotePageNum);
	for(i=0;i<32;i++)
	 {
		 MYUSART_SendData(Byte32[i]);
		 temp += Byte32[i];
	 }
	temp =0x01+36+WriteNotepad+NotePageNum+temp;
	SendCheck(temp);
	data=Recive_response(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//������AS608_ReadNotepad
//���ܣ�  ��ȡFLASH�û�����128bytes����
//����:  NotePageNum(0~15)
//˵��:  ģ�鷵��ȷ����+�û���Ϣ
u8 AS608_ReadNotepad(u8 NotePageNum,u8 *Byte32)
{
	u16 temp;
	u8  ensure,i;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x04);
	Sendcmd(ReadNotepad);
	MYUSART_SendData(NotePageNum);
	temp = 0x01+0x04+ReadNotepad+NotePageNum;
	SendCheck(temp);
	data=Recive_response(2000);
	if(data)
	{
		ensure=data[9];
		for(i=0;i<32;i++)
		{
			Byte32[i]=data[10+i];
		}
	}
	else
		ensure=0xff;
	return ensure;
}
//��������AS608_HighSpeedSearch
//���ܣ��� CharBuffer1��CharBuffer2�е������ļ��������������򲿷�ָ�ƿ⡣
//		  �����������򷵻�ҳ��,��ָ����ڵ�ȷ������ָ�ƿ��� ���ҵ�¼ʱ����
//		  �ܺõ�ָ�ƣ���ܿ�������������
//����:  BufferID�� StartPage(��ʼҳ)��PageNum��ҳ����
//˵��:  ģ�鷵��ȷ����+ҳ�루����ָ��ģ�壩
u8 AS608_HighSpeedSearch(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x08);
	Sendcmd(HighSpeedSearch);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(StartPage>>8);
	MYUSART_SendData(StartPage);
	MYUSART_SendData(PageNum>>8);
	MYUSART_SendData(PageNum);
	temp = 0x01+0x08+HighSpeedSearch+BufferID
	+(StartPage>>8)+(u8)StartPage
	+(PageNum>>8)+(u8)PageNum;
	SendCheck(temp);
	data=Recive_response(2000);
 	if(data)
	{
		ensure=data[9];
		p->pageID 	=(data[10]<<8) +data[11];
		p->mathscore=(data[12]<<8) +data[13];
	}
	else
		ensure=0xff;
	return ensure;
}
//����Чģ����� AS608_ValidTempleteNum
//���ܣ�����Чģ�����
//����: ��
//˵��: ģ�鷵��ȷ����+��Чģ�����ValidN
u8 AS608_ValidTempleteNum(u16 *ValidN)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(ValidTempleteNum);
	temp = 0x01+0x03+ValidTempleteNum;
	SendCheck(temp);
	data=Recive_response(2000);
	if(data)
	{
		ensure=data[9];
		*ValidN = (data[10]<<8) +data[11];
	}		
	else
		ensure=0xff;
	
	if(ensure==0x00)
	{
		printf("\r\n��Чָ�Ƹ���=%d\r\n",(data[10]<<8)+data[11]);
	}
	else
		printf("%s\r\n",EnsureMessage(ensure));
	return ensure;
}

//ģ��Ӧ���ȷ������Ϣ����
//���ܣ�����ȷ���������Ϣ������Ϣ
//����: ensure
const char *EnsureMessage(u8 ensure) 
{
	const char *p;
	switch(ensure)
	{
		case  0x00:
			p="OK";break;		
		case  0x01:
			p="���ݰ����մ���";break;
		case  0x02:
			p="��������û����ָ";break;
		case  0x03:
			p="¼��ָ��ͼ��ʧ��";break;
		case  0x04:
			p="ָ��ͼ��̫�ɡ�̫��������������";break;
		case  0x05:
			p="ָ��ͼ��̫ʪ��̫��������������";break;
		case  0x06:
			p="ָ��ͼ��̫�Ҷ�����������";break;
		case  0x07:
			p="ָ��ͼ����������������̫�٣������̫С��������������";break;
		case  0x08:
			p="ָ�Ʋ�ƥ��";break;
		case  0x09:
			p="û������ָ��";break;
		case  0x0a:
			p="�����ϲ�ʧ��";break;
		case  0x0b:
			p="����ָ�ƿ�ʱ��ַ��ų���ָ�ƿⷶΧ";
		case  0x10:
			p="ɾ��ģ��ʧ��";break;
		case  0x11:
			p="���ָ�ƿ�ʧ��";break;	
		case  0x15:
			p="��������û����Чԭʼͼ��������ͼ��";break;
		case  0x18:
			p="��д FLASH ����";break;
		case  0x19:
			p="δ�������";break;
		case  0x1a:
			p="��Ч�Ĵ�����";break;
		case  0x1b:
			p="�Ĵ����趨���ݴ���";break;
		case  0x1c:
			p="���±�ҳ��ָ������";break;
		case  0x1f:
			p="ָ�ƿ���";break;
		case  0x20:
			p="��ַ����";break;
		default :
			p="ģ�鷵��ȷ��������";break;
	}
 return p;	
}





