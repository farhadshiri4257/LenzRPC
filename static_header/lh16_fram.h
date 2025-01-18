#ifndef __LH16_FRAM_H__
#define __LH16_FRAM_H__

#define FRAM_SIZE	(2048)

int lh16_fram_read(uint32_t addr, uint32_t len, void *pbuf);
int lh16_fram_write(uint32_t addr, uint32_t len, void *pbuf);

#endif // #ifndef __LH16_FRAM_H__