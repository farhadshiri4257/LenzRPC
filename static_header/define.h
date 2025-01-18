#ifndef DEFINE_H
#define DEFINE_H

// MAIN_PROJECT define in compile time symbol switch -d.

#ifdef MAIN_PROJECT
#include "posapi.h"
#endif
#define _GNU_SOURCE

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <malloc.h>
#include <poll.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/mount.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/time.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <linux/limits.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#ifndef OsLog
#define OsLog(TYPE_LOG, ...) ((void)ALOG(TYPE_LOG, LOG_TAG, __VA_ARGS__))
//#define OsLog(TYPE_LOG,...) ((void)ALOG_VA(TYPE_LOG, LOG_TAG, __VA_ARGS__))
#endif

#define NORMAL_TIMEOUT 0
#define EXTRA_TIMEOUT 1
#define ACQUIRE_LOCK_T 0
#define HTTP_REQ_IS_OK 200

#undef NULL
#define NULL 0

#define __USE_XOPEN2K 1

#define LCD_LINE 16
#define MAIN_MENU 0x0000
#define INFINITE_LOOP 1

#define LENZ_DEV 1
#define SIGCARD 32
#define SIGQRCODE 33
#define SIGSENTGPSDATA SIGUSR1

#define WAIT_UNTIL_FOR_SEMAPHORE_LOCK 10
#define WAIT_UNTIL_FOR_SEMAPHORE_LOCK_FOR_SENT_GPS 10
#define WAIT_UNTIL_FOR_MUTEX_LOCK 10

#define BOOL unsigned char
#define CHAR char
#define SHORT short
#define SWORD signed short
#define LDWORD unsigned long long
#define SDWORD signed long
#define BYTE uint8_t
#define WORD uint16_t
#define LONG uint32_t
#define DWORD uint32_t
#define INT32 int_least32_t
#define TRUE 1
#define FALSE 0
#define null_ptr NULL
#define ZERO_SIZE 0
#define READ_FILE "r"
#define WRITE_FILE "w"
#define MAX_SIZE_JSON_BUFFER 512

#define THREAD_STACK_SIZE 32767 // by default thread stack size is 65535 bytes
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define IS_NUMBER(x) (x >= '0' && x <= '9')
#define IS_EN_LETTER(x) ((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z'))
#define IS_HEX_LETTER(x) ((x >= 'A' && x <= 'F') || (x >= 'a' && x <= 'f'))

#define LOWORD(l) ((WORD)(l))
#define HIWORD(l) ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w) ((BYTE)(w))
#define HIBYTE(w) ((BYTE)(((WORD)(w) >> 8) & 0xFF))

#define __rte_aligned(a) __attribute__((__aligned__(a)))
#define __rte_packed_aligned(type) __attribute__((packed, aligned(__alignof__(type))))
#define __rte_alignas(type) alignas(alignof(type))
#define __rte_aligned_max __attribute__((aligned))
#define __rte_aligned_cachline __rte_aligned(RTE_CACHELINE)
#define	RTE_DIM(a)	(sizeof (a) / sizeof ((a)[0]))

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define MALLOC(p, s)            \
	do                          \
	{                           \
		if (!((p) = malloc(s))) \
		{                       \
			exit(EXIT_FAILURE); \
		}                       \
	} while (0)

#define FREE(p)   \
	do            \
	{             \
		free(p);  \
		p = NULL; \
	} while (0)

