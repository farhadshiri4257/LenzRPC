#ifndef __M3_ICCARD_H__
#define __M3_ICCARD_H__

#include "stdint.h"
#include "ERRCode.h"

#ifdef __cplusplus
extern "C"{
#endif

/********************卡片返回值定义**********************/
#define	ICCARD_MAX_SLOT		4
#define	SAM1_CARD_SLOT		1
#define	SAM2_CARD_SLOT		2
#define	SAM3_CARD_SLOT		3
#define	SAM4_CARD_SLOT		4

/*********************其他参数宏定义**********************/
#define	ICCARD_VOLTAGE_18V         	(uint8_t)0x02     		/*设置为1.8V卡系统*/
#define	ICCARD_VOLTAGE_3V			(uint8_t)0x01			/*设置为3V卡系统*/
#define	ICCARD_VOLTAGE_5V			(uint8_t)0x00	 		/*设置为5V卡系统*/
#define	ICCARD_VOLTAGE_DEFAULT		ICCARD_VOLTAGE_5V		/*设置为5V卡系统（默认模式）*/
	
#define	ICCARD_MODE_EMV			(uint8_t)0x00 		/*设置按照EMV2000规范上电*/
#define	ICCARD_MODE_38400		(uint8_t)0x01  		/*设置按照38400BPS速率上电*/
#define	ICCARD_MODE_19200		(uint8_t)0x02	 	/*设置按照19200BPS速率上电(暂不支持)*/ 
#define	ICCARD_MODE_ISO			(uint8_t)0x03	 	/*设置按照9600，ISO规范上电*/
#define	ICCARD_MODE_SHEBAO		(uint8_t)0x04	 	/*设置按照社保卡规范上电(暂不支持)*/
#define	ICCARD_MODE_57600		(uint8_t)0x05	 	/*设置按照57600BPS速率上电(暂不支持)*/
#define	ICCARD_MODE_TCR       	(uint8_t)0x06     	/*设置按照9600速率上电，不支持协商*/
#define	ICCARD_MODE_DEFAULT		ICCARD_MODE_EMV		/*设置按照EMV2000规范上电（默认模式）*/


/********************卡片返回值定义**********************/
#define	ICCARD_ERR_BYTE      		-20	// T＝1的分组字节错误
#define	ICCARD_ERR_BLOCK     		-21  	// T＝1的分组错误
#define	ICCARD_ERR_TIMEOUT      	ERR_TIMEOUT// 超时错误 
#define	ICCARD_ERR_EARLY_ANSWER  	-22  	// 过早应答
#define	ICCARD_ERR_HARDWARE  		-23  	// 其他数据错误
#define	ICCARD_ERR_OTHER     		-24  	// 其他数据错误

#define	ICCARD_ERR_NONE				ERR_NONE  	/*成功*/
#define	ICCARD_ERR_ATR				-11		/*上电时读卡片回送ATR错误*/
#define	ICCARD_ERR_NOCARD			-12		/*缺卡(SAM卡无此返回)*/
#define	ICCARD_ERR_DATA				-13		/*读卡片回送数据错*/
#define	ICCARD_ERR_NOPOWER			-14		/*未上电*/
#define	ICCARD_ERR_SW				-15		/*SW1!=0X90或者SW2!=0X00*/
#define	ICCARD_ERR_FORRESP      	-16		/*CARD_SW1!=0X90或CARD_SW2!=0,要求应用程序在
												  调用选择文件命令时（CLA=00，INS=A4）收到这
												  个状态时，发出Le=0的get_response命令，以取
												  回后续的内容	*/
												  	
#define	ICCARD_ERROR         	ERR_FAILED			// 其他错误											  	
#define	ICCARD_ERR_BUSY    	    ERR_BUSY			// 该卡座正在使用中												  								
#define	ICCARD_ERR_DISABLED    	ERR_DEVICE			// 禁止使用该卡座
#define	ICCARD_ERR_PARAM       	ERR_PARAM			// 参数错误
#define	ICCARD_ERR_ALLOC      	ERR_DEVICE			// 内存分配失败
#define	ICCARD_ERR_FAILED    	ERR_FAILED			// 其他错误

#define ERR_SAM_BASE            (-6000)
typedef enum{
	ICCARD_OK = 0,
	ICCARD_ERRPAR = ERR_SAM_BASE-1,
	ICCARD_TIMEOUT = ERR_SAM_BASE-2,
	ICCARD_POWER = ERR_SAM_BASE-3,
	ICCARD_PARITY = ERR_SAM_BASE-4,
	ICCARD_TCK = ERR_SAM_BASE-5,
	ICCARD_PROTOCOL = ERR_SAM_BASE-6,	
}ICCARD_RESULT;

// IC卡模块API出口表定义
int iccard_init_module(int CardSlot, uint8_t ucVolMode, uint8_t ucPowerMode);
int iccard_reset(int CardSlot, uint32_t *puiATRLen, void *pvATRData, uint8_t *pvPtl);
int iccard_release(int CardSlot);    
int iccard_exchange_apdu(int CardSlot, uint32_t uiSendLen, const void *pvSendData, uint32_t *puiRecLen, void *pvRecData);
int iccard_cold_reset(int CardSlot, uint32_t *puiATRLen, void *pvATRData, uint8_t *pvPtl);
int iccard_warm_reset(int CardSlot, uint32_t *puiATRLen, void *pvATRData, uint8_t *pvPtl);

#ifdef __cplusplus
}
#endif

#endif		// #ifndef __M3_ICCARD_H__
