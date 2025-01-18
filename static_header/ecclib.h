#ifndef __ECCLIB_H__
#define __ECCLIB_H__

#include <stdint.h>

// ECCժҪ��ǩ
// uiKeySize	--	ecc������Կ����(Ŀǰֻ֧��24\32����SECP192K1\SECP256K1)
// ucCompressed	--	ecc��Կѹ������0x02/0x03
// pQx			-- 	�û���Կ(24\32)
// pMsg			--	ժҪ����
// uiMsgLen		--	ժҪ���ݳ���
// pSignData	--	ǩ������(48\64)
int ecc_verify_hash(uint32_t uiKeySize, uint8_t ucCompressed, uint8_t *pQx, uint8_t *pMsg, uint32_t uiMsgLen, uint8_t *pSignData);

// sm2ժҪ��ǩ
// pPukKey		-- 	�û���Կ(64�ֽ�)
// pSM3Hash		--	ժҪ����(hash 32�ֽ�)
// pSignData	--	ǩ������(64)
int sm2_verify_hash(uint8_t* pPukKey, uint8_t *pSM3Hash, uint8_t *pSignResult);

// ����sm2��Կ��
// pubkey		--	��Կ(64�ֽ�)
// pSignData	--	˽Կ(32�ֽ�)
int sm2_keypair_gen(uint8_t *pubkey, uint8_t*prikey);

// sm2ժҪǩ��
// prikey		-- 	�û�˽Կ(32�ֽ�)
// psm3hash		--	ժҪ����(hash 32�ֽ�)
// psigndata	--	ǩ���������(64�ֽ�) 
int sm2_sign_hash(uint8_t* prikey, uint8_t *psm3hash, uint8_t *psigndata);

// uiKeySize	--	sm2������Կ����(sm2 256)
// ucCompressed	--	sm2��Կѹ������0x02/0x03
// pQx			-- 	�û���Կ(32)
// pMsg			--	ժҪ����
// uiMsgLen		--	ժҪ���ݳ���
// pSignData	--	ǩ������(64)
int sm2_verify_hash_compress(uint32_t uiKeySize, uint8_t ucCompressed, uint8_t *pQx, 
                                        uint8_t *uiserid, uint32_t id_len, uint8_t *pSM3Hash, 
                                        uint32_t SM3Hash_len, uint8_t *pSignResult);

#endif // #ifndef __ECCLIB_H__

