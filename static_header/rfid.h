#ifndef _RFID_H_
#define _RFID_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define RFID_MAGIC 'x' 
#define IOCTRL_NFC_SET_DELAY                            _IO(RFID_MAGIC, 3) 
#define IOCTRL_NFC_LED_CONTROL                          _IO(RFID_MAGIC, 4) 
#define IOCTRL_NFC_ISO1443_REQA                         _IO(RFID_MAGIC, 5) 
#define IOCTRL_NFC_ISO1443_WUPA                         _IO(RFID_MAGIC, 6) 
#define IOCTRL_NFC_ISO1443_ANTICOLLION                  _IO(RFID_MAGIC, 7) 
#define IOCTRL_NFC_ISO1443_SELECT                       _IO(RFID_MAGIC, 8) 
#define IOCTRL_NFC_ISO1443_HALTA                        _IO(RFID_MAGIC, 9) 
#define IOCTRL_NFC_ISO1443_RATS                         _IO(RFID_MAGIC, 10) 
#define IOCTRL_NFC_ISO1443_PPPS                         _IO(RFID_MAGIC, 11) 
#define IOCTRL_NFC_ISO1443_REQB                         _IO(RFID_MAGIC, 12) 
#define IOCTRL_NFC_ISO1443_WUPB                         _IO(RFID_MAGIC, 13) 
#define IOCTRL_NFC_ISO1443_ATTRIB                       _IO(RFID_MAGIC, 14) 
#define IOCTRL_NFC_ISO1443_GET_UIDB                     _IO(RFID_MAGIC, 15) 
#define IOCTRL_NFC_ISO1443_HALTB                        _IO(RFID_MAGIC, 16) 
#define IOCTRL_NFC_ISO1443_POLL                         _IO(RFID_MAGIC, 17) 
#define IOCTRL_NFC_ISO1443_ACTIVATE                     _IO(RFID_MAGIC, 18) 
#define IOCTRL_NFC_ISO1443_EXCHANGE                     _IO(RFID_MAGIC, 19) 
#define IOCTRL_NFC_ISO1443_TCL_EXCHANGE                 _IO(RFID_MAGIC, 20) 
#define IOCTRL_NFC_ISO1443_TCL_APDU                     _IO(RFID_MAGIC, 21) 
#define IOCTRL_NFC_ISO1443_DEACTIVATE                   _IO(RFID_MAGIC, 22) 
#define IOCTRL_NFC_ISO1443_CHECK_CARD_EXIT              _IO(RFID_MAGIC, 23) 
#define IOCTRL_NFC_ISO1443_MIFARE_ONE_POLL              _IO(RFID_MAGIC, 24) 
#define IOCTRL_NFC_ISO1443_MIFARE_ONE_RELEASE           _IO(RFID_MAGIC, 25) 
#define IOCTRL_NFC_ISO1443_MIFARE_ONE_AUTHENTICATE      _IO(RFID_MAGIC, 26) 
#define IOCTRL_NFC_ISO1443_MIFARE_ONE_READ_BLOCK        _IO(RFID_MAGIC, 27) 
#define IOCTRL_NFC_ISO1443_MIFARE_ONE_WRITE_BLOCK       _IO(RFID_MAGIC, 28) 
#define IOCTRL_NFC_ISO1443_MIFARE_ONE_SET_VALUE         _IO(RFID_MAGIC, 29) 
#define IOCTRL_NFC_ISO1443_MIFARE_ONE_INCREMENT         _IO(RFID_MAGIC, 30) 
#define IOCTRL_NFC_ISO1443_MIFARE_ONE_DECREMENT         _IO(RFID_MAGIC, 31) 
#define IOCTRL_NFC_ISO1443_MIFARE_ONE_RESTORE           _IO(RFID_MAGIC, 32) 
#define IOCTRL_NFC_ISO1443_MIFARE_ONE_TRANSFER          _IO(RFID_MAGIC, 33) 
#define IOCTRL_NFC_ISO1443_MIFARE_ONE_GET_VALUE         _IO(RFID_MAGIC, 34) 
#define IOCTRL_NFC_ISO1443_MIFARE_ULTRALIGHT_POLL       _IO(RFID_MAGIC, 35) 
#define IOCTRL_NFC_ISO1443_MIFARE_ULTRALIGHT_RELEASE    _IO(RFID_MAGIC, 36) 
#define IOCTRL_NFC_ISO1443_MIFARE_ULTRALIGHT_READ_PAGE  _IO(RFID_MAGIC, 37) 
#define IOCTRL_NFC_ISO1443_MIFARE_ULTRALIGHT_WRITE_PAGE _IO(RFID_MAGIC, 38) 

