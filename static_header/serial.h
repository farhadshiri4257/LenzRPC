#ifndef __SERIAL_H__
#define __SERIAL_H__

// port宏定义
#define SERIAL_COM_S20_DEBUG	0
#define SERIAL_COM_S20_MAX      1

//主板上的串口
#define SERIAL_DEV_DEBUG	"/dev/ttyHSL0"

/**
 * configTty设置串口数据，校验位,速率，停止位
 * @param nBits 类型  int数据位 取值 位7或8
 * @param nEvent 类型  char 校验类型 取值N ,E, O,,S
 * @param nSpeed 类型 int 速率 取值 2400,4800,9600,115200
 * @param nStop 类型 int 停止位 取值1 或者 2
 * @param nCTSRTS 类型 int 流控开关 取值 1-打开CTSRTS流控 2-关闭CTSRTS流控
 */
int SC20_serial_open(uint8_t port, int nBits,char nEvent,int nSpeed,int nStop,int nCTSRTS);
int SC20_serial_close(uint8_t port);
int SC20_serial_check(uint8_t port, uint8_t *pucBufStatus);
int SC20_serial_read(uint8_t port, void *pcBuf, uint32_t uiSecondMs, uint32_t uiLength, uint32_t *puiActLen);
int SC20_serial_send(uint8_t port, uint32_t uiLen, void *pcBuf);
int SC20_serial_clear(uint8_t port);

#endif

