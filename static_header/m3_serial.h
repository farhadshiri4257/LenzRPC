#ifndef __M3_SERIAL_H_
#define __M3_SERIAL_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

#define SERIAL_RS232	(0x00)
#define SERIAL_RS485	(0x01)
#define SERIAL_24G		(0x03)
#define SERIAL_IDCARD	(0x04)

int serial_open(uint8_t port, int bps, int bits);
int serial_close(uint8_t port);
int serial_check(uint8_t port, uint8_t *pucBufStatus);
int serial_read(uint8_t port, void *pcBuf, uint32_t uiSecondMs, uint32_t uiLength, uint32_t *puiActLen);
int serial_send(uint8_t port, uint32_t uiLen, void *pcBuf);
int serial_clear(uint8_t port);
int serial_set9bmode(uint8_t port, uint8_t mode);
int serial_set9bma(uint8_t port, uint8_t ma1, uint8_t ma2);

#ifdef __cplusplus
}
#endif

#endif	// #ifndef __M3_SERIAL_H_