#define FAILURE -1
#define SUCCESSFUL 0
#define ERR_NULL_POINTER 1
#define ERR_INVALID_DATA 2
#define ERR_INVALID_CARD 3
#define ERR_READ_ETICKET_DATA 4
#define ERR_UPDATE_ETICKET_DATA 5
#define ERR_INSUFFICIENT_BALANCE 6
#define ERR_TICKET_IS_EXPIRED 7
#define ERR_PROCESS_ACC_TABLE 8
#define ERR_INVALID_ACC_TABLE 9
#define ERR_RETRIEVE_ACC_CFG 10
#define ERR_UPDATE_ACC_CFG 11
#define ERR_ACC_CFG_NOT_SET 12
#define ERR_FARE_CFG_NOT_SET 13
#define ERR_SETT_FILE_NOT_EXIST 14
#define ERR_GET_SETT_FILE_INFO 15
#define ERR_ARCHIVE_SETT_FILE 16
#define ERR_PROCESS_CONN_CFG_FILE 17
#define ERR_CONN_CFG_NOT_SET 18
#define ERR_RETRIEVE_CONN_CFG 19
#define ERR_UPDATE_CONN_CFG 20
#define ERR_CONNECTION_ERR 21
#define ERR_SEND_FILE_ERR 22
#define ERR_TICKET_NOT_ACCEPTED 23
#define ERR_PROCESS_ROUT_CONFIG 24
#define ERR_INVALID_ROUT_CONFIG 25
#define ERR_INCOMPATIBLE_ROUT_CONFIG 26
#define ERR_UPDATE_ROUT_CFG 27
#define ERR_ROUT_CFG_NOT_SET 28
#define ERR_RETRIEVE_ROUT_CFG 29
#define ERR_ROUT_CODE_NOT_FOUND 30
#define ERR_GET_TERMINAL_CONFIG 31
#define ERR_CANCEL_BY_USER 32
#define ERR_PROCESS_TERM_CFG_FILE 33
#define ERR_KEY_MAP_NOT_FOUND 34
#define ERR_RETRIEVE_KEY_MAP 35
#define ERR_UPDATE_KEY_MAP 36
#define ERR_PROCESS_KEY_MAP 37
#define ERR_CARD_IS_BLOCKED 38
#define ERR_CARD_IS_INACTIVE 39
#define ERR_CARD_WAS_BLOCKED 40
#define ERR_CARD_INCOMPLETE_TRN 41
#define ERR_BANK_CARD_INVALID 42
#define ERR_BALANCE_CFG_NOT_SET 43
#define ERR_RETRIEVE_BALANCE_CFG 44
#define ERR_UPDATE_BALANCE_CFG 45
#define ERR_PROCESS_BALANCE_TABLE 46
#define ERR_INVALID_BALANCE_TABLE 47
#define ERR_NOT_CHARGEABLE_CARD 48
#define ERR_SAM_INIT 49
#define ERR_SAM_ERROR 50
#define ERR_SAM_ACTIVATION 51
#define ERR_IPGD_CFG_NOT_SET 52
#define ERR_RETRIEVE_IPGD_CFG 53
#define ERR_UPDATE_IPGD_CFG 54
#define ERR_PROCESS_IPGD_TABLE 55
#define ERR_INVALID_IPGD_TABLE 56
#define ERR_CARD_EXPIRED 57
#define ERR_CARD_NOT_ISSUE 58
#define ERR_SPECIAL_CARD_EXTENSION 59
#define ERR_UPPER_LIMIT 60
#define ERR_ETICKET_CHARGE_FAILED 61
#define ERR_ETICKET_INCOMPLETE_CHARGE 62
#define ERR_UPDATE_SETT_FILES 63
#define ERR_ETICKET_CARDTYPE_DOESNT_EXIST 64
#define ERR_ETICKET_DOES_NOT_DETECT 66
#define ERR_CONFIG_NOT_AVALIBALE 67

#define	EPERM		 1	/* Operation not permitted */
#define	ENOENT		 2	/* No such file or directory */
#define	ESRCH		 3	/* No such process */
#define	EINTR		 4	/* Interrupted system call */
#define	EIO		 5	/* I/O error */
#define	ENXIO		 6	/* No such device or address */
#define	E2BIG		 7	/* Argument list too long */
#define	ENOEXEC		 8	/* Exec format error */
#define	EBADF		 9	/* Bad file number */
#define	ECHILD		10	/* No child processes */
#define	EAGAIN		11	/* Try again */
#define	ENOMEM		12	/* Out of memory */
#define	EACCES		13	/* Permission denied */
#define	EFAULT		14	/* Bad address */
#define	ENOTBLK		15	/* Block device required */
#define	EBUSY		16	/* Device or resource busy */
#define	EEXIST		17	/* File exists */
#define	EXDEV		18	/* Cross-device link */
#define	ENODEV		19	/* No such device */
#define	ENOTDIR		20	/* Not a directory */
#define	EISDIR		21	/* Is a directory */
#define	EINVAL		22	/* Invalid argument */
#define	ENFILE		23	/* File table overflow */
#define	EMFILE		24	/* Too many open files */
#define	ENOTTY		25	/* Not a typewriter */
#define	ETXTBSY		26	/* Text file busy */
#define	EFBIG		27	/* File too large */
#define	ENOSPC		28	/* No space left on device */
#define	ESPIPE		29	/* Illegal seek */
#define	EROFS		30	/* Read-only file system */
#define	EMLINK		31	/* Too many links */
#define	EPIPE		32	/* Broken pipe */
#define	EDOM		33	/* Math argument out of domain of func */
#define	ERANGE		34	/* Math result not representable */

