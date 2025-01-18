/**
 * @file MessageProto.h
 * @author f.shiri (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-08-1
 *
 * @copyright Copyright (c) 2019-2025
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef __MESSAGE_PROTOCOL_H__
#define __MESSAGE_PROTOCOL_H__ 1
/***************************************************************************************************************
 * IMPORTANT @note:
 *  1) Note    : if set -DVERBOSE_INFO flag in make file, show the log message info in runtime mode.
 *
 *  2) WARNING : all defined macro (*_SIZE) size, must be aligned to VIRTUAL_PAGE_SIZE,
 *               usually is 64 in endpoint and 32 in device if ABI is ARMV7A.
 *
 *  3) WARNING : all data model defined in structures, the care of cpu cache coherent by compiler builtin
 *               attributes, therefore should not transposition fields order in the structures.
 *
 *  4) WARNING : don't change this behavior!!!
 *               becuase all structure protobuf model aligned, when sending by device
 *               therefore endpoint is used aligning recv data stream and map to entity model.
 *  5) @attention:
 *               how to add new API operation in ProtoBuf Proxy Library:
 *               1) define a two new structure in specfic style
 *                  1-1) structre Input...
 *                  1-2) structre Output...
 *               2) define macro for name of RPC function
 *               3) define macro for siganture of RPC function
 *               4) add new structrue in server_context structre
 *               5) add in rpc_name_lookup_t enum type
 *               6) add in lookup table RPC name
 *               7) add new api function name in ProtoLib.cpp
 *               8) add new api function name in Proxy class
 *               9) on the function proxy class only need create new object from new API class in section 10
 *               10) create new class inhreted from proto_proxy::interface::IEntityModel
 *               11) in the new model class must be defined serialize_entity() operation
 *                12) set the header and body the serialize_entity() method
 ***************************************************************************************************************/

/**************************************************************************************************************]
|                                map for Protocol Buffer Frame content                                         |
[**************************************************************************************************************]
|                                 Table of Needed Stream Buffer Field                                          |
|--------------------------------------------------------------------------------------------------------------|
|   0       1      2       3       4       5      6       7       8       9      10      11       12 ...  32   |
|------- ------- ------ ------- ------- ------ ------- ------- ------- ------- ------- ------- ------- --------|
|                                      Header of Stream Buffer                                                 |
|--------------------------------------------------------------------------------------------------------------|
|  16   |  16   |  17  |  17   |  17   |  18  |   9   |   ... |   ... |  ...  |  ...  |  ...  |  ...  |   ...  |
|------- ------- ------ ------- ------- ------ ------- ------- ------- ------- ------- ------- ------- --------|
| Magic Byte-16 | CRC chk Byte-16 |  MacAddr Byte-17  | HWAdrr Byte-17 | RPCFunc Byte-17   | ACK-Magic Byte-16 |
|    max-16     |      max-16     |      max-32       |    max-32      |   max-32          |      max-16       |
|--------------------------------------------------------------------------------------------------------------|
|                                                                                                              |
|                                         pay-load model data                                                  |
|                                              max-1024                                                        |
|                                                                                                              |
\**************************************************************************************************************/

/*only include commen header with device and RSW server */
#include <zlib.h>

/*IMPORTANT NOTE:Maximum size is : 32-byte value + 1-byte null-terminated, not checked in prorobuf library! */
#define REQUEST_INPUT_FIELD_SIZE 32
#define REQUEST_INPUT_FIELD_SIZE_ACTUAL 31
/*IMPORTANT NOTE:Maximum size is : 32-byte value + 1-byte null-terminated, not checked in prorobuf library! */
#define REQUEST_OUTPUT_FIELD_SIZE 32
#define REQUEST_OUTPUT_FIELD_SIZE_ACTUAL 31
/*IMPORTANT NOTE:Maximum size is : 32-byte value + 1-byte null-terminated, not checked in prorobuf library! */
#define SERIAL_HW_FIELD_SIZE 32
#define SERIAL_HW_FIELD_SIZE_ACTUAL 31
/*IMPORTANT NOTE:Maximum size is : 32-byte(256bit) value + 1-byte null-terminated, not checked in prorobuf library! */
#define REQUEST_KEY_SIZE 32 + 1 // 256bit needed
/*IMPORTANT NOTE:Maximum size is : 32-byte value + 1-byte null-terminated, not checked in prorobuf library! */
#define QR_BUFFER_SIZE 32
#define QR_BUFFER_SIZE_ACTUAL 31
/*IMPORTANT NOTE:Maximum size is : 31-byte value + 1-byte null-terminated, not checked in prorobuf library! */
#define UID_BUFFER_SIZE 32
#define UID_BUFFER_SIZE_ACTUAL 31
/*IMPORTANT NOTE:Maximum size is : 31-byte value + 1-byte null-terminated, not checked in prorobuf library! */
#define DATE_BUFFER_SIZE 32
#define DATE_BUFFER_SIZE_ACTUAL 31
/*IMPORTANT NOTE:Maximum size is : 31-byte value + 1-byte null-terminated, not checked in prorobuf library! */
#define MESSAGE_BUFFER_SIZE 32
#define MESSAGE_BUFFER_SIZE_ACTUAL 31
/*IMPORTANT NOTE:Maximum size is : 32-byte value + 1-byte null-terminated, not checked in prorobuf library! */
#define VOUCHER_BUFFER_SIZE 32
#define VOUCHER_BUFFER_SIZE_ACTUAL 31
/*IMPORTANT NOTE:Maximum size is : 31-byte value + 1-byte null-terminated, not checked in prorobuf library! */
#define CARD_SERIAL_SIZE 32
#define CARD_SERIAL_SIZE_ACTUAL 31
/*IMPORTANT NOTE:Maximum size is : 31-byte value + 1-byte null-terminated, not checked in prorobuf library! */
#define VALIDATION_BUFFER_SIZE 32
#define VALIDATION_BUFFER_SIZE_ACTUAL 31
/*IMPORTANT NOTE:Maximum size is : 31-byte value + 1-byte null-terminated, not checked in prorobuf library! */
#define WALLET_UID_BUFFER_SIZE 32
#define WALLET_UID_BUFFER_SIZE_ACTUAL 31
/*IMPORTANT NOTE:Maximum size is : 32-byte value + 1-byte null-terminated, not checked in prorobuf library! */
#define RRN_ID_SIZE 32
#define RRN_ID_SIZE_ACTUAL 31
/*IMPORTANT NOTE:Maximum size is : 8-byte value(64bit) + 1-byte null-terminated, not checked in prorobuf library! */
#define HASH_KEY_BLOCK2_SIZE 8 + 1

#define RPC_VOID void
#define RPC_ERROR_CTX error_context_t *
#ifdef __cplusplus
#define NO_ERROR nullptr
using protohdr_t = struct msghdr;
using protobuf_t = struct iovec;
#else
#define NO_ERROR NULL
#endif

/*this def type needed only in device */
#ifdef END_POINT_IS_DEVICE
#define byte8_t char
#endif

#define ARMABI_V7A 1 // because device lenz-e60 CPU ABI armeabi-v7a - 32 bit arm architecture
#ifdef ARMABI_V7A
#ifndef RTE_CACHELINE
#define RTE_CACHELINE 32
#endif
#endif

