
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define KBD_HOLD_KEY_SCAN_ON


#define KEY_EVENT_BUF_SIZE		20

#define KBD_ROW_SUM			5	/* number of row */
#define KBD_COLUMN_SUM		5	/* number of column */
#define KBD_SAMPLE_COUNT	2	/* for debounce */
#define KBD_SAMPLE_COUNT_FOR_HOLD_KEY	50	/* for debounce */

//#define RCC_KBD_ROWS_PORT	RCC_APB2Periph_GPIOA
//#define KBD_ROWS_PORT		GPIOA
//#define KBD_ROWS_PIN	(KBD_ROW0_PIN | KBD_ROW1_PIN | KBD_ROW2_PIN | KBD_ROW3_PIN | KBD_ROW4_PIN)	

/*
*	ROW0��PA6	ROW1��PA7	ROW2��PC4	ROW3��PC5	ROW4��PB0 					
*/
#define RCC_KBD_ROW0_PORT	RCC_APB2Periph_GPIOA	// ��0�ж˿�ʱ��
#define KBD_ROW0_PORT		GPIOA                   // ��0�ж˿�
#define KBD_ROW0_PIN		GPIO_Pin_6              // ��0�ж˿�����

#define RCC_KBD_ROW1_PORT	RCC_APB2Periph_GPIOA	// ��1�ж˿�ʱ��
#define KBD_ROW1_PORT		GPIOA                   // ��1�ж˿�
#define KBD_ROW1_PIN		GPIO_Pin_7              // ��1�ж˿�����

#define RCC_KBD_ROW2_PORT	RCC_APB2Periph_GPIOC	// ��2�ж˿�ʱ��
#define KBD_ROW2_PORT		GPIOC                   // ��2�ж˿�
#define KBD_ROW2_PIN		GPIO_Pin_4              // ��2�ж˿�����

#define RCC_KBD_ROW3_PORT	RCC_APB2Periph_GPIOC	// ��3�ж˿�ʱ��
#define KBD_ROW3_PORT		GPIOC                   // ��3�ж˿�
#define KBD_ROW3_PIN		GPIO_Pin_5              // ��3�ж˿�����

#define RCC_KBD_ROW4_PORT	RCC_APB2Periph_GPIOB	// ��4�ж˿�ʱ��
#define KBD_ROW4_PORT		GPIOB                   // ��4�ж˿�
#define KBD_ROW4_PIN		GPIO_Pin_0              // ��4�ж˿�����

//#define RCC_KBD_COLUMNS_PORT	RCC_APB2Periph_GPIOB
//#define KBD_COLUMNS_PORT		GPIOB
//#define KBD_COLUMNS_PIN		(KBD_COLUMN0_PIN | KBD_COLUMN1_PIN | KBD_COLUMN2_PIN | KBD_COLUMN3_PIN | KBD_COLUMN4_PIN)

/*
*	COLUMN0��PB10	COLUMN1��PB11	COLUMN2��PB12	COLUMN3��PB13	COLUMN4��PB14 					
*/
#define RCC_KBD_COLUMN0_PORT	RCC_APB2Periph_GPIOB	// ��0�ж˿�ʱ��
#define KBD_COLUMN0_PORT		GPIOB					// ��0�ж˿�
#define KBD_COLUMN0_PIN			GPIO_Pin_12				// ��0�ж˿�����

#define RCC_KBD_COLUMN1_PORT	RCC_APB2Periph_GPIOB	// ��1�ж˿�ʱ��
#define KBD_COLUMN1_PORT		GPIOB                   // ��1�ж˿�
#define KBD_COLUMN1_PIN			GPIO_Pin_13             // ��1�ж˿�����
                                                        
#define RCC_KBD_COLUMN2_PORT	RCC_APB2Periph_GPIOB	// ��2�ж˿�ʱ��
#define KBD_COLUMN2_PORT		GPIOB                   // ��2�ж˿�
#define KBD_COLUMN2_PIN			GPIO_Pin_14             // ��2�ж˿�����
                                                        
#define RCC_KBD_COLUMN3_PORT	RCC_APB2Periph_GPIOB	// ��3�ж˿�ʱ��
#define KBD_COLUMN3_PORT		GPIOB                   // ��3�ж˿�
#define KBD_COLUMN3_PIN			GPIO_Pin_15             // ��3�ж˿�����
                                                        
#define RCC_KBD_COLUMN4_PORT	RCC_APB2Periph_GPIOC    // ��4�ж˿�ʱ��
#define KBD_COLUMN4_PORT		GPIOC                   // ��4�ж˿�
#define KBD_COLUMN4_PIN			GPIO_Pin_6              // ��4�ж˿�����


//define key event
enum KEY_EVENT
{
	KBD_KEY_DONW = 0,	// ��������
	KBD_KEY_UP = 1,		// �����ɿ�
	KBD_KEY_HOLD = 2,
};

/* keyboard event struct */
typedef struct 
{
	unsigned char key_num; 		// ������
	unsigned char key_event; 	// ��Ӧ������ʱ��		
}KBD_EVENT;

// kbd FIFO struct
typedef struct 
{	
	KBD_EVENT key_evt_buf[KEY_EVENT_BUF_SIZE];
	short evt_r_indx;
	short evt_w_indx;
}KEY_EVT_FIFO;

//define KEY num
enum KBD_NUM 
{
	KBD_KEY0 = 0,
	KBD_KEY1,
	KBD_KEY2,
	KBD_KEY3,
	KBD_KEY4,
	KBD_KEY5,
	KBD_KEY6,
	KBD_KEY7,
	KBD_KEY8,
	KBD_KEY9,
	KBD_UP,
	KBD_DOWN,
	KBD_LEFT,
	KBD_RIGHT,
	KBD_PROJ1,
	KBD_PROJ2,
	KBD_PROJ3,
	KBD_PROJ4,
	KBD_PROJ5,
	KBD_FUNC,
	KEY_HASH_KEY,
	KEY_KPASTERISK,
	KEY_BACKSPACE,
	KEY_ENTER,
	KEY_ESC,
	KEY_INVALID
};


void kbd_init(void);	// ������ʼ��
void kbd_scan(void);	// ����ɨ��
void init_kbd_gpio(void);	// ��ʼ�� �����ܽ�
int get_kbd_fifo(KBD_EVENT* evt);	// ��ȡ����FIFO�е���Ϣ
void print_key_info(KBD_EVENT* key);	// ��ӡ �����¼�����Ϣ

#endif /* _KEYBOARD_H_ */