#define LEN_OF_BUFFER_TO_LOG 1024 * 8
#define MAX_LOG_BUFFER 256
#define SYS_LANGUAGE_FLAG_ENG 0x00
#define SYS_LANGUAGE_FLAG_FRS 0x01
#define SYS_LANGUAGE_FLAG_CHN 0x02

#define BACK_COLOR_SHIFT_STARTED 0X6085FF // 0X7B68EE
#define DISP_COLOR_BACKGROUND_BLUE 0X6085FF
#define MAIN_BACK_BMP_IMAGE "/data/afpos/bin/e60demo/welcome1.bmp" //"/data/welcome1.bmp"
#define SIGNAL_FIVE "/data/afpos/res/signal_bmp/WnetSingalFive.bmp"
#define SIGNAL_FOUR "/data/afpos/res/signal_bmp/WnetSingalFour.bmp"
#define SIGNAL_THREE "/data/afpos/res/signal_bmp/WnetSingalThree.bmp"
#define SIGNAL_TWO "/data/afpos/res/signal_bmp/WnetSingalTwo.bmp"
#define SIGNAL_ONE "/data/afpos/res/signal_bmp/WnetSingalOne.bmp"
#define SIGNAL_ZERO "/data/afpos/res/signal_bmp/WnetSingalNull.bmp"

#define VERSION_INFO_FILE_DEV   "./version_info.h"
#define VERSION_INFO_FILE_LOCAL "./data/afpos/bin/e60demo/version_info.h"

#define INFINITE_LOOP_CONDITION TRUE
#define INFINITE_TIMEOUT 0xFFFFFFF

#define SW_VERSION_MAJOR 1
#define SW_VERSION_MINOR 0
#define SW_VERSION_PATCH 1
#define SW_VERSION_BUGFIX 1
#define SW_RELEASE_DATE 13991111

#define SW_VERSION_STR "1.0.1.1"
#define SW_RELEASE_DATE_STR "1400/05/01"

#define MIFARE_UID_DATA_SIZE 4
#define REQ_CMD_LEN 4
#define REQ_CMD_DATA "\x91\x10\x00\x00"
#define QCARD_KEY_LEN 7
#define DATA_IN_LEN 4
#define SEND_DATA_LEN 10
#define IN	   // input parameter call by value or const value
#define OUT	   // output parameter call by reference can't const value
#define IN_OUT // input and output parameter, array - link list - ...
#define LENGTH_OF_WEEK_STR 16
#define LENGTH_OF_REALTIME_STR 16
#define LENGTH_OF_DATE_STR 15
#define QR_CODE_BUFFER_LENGTH 2048
#define MODULE_SERIAL_SIZE 32 
#define GET_RAW_GEO_POINT 0 
#define GET_NMEA_GEO_POINT 1

typedef enum
{
	STATE_OUT_OF_SERVICE = 0,
	STATE_IN_SERVICE = 1,
	SEARCHING_FALL_THROUGH = 2,
	REGISTRATION_DENIED = 3,
	UNKNOWN = 4,
	REGISTERED_ROAMING = 5
} NET_Status;

typedef enum
{
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR
} LOG_T;

/* IC card request APDU */
typedef struct
{
	BYTE Cmd[REQ_CMD_LEN];
	INT32 LC;
	BYTE DataIn[512];
	INT32 LE;
} ST_APDU_REQ;

/* IC card response APDU */
typedef struct
{
	INT32 LenOut;
	BYTE DataOut[512];
	BYTE SWA;
	BYTE SWB;
} ST_APDU_RSP;

typedef struct
{
	BYTE m_byUid[9];
	BYTE m_byUidLen;
	BYTE m_byaPan[20];
	BYTE m_byCategory;
	BYTE m_bySubtype;
	BYTE m_byaCode[13];
	BYTE m_PhoneNo[13];
	DWORD m_dwExpirationDate;
} PersonnelCard;

