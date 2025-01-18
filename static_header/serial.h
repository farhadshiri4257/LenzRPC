#ifndef __SERIAL_H__
#define __SERIAL_H__

// port�궨��
#define SERIAL_COM_S20_DEBUG	0
#define SERIAL_COM_S20_MAX      1

//�����ϵĴ���
#define SERIAL_DEV_DEBUG	"/dev/ttyHSL0"

/**
 * configTty���ô������ݣ�У��λ,���ʣ�ֹͣλ
 * @param nBits ����  int����λ ȡֵ λ7��8
 * @param nEvent ����  char У������ ȡֵN ,E, O,,S
 * @param nSpeed ���� int ���� ȡֵ 2400,4800,9600,115200
 * @param nStop ���� int ֹͣλ ȡֵ1 ���� 2
 * @param nCTSRTS ���� int ���ؿ��� ȡֵ 1-��CTSRTS���� 2-�ر�CTSRTS����
 */
int SC20_serial_open(uint8_t port, int nBits,char nEvent,int nSpeed,int nStop,int nCTSRTS);
int SC20_serial_close(uint8_t port);
int SC20_serial_check(uint8_t port, uint8_t *pucBufStatus);
int SC20_serial_read(uint8_t port, void *pcBuf, uint32_t uiSecondMs, uint32_t uiLength, uint32_t *puiActLen);
int SC20_serial_send(uint8_t port, uint32_t uiLen, void *pcBuf);
int SC20_serial_clear(uint8_t port);

#endif