#define __rte_align_cache_line __attribute__((__aligned__(RTE_CACHELINE)))
#define MAGIC_BYTE 0xbebfbcc7c7bcbfbe // 8
#define MSB_BIT_MASK_MAGIC 0xFFFFFFFF00000000
#define LSB_BIT_MASK_CRC 0x0000000000000FFF
#define ACK_MAGIC_BYTE "c2c3c4c5d2d3d4d5"    // 8
#define ACK_MAGIC_BYTE_LE "5d4d3d2d5c4c3c2c" // 8
#define MISS_MAGIC_BYTE 0xc1c1c1c1d3d3d3d3   // 8
#define BONDING_BYTE "1@1@1@1@1@1@1@#"
#define CRC32_PATTERN "????????????????"
#define MESSAGE_FRAME_CORRUPTED "fefe"
#define RES_CURL(A) (A << 15)
#define RES_CURL_EXTRACT(A) (A >> 15)
#define RES_RSW(A) (A & 0x0000FFFF)
#define MAGIC_BYTE_SIZE 8
#define RSW_ETH_ALEN 32 /*redefine Octets in one ethernet addr, original define in if_ether.h	 */
#define CRC32_BUFFER_LEN 32
#define REMOTE_FUNCTION_NAME_SIZE 32
#define MAX_INPUT_MODEL_SIZE 1024
#define TABLE_SIZE 32
#define BONDING_BYTE_SIZE 16
#define MAX_PROTO_BUF_MODEL_SIZE (MAX_INPUT_MODEL_SIZE - (MAGIC_BYTE_SIZE + RSW_ETH_ALEN + CRC32_BUFFER_LEN + REMOTE_FUNCTION_NAME_SIZE))
#define MAX_BUF_FOR_STREAM 2048
#define MAX_BUF_FOR_SENDING (MAX_INPUT_MODEL_SIZE - (MAGIC_BYTE_SIZE + RSW_ETH_ALEN + CRC32_BUFFER_LEN + REMOTE_FUNCTION_NAME_SIZE))
#define CONFIG_HOST_BUFFER_SIZE 32
#define WALLET_SERVICE_NO_RESPOND -1

/**
 * @brief table size of fields in protobuf stream
 *
 * @return typedef struct
 */
typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /** @brief  */
    uint8_t table[TABLE_SIZE];
} table_size_t;

/**
 * @brief payload structure for protobuf stream
 *
 */
typedef struct
{
    /** @brief  */
    uint64_t magic_buf;
    /** @brief  */
    uint64_t crc_chk;
    /** @brief  */
    table_size_t header;
    /** @brief  */
    byte8_t stream[MAX_BUF_FOR_SENDING];
} pay_load_t;

/**
 * @brief Proxy Library config context.
 *
 */
typedef struct
{
    uint32_t receive_buffer_size;
    uint32_t send_buffer_size;
    uint32_t interval_transaction_timeout;
    uint32_t ticket_timer_timeout;
    uint32_t send_try_again;
    byte8_t rsw_host_main[CONFIG_HOST_BUFFER_SIZE];
    byte8_t rsw_port_main[CONFIG_HOST_BUFFER_SIZE];
    byte8_t rsw_host_standby1[CONFIG_HOST_BUFFER_SIZE];
    byte8_t rsw_port_standby1[CONFIG_HOST_BUFFER_SIZE];
    byte8_t rsw_host_standby2[CONFIG_HOST_BUFFER_SIZE];
    byte8_t rsw_port_standby2[CONFIG_HOST_BUFFER_SIZE];
    byte8_t rsw_host_standby3[CONFIG_HOST_BUFFER_SIZE];
    byte8_t rsw_port_standby3[CONFIG_HOST_BUFFER_SIZE];
} rswConfigContext_t;

/**
 * @brief error code for proxy library.
 *
 */
typedef enum
{
    DONE_PROC = 0,
    ERR_INSTANCE_IS_NULL = 1,
    ERR_INPUT_MODEL_IS_NULL = 2,
    ERR_OUTPUT_MODEL_IS_NULL = 3,
    ERR_SERVER_INSTANCE_FAILED = 4,
    ERR_HANDLE_INVALID = 5,
    ERR_SO_RCVTIMEO = 6,
    ERR_SO_SNDTIMEO = 7,
    ERR_SO_RCVLOWAT = 8,
    ERR_IN_NETWORK_CONNECTION = 9,
    ERR_ADDR_INFO_TRANSLATE = 10,
    ERR_IN_CONNECT_SERVER = 11,
    ERR_IN_SOCKET_HANDLE = 12,
    ERR_MAC_ADDR_NOT_FOUND = 13,
    ERR_DEVICE_LIBRARY_LOAD = 14,
    ERR_DEVICE_LIBRARY_SYMBOL = 15,
    ERR_SEND_BYTE_IS_ZERO = 16,
    ERR_RECV_BYTE_IS_ZERO = 17,
    ERR_SO_ZEROCOPY = 18,
    ERR_RECV_TIMEOUT = 19,
    ERR_SERIAL_HW = 20,
    ERR_SERVER_LAST_INIT = 21,
    ERR_SERVER_LAST_DEINIT = 22,
    ERR_MAC_ADDR_IS_NULL = 23,
    ERR_STREAM_CORRUPTED = 24,
    ERR_SO_RCVBUF = 25,
    ERR_SO_SNDBUF = 26,
    ERR_GET_SO_SNDBUF = 27,
    ERR_NOT_SET_SENDBUF = 28,
    ERR_RSW_PEER_RESET = 29,
    EVENT_HANDLE_ERROR = 30,
    TIMER_EVENT_ERROR = 31,
    ERR_RSW_CONFIG_IS_NULL = 32
} error_num_t;

/**
 * @brief responce error code from RSW.
 *
 */
typedef enum
{
    REQUEST_WITH_CURL_ERROR = 1100,
    REQUEST_IS_OK = 1200,          // transaction process is okay
    REQUEST_TIMEOUT_ISSUER = 1300, //
    REQUEST_TIMEOUT_CORE = 1400,   // transaction failed because pcap_core_service can't handle transaction_msg
    RESPONSE_JSON_ERROR = 1500
} server_req_status_t;

/**
 * @brief error context for error handling by protobuf library
 *
 */
typedef struct
{
    error_num_t error_num;
    int error_no;
} error_context_t;

/**
 * @brief index table for used when serialize entity model to protobuf stream
 *
 */
typedef enum
{
#define MAGIC_KEY_IDX_ 0
    MAGIC_KEY_IDX = MAGIC_KEY_IDX_,
#define CRC_SIZE_IDX_ 1
    CRC_SIZE_IDX = CRC_SIZE_IDX_,
#define TABLE_SIZE_IDX_ 2
    TABLE_SIZE_IDX = TABLE_SIZE_IDX_,
#define MAC_ADDR_IDX_ 3
    MAC_ADDR_IDX = MAC_ADDR_IDX_,
#define CRC32_BUF_IDX_ 4
    CRC32_BUF_IDX = CRC32_BUF_IDX_,
#define RPC_FUNC_IDX_ 5
    RPC_FUNC_IDX = RPC_FUNC_IDX_,
#define MODE_IDX_1_ RPC_FUNC_IDX_ + 1
    MODEL_IDX_1 = MODE_IDX_1_,
#define MODE_IDX_2_ RPC_FUNC_IDX_ + 2
    MODEL_IDX_2 = MODE_IDX_2_,
#define MODE_IDX_3_ RPC_FUNC_IDX_ + 3
    MODEL_IDX_3 = MODE_IDX_3_,
#define MODE_IDX_4_ RPC_FUNC_IDX_ + 4
    MODEL_IDX_4 = MODE_IDX_4_,
#define MODE_IDX_5_ RPC_FUNC_IDX_ + 5
    MODEL_IDX_5 = MODE_IDX_5_,
#define MODE_IDX_6_ RPC_FUNC_IDX_ + 6
    MODEL_IDX_6 = MODE_IDX_6_,
#define MODE_IDX_7_ RPC_FUNC_IDX_ + 7
    MODEL_IDX_7 = MODE_IDX_7_,
#define MODE_IDX_8_ RPC_FUNC_IDX_ + 8
    MODEL_IDX_8 = MODE_IDX_8_,
#define MODE_IDX_9_ RPC_FUNC_IDX_ + 9
    MODEL_IDX_9 = MODE_IDX_9_,
#define MODE_IDX_10_ RPC_FUNC_IDX_ + 10
    MODEL_IDX_10 = MODE_IDX_10_,
#define MODE_IDX_11_ RPC_FUNC_IDX_ + 11
    MODEL_IDX_11 = MODE_IDX_11_,
#define MODE_IDX_12_ RPC_FUNC_IDX_ + 12
    MODEL_IDX_12 = MODE_IDX_12_,
#define MODE_IDX_13_ RPC_FUNC_IDX_ + 13
    MODEL_IDX_13 = MODE_IDX_13_,
#define MODE_IDX_14_ RPC_FUNC_IDX_ + 14
    MODEL_IDX_14 = MODE_IDX_14_,
#define MODE_IDX_15_ RPC_FUNC_IDX_ + 15
    MODEL_IDX_15 = MODE_IDX_15_,
#define MODE_IDX_16_ RPC_FUNC_IDX_ + 16
    MODEL_IDX_16 = MODE_IDX_16_,
#define MODE_IDX_17_ RPC_FUNC_IDX_ + 17
    MODEL_IDX_17 = MODE_IDX_17_,
#define MODE_IDX_18_ RPC_FUNC_IDX_ + 18
    MODEL_IDX_18 = MODE_IDX_18_,
#define MODE_IDX_19_ RPC_FUNC_IDX_ + 19
    MODEL_IDX_19 = MODE_IDX_19_,
#define MODE_IDX_20_ RPC_FUNC_IDX_ + 20
    MODEL_IDX_20 = MODE_IDX_20_,
#define MODE_IDX_21_ RPC_FUNC_IDX_ + 21
    MODEL_IDX_21 = MODE_IDX_21_,
#define MODE_IDX_22_ RPC_FUNC_IDX_ + 22
    MODEL_IDX_22 = MODE_IDX_22_,
#define MODE_IDX_23_ RPC_FUNC_IDX_ + 23
    MODEL_IDX_23 = MODE_IDX_23_,
#define MODE_IDX_24_ RPC_FUNC_IDX_ + 24
    MODEL_IDX_24 = MODE_IDX_24_,
#define MODE_IDX_25_ RPC_FUNC_IDX_ + 25
    MODEL_IDX_25 = MODE_IDX_25_,
#define MODE_IDX_26_ RPC_FUNC_IDX_ + 26
    MODEL_IDX_26 = MODE_IDX_26_,
} table_index_t;