typedef struct
{
	struct sockaddr_in stSockAddress;
	INT32 SocketId;
	INT32 Port;
	CHAR IP[15];
} SocketWrapper;

typedef struct
{
	WORD m_wCurrencyCode;
	DWORD m_dwStationId;
	BYTE m_bySamslot;
	// BYTE	m_byaSamClientUid[64];
	// BYTE	m_byaSamOperatorUid[64];
	DWORD m_dwRoutCode;
	BYTE m_byaSamPin[2];
	BYTE m_bySamP1;
	BYTE m_bySamP2;
	BYTE m_FTPRetryTime;
	BYTE m_fileMaxTrnCount;
	BYTE m_maxArchivedFileCount;
	BYTE m_idpaymentDescribtion[100];
	DWORD m_timeInterval;
	DWORD m_idpaymentAmount;
	DWORD m_dwShiftStartTime;
	DWORD m_dwShiftEndTime;
	BYTE m_byMessageDisplayTimout;
	BYTE m_adminLoginEnable;
	BYTE m_byaSamAdminPin[5];
	DWORD m_dwConnTimeOut;
	BYTE m_bySohaFTPEnable;
	BYTE m_bySohaWebServiceEnable;
	BYTE m_bySwitchVersion;
	DWORD m_dwAgentType;
	DWORD m_dwSettTime;
	DWORD m_dwSettRetryIntervalTime;
} sTerminalConfig;

typedef struct
{
	BYTE m_voucherServerUrl[50];
	BYTE m_voucherServerUser[20];
	BYTE m_voucherServerPass[20];
	BYTE m_settingServerUrl[50];
	BYTE m_byaFtpIpAddr[16];
	WORD m_wFtpPort;
	BYTE m_byaFtpUser[50];
	BYTE m_byaFtpPass[50];
	BYTE m_ezpayServerUrl[50];
	BYTE m_ezpayServerUser[20];
	BYTE m_ezpayServerPass[20];
	DWORD m_wSohaWebServiceServerPort;
	BYTE m_byaSohaWebServiceServerIpAddr[16];
	BYTE m_byaWebServiceUser[20];
	BYTE m_byaWebServicePass[20];
} sConnectionConfig;

typedef struct
{
	DWORD m_dwTrnNo;
	DWORD m_dwNextAutoSettTryDate;
	DWORD m_dwNextAutoSettTryTime;
	DWORD m_dwLastAutoSettTryDate;
	DWORD m_dwLastAutoSettTryTime;
	DWORD m_dwLastManualSettTryDate;
	DWORD m_dwLastManualSettTryTime;
	DWORD m_dwLastAutoFTPSettSuccessDate;
	DWORD m_dwLastAutoFTPSettSuccessTime;
	DWORD m_dwLastAutoWebSettSuccessDate;
	DWORD m_dwLastAutoWebSettSuccessTime;
	DWORD m_dwLastManualFTPSettSuccessDate;
	DWORD m_dwLastManualFTPSettSuccessTime;
	DWORD m_dwLastManualWebSettSuccessDate;
	DWORD m_dwLastManualWebSettSuccessTime;
	DWORD m_dwFailedAutoFTPTryCount;
	DWORD m_dwFailedManualFTPTryCount;
	DWORD m_dwFailedAutoWebTryCount;
	DWORD m_dwFailedManualWebTryCount;
} sTerminalInternalConfig;

typedef struct
{
	BYTE m_byExit;
	BYTE m_byRefresh;
	BYTE m_language;
	sConnectionConfig m_sConnConfig;
	sTerminalConfig m_sTerminalConfig;
	sTerminalInternalConfig m_sTermIntConfig;
} stGlVar;

#define URL_SIZE 256 

typedef struct
{
	int32_t GpsIntervalTime;
	int32_t SendGpsDataToServer;
	uint16_t ntp_port;
	uint16_t NMEA_raw_geo_point;
	int32_t dayLight_ntp;
	int32_t diff_hour_ntp;
	int32_t diff_min_ntp;
	CHAR ntpServerUrl[URL_SIZE];
	CHAR avlUrlPath[URL_SIZE];
} devConfigContext_t;

typedef struct
{
    int32_t version;
    int32_t major;
    int32_t minor;
    int32_t revision;
} version_info_t;

#ifdef MAIN_PROJECT
	stGlVar g_stGlVar;
#endif

#endif
