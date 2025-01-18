#ifndef __ECCLIB_H__
#define __ECCLIB_H__

#include <stdint.h>

// ECC摘要验签
// uiKeySize	--	ecc曲线密钥长度(目前只支持24\32，即SECP192K1\SECP256K1)
// ucCompressed	--	ecc公钥压缩参数0x02/0x03
// pQx			-- 	用户公钥(24\32)
// pMsg			--	摘要数据
// uiMsgLen		--	摘要数据长度
// pSignData	--	签名数据(48\64)
int ecc_verify_hash(uint32_t uiKeySize, uint8_t ucCompressed, uint8_t *pQx, uint8_t *pMsg, uint32_t uiMsgLen, uint8_t *pSignData);

// sm2摘要验签
// pPukKey		-- 	用户公钥(64字节)
// pSM3Hash		--	摘要数据(hash 32字节)
// pSignData	--	签名数据(64)
int sm2_verify_hash(uint8_t* pPukKey, uint8_t *pSM3Hash, uint8_t *pSignResult);

// 生成sm2密钥对
// pubkey		--	公钥(64字节)
// pSignData	--	私钥(32字节)
int sm2_keypair_gen(uint8_t *pubkey, uint8_t*prikey);

// sm2摘要签名
// prikey		-- 	用户私钥(32字节)
// psm3hash		--	摘要数据(hash 32字节)
// psigndata	--	签名数据输出(64字节) 
int sm2_sign_hash(uint8_t* prikey, uint8_t *psm3hash, uint8_t *psigndata);

// uiKeySize	--	sm2曲线密钥长度(sm2 256)
// ucCompressed	--	sm2公钥压缩参数0x02/0x03
// pQx			-- 	用户公钥(32)
// pMsg			--	摘要数据
// uiMsgLen		--	摘要数据长度
// pSignData	--	签名数据(64)
int sm2_verify_hash_compress(uint32_t uiKeySize, uint8_t ucCompressed, uint8_t *pQx, 
                                        uint8_t *uiserid, uint32_t id_len, uint8_t *pSM3Hash, 
                                        uint32_t SM3Hash_len, uint8_t *pSignResult);

#endif // #ifndef __ECCLIB_H__