#ifdef __cplusplus
#define DEFINE_API_ROUTINE_INOUT(ROUTINE_NAME, TYPE, IN, OUT) extern "C" RPC_ERROR_CTX ROUTINE_NAME(const IN##TYPE *const *, OUT##TYPE **);
#else
#define DEFINE_API_ROUTINE_INOUT(ROUTINE_NAME, TYPE, IN, OUT) RPC_ERROR_CTX ROUTINE_NAME(const IN##TYPE *const *, OUT##TYPE **);
#endif

/*C style API function prototype
  Important: this helper functions bounded into independent shared library for
             proxy with protobuf in router switch server and device terminal.
 */

/**
 * @brief کد پیغام
 *
 */
typedef enum
{
    INITIALIZER_REQ_MSG = 0,
    MSG_1 = 1,
    MSG_2 = 2
} request_message_t;

/**
 * @brief تایپ بلیت voucher
 *
 */
typedef enum
{
    INITIALIZER_VOUCHER_TYPE = 0,
    V_1 = 1,
    V_2 = 2
} voucher_type_t;

/**
 * @brief o	وضعیت تراکنش
 *
 */
typedef enum
{
    INITIALIZER_TRANS_STATUS = 0,
    STATE1 = 0,
    STATE2 = 1
} transaction_status_t;

/**
 * @brief o	کد وضعیت تراکنش
 *
 */
typedef enum
{
    INITIALIZER_STATUS_CODE = 0,
    TRANS_SUCCESS = 100,              // تراکنش موفق
    TRANS_CANCELED_USER = 200,        // تراکنش کنسل از سمت کاربر
    TRANS_FAILED_SWITCH = 300,        // تراکنش ناموفق - خطای تراکنش سمت سوئیچ
    TRANS_FAILED_WALLET = 301,        // تراکنش ناموفق - خطای تراکنش سمت کیف پول
    TRANS_FAILED_ONLINE_SWITCH = 302, // تراکنش ناموفق - خطای تراکنش سمت سامانه کارت بلیت آنلاین
    TRANS_FAILED_DEVICE = 303         // تراکنش ناموفق - خطای تراکنش سمت سامانه کارت بلیت آنلاین
} transaction_status_code_t;

/**
 * @brief کد نوع تراکنش
 *
 */
typedef enum
{
    INITIALIZER_TRANS_TYPE = 0,
    CARD_CONSUME_TRANSACTION = 220013, // تراکنش مصرف کارت
    CARD_ISSUANCE = 220043,            // صدور کارت
    CARD_CHARGE = 220060,              // شارژ کارت
    CARD_CHARGE_CANCEL = 220061,       // لغو شارژ کارت
    CARD_REFUND = 220073,              // استرداد کارت
    CARD_BLOCK = 220075,               // مسدود کردن کارت
    TOTAL_SETTLEMENT = 220090,         // تسویه تام
    LOGIN = 220111,                    // login
    LOGOUT = 220112,                   // logout
    PRESENCE = 220113,                 // presence
    CARD_BANK_BUY = 220014,            // خرید کارت بانکی
    CARD_CONSUME_ID = 320040,          // مصرف شناسه کارت
    CONSUME_QR = 320041,               // مصرف شناسه QR
    CONSUME_VOUCHER = 320043,          // مصرف ووچر
    CHARGE_CARD_ID = 320061,           // شارژ شناسه کارت
    CHARGE_QR_ID = 320062,             // شارژ شناسه QR
    MOBILE_PIN = 444444,               // پین موبایل
} transaction_type_t;

/**
 * @brief نام صادر کننده
 *
 */
typedef enum
{
    INITIALIZER_ISSUER_TYPE = 0,
    EASY_CITY_TEHRAN = 01,       // ایزی سیتی تهران
    EASY_CITY_MASHHAD = 02,      // ایزی سیتی مشهد
    EASY_CITY_QOM = 03,          // ایزی سیتی قم
    EASY_CITY_AHWAZ = 04,        // ایزی سیتی اهواز
    EASY_CITY_CLOUD_RAILWAY = 41 // ایزی سیتی کلود راه آهن
} issuer_type_t;

/**
 * @brief کمپانی
 *
 */
typedef enum
{
    INITIALIZER_COMPANY_CODE = 0,
    MUNICIPALITY_TEHRAN = 001,     // شهرداری تهران
    MUNICIPALITY_ESLAMSHARE = 002, // شهرداری اسلامشهر
    MUNICIPALITY_PAKDASHT = 003,   // شهرداری پاکدشت
    MUNICIPALITY_VARAMIN = 004,    // شهرداری ورامین
    MUNICIPALITY_MASHHAD = 020,    // شهرداری مشهد
    MUNICIPALITY_QOM = 030,        // شهرداری قم
    MUNICIPALITY_AHWAZ = 040,      // شهرداری اهواز
    COMPANY_RAILWAY = 300,         // شرکت راه آهن
} company_code_t;

/**
 * @brief o	وضعیت احراز
 *
 */
typedef enum
{
    UNKNOWN_APPROVED = 0,                      // ناشناس
    NATIONAL_CODE_WITHOUT_APPROVED = 1,        // خوداظهاری کد ملی بدون تایید
    NATIONAL_CODE_APPROVED_WITH_SMS = 2,       // خود اظهاری کد ملی و تایید پیامکی
    NATIONAL_CODE_WITH_PRESENTED_APPROVED = 5, // تایید حضوری کد ملی و شماره موبایل
    APPROVED_WITH_SHAHKAR = 6,                 // تایید شاهکار
    INITIALIZER_VERIFY_STATUS = 7
} verification_status_t;

/**
 * @brief دسته کاربری
 *
 */
typedef enum
{
    INITIALIZER_USER_GROUP = 0,
    NORMAL_USERS = 50,             // کاربران عادی
    PROXY_USERS = 55,              // کاربران نیابت
    MUNICIPAL_EMPLOYEES_USERS = 60 // کاربران کارمند شهرداری
} user_group_t;

/**
 * @section RPC_RequestForConnect
 * @brief After being called by the device, the function compares the device serial with the existing serials.
 *        If the device is among the approved devices,
 *        it creates a communication channel with the device and sends the following outputs to the device.
 *        The validation string is generated by the device. This information string is the MAC address of the device.
 */
typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*1:*/ byte8_t serial_hw[SERIAL_HW_FIELD_SIZE];             // سریال فیزیکی دستگاه
    /*2:*/ byte8_t validation_buffer[REQUEST_INPUT_FIELD_SIZE]; // رشته اعتبارسنجی دستگاه
} InputRequestForConnect_t;

typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*0:*/ int32_t rsw_request_status;                    // request status from RSW
    /*1:*/ byte8_t serial_logic_hw[SERIAL_HW_FIELD_SIZE]; // سریال منطقی دستگاه (8 رقم)
    /*2:*/ byte8_t pub_key[REQUEST_OUTPUT_FIELD_SIZE];    // کلید عمومی
    /*3:*/ byte8_t token[REQUEST_OUTPUT_FIELD_SIZE];      // توکن
} OutputRequestForConnect_t;

// RPC_ERROR_CTX RequestForConnect(const InputRequestForConnect_t *const *, OutputRequestForConnect_t **);
#define RPC_REQUEST_FOR_CONNECT "ReqForCon"
DEFINE_API_ROUTINE_INOUT(RequestForConnect, RequestForConnect_t, Input, Output);

/*----------------------------------------------------------------------------------------------------------------------------------*/
/**
 * @section RequestForKeys
 * @brief If the token is valid,
 *        the function will send the 256-bit AES256 key and the 256-bit MD5 key to the device.
 */
typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*1:*/ byte8_t serial_hw[SERIAL_HW_FIELD_SIZE];       // سریال فیزیکی دستگاه
    /*2:*/ byte8_t serial_logic_hw[SERIAL_HW_FIELD_SIZE]; // سریال منطقی دستگاه
    /*3:*/ byte8_t token[REQUEST_OUTPUT_FIELD_SIZE];      // توکن
} InputRequestForKeys_t;

typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*0:*/ int32_t rsw_request_status;                  // request status from RSW
    /*1:*/ byte8_t AES256_serial_key[REQUEST_KEY_SIZE]; // کلید 256 بیتی AES256 برای رشته 14 رقمی
    /*2:*/ byte8_t AES256_qr_key[REQUEST_KEY_SIZE];     // کلید 256 بیتی AES256 برای کل QR
    /*3:*/ byte8_t MD5_salt[REQUEST_KEY_SIZE];          // کلید(Salt) 256 بیتی MD5 برای هش  ترمینال کارت بلیت
} OutputRequestForKeys_t;

// RPC_ERROR_CTX RequestForKeys(const InputRequestForKeys_t *const *, OutputRequestForKeys_t **);
#define RPC_REQUEST_FOR_KEYS "ReqForKey"
DEFINE_API_ROUTINE_INOUT(RequestForKeys, RequestForKeys_t, Input, Output);

/*----------------------------------------------------------------------------------------------------------------------------------*/
/**
 * @section RedirectToIssuer
 * @brief Based on the pattern defined in the switch,
 *        the switch detects the issuer and directs the transaction
 *        to the issuer and returns the result to the device.
 */
typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*1:*/ uint32_t transaction_fee;                   // مبلغ تراکنش
    /*2:*/ byte8_t transaction_date[DATE_BUFFER_SIZE]; // تاریخ تراکنش

    /** @brief transaction_UID: Time stamp (10 digits) + logical serial number of the device (8 digits) */
    /*3:*/ byte8_t transaction_UID[UID_BUFFER_SIZE];      // شناسه یکتای عملیات
    /*4:*/ byte8_t serial_hw[SERIAL_HW_FIELD_SIZE];       // سریال فیزیکی دستگاه
    /*5:*/ byte8_t serial_logic_hw[SERIAL_HW_FIELD_SIZE]; // سریال منطقی دستگاه
    /*6:*/ byte8_t QR_buffer[QR_BUFFER_SIZE];             // رشته QR
} InputRedirectToIssuer_t;

typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*0:*/ int32_t rsw_request_status;                      // request status from RSW
    /*1:*/ request_message_t msg_type;                      // کد پیغام
    /*2:*/ int64_t wallet_payment_rest;                     // مانده کیف پول
    /*3:*/ byte8_t wallet_effective_date[DATE_BUFFER_SIZE]; // تاریخ درج در کیف پول

    /** @brief server_UID: Random number (10 digits) + logical serial number of the device (8 digits) */
    /*4:*/ byte8_t server_UID[UID_BUFFER_SIZE]; // شناسه یکتای سرور

    /** @brief transaction_UID: Time stamp (10 digits) + logical serial number of the device (8 digits) */
    /*5:*/ byte8_t transaction_UID[UID_BUFFER_SIZE];    // شناسه یکتای عملیات
    /*6:*/ byte8_t message_buffer[MESSAGE_BUFFER_SIZE]; // پیغام
} OutputRedirectToIssuer_t;
// RPC_ERROR_CTX RedirectToIssuer(const InputRedirectToIssuer_t *const *, OutputRedirectToIssuer_t **);
#define RPC_REDIRECT_TO_ISSUER "RedirToIssu"
DEFINE_API_ROUTINE_INOUT(RedirectToIssuer, RedirectToIssuer_t, Input, Output);

/*----------------------------------------------------------------------------------------------------------------------------------*/
/**
 * @section TerminalConsumeVoucher
 * @brief consumer function worked with voucher ticket
 *
 */
typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*1:*/ voucher_type_t voucher_type; // o	تایپ بلیت
    /*2:*/ int64_t voucher_serial;      // o	سریال ووچر
    /*3:*/ int64_t voucher_fee;         // o	مبلغ
    /*4:*/ double latitude;             // o	نقطه x
    /*5:*/ double longitude;            // o	نقطه y

    /** @brief transaction_UID: Time stamp (10 digits) + logical serial number of the device (8 digits) */
    /*6:*/ byte8_t transaction_UID[UID_BUFFER_SIZE];            // o	شناسه یکتای عملیات
    /*7:*/ byte8_t serial_logic_hw[SERIAL_HW_FIELD_SIZE];       // o	سریال منطقی دستگاه
    /*8:*/ byte8_t serial_hw[SERIAL_HW_FIELD_SIZE];             // o	سریال فیزیکی دستگاه
    /*9:*/ byte8_t voucher_message_buffer[VOUCHER_BUFFER_SIZE]; // o	رشته ووچر
} InputTerminalConsumeVoucher_t;

typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*0:*/ int32_t rsw_request_status;         // request status from RSW
    /*1:*/ request_message_t voucher_msg_type; // o	کد پیغام

    /** @brief server_UID: Random number (10 digits) + logical serial number of the device (8 digits) */
    /*2:*/ byte8_t server_UID[UID_BUFFER_SIZE]; // o	شناسه یکتای سرور

    /** @brief transaction_UID: Time stamp (10 digits) + logical serial number of the device (8 digits) */
    /*3:*/ byte8_t transaction_UID[UID_BUFFER_SIZE];    // o	شناسه یکتای عملیات
    /*4:*/ byte8_t message_buffer[MESSAGE_BUFFER_SIZE]; // o	پیغام
} OutputTerminalConsumeVoucher_t;
// RPC_ERROR_CTX TerminalConsumeVoucher(const InputTerminalConsumeVoucher_t *const *, OutputTerminalConsumeVoucher_t **);
#define RPC_TERMINAL_CONSUME_VOUCHER "TerConVou"
DEFINE_API_ROUTINE_INOUT(TerminalConsumeVoucher, TerminalConsumeVoucher_t, Input, Output);

/*----------------------------------------------------------------------------------------------------------------------------------*/
/**
 * @section TerminalCardConsume
 * @brief The function first adds the serial number of the ticket card with the 14-digit number of the first string
 *        and the result with the 14-digit string of the wallet ID,
 *        and hashes the result with the MD5 algorithm of the key that exists only at the terminal.
 *        Compares the first 8 bytes of the generated hash with the 8 bytes of the hash sent by the function.
 *        If these two values are equal, it will enter the next step, and otherwise it will return the error "invalidity of the ticket card on the server side".
 *        If the validity of the server-side card is confirmed,
 *        the transaction will be queued to be sent to the wallet, and the debit function from the wrapper wallet will be called.
 *
 */
typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*1:*/ transaction_type_t transaction_type;            // o	نوع تراکنش
    /*2:*/ company_code_t company_id;                      // o	کد کمپانی (کد شهر سابق)
    /*3:*/ user_group_t user_group_id;                     // o	دسته کاربری (تایپ کارت سابق)
    /*4:*/ verification_status_t verification_status_id;   // o	وضعیت احراز
    /*5:*/ issuer_type_t issuer_id;                        // o	صادرکننده
    /*6:*/ uint32_t transaction_fee;                       // o	مبلغ تراکنش
    /*7:*/ uint32_t transaction_counter;                   // o	شمارنده تراکنش(روی کارت-seq)
    /*8:*/ long double latitude;                           // o	نقطه x
    /*9:*/ long double longitude;                          // o	نقطه y
    /*10:*/ byte8_t hash_key_block2[HASH_KEY_BLOCK2_SIZE]; // o	هش سرور (8 بایت دوم بلاک 3)
    /*11:*/ byte8_t expired_date_id[DATE_BUFFER_SIZE];     // o	تاریخ انقضا شناسه
    /*12:*/ byte8_t transaction_date[DATE_BUFFER_SIZE];    // o	تاریخ تراکنش

    /** @brief transaction_UID: Time stamp (10 digits) + logical serial number of the device (8 digits) */
    /*13:*/ byte8_t transaction_UID[UID_BUFFER_SIZE];          // o	شناسه یکتای عملیات
    /*14:*/ byte8_t wallet_uid_buffer[WALLET_UID_BUFFER_SIZE]; // o	رشته شناسه کیف پول
    /*15:*/ byte8_t validation_buffer[VALIDATION_BUFFER_SIZE]; // o	رشته 14 رقمی اعتبارسنجی
    /*16:*/ byte8_t card_serial[CARD_SERIAL_SIZE];             // o	سریال کارت بلیت
    /*17:*/ byte8_t serial_hw[SERIAL_HW_FIELD_SIZE];           // o	سریال دستگاه
    /*18:*/ byte8_t serial_logic_hw[SERIAL_HW_FIELD_SIZE];     // o	سریال منطقی دستگاه

} InputTerminalCardConsume_t;

typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*0:*/ int32_t rsw_request_status;                      // request status from RSW
    /*1:*/ request_message_t msg_type;                      // o	کد پیغام
    /*2:*/ int64_t wallet_payment_rest;                     // مانده کیف پول
    /*3:*/ byte8_t wallet_effective_date[DATE_BUFFER_SIZE]; // تاریخ درج در کیف پول

    /** @brief server_UID: Random number (10 digits) + logical serial number of the device (8 digits) */
    /*4:*/ byte8_t server_UID[UID_BUFFER_SIZE]; // o	شناسه یکتای سرور

    /** @brief transaction_UID: Time stamp (10 digits) + logical serial number of the device (8 digits) */
    /*5:*/ byte8_t transaction_UID[UID_BUFFER_SIZE];    // o	شناسه یکتای عملیات
    /*6:*/ byte8_t message_buffer[MESSAGE_BUFFER_SIZE]; // o	پیغام
} OutputTerminalCardConsume_t;

// RPC_ERROR_CTX TerminalCardConsume(const InputTerminalCardConsume_t *const *, OutputTerminalCardConsume_t **);
#define RPC_TERMINAL_CARD_CONSUME "TermCardCon"
DEFINE_API_ROUTINE_INOUT(TerminalCardConsume, TerminalCardConsume_t, Input, Output);

/*----------------------------------------------------------------------------------------------------------------------------------*/
/**
 * @section TerminalGroupCardConsume
 * @brief
 *
 * @return typedef struct
 */
typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*1:*/ transaction_type_t transaction_type;            // o	نوع تراکنش
    /*2:*/ company_code_t company_id;                      // o	کد کمپانی (کد شهر سابق)
    /*3:*/ user_group_t user_group_id;                     // o	دسته کاربری (تایپ کارت سابق)
    /*4:*/ verification_status_t verification_status_id;   // o	وضعیت احراز
    /*5:*/ issuer_type_t issuer_id;                        // o	صادرکننده
    /*6:*/ uint32_t transaction_fee;                       // o	مبلغ تراکنش
    /*7:*/ uint32_t transaction_counter;                   // o	شمارنده تراکنش(روی کارت-seq)
    /*8:*/ long double latitude;                           // o	نقطه x
    /*9:*/ long double longitude;                          // o	نقطه y
    /*10:*/ byte8_t hash_key_block2[HASH_KEY_BLOCK2_SIZE]; // o	هش سرور (8 بایت دوم بلاک 3)
    /*11:*/ byte8_t expired_date_id[DATE_BUFFER_SIZE];     // o	تاریخ انقضا شناسه
    /*12:*/ byte8_t transaction_date[DATE_BUFFER_SIZE];    // o	تاریخ تراکنش

    /** @brief transaction_UID: Time stamp (10 digits) + logical serial number of the device (8 digits) */
    /*13:*/ byte8_t transaction_UID[UID_BUFFER_SIZE];          // o	شناسه یکتای عملیات
    /*14:*/ byte8_t validation_buffer[VALIDATION_BUFFER_SIZE]; // o	رشته 14 رقمی اعتبارسنجی
    /*15:*/ byte8_t wallet_uid_buffer[WALLET_UID_BUFFER_SIZE]; // o	رشته شناسه کیف پول
    /*16:*/ byte8_t card_serial[CARD_SERIAL_SIZE];             // o	سریال کارت بلیت
    /*17:*/ byte8_t serial_hw[SERIAL_HW_FIELD_SIZE];           // o	سریال دستگاه
    /*18:*/ byte8_t serial_logic_hw[SERIAL_HW_FIELD_SIZE];     // o	سریال منطقی دستگاه
} InputTerminalGroupCardConsume_t;

typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*0:*/ int32_t rsw_request_status;                      // request status from RSW
    /*1:*/ request_message_t msg_type;                      // o	کد پیغام
    /*2:*/ int64_t wallet_payment_rest;                     // o  مانده کیف پول
    /*3:*/ byte8_t wallet_effective_date[DATE_BUFFER_SIZE]; // o  تاریخ درج در کیف پول

    /** @brief server_UID: Random number (10 digits) + logical serial number of the device (8 digits) */
    /*4:*/ byte8_t server_UID[UID_BUFFER_SIZE]; // o	شناسه یکتای سرور

    /** @brief transaction_UID: Time stamp (10 digits) + logical serial number of the device (8 digits) */
    /*5:*/ byte8_t transaction_UID[UID_BUFFER_SIZE];    // o	شناسه یکتای عملیات
    /*6:*/ byte8_t message_buffer[MESSAGE_BUFFER_SIZE]; // o	پیغام
} OutputTerminalGroupCardConsume_t;

// RPC_ERROR_CTX TerminalGroupCardConsume(const InputTerminalGroupCardConsume_t *const *, OutputTerminalGroupCardConsume_t **);
#define RPC_TERMINAL_GROUP_CARD_CONSUME "TermGrpCCon"
DEFINE_API_ROUTINE_INOUT(TerminalGroupCardConsume, TerminalGroupCardConsume_t, Input, Output);

/*----------------------------------------------------------------------------------------------------------------------------------*/
/**
 * @section TerminalQRConsume
 * @brief The time function compares the Timestamp with the current time if its value is less than n (n is in the configuration of the switch),
 *         the transaction is placed in the queue to be sent to the wallet,
 *         and the subtraction function from the rapper wallet is called.
 *
 */
typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*1:*/ transaction_type_t transaction_type;          // o	نوع تراکنش
    /*2:*/ company_code_t company_id;                    // o	کد کمپانی (کد شهر سابق)
    /*3:*/ user_group_t user_group_id;                   // o	دسته کاربری (تایپ کارت سابق)
    /*4:*/ verification_status_t verification_status_id; // o	وضعیت احراز
    /*5:*/ int32_t steeringhandl_id;                     // o	دسته هدایت
    /*6:*/ issuer_type_t issuer_id;                      // o	صادرکننده
    /*7:*/ uint32_t transaction_fee;                     // o	مبلغ تراکنش
    /*8:*/ uint32_t transaction_counter;                 // o	شمارنده تراکنش(روی کارت-seq)
    /*9:*/ long double latitude;                         // o	نقطه x
    /*10:*/ long double longitude;                       // o	نقطه y
    /*11:*/ int64_t timestamp;                           // o	TimeStamp روی رشته QR
    /*12:*/ byte8_t expired_date_id[DATE_BUFFER_SIZE];   // o	تاریخ انقضا شناسه
    /*13:*/ byte8_t transaction_date[DATE_BUFFER_SIZE];  // o	تاریخ تراکنش

    /** @brief transaction_UID: Time stamp (10 digits) + logical serial number of the device (8 digits) */
    /*14:*/ byte8_t transaction_UID[UID_BUFFER_SIZE];          // o	شناسه یکتای عملیات
    /*15:*/ byte8_t validation_buffer[VALIDATION_BUFFER_SIZE]; // o	رشته 14 رقمی اعتبارسنجی
    /*16:*/ byte8_t wallet_uid_buffer[WALLET_UID_BUFFER_SIZE]; // o	رشته شناسه کیف پول
    /*17:*/ byte8_t card_serial[CARD_SERIAL_SIZE];             // o	سریال کارت بلیت
    /*18:*/ byte8_t serial_hw[SERIAL_HW_FIELD_SIZE];           // o	سریال دستگاه
    /*19:*/ byte8_t serial_logic_hw[SERIAL_HW_FIELD_SIZE];     // o	سریال منطقی دستگاه
} InputTerminalQRConsume_t;

typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*0:*/ int32_t rsw_request_status;                      // request status from RSW
    /*1:*/ request_message_t msg_type;                      // o	کد پیغام
    /*2:*/ int64_t wallet_payment_rest;                     // o  مانده کیف پول
    /*3:*/ byte8_t wallet_effective_date[DATE_BUFFER_SIZE]; // o  تاریخ درج در کیف پول

    /** @brief server_UID: Random number (10 digits) + logical serial number of the device (8 digits) */
    /*4:*/ byte8_t server_UID[UID_BUFFER_SIZE]; // o	شناسه یکتای سرور

    /** @brief transaction_UID: Time stamp (10 digits) + logical serial number of the device (8 digits) */
    /*5:*/ byte8_t transaction_UID[UID_BUFFER_SIZE];    // o	شناسه یکتای عملیات
    /*6:*/ byte8_t message_buffer[MESSAGE_BUFFER_SIZE]; // o	پیغام
} OutputTerminalQRConsume_t;

// RPC_ERROR_CTX TerminalQRConsume(const InputTerminalQRConsume_t *const *, OutputTerminalQRConsume_t **);
#define RPC_TERMINAL_QR_CONSUME "TermQRCon"
DEFINE_API_ROUTINE_INOUT(TerminalQRConsume, TerminalQRConsume_t, Input, Output);

/*----------------------------------------------------------------------------------------------------------------------------------*/
/**
 * @section TerminalGroupQRConsume
 * @brief The time function compares the Timestamp with the current time if its value is less than n (n is in the configuration of the switch),
 *         the transaction is placed in the queue to be sent to the wallet,
 *         and the subtraction function from the rapper wallet is called.
 *
 */
typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*1:*/ transaction_type_t transaction_type;          // o	نوع تراکنش
    /*2:*/ company_code_t company_id;                    // o	کد کمپانی (کد شهر سابق)
    /*3:*/ user_group_t user_group_id;                   // o	دسته کاربری (تایپ کارت سابق)
    /*4:*/ verification_status_t verification_status_id; // o	وضعیت احراز
    /*5:*/ issuer_type_t issuer_id;                      // o	صادرکننده
    /*6:*/ int32_t steeringhandl_id;                     // o	دسته هدایت
    /*7:*/ uint32_t transaction_fee;                     // o	مبلغ تراکنش
    /*8:*/ uint32_t transaction_counter;                 // o	شمارنده تراکنش(روی کارت-seq)
    /*9:*/ long double latitude;                         // o	نقطه x
    /*10:*/ long double longitude;                       // o	نقطه y
    /*11:*/ int64_t timestamp;                           // o	TimeStamp روی رشته QR
    /*12:*/ byte8_t expired_date_id[DATE_BUFFER_SIZE];   // o	تاریخ انقضا شناسه
    /*13:*/ byte8_t transaction_date[DATE_BUFFER_SIZE];  // o	تاریخ تراکنش

    /** @brief transaction_UID: Time stamp (10 digits) + logical serial number of the device (8 digits) */
    /*14:*/ byte8_t transaction_UID[UID_BUFFER_SIZE];          // o	شناسه یکتای عملیات
    /*15:*/ byte8_t validation_buffer[VALIDATION_BUFFER_SIZE]; // o	رشته 14 رقمی اعتبارسنجی
    /*16:*/ byte8_t wallet_uid_buffer[WALLET_UID_BUFFER_SIZE]; // o	رشته شناسه کیف پول
    /*17:*/ byte8_t card_serial[CARD_SERIAL_SIZE];             // o	سریال کارت بلیت
    /*18:*/ byte8_t serial_hw[SERIAL_HW_FIELD_SIZE];           // o	سریال دستگاه
    /*19:*/ byte8_t serial_logic_hw[SERIAL_HW_FIELD_SIZE];     // o	سریال منطقی دستگاه
} InputTerminalGroupQRConsume_t;

typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*0:*/ int32_t rsw_request_status;                      // request status from RSW
    /*1:*/ request_message_t msg_type;                      // o	کد پیغام
    /*2:*/ int64_t wallet_payment_rest;                     // o  مانده کیف پول
    /*3:*/ byte8_t wallet_effective_date[DATE_BUFFER_SIZE]; // o  تاریخ درج در کیف پول

    /** @brief server_UID: Random number (10 digits) + logical serial number of the device (8 digits) */
    /*4:*/ byte8_t server_UID[UID_BUFFER_SIZE]; // o	شناسه یکتای سرور

    /** @brief transaction_UID: Time stamp (10 digits) + logical serial number of the device (8 digits) */
    /*5:*/ byte8_t transaction_UID[UID_BUFFER_SIZE];    // o	شناسه یکتای عملیات
    /*6:*/ byte8_t message_buffer[MESSAGE_BUFFER_SIZE]; // o	پیغام
} OutputTerminalGroupQRConsume_t;

// RPC_ERROR_CTX TerminalGroupQRConsume(const InputTerminalGroupQRConsume_t *const *, OutputTerminalGroupQRConsume_t **);
#define RPC_TERMINAL_GROUP_QR_CONSUME "TermGrQRCon"
DEFINE_API_ROUTINE_INOUT(TerminalGroupQRConsume, TerminalGroupQRConsume_t, Input, Output);

/*----------------------------------------------------------------------------------------------------------------------------------*/
/**
 * @section TerminalInquery
 * @brief By receiving the above inputs,
 *        the function checks the transaction status and answers the query.
 *
 */
typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*1:*/ byte8_t wallet_uid_buffer[WALLET_UID_BUFFER_SIZE]; // o	رشته شناسه کیف پول

    /** @brief server_UID: Random number (10 digits) + logical serial number of the device (8 digits) */
    /*2:*/ byte8_t server_UID[UID_BUFFER_SIZE]; // o	شناسه یکتای سرور

} InputTerminalInquery_t;

typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*0:*/ int32_t rsw_request_status;                        // request status from RSW
    /*1:*/ transaction_status_code_t transaction_status_code; // o	کد وضعیت تراکنش
    /*2:*/ transaction_status_t transaction_status;           // o	وضعیت تراکنش
    /*3:*/ uint32_t transaction_fee;                          // o	مبلغ تراکنش
    /*4:*/ byte8_t transaction_date[DATE_BUFFER_SIZE];        // o	تاریخ تراکنش
    /*5:*/ byte8_t serial_hw[SERIAL_HW_FIELD_SIZE];           // o	سریال دستگاه

    /** @brief server_UID: Random number (10 digits) + logical serial number of the device (8 digits) */
    /*6:*/ byte8_t server_UID[UID_BUFFER_SIZE];         // o	شناسه یکتای سرور
    /*7:*/ byte8_t message_buffer[MESSAGE_BUFFER_SIZE]; // o	پیغام وضعیت تراکنش
} OutputTerminalInquery_t;

// RPC_ERROR_CTX TerminalInquery(const InputTerminalInquery_t *const *, OutputTerminalInquery_t **);
#define RPC_TERMINAL_INQUERY "TermInqu"
DEFINE_API_ROUTINE_INOUT(TerminalInquery, TerminalInquery_t, Input, Output);

