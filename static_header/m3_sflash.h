#ifndef __M3_SFLASH_H__
#define __M3_SFLASH_H__
#include <stdint.h>  

#ifdef __cplusplus
extern "C"{
#endif
#define SFLASH_APP_START_ADDR		(4096 * 4)		// 16K 用于系统配置区 应用不可写

int sflash_read(uint32_t addr, uint32_t len, void *pbuf);
int sflash_write(uint32_t addr, uint32_t len, const void *pbuf);

#ifdef __cplusplus
}
#endif
 
#endif	//#ifndef __M3_SFLASH_H__