typedef struct _tagRfidCommData
{
	int err;
	unsigned int m_WriteLen;
	unsigned char m_WriteBuf[300];
	unsigned int m_ReadLen;
	unsigned char  m_ReadBuf[300];
} CRfidCommData, *PRfidCommData;

typedef struct _tagRfidAtqa
{
  int err;
  unsigned char atqa[2];
}CRfidAtqa,*PRfidAtqa;

typedef struct _tagRfidAnticollion
{
	int err;
	unsigned char sel;
	unsigned char uid[10];
}CRfidAnticollion,*PRfidAnticollion;

typedef struct _tagRfidSelectData
{
	int err;
	unsigned char sel;
	unsigned char uid[10];
	unsigned char sak[20];
}CRfidSelectData,*PRfidSelectData;

typedef struct _tagRfidPollData
{
	int	      err;
	unsigned char mode;
	unsigned char uidlen;
	unsigned char uid[10];
	unsigned char cardtype;
}CRfidPollData,*PRfidPollData;

typedef struct _tagRfidActiveData
{
	int err;
	unsigned char cid;
	unsigned char fsdi;
	unsigned char len;
	unsigned char info[256];
}CRfidAcivateData,*PRfidActivateData;

typedef struct _tagRfidRatsData
{
	int err;
	unsigned char cid;
	unsigned char fsdi;
	unsigned char atslen;
	unsigned char ats[100];
}CRfidRatsData,*PRfidRatsData;

typedef struct _tagRfidPPPSData
{
  int err;
  unsigned char cid;
}CRfidPPPSData,*PRfidPPPSData;

typedef struct _tagRfidReqb
{
  int err;
	unsigned char afi;
	unsigned char lenofatqb;
	unsigned char atqb[13];
}CRfidrReqbData,*PRfidReqbData;

typedef struct _tagRfidAttrib
{
        int err;
	unsigned char cid;
	unsigned char fsdi;
	unsigned char pupi[4];
	unsigned char len;
	unsigned char info[256];
}CRfidrAttribData,*PRfidAttribData;

typedef struct _tagRfidGetUidb
{
  int err;
	unsigned char uidlen;
	unsigned char uid[20];
}CRfidrGetUidbData,*PRfidGetUidbData;

typedef struct _tagRfidHaltb
{
  int err;
	unsigned char pupi[4];
}CRfidrHaltbData,*PRfidHaltbData;

typedef struct _tagRfidError
{
  int err;
}CRfidrErrorData,*PRfidErrorData;

typedef struct _tagRfidUid
{
  int err;
	unsigned char uid[10];
}CRfidrUidData,*PRfidUidData;

typedef struct _tagRfidM1Auth
{
  int err;
	unsigned char blockno;
	unsigned char keytype;
	unsigned char key[256];
	unsigned char uid[4];
}CRfidrM1AuthData,*PRfidM1AuthData;

typedef struct _tagRfidM1BlockData
{
  int err;
	unsigned char blockno;
	unsigned char blockdata[256];
}CRfidrM1BlockData,*PRfidM1BlockData;

typedef struct _tagRfidM1PageData
{
  int err;
	unsigned char page;
	unsigned char data[256];
}CRfidrM1PageData,*PRfidM1PageData;

typedef struct _tagRfidM1SetValue
{
  int err;
	unsigned char blockno;
	unsigned int value;
}CRfidrM1SetValueData,*PRfidM1SetValueData;

typedef struct _tagRfidM1BlockNo
{
  int err;
	unsigned char blockno;
	unsigned int value;
}CRfidrM1BlockNoData,*PRfidM1BlockNoData;
#endif