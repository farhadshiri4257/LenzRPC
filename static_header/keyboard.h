#ifndef __INPUT_LISTENER_H__
#define __INPUT_LISTENER_H__

#define	KEY_LH16_UP				(0x06)		// �Ϸ���
#define	KEY_LH16_DOWN			(0x07)		// �·���
#define	KEY_LH16_ENTER			(0x0D)		// ȷ���� 
#define	KEY_LH16_CANCEL  		(0x1B)		// ȡ����

int kb_getkey(int wait);
int kb_hit(void);
void kb_flush(void);
int init_input_listener();
void exit_input_listener(void);

#endif
