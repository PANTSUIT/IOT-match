#ifndef __BSP_FLASH_H__
#define __BSP_FLASH_H__

#include "stm32f10x.h"
#include <string.h>
#include <stdio.h>
#include "Init.h"
//#define FLASH_START_ADDRESS 	(0x8000000)	//
//#define FLASH_BASE_ADDRESS  	(0*1024)    //loader size 0K
//#define FLASH_LIMIT_MAX_SIZE	(256*2048)  //flash limit max
//#define FLASH_BLOCK_SIZE    	(2048)      //2K
//#define FLASH_BLOCK_NUMBER  	(256)       //256���� ����


//#define MIN_FLASH_ADDRESS = FLASH_START_ADDRESS + FLASH_BASE_ADDRESS;
//#define MAX_FLASH_ADDRESS = FLASH_START_ADDRESS + FLASH_LIMIT_MAX_SIZE;

//#define FLASH_PAGE_SIZE  FLASH_BLOCK_SIZE
//#define FLASH_MIN_ADDR   MIN_FLASH_ADDRESS
//#define FLASH_MAX_ADDR   MAX_FLASH_ADDRESS

// STM32VET6 MCU flash ��СΪ512k
// ����һ���������洢����
#define MCU_FLASH_BASE_ADDR			(0x08000000)	// FLASH ����ַ
#define MCU_FLASH_BLOCK_SIZE		(2048)			// ���С 2k	�Կ��ҵ�λ���в���
#define MCU_FLASH_BLOCK_NUMER		(256)			// 256 ����
	
#define MCU_FLASH_MIN_ADDR			MCU_FLASH_BASE_ADDR		// FLASH ��С��ַ
#define MCU_FLASH_MAX_ADDR			(MCU_FLASH_BASE_ADDR + MCU_FLASH_BLOCK_NUMER * MCU_FLASH_BLOCK_SIZE - 1)  // FLASH ����ַ
	
#define MCU_FLASH_PAGE_SIZE			MCU_FLASH_BLOCK_SIZE	// FLASH ҳ��С
#define MCU_FLASH_PAGE_NUMBER		MCU_FLASH_BLOCK_NUMER	// FALSH ҳ����
	
#define MCU_FLASH_CODE_SIEZ			( 12* MCU_FLASH_BLOCK_SIZE )	// 	����ռ��FLASH��СΪ 24K
#define MCU_FLASH_CODE_END_ADDR		(MCU_FLASH_BASE_ADDR + MCU_FLASH_CODE_SIZE - 1)	// ����ν�����ַ



int32_t bsp_flash_write     (uint32_t addr,uint8_t* pbuf,int32_t len);

int32_t bsp_flash_write_num(uint32_t addr,uint8_t *pdata,uint8_t *pnew,int32_t len,int32_t *pnum,int32_t sum);

int32_t bsp_flash_read		(uint32_t addr,uint8_t* p_dest,int32_t len);

int32_t bsp_flash_read_num(uint32_t addr,uint8_t *pdata,int32_t len,int32_t num,int32_t sum);

int32_t bsp_flash_erase		(uint32_t address,uint32_t len);

uint32_t *bsp_flash_find(uint32_t addr,uint8_t *pdata,int32_t len,int32_t sum);


int32_t demo_flash(void);

#endif //__BSP_FLASH_H__

