#ifndef __SM3_H_
#define __SM3_H_

#include <stdint.h>
//#include "errcode.h"

/*
------------------------------------------------------------------------------------------------------------------------
*                                                      SMS3 Algorithm
------------------------------------------------------------------------------------------------------------------------
*/
typedef struct {
	uint32_t total[2];     /*!< number of bytes processed  */
	uint32_t state[8];     /*!< intermediate digest state  */
	uint8_t buffer[64];    /*!< data block being processed */

	uint8_t ipad[64];      /*!< HMAC: inner padding        */
	uint8_t opad[64];      /*!< HMAC: outer padding        */
}sm3_context;

int sm3_start(sm3_context *ctx);
int sm3_update(sm3_context *ctx, uint8_t *input, int ilen);
int sm3_finish(sm3_context *ctx, uint8_t output[32]);

int sm3(uint8_t *input, int ilen, uint8_t output[32]);  // 调用这个

int sm3_hmac_start(sm3_context *ctx, uint8_t *key, int keylen);
int sm3_hmac_update(sm3_context *ctx, uint8_t *input, int ilen);
int sm3_hmac_finish(sm3_context *ctx, uint8_t output[32]);
int sm3_hmac(uint8_t *key, int keylen, uint8_t *input, int ilen,uint8_t output[32]);


#endif
