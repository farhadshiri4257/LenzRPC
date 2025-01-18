#ifndef __M3_ICCARD_H__
#define __M3_ICCARD_H__

#include "stdint.h"
#include "ERRCode.h"

#ifdef __cplusplus
extern "C"{
#endif

/********************��Ƭ����ֵ����**********************/
#define	ICCARD_MAX_SLOT		4
#define	SAM1_CARD_SLOT		1
#define	SAM2_CARD_SLOT		2
#define	SAM3_CARD_SLOT		3
#define	SAM4_CARD_SLOT		4

/*********************���������궨��**********************/
#define	ICCARD_VOLTAGE_18V         	(uint8_t)0x02     		/*����Ϊ1.8V��ϵͳ*/
#define	ICCARD_VOLTAGE_3V			(uint8_t)0x01			/*����Ϊ3V��ϵͳ*/
#define	ICCARD_VOLTAGE_5V			(uint8_t)0x00	 		/*����Ϊ5V��ϵͳ*/
#define	ICCARD_VOLTAGE_DEFAULT		ICCARD_VOLTAGE_5V		/*����Ϊ5V��ϵͳ��Ĭ��ģʽ��*/
	
#define	ICCARD_MODE_EMV			(uint8_t)0x00 		/*���ð���EMV2000�淶�ϵ�*/
#define	ICCARD_MODE_38400		(uint8_t)0x01  		/*���ð���38400BPS�����ϵ�*/
#define	ICCARD_MODE_19200		(uint8_t)0x02	 	/*���ð���19200BPS�����ϵ�(�ݲ�֧��)*/ 
#define	ICCARD_MODE_ISO			(uint8_t)0x03	 	/*���ð���9600��ISO�淶�ϵ�*/
#define	ICCARD_MODE_SHEBAO		(uint8_t)0x04	 	/*���ð����籣���淶�ϵ�(�ݲ�֧��)*/
#define	ICCARD_MODE_57600		(uint8_t)0x05	 	/*���ð���57600BPS�����ϵ�(�ݲ�֧��)*/
#define	ICCARD_MODE_TCR       	(uint8_t)0x06     	/*���ð���9600�����ϵ磬��֧��Э��*/
#define	ICCARD_MODE_DEFAULT		ICCARD_MODE_EMV		/*���ð���EMV2000�淶�ϵ磨Ĭ��ģʽ��*/


/********************��Ƭ����ֵ����**********************/
#define	ICCARD_ERR_BYTE      		-20	// T��1�ķ����ֽڴ���
#define	ICCARD_ERR_BLOCK     		-21  	// T��1�ķ������
#define	ICCARD_ERR_TIMEOUT      	ERR_TIMEOUT// ��ʱ���� 
#define	ICCARD_ERR_EARLY_ANSWER  	-22  	// ����Ӧ��
#define	ICCARD_ERR_HARDWARE  		-23  	// �������ݴ���
#define	ICCARD_ERR_OTHER     		-24  	// �������ݴ���

#define	ICCARD_ERR_NONE				ERR_NONE  	/*�ɹ�*/
#define	ICCARD_ERR_ATR				-11		/*�ϵ�ʱ����Ƭ����ATR����*/
#define	ICCARD_ERR_NOCARD			-12		/*ȱ��(SAM���޴˷���)*/
#define	ICCARD_ERR_DATA				-13		/*����Ƭ�������ݴ�*/
#define	ICCARD_ERR_NOPOWER			-14		/*δ�ϵ�*/
#define	ICCARD_ERR_SW				-15		/*SW1!=0X90����SW2!=0X00*/
#define	ICCARD_ERR_FORRESP      	-16		/*CARD_SW1!=0X90��CARD_SW2!=0,Ҫ��Ӧ�ó�����
												  ����ѡ���ļ�����ʱ��CLA=00��INS=A4���յ���
												  ��״̬ʱ������Le=0��get_response�����ȡ
												  �غ���������	*/
												  	
#define	ICCARD_ERROR         	ERR_FAILED			// ��������											  	
#define	ICCARD_ERR_BUSY    	    ERR_BUSY			// �ÿ�������ʹ����												  								
#define	ICCARD_ERR_DISABLED    	ERR_DEVICE			// ��ֹʹ�øÿ���
#define	ICCARD_ERR_PARAM       	ERR_PARAM			// ��������
#define	ICCARD_ERR_ALLOC      	ERR_DEVICE			// �ڴ����ʧ��
#define	ICCARD_ERR_FAILED    	ERR_FAILED			// ��������

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

// IC��ģ��API���ڱ���
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