/*----------------------------------------------------------------------------------------------------------------------------------*/
/**
 * @section TerminalInqueryGroup
 * @brief The function receives an array of the above inputs and answers the query in an array.
 *
 */
typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*1:*/ byte8_t wallet_uid_buffer[WALLET_UID_BUFFER_SIZE]; // o	رشته شناسه کیف پول
    /*2:*/ byte8_t server_UID[UID_BUFFER_SIZE];               // o	شناسه یکتای سرور

} InputTerminalInqueryGroup_t;

typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*0:*/ int32_t rsw_request_status;                        // request status from RSW
    /*1:*/ transaction_status_code_t transaction_status_code; // o	کد وضعیت تراکنش
    /*2:*/ transaction_status_t transaction_status;           // o	وضعیت تراکنش
    /*3:*/ uint32_t transaction_fee;                          // o	مبلغ تراکنش
    /*4:*/ byte8_t transaction_date[DATE_BUFFER_SIZE];        // o	تاریخ تراکنش
    /*5:*/ byte8_t serial_hw[SERIAL_HW_FIELD_SIZE];           // o	سریال دستگاه

    /** @brief server_UID: Random number (10 digits) + logical serial number of the device (8 digits) */
    /*6:*/ byte8_t server_UID[UID_BUFFER_SIZE];         // o	شناسه یکتای سرور
    /*7:*/ byte8_t message_buffer[MESSAGE_BUFFER_SIZE]; // o	پیغام وضعیت تراکنش
} OutputTerminalInqueryGroup_t;
// RPC_ERROR_CTX TerminalInqueryGroup(const InputTerminalInqueryGroup_t *const *, OutputTerminalInqueryGroup_t **);
#define RPC_TERMINAL_INQUERY_GROUP "TermInquGrp"
DEFINE_API_ROUTINE_INOUT(TerminalInqueryGroup, TerminalInqueryGroup_t, Input, Output);

/*----------------------------------------------------------------------------------------------------------------------------------*/
/**
 * @section TerminalCardChargeWallet
 * @brief Validation of this part is done like deduction validation.
 *
 */
typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*1:*/ transaction_type_t transaction_type;                // o	نوع تراکنش
    /*2:*/ int32_t bank_switch_code;                           // o	کد سوییچ بانکی
    /*3:*/ company_code_t company_id;                          // o	کد کمپانی (کد شهر سابق)
    /*4:*/ user_group_t user_group_id;                         // o	دسته کاربری (تایپ کارت سابق)
    /*5:*/ verification_status_t verification_status_id;       // o	وضعیت احراز
    /*6:*/ issuer_type_t issuer_id;                            // o	صادرکننده
    /*7:*/ uint32_t transaction_fee;                           // o	مبلغ تراکنش
    /*8:*/ uint32_t transaction_counter;                       // o	شمارنده تراکنش(روی کارت-seq)
    /*9:*/ long double latitude;                               // o	نقطه x
    /*10:*/ long double longitude;                             // o	نقطه y
    /*11:*/ byte8_t hash_key_block2[HASH_KEY_BLOCK2_SIZE];     // o	هش سرور (8 بایت دوم بلاک 3)
    /*12:*/ byte8_t expired_date_id[DATE_BUFFER_SIZE];         // o	تاریخ انقضا شناسه
    /*13:*/ byte8_t transaction_date[DATE_BUFFER_SIZE];        // o	تاریخ تراکنش
    /*14:*/ byte8_t wallet_uid_buffer[WALLET_UID_BUFFER_SIZE]; // o	رشته شناسه کیف پول

    /** @brief transaction_UID: Time stamp (10 digits) + logical serial number of the device (8 digits) */
    /*15:*/ byte8_t transaction_UID[UID_BUFFER_SIZE];          // o	شناسه یکتای عملیات
    /*16:*/ byte8_t card_serial[CARD_SERIAL_SIZE];             // o	سریال کارت بلیت
    /*17:*/ byte8_t validation_buffer[VALIDATION_BUFFER_SIZE]; // o	رشته 14 رقمی اعتبارسنجی
    /*18:*/ byte8_t serial_hw[SERIAL_HW_FIELD_SIZE];           // o	سریال دستگاه
    /*19:*/ byte8_t serial_logic_hw[SERIAL_HW_FIELD_SIZE];     // o	سریال منطقی دستگاه
    /*20:*/ byte8_t RRN_id[RRN_ID_SIZE];                       // o	RRN بانکی
} InputTerminalCardChargeWallet_t;

typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*0:*/ int32_t rsw_request_status;                      // request status from RSW
    /*1:*/ request_message_t msg_type;                      // o	کد پیغام
    /*2:*/ int64_t wallet_payment_rest;                     // o  مانده کیف پول
    /*3:*/ byte8_t wallet_effective_date[DATE_BUFFER_SIZE]; // o  تاریخ درج در کیف پول

    /** @brief server_UID: Random number (10 digits) + logical serial number of the device (8 digits) */
    /*4:*/ byte8_t server_UID[UID_BUFFER_SIZE]; // o	شناسه یکتای سرور

    /** @brief transaction_UID: Time stamp (10 digits) + logical serial number of the device (8 digits) */
    /*5:*/ byte8_t transaction_UID[UID_BUFFER_SIZE];    // o	شناسه یکتای عملیات
    /*6:*/ byte8_t message_buffer[MESSAGE_BUFFER_SIZE]; // o	پیغام
} OutputTerminalCardChargeWallet_t;

// RPC_ERROR_CTX TerminalCardChargeWallet(const InputTerminalCardChargeWallet_t *const *, OutputTerminalCardChargeWallet_t **);
#define RPC_TERMINAL_CARD_CHARGE_WALLET "TermCChrgeWlt"
DEFINE_API_ROUTINE_INOUT(TerminalCardChargeWallet, TerminalCardChargeWallet_t, Input, Output);

/*----------------------------------------------------------------------------------------------------------------------------------*/
/**
 * @section TerminalInqueryLastTransaction
 * @brief Inquiry of recent transactions.
 *
 */
typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /*1:*/ int32_t transaction_count;                         // o	تعداد تراکنش های درخواستی (* حداکثر تراکنش درخواستی 20 تراکنش می باشد)
    /*2:*/ byte8_t wallet_uid_buffer[WALLET_UID_BUFFER_SIZE]; // o	رشته شناسه کیف پول
} InputTerminalInqueryLastTransaction_t;

typedef struct __attribute__((__aligned__(RTE_CACHELINE)))
{
    /** @brief server_UID: Random number (10 digits) + logical serial number of the device (8 digits) */
    /*0:*/ int32_t rsw_request_status;                        // request status from RSW
    /*1:*/ transaction_type_t transaction_type;               // o	نوع تراکنش (شارژ یا مصرف)
    /*2:*/ transaction_status_t transaction_status;           // o	وضعیت تراکنش
    /*3:*/ transaction_status_code_t transaction_status_code; // o	کد وضعیت تراکنش
    /*4:*/ int32_t bank_switch_code;                          // o	کد سوییچ بانکی
    /*5:*/ uint32_t transaction_fee;                          // o	مبلغ تراکنش
    /*6:*/ byte8_t transaction_date[DATE_BUFFER_SIZE];        // o	تاریخ تراکنش
    /*7:*/ byte8_t server_UID[UID_BUFFER_SIZE];               // o	شناسه یکتای سرور
    /*8:*/ byte8_t serial_hw[SERIAL_HW_FIELD_SIZE];           // o	سریال دستگاه
    /*9:*/ byte8_t RRN_id[RRN_ID_SIZE];                       // o	RRN بانکی
    /*10:*/ byte8_t message_buffer[MESSAGE_BUFFER_SIZE];      // o	پیغام وضعیت تراکنش
} OutputTerminalInqueryLastTransaction_t;

// RPC_ERROR_CTX TerminalInqueryLastTransaction(const InputTerminalInqueryLastTransaction_t *const *, OutputTerminalInqueryLastTransaction_t **);
#define RPC_TERMINAL_INQUERY_LAST_TRANSACTION "TermInquLTrans"
DEFINE_API_ROUTINE_INOUT(TerminalInqueryLastTransaction, TerminalInqueryLastTransaction_t, Input, Output);

