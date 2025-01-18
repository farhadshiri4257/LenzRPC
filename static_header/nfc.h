#ifndef __NFC_H_
#define __NFC_H_

#include "ERRCode.h"

/**********************************************************************

                         ��������
                          
***********************************************************************/
// Mifare����֤ʱ����
#define     NFC_KEYA	            0           // A������
#define     NFC_KEYB	            1           // B������

// ��Ƭ����
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
	uint32_t WaitTime_PollTypeA;		/** TypeAѰ���ڼ䵥��ָ��ȴ�ʱ��			**/
	uint32_t WaitTime_PollTypeB;		/** TypeBѰ���ڼ䵥��ָ��ȴ�ʱ��			**/
	uint32_t WaitTime_Mifare;			/** Mifare One�������ڼ�ָ��ȴ�ʱ��		**/
	uint32_t WaitTime_RATS;				/** ����Pro��ָ��ȴ�ʱ��					**/
	uint32_t WaitTime_CardResponse;		/** T=CL�������ȴ�����Ӧ��ʱʱ��			**/
	uint32_t WaitTime_CompleteCmd;		/** �ȴ��ն���ɱ������ݷ��ͽ�����ʱ��		**/
	uint32_t WaitTime_DelayBefore;		/** ������һ��ָ��ǰ�ĵȴ�ʱ��				**/
	uint32_t WaitTime_IdleRF;			/** ��λ�ز��ĵȴ�ʱ��						**/
}NFC_WAITTIME_STRUCT;


/**********************************************************************

                         ����ֵ
                          
***********************************************************************/ 
#define	NFC_ERR_NONE				ERR_NONE
#define	NFC_ERR_TIMEOUT				-10			/** �ǽӴ�����ʱδ��Ӧ **/
#define	NFC_ERR_PARITY				-11			/** �ǽӴ�������������żУ��� **/
#define	NFC_ERR_CRC					-12			/** �ǽӴ�����������CRCУ��� **/
#define	NFC_ERR_CODE				-13			/** �ǽӴ����������ݱ��벻���Ϲ淶 **/
#define	NFC_ERR_COLL				-14			/** �࿨��ͻ **/
#define	NFC_ERR_OVERFLOW			-15			/** �ǽӴ�����ģ���ڲ���������� **/
#define	NFC_ERR_FRAMING				-16			/** �ǽӴ������ش���֡���� **/
#define	NFC_ERR_OTHER				-17			/** �������� **/
#define	NFC_ERR_INVALID_CMD			-18			/** ��Чָ�� **/
#define	NFC_ERR_BITCOUNT			-19			/** �ǽӴ�������λ������ **/
#define	NFC_ERR_BYTECOUNT			-20			/** �ǽӴ��������ֽ������� **/
#define	NFC_ERR_BCC					-21			/** �ǽӴ�TypeA������UIDʱBCCУ��� **/
#define	NFC_ERR_TCL					-22			/** ��Ƭ��֧��TCLЭ�� **/
#define	NFC_ERR_SW					-23			/** ֧��ISO7816-4�淶��Ƭδ������Ч״̬�� **/
#define	NFC_ERR_M1					-24			/** ��M1�� **/
#define	NFC_ERR_DESFIRE				-25			/** ��DESFire�� **/
#define	NFC_ERR_UL					-26			/** ��Ultralight�� **/
#define	NFC_ERR_AUTH				-27			/** M1������δ��Ȩ��֤����֤ʧ�� **/
#define	NFC_ERR_M1_VALUE			-28			/** M1��ֵ����ʧ�� **/
#define	NFC_ERR_M1_WRITE			-29			/** M1��д�����ݲ���ʧ�� **/
#define	NFC_ERR_RESET				-30			/** NFCģ�鸴λʧ��**/
#define	NFC_ERR_CARD_EXIST			-31			/** ��δ�뿪��Ӧ��**/

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

