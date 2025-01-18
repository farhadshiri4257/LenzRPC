# LenzRPC
This library is used to implement messages exchanged between servers in a distributed micropayment system for public transportation systems.
This library is designed using C++ and RPC techniques using NDK technology, without the use of dependent libraries. This source is developed for use in devices that are Android (Linux-based) operating systems and can be compatible with ARMv7 processor architecture.
![Activity Model - Copy](https://github.com/user-attachments/assets/8f5424f6-502d-4c5a-bc07-a626dbd07c69)

![RPC](https://github.com/user-attachments/assets/a35297fe-d127-4e50-af14-b324f13959ed)

> [!TIP]
> # Distributed system
> Where tasks execute on different nodes in a distributed system, the goal is to make communication between them as easy and reliable as possible.
> Unfortunately, communication often occurs between heterogeneous processors across an unreliable network, and in practice, complex communication
> protocols are required. What is needed is to provide mechanisms whereby:
> - Tasks do not have to deal with the underlying form of messages.
>   For example, they do not need to translate data into bit strings suitable for transmission or to break up the message into packets.
>
> - All messages received by user tasks can be assumed to be intact and in good condition.
>   For example, if messages are broken into packets, the run—time system will only deliver them when all packets have arrived at
>   the receiving node and can be properly reassembled. Furthermore, if the bits in a message have been scrambled, the message
>   either is not delivered or is reconstructed before delivery; clearly, some redundant information is required for error checking and correction.
>
> - Messages received by a task are the kind that the task expects. The task does not need to perform run-time checks.
>
> - Tasks are not restricted to communication only in terms of a predefined built-in set of types. Instead, tasks can communicate in terms of
>   values of interest to the application. Ideally, if the application is defined using abstract data types or classes, then the values of these types can be communicated in messages.
>
> It is possible to identify three main de facto standards by which distributed programs can communicate with each other:
> - by using an externally—defined application programmer’s interface (API), such as sockets, to access network transport protocols.
> - by using the remote procedure call (RPC) paradigm.
> - by using the distributed object paradigm.
> The remainder of this subsection will consider RPCS and distributed objects, including the Common Object Request Broker Architecture (CORBA).


# Features
- Very fast and without additional overhead for execution.
- Transporting smaller messages due to message data compression.
- Adding various encryption methods in sending messages.
- Easy and practical use as functions.
- Sending and receiving data encapsulated in structures as function input and output parameters.
- No need for any plain text such as JSON or XML.
- No need to serialize and deserialize sent and received data.
- Compatible with Android NDK.
- Using service functions in other platforms such as Java using JNI.
- Implemented with modern C++ standards in Android NDK.
- Using session-based techniques to prevent increased unused connections.
- Implementing socket programming concepts in RISC architecture.
- Sending packets using segmentation concepts.
- Implementing object-oriented and Meta-programming concepts.
- Very easy porting of functions to the C language.
- The simple addition of new functions by defining a new class, without the need to change or define the format sent.
  ...
  
# Usage
```c++
#include "MessageProto.h"

const server_context_t *g_server __rte_align_cache_line = NULL;
byte8_t *mac_addr = (byte8_t *)calloc(1 * RSW_ETH_ALEN, sizeof(RSW_ETH_ALEN)); // no need longer,
ExecCmd("getprop persist.service.bdroid.bdaddr", mac_addr, 0);

g_server = (server_context_t *)calloc(1 * sizeof(server_context_t), sizeof(server_context_t));
const byte8_t *instance_err_msg = PROTOBUF_RPC_ROUTINE(GetInstanceProtoBufLib((server_context_t *const *)&g_server, (const byte8_t **)&mac_addr, (const rswConfigContext_t **)&g_rswConfigContext));
free(mac_addr);

if (likely(instance_err_msg == NULL) && likely(g_server != NULL))
{
  const byte8_t *instance_err_msg = PROTOBUF_RPC_ROUTINE(g_server->OpenSession());
  if (likely(instance_err_msg == NULL))
  {
    InputTerminalCardConsume_t *p_InputTerminalCardConsume = (InputTerminalCardConsume_t *)malloc(sizeof(InputTerminalCardConsume_t));
    memset(p_InputTerminalCardConsume, 0, sizeof(InputTerminalCardConsume_t));
    OutputTerminalCardConsume_t *p_OutputTerminalCardConsume = (OutputTerminalCardConsume_t *)malloc(sizeof(OutputTerminalCardConsume_t));
    memset(p_OutputTerminalCardConsume, 0, sizeof(OutputTerminalCardConsume_t));

    p_InputTerminalCardConsume->transaction_type = CARD_CONSUME_TRANSACTION;
    p_InputTerminalCardConsume->company_id = MUNICIPALITY_PAKDASHT;
    p_InputTerminalCardConsume->user_group_id = PROXY_USERS;
    p_InputTerminalCardConsume->verification_status_id = APPROVED_WITH_SHAHKAR;
    p_InputTerminalCardConsume->issuer_id = EASY_CITY_TEHRAN;
    p_InputTerminalCardConsume->transaction_fee = 1;
    p_InputTerminalCardConsume->transaction_counter = 20;
    p_InputTerminalCardConsume->latitude = 35.71901451770399;
    p_InputTerminalCardConsume->longitude = 51.37402605857007;
    memcpy(p_InputTerminalCardConsume->hash_key_block2, "hash_key", HASH_KEY_BLOCK2_SIZE);
    memcpy(p_InputTerminalCardConsume->expired_date_id, "2023-07-09T16:30:06.581Z", DATE_BUFFER_SIZE);
    memcpy(p_InputTerminalCardConsume->transaction_date, "1984-02-08T14:42:57.164Z", DATE_BUFFER_SIZE);
    memcpy(p_InputTerminalCardConsume->wallet_uid_buffer, "79733826764576", WALLET_UID_BUFFER_SIZE);
    memcpy(p_InputTerminalCardConsume->validation_buffer, "macaddr", VALIDATION_BUFFER_SIZE);
    memcpy(p_InputTerminalCardConsume->card_serial, "5656565665656", CARD_SERIAL_SIZE);
    memcpy(p_InputTerminalCardConsume->serial_hw, g_hardware_serial_number, SERIAL_HW_FIELD_SIZE);
    memcpy(p_InputTerminalCardConsume->serial_logic_hw, "12345678", SERIAL_HW_FIELD_SIZE);

    /*this generates a random number better solution because transaction UID must be unique per device! */
    const uint_fast32_t hw_serial_hash = generateHash32((ubyte8_t *)g_hardware_serial_number);
    const time_t time_stamp = time(NULL);
    srand((uint32_t)(hw_serial_hash + time_stamp));
    const int32_t _tran_uid = rand();
    snprintf(p_InputTerminalCardConsume->transaction_UID, UID_BUFFER_SIZE, "%d", _tran_uid); // only numeric, because RSW convert to numeric

    const byte8_t *req_err_msg = PROTOBUF_RPC_ROUTINE(g_server->RPC_TerminalCardConsume((const InputTerminalCardConsume_t *const *)&p_InputTerminalCardConsume, &p_OutputTerminalCardConsume));
    response_res = parseServerRequestStatus(p_OutputTerminalCardConsume->rsw_request_status);
    if (response_res == WALLET_SERVICE_NO_RESPOND)
    {
      char l_szAmount[128] = {0};
      sprintf(l_szAmount, "      پاسخی از کیف پول دریافت نشد!!     ");
      uiDisplayMsgIdAndString(MESSAGE_INDEX, l_szAmount, NULL, UI_DISPLAY_WAIT_TYPE_ANY_KEY, UI_DISPLAY_MSG_TYPE_ERROR_ALARM, FONT_SIZE12);
    }
    if (response_res != REQUEST_IS_OK || (req_err_msg != NULL && strlen(req_err_msg) > 0))
    {
      if (req_err_msg != NULL)
        OsLog(LOG_DEBUG, "[[ERROR]] : (%s) \r\n", req_err_msg);
    } else if (response_res == REQUEST_IS_OK)
    {
      OsLog(LOG_DEBUG, "[[!!!!!!!!!@@@###@@]] :msg_type(%d) \r\n", p_OutputTerminalCardConsume->msg_type);
      OsLog(LOG_DEBUG, "[[!!!!!!!!!@@@###@@]] :wallet_payment_rest(%lld) \r\n", p_OutputTerminalCardConsume->wallet_payment_rest);
      OsLog(LOG_DEBUG, "[[!!!!!!!!!@@@###@@]] :wallet_effective_date(%s) \r\n", p_OutputTerminalCardConsume->wallet_effective_date);
      OsLog(LOG_DEBUG, "[[!!!!!!!!!@@@###@@]] :server_UID(%s) \r\n", p_OutputTerminalCardConsume->server_UID);
      OsLog(LOG_DEBUG, "[[!!!!!!!!!@@@###@@]] :transaction_UID(%s) \r\n", p_OutputTerminalCardConsume->transaction_UID);
      OsLog(LOG_DEBUG, "[[!!!!!!!!!@@@###@@]] :message_buffer(%s) \r\n", p_OutputTerminalCardConsume->message_buffer);
    }         
  }
}
```
# Dependency
- install [snapd] service. 
- install [snap andriod-adb --edge --devmode]
- install [glibc, glibc-devel, libstdc++, linux-glibc, glibc-devel-static]
- install [android-ndk-r15c]

# Clean & Building Library
```script
#!/bin/sh
rm -f -r -d libs
rm -f -r -d obj
ndk-build NDK_PROJECT_PATH=./ APP_BUILD_SCRIPT=./BuildBinary.mk NDK_APPLICATION_MK=./Application.mk NDK_TOOLCHAIN_VERSION=4.8 clean
clear
ndk-build -j8 NDK_PROJECT_PATH=./ NDK_APPLICATION_MK=./Application.mk 
```

# Installing and Running
add /library_path/-lprotobuf into BuildBinary.mk file, in the main project.