/*----------------------------------------------------------------------------------------------------------------------------------*/
typedef byte8_t (*fptr_kill_connection_server_t)();
typedef RPC_ERROR_CTX (*fptr_build_connection_server_t)();
typedef RPC_ERROR_CTX (*fptr_request_for_connect_t)(const InputRequestForConnect_t *const *, OutputRequestForConnect_t **);
typedef RPC_ERROR_CTX (*fptr_request_for_keys_t)(const InputRequestForKeys_t *const *, OutputRequestForKeys_t **);
typedef RPC_ERROR_CTX (*fptr_redirect_to_issuer_t)(const InputRedirectToIssuer_t *const *, OutputRedirectToIssuer_t **);
typedef RPC_ERROR_CTX (*fptr_terminal_consume_voucher)(const InputTerminalConsumeVoucher_t *const *, OutputTerminalConsumeVoucher_t **);
typedef RPC_ERROR_CTX (*fptr_terminal_card_consume)(const InputTerminalCardConsume_t *const *, OutputTerminalCardConsume_t **);
typedef RPC_ERROR_CTX (*fptr_terminal_group_card_consume)(const InputTerminalGroupCardConsume_t *const *, OutputTerminalGroupCardConsume_t **);
typedef RPC_ERROR_CTX (*fptr_terminal_QR_consume)(const InputTerminalQRConsume_t *const *, OutputTerminalQRConsume_t **);
typedef RPC_ERROR_CTX (*fptr_terminal_group_QR_consume)(const InputTerminalGroupQRConsume_t *const *, OutputTerminalGroupQRConsume_t **);
typedef RPC_ERROR_CTX (*fptr_terminal_inquery)(const InputTerminalInquery_t *const *, OutputTerminalInquery_t **);
typedef RPC_ERROR_CTX (*fptr_terminal_inquery_group)(const InputTerminalInqueryGroup_t *const *, OutputTerminalInqueryGroup_t **);
typedef RPC_ERROR_CTX (*fptr_terminal_card_charge_wallet)(const InputTerminalCardChargeWallet_t *const *, OutputTerminalCardChargeWallet_t **);
typedef RPC_ERROR_CTX (*fptr_terminal_inquery_last_transaction)(const InputTerminalInqueryLastTransaction_t *const *, OutputTerminalInqueryLastTransaction_t **);

/**
 * @brief use (PIC) style, API function.
 *
 */
typedef struct server_context
{
    // offset of OpenSession()
    fptr_build_connection_server_t OpenSession; /*:1*/
    // offset of CloseSession()
    fptr_kill_connection_server_t CloseSession; /*:2*/
    // offset of RequestForConnect()
    fptr_request_for_connect_t RPC_RequestForConnect; /*:3*/
    // offset of RequestForKeys
    fptr_request_for_keys_t RPC_RequestForKeys; /*:4*/
    // offset of RedirectToIssuer
    fptr_redirect_to_issuer_t RPC_RedirectToIssuer; /*:5*/
    // offset of TerminalConsumeVoucher
    fptr_terminal_consume_voucher RPC_TerminalConsumeVoucher; /*:6*/
    // offset of TerminalCardConsume
    fptr_terminal_card_consume RPC_TerminalCardConsume; /*:7*/
    // offset of fptr_terminal_group_card_consume
    fptr_terminal_group_card_consume RPC_TerminalGroupCardConsume; /*:8*/
    // offset of TerminalQRConsume
    fptr_terminal_QR_consume RPC_TerminalQRConsume; /*:9*/
    // offset of TerminalGroupQRConsume
    fptr_terminal_group_QR_consume RPC_TerminalGroupQRConsume; /*:10*/
    // offset of TerminalInquery
    fptr_terminal_inquery RPC_TerminalInquery; /*:11*/
    // offset of TerminalInqueryGroup
    fptr_terminal_inquery_group RPC_TerminalInqueryGroup; /*:12*/
    // offset of TerminalCardChargeWallet
    fptr_terminal_card_charge_wallet RPC_TerminalCardChargeWallet; /*:13*/
    // offset of TerminalInqueryLastTransaction
    fptr_terminal_inquery_last_transaction RPC_TerminalInqueryLastTransaction; /*:14*/
} server_context_t;

RPC_ERROR_CTX OpenSession();
byte8_t CloseSession();

#ifdef __cplusplus
extern "C" RPC_ERROR_CTX GetInstanceProtoBufLib(server_context_t *const *, const byte8_t **, const rswConfigContext_t **);

/** @attention this lookup table is very important,
 *             needed by RSW server.
 *             if add new function API in this header, certainly must be add name func API in this list.
 */

typedef enum
{
    /**
     * @attention !!! don't change the order num in enum type !!!!
     */
    REQUEST_FOR_CONNECT = 0,
    REQUEST_FOR_KEYS = 1,
    REDIRECT_TO_ISSUER = 2,
    TERMINAL_CONSUME_VOUCHER = 3,
    TERMINAL_CARD_CONSUME = 4,
    TERMINAL_GROUP_CARD_CONSUME = 5,
    TERMINAL_QR_CONSUME = 6,
    TERMINAL_GROUP_QR_CONSUME = 7,
    TERMINAL_INQUERY = 8,
    TERMINAL_INQUERY_GROUP = 9,
    TERMINAL_CARD_CHARGE_WALLET = 10,
    TERMINAL_INQUERY_LAST_TRANSACTION = 11,
    OPTIONAL_ENTITY = 12
} rpc_name_lookup_t;

typedef enum
{
    CONSUMER = 0,
    SERVICER = 1,
    REPORTER = 2
} rpc_type_t;

typedef enum
{
    MAIN_KEY_IDX = 0,
    REQUEST_ID_KEY_IDX = 1,
    RESPONSE_ID_KEY_IDX = 2,
    INSERT_WALLET_DATA_KEY_IDX = 3,
    WALLET_BALANAC_KEY_IDX = 4
} response_fields_t;

__attribute__((__unused__)) static const byte8_t *json_response_fields[]{"result",
                                                                         "requestId",
                                                                         "responseId",
                                                                         "insertInWalletDate",
                                                                         "walletBalance" /**/};

__attribute__((__unused__)) static const byte8_t *lookup_table[]{RPC_REQUEST_FOR_CONNECT,
                                                                 RPC_REQUEST_FOR_KEYS,
                                                                 RPC_REDIRECT_TO_ISSUER,
                                                                 RPC_TERMINAL_CONSUME_VOUCHER,
                                                                 RPC_TERMINAL_CARD_CONSUME,
                                                                 RPC_TERMINAL_GROUP_CARD_CONSUME,
                                                                 RPC_TERMINAL_QR_CONSUME,
                                                                 RPC_TERMINAL_GROUP_QR_CONSUME,
                                                                 RPC_TERMINAL_INQUERY,
                                                                 RPC_TERMINAL_INQUERY_GROUP,
                                                                 RPC_TERMINAL_CARD_CHARGE_WALLET,
                                                                 RPC_TERMINAL_INQUERY_LAST_TRANSACTION /**/};
__attribute__((__unused__)) static unsigned long crc32_generator(const Bytef *stream, int32_t stream_len, int32_t displacement)
{
    unsigned long crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc,
                &stream[displacement],
                stream_len - displacement);
    return (crc);
}
#else
/** @brief  */
RPC_ERROR_CTX GetInstanceProtoBufLib(server_context_t *const *, const byte8_t **, const rswConfigContext_t **);
/** @brief  */
const byte8_t *parseErrorMessage(const error_context_t *error_context);
/** @brief  */
#define PROTOBUF_RPC_ROUTINE(ERR_CTX) parseErrorMessage(ERR_CTX);
/** @brief  */
int32_t parseServerRequestStatus(int32_t);
/** @brief  */
typedef unsigned char ubyte8_t;
/** @brief  */
static inline uint_fast32_t
/** @brief  */
generateHash32(const ubyte8_t *stream)
{
    uint_fast32_t h = 0, g;
    while (*stream)
    {
        h = (h << 4) + *stream++;
        if ((g = h & 0xf0000000))
            h ^= g >> 24;
        h &= ~g;
    }
    return h;
}
#endif

//-------
#endif
