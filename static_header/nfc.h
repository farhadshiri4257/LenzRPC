#ifndef __NFC_H_
#define __NFC_H_

#include "ERRCode.h"

/**********************************************************************

                         参数定义
                          
***********************************************************************/
// Mifare卡认证时参数
#define     NFC_KEYA	            0           // A套密码
#define     NFC_KEYB	            1           // B套密码

// 卡片类型
#define 	NFC_S50                 0
#define 	NFC_S70                 1
#define 	NFC_PRO             	2
#define 	NFC_PRO_S50             3
#define 	NFC_PRO_S70             4
#define 	NFC_TYPEB_CARD          5
#define 	NFC_TYPEB_TCL			6
#define 	NFC_ULTRALIGHT			7
#define 	NFC_DESFIRE				8
#define 	NFC_TYPEA_CARD			9

typedef struct{
	uint32_t WaitTime_PollTypeA;		/** TypeA寻卡期间单条指令等待时间			**/
	uint32_t WaitTime_PollTypeB;		/** TypeB寻卡期间单条指令等待时间			**/
	uint32_t WaitTime_Mifare;			/** Mifare One卡操作期间指令等待时间		**/
	uint32_t WaitTime_RATS;				/** 激活Pro卡指令等待时间					**/
	uint32_t WaitTime_CardResponse;		/** T=CL卡操作等待卡响应超时时间			**/
	uint32_t WaitTime_CompleteCmd;		/** 等待终端完成本次数据发送接收总时间		**/
	uint32_t WaitTime_DelayBefore;		/** 发送下一条指令前的等待时间				**/
	uint32_t WaitTime_IdleRF;			/** 复位载波的等待时间						**/
}NFC_WAITTIME_STRUCT;


/**********************************************************************

                         返回值
                          
***********************************************************************/ 
#define	NFC_ERR_NONE				ERR_NONE
#define	NFC_ERR_TIMEOUT				-10			/** 非接触卡超时未响应 **/
#define	NFC_ERR_PARITY				-11			/** 非接触卡返回数据奇偶校验错 **/
#define	NFC_ERR_CRC					-12			/** 非接触卡返回数据CRC校验错 **/
#define	NFC_ERR_CODE				-13			/** 非接触卡返回数据编码不符合规范 **/
#define	NFC_ERR_COLL				-14			/** 多卡冲突 **/
#define	NFC_ERR_OVERFLOW			-15			/** 非接触读卡模块内部缓冲区溢出 **/
#define	NFC_ERR_FRAMING				-16			/** 非接触卡返回错误帧序列 **/
#define	NFC_ERR_OTHER				-17			/** 其他错误 **/
#define	NFC_ERR_INVALID_CMD			-18			/** 无效指令 **/
#define	NFC_ERR_BITCOUNT			-19			/** 非接触卡返回位数不对 **/
#define	NFC_ERR_BYTECOUNT			-20			/** 非接触卡返回字节数不对 **/
#define	NFC_ERR_BCC					-21			/** 非接触TypeA卡返回UID时BCC校验错 **/
#define	NFC_ERR_TCL					-22			/** 卡片不支持TCL协议 **/
#define	NFC_ERR_SW					-23			/** 支持ISO7816-4规范卡片未返回有效状态码 **/
#define	NFC_ERR_M1					-24			/** 非M1卡 **/
#define	NFC_ERR_DESFIRE				-25			/** 非DESFire卡 **/
#define	NFC_ERR_UL					-26			/** 非Ultralight卡 **/
#define	NFC_ERR_AUTH				-27			/** M1卡扇区未授权验证或验证失败 **/
#define	NFC_ERR_M1_VALUE			-28			/** M1卡值操作失败 **/
#define	NFC_ERR_M1_WRITE			-29			/** M1卡写块数据操作失败 **/
#define	NFC_ERR_RESET				-30			/** NFC模块复位失败**/
#define	NFC_ERR_CARD_EXIST			-31			/** 卡未离开感应区**/

int nfc_set_2g4_param(void);

int nfc_open(void);
int nfc_close(void);

int nfc_iso14443_reqa(uint8_t *atqa);
int nfc_iso14443_wupa(uint8_t *atqa);
int nfc_iso14443_anticollion(uint8_t sel, uint8_t *uid);
int nfc_iso14443_select(uint8_t sel, 
	                        uint8_t *uid, 
	                        uint8_t *sak);
int nfc_iso14443_halta(void);
int nfc_iso14443_rats(uint8_t cid, 
	                      uint8_t fsdi, 
	                      uint8_t *atslen, 
	                      uint8_t *ats);
int nfc_iso14443_ppps(uint8_t cid);
int nfc_iso14443_reqb(uint8_t afi, 
	                       uint8_t *lenofatqb, 
	                       uint8_t *atqb);

int nfc_iso14443_wupb(uint8_t afi, 
	                        uint8_t *lenofatqb, 
	                        uint8_t *atqb);

int nfc_iso14443_attrib(uint8_t cid,
							uint8_t fsdi,
							const uint8_t *pupi, 
							uint8_t *len, 
							uint8_t *info);
int nfc_iso14443_get_uidb(uint8_t *uidlen, uint8_t *uid);
int nfc_iso14443_haltb(const uint8_t *pupi);

int nfc_iso14443_poll(int mode,
							uint8_t *uidlen,
							uint8_t *uid,
							uint8_t *cardtype);

int nfc_iso14443_activate(uint8_t cid, 
	                          uint8_t fsdi,
	                          uint8_t *len, 
	                          uint8_t *info);

int nfc_iso14443_exchange(int sendlen, 
									const uint8_t *senddata,
									uint32_t *recvlen,
									uint8_t *recvdata);

int nfc_iso14443_exchange_b(int sendlen, 
									  const uint8_t *senddata,
									  uint32_t *recvlen,
									  uint8_t *recvdata);

int nfc_iso14443_tcl_exchange(int sendlen, 
									const uint8_t *senddata,
									uint32_t *recvlen,
									uint8_t *recvdata);

int nfc_iso14443_tcl_apdu(int sendlen, 
								const uint8_t *senddata,
								uint32_t *recvlen,
								uint8_t *recvdata);

int nfc_iso14443_deactivate(void);
int nfc_iso14443_check_card_exit(void);


int mifare_one_poll(uint8_t *uid);
int mifare_one_release(void);
int mifare_one_authenticate(uint8_t blockno, 
				                             uint8_t keytype, 
				                             const uint8_t *key, 
				                             const uint8_t *uid);
int mifare_one_read_block(uint8_t blockno,	uint8_t *blockdata);
int mifare_one_write_block(uint8_t blockno,const uint8_t *blockdata);
int mifare_one_set_value(uint8_t blockno, uint32_t value);
int mifare_one_increment(uint8_t blockno, uint32_t value);
int mifare_one_decrement(uint8_t blockno, uint32_t value);
int mifare_one_restore(uint8_t blockno);
int mifare_one_transfer(uint8_t blockno);
int mifare_one_get_value(uint8_t blockno, uint32_t *value);

int mifare_ultralight_poll(uint8_t *uid);
int mifare_ultralight_release(void);
int mifare_ultralight_read_page(uint8_t page,uint8_t *data);
int mifare_ultralight_write_page(uint8_t page,const uint8_t *data);

#endif

