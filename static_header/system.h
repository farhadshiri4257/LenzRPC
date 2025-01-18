#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>

// ����LED״̬
#define	LED_LH16_RED    	    0x00 // RF��ɫָʾ��
#define	LED_LH16_GREEN    		0x01 // RF��ɫָʾ��
#define	LED_LH16_ONLINE	   		0x02 // GPRSָʾ��
#define	LED_LH16_FLASH	    	0x03 // ����ͷ�����

void sys_led_control(int led, int stat);
void sys_beep(uint32_t uiMs);
int sys_get_random(uint32_t uiLen, uint8_t *pucRandom);
int sys_get_system_version(char *pVersion);
void sys_enable_adb_driver(void);								// adbʹ��;
void sys_enable_usb_storage_driver(void);					// u��ʹ��
int ExecCmd(char *cmd, char *out, unsigned int redirectStdErr);		// ִ������������
int send_at_cmd(int commandId,char *request_at,char *respond_at);



#endif // #ifndef __SYSTEM_H__

