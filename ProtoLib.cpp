/**
 * @file ProtoLib.cpp
 * @author f.shiri (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-08-21
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
#include "Header/Proxy/Proxy.h"
#include "Header/Helper/Singleton.h"
#include "string"
#include <algorithm>

using namespace proto_proxy;

/** @attention g_cproxy_ctrlr is private variable in this module therefore can't with extern use in other module */
const ::dataModel::cproxy_ctrlr_t *g_cproxy_ctrlr = nullptr;

/** @attention g_error_context is private variable in this module therefore can't with extern use in other module */
error_context_t *g_error_context = nullptr;

/** @attention server is private variable in this module therefore can't with extern use in other module,
 *             because  BSS offset pointer as server variable injected to proxy class
 */
const ::network::cserver_ctrlr_t *g_server;

int32_t monitor_event_fd;


NAMESPACE(global)
{
    /** @brief device hardware serial number buffer, this variable can be use extern in other module. */
    std::string g_hardware_serial;
    std::string g_mac_address;
    const rswConfigContext_t *g_rswConfigContext;
}

extern "C"
{
    RPC_ERROR_CTX GetInstanceProtoBufLib(server_context_t *const *server_context, const byte8_t **mac_addr, const rswConfigContext_t **rswConfigContext)
    {
        if ((*mac_addr) == NULL)
        {
            g_error_context->error_num = ERR_MAC_ADDR_IS_NULL;
            return (g_error_context);
        }
        if ((*rswConfigContext) == NULL)
        {
            g_error_context->error_num = ERR_RSW_CONFIG_IS_NULL;
            return (g_error_context);
        }
        global::g_rswConfigContext = *rswConfigContext;

        global::g_mac_address.append((*mac_addr) + 0);
        global::g_mac_address.erase(
            std::find_if(global::g_mac_address.begin(), global::g_mac_address.end(), [](byte8_t byte)
                         { return byte == 0x0A; }));
        LOG_INFO("global::g_mac_address(%s)", global::g_mac_address.c_str());

        (*server_context)->OpenSession = &OpenSession;                                       /*:1*/
        (*server_context)->CloseSession = &CloseSession;                                     /*:2*/
        (*server_context)->RPC_RequestForConnect = &RequestForConnect;                           /*:3*/
        (*server_context)->RPC_RequestForKeys = &RequestForKeys;                                 /*:4*/
        (*server_context)->RPC_RedirectToIssuer = &RedirectToIssuer;                             /*:5*/
        (*server_context)->RPC_TerminalConsumeVoucher = &TerminalConsumeVoucher;                 /*:6*/
        (*server_context)->RPC_TerminalCardConsume = &TerminalCardConsume;                       /*:7*/
        (*server_context)->RPC_TerminalGroupCardConsume = &TerminalGroupCardConsume;             /*:8*/
        (*server_context)->RPC_TerminalQRConsume = &TerminalQRConsume;                           /*:9*/
        (*server_context)->RPC_TerminalGroupQRConsume = &TerminalGroupQRConsume;                 /*:10*/
        (*server_context)->RPC_TerminalInquery = &TerminalInquery;                               /*:11*/
        (*server_context)->RPC_TerminalInqueryGroup = &TerminalInqueryGroup;                     /*:12*/
        (*server_context)->RPC_TerminalCardChargeWallet = &TerminalCardChargeWallet;             /*:13*/
        (*server_context)->RPC_TerminalInqueryLastTransaction = &TerminalInqueryLastTransaction; /*:14*/

        LOG_INFO("GetInstanceProtoBufLib invoked.!");

        return (NO_ERROR);
    }
}

/**
 * @brief create new instance of proxy protocol API class in Runtime mode.
 * @attention this function is called only once!
 * @return RPC_ERROR_CTX error context object created in runtime mode and contents of the error code.
 */
RPC_ERROR_CTX OpenSession()
{
    if (g_server != nullptr)
    {
        g_error_context->error_num = ERR_SERVER_LAST_INIT;
        return (g_error_context);
    }
    g_error_context = (error_context_t *)malloc(sizeof(error_context_t));
    memset(g_error_context, 0, sizeof(error_context_t));

    monitor_event_fd = init_event_handler(MAX_EVENT_IO);
    if (monitor_event_fd == -1)
    {
        g_error_context->error_num = EVENT_HANDLE_ERROR;
        return (g_error_context);
    }

    g_server = new (std::nothrow)::network::cserver_ctrlr_t(monitor_event_fd);
    PROXY_INSTANCE_CHECK(g_server);
    auto error_num = const_cast<::network::cserver_ctrlr_t *>(g_server)->bonding_low_level_routine();
    if (error_num != DONE_PROC)
        g_error_context->error_num = error_num;

    error_num = const_cast<proto_proxy::network::cserver_ctrlr_t *>(g_server)->init_session_monitoring();
    if (error_num != DONE_PROC)
        g_error_context->error_num = error_num;
    /** @attention created cproxy_ctrlr_t operation must be the end of init operation */
    g_cproxy_ctrlr = ::immutable::csingleton_t<::dataModel::cproxy_ctrlr_t>::newInstancePtr(*g_server);
    PROXY_INSTANCE_CHECK(g_cproxy_ctrlr);
    LOG_INFO("OpenSession invoked.!");

    return (NO_ERROR);
}

/**
 * @brief destroy proxy instance and free resources allocated in global section
 * @attention this function is called only once!
 * @return true
 * @return false
 */
byte8_t CloseSession()
{
    if (g_server == nullptr)
        return (ERR_SERVER_LAST_DEINIT);

    deinit_event_handler(monitor_event_fd);

    const auto res = ::immutable::csingleton_t<::dataModel::cproxy_ctrlr_t>::destroy(g_cproxy_ctrlr);
    if (g_error_context != nullptr)
    {
        free(g_error_context);
        g_error_context = nullptr;
    }

    /*close and shutdown when server object is removed!*/
    if (g_server != nullptr)
    {
        delete g_server;
        g_server = nullptr;
        LOG_INFO("Server is disconnected.!");
    }
    LOG_INFO("CloseSession invoked.!");
    return (res);
}

/**
 * @brief After being called by the device, the function compares the device serial with the existing serials.
 *        If the device is among the approved devices,
 *        it creates a communication channel with the device and sends the following outputs to the device.
 *        The validation string is generated by the device. This information string is the MAC address of the device.
 *
 * @param InputRequestForConnect
 * @param OutputRequestForConnect
 * @return RPC_ERROR_CTX error context object created in runtime mode and contents of the error code.
 */
RPC_ERROR_CTX RequestForConnect(const InputRequestForConnect_t *const *InputRequestForConnect, OutputRequestForConnect_t **OutputRequestForConnect)
{

    PROXY_INSTANCE_CHECK(g_cproxy_ctrlr);

    return (g_cproxy_ctrlr->request_for_connect(InputRequestForConnect, OutputRequestForConnect));
}

/**
 * @brief If the token is valid,
 *        the function will send the 256-bit AES256 key and the 256-bit MD5 key to the device.
 *
 *
 * @param in_request_keys
 * @param out_request_keys
 * @return RPC_ERROR_CTX error context object created in runtime mode and contents of the error code.
 */
RPC_ERROR_CTX RequestForKeys(const InputRequestForKeys_t *const *in_request_keys, OutputRequestForKeys_t **out_request_keys)
{
    PROXY_INSTANCE_CHECK(g_cproxy_ctrlr);

    return (g_cproxy_ctrlr->request_for_keys(in_request_keys, out_request_keys));
}

/**
 * @brief Based on the pattern defined in the switch,
 *        the switch detects the issuer and directs the transaction
 *        to the issuer and returns the result to the device.
 *
 * @param in_redirect_issuer
 * @param out_redirect_issuer
 * @return RPC_ERROR_CTX error context object created in runtime mode and contents of the error code.
 */
RPC_ERROR_CTX RedirectToIssuer(const InputRedirectToIssuer_t *const *in_redirect_issuer, OutputRedirectToIssuer_t **out_redirect_issuer)
{
    PROXY_INSTANCE_CHECK(g_cproxy_ctrlr);

    return (g_cproxy_ctrlr->redirect_to_issuer(in_redirect_issuer, out_redirect_issuer));
}

/**
 * @brief consumer function worked with voucher ticket
 *
 * @param in_terminal_consume_voucher
 * @param out_terminal_consume_voucher
 * @return RPC_ERROR_CTX error context object created in runtime mode and contents of the error code.
 */
RPC_ERROR_CTX TerminalConsumeVoucher(const InputTerminalConsumeVoucher_t *const *in_terminal_consume_voucher, OutputTerminalConsumeVoucher_t **out_terminal_consume_voucher)
{
    PROXY_INSTANCE_CHECK(g_cproxy_ctrlr);

    return (g_cproxy_ctrlr->terminal_consume_voucher(in_terminal_consume_voucher, out_terminal_consume_voucher));
}

/**
 * @brief The function first adds the serial number of the ticket card with the 14-digit number of the first string
 *        and the result with the 14-digit string of the wallet ID,
 *        and hashes the result with the MD5 algorithm of the key that exists only at the terminal.
 *        Compares the first 8 bytes of the generated hash with the 8 bytes of the hash sent by the function.
 *        If these two values are equal, it will enter the next step,
 *        and otherwise it will return the error "invalidity of the ticket card on the server side".
 *        If the validity of the server-side card is confirmed,
 *        the transaction will be queued to be sent to the wallet, and the debit function from the wrapper wallet will be called.
 *
 * @param in_terminal_card_consume
 * @param out_terminal_card_consume
 * @return RPC_ERROR_CTX error context object created in runtime mode and contents of the error code.
 */
RPC_ERROR_CTX TerminalCardConsume(const InputTerminalCardConsume_t *const *in_terminal_card_consume, OutputTerminalCardConsume_t **out_terminal_card_consume)
{
    PROXY_INSTANCE_CHECK(g_cproxy_ctrlr);

    return (g_cproxy_ctrlr->terminal_card_consume(in_terminal_card_consume, out_terminal_card_consume));
}

/**
 * @brief
 *
 * @param in_terminal_group_card
 * @param out_terminal_group_card
 * @return RPC_ERROR_CTX error context object created in runtime mode and contents of the error code.
 */
RPC_ERROR_CTX TerminalGroupCardConsume(const InputTerminalGroupCardConsume_t *const *in_terminal_group_card, OutputTerminalGroupCardConsume_t **out_terminal_group_card)
{
    PROXY_INSTANCE_CHECK(g_cproxy_ctrlr);

    return (g_cproxy_ctrlr->terminal_group_card_consume(in_terminal_group_card, out_terminal_group_card));
}

/**
 * @brief The time function compares the Timestamp with the current time if its value is less than n (n is in the configuration of the switch),
 *         the transaction is placed in the queue to be sent to the wallet,
 *         and the subtraction function from the rapper wallet is called.
 *
 * @param in_terminal_QRConsume
 * @param out_terminal_QRConsume
 * @return RPC_ERROR_CTX error context object created in runtime mode and contents of the error code.
 */
RPC_ERROR_CTX TerminalQRConsume(const InputTerminalQRConsume_t *const *in_terminal_QRConsume, OutputTerminalQRConsume_t **out_terminal_QRConsume)
{
    PROXY_INSTANCE_CHECK(g_cproxy_ctrlr);

    return (g_cproxy_ctrlr->terminal_QR_consume(in_terminal_QRConsume, out_terminal_QRConsume));
}

/**
 * @brief The time function compares the Timestamp with the current time if its value is less than n (n is in the configuration of the switch),
 *         the transaction is placed in the queue to be sent to the wallet,
 *         and the subtraction function from the rapper wallet is called.
 *
 * @param in_group_QRConsume
 * @param out_group_QRConsume
 * @return RPC_ERROR_CTX error context object created in runtime mode and contents of the error code.
 */
RPC_ERROR_CTX TerminalGroupQRConsume(const InputTerminalGroupQRConsume_t *const *in_group_QRConsume, OutputTerminalGroupQRConsume_t **out_group_QRConsume)
{
    PROXY_INSTANCE_CHECK(g_cproxy_ctrlr);

    return (g_cproxy_ctrlr->terminal_group_QR_consume(in_group_QRConsume, out_group_QRConsume));
}

/**
 * @brief By receiving the above inputs,
 *        the function checks the transaction status and answers the query.
 *
 * @param in_terminal_inquery
 * @param out_terminal_inquery
 * @return RPC_ERROR_CTX error context object created in runtime mode and contents of the error code.
 */
RPC_ERROR_CTX TerminalInquery(const InputTerminalInquery_t *const *in_terminal_inquery, OutputTerminalInquery_t **out_terminal_inquery)
{
    PROXY_INSTANCE_CHECK(g_cproxy_ctrlr);

    return (g_cproxy_ctrlr->terminal_inquery(in_terminal_inquery, out_terminal_inquery));
}

/**
 * @brief The function receives an array of the above inputs and answers the query in an array.
 *
 * @param in_terminal_inquery
 * @param out_terminal_inquery
 * @return RPC_ERROR_CTX error context object created in runtime mode and contents of the error code.
 */
RPC_ERROR_CTX TerminalInqueryGroup(const InputTerminalInqueryGroup_t *const *in_terminal_inquery, OutputTerminalInqueryGroup_t **out_terminal_inquery)
{
    PROXY_INSTANCE_CHECK(g_cproxy_ctrlr);

    return (g_cproxy_ctrlr->terminal_inquery_group(in_terminal_inquery, out_terminal_inquery));
}

/**
 * @brief Validation of this part is done like deduction validation.
 *
 * @return RPC_ERROR_CTX error context object created in runtime mode and contents of the error code.
 */
RPC_ERROR_CTX TerminalCardChargeWallet(const InputTerminalCardChargeWallet_t *const *in_terminal_card_charge, OutputTerminalCardChargeWallet_t **out_terminal_card_charge)
{
    PROXY_INSTANCE_CHECK(g_cproxy_ctrlr);

    return (g_cproxy_ctrlr->terminal_card_charge_wallet(in_terminal_card_charge, out_terminal_card_charge));
}

/**
 * @brief Inquiry of recent transactions.
 *
 * @param in_terminal_inquery
 * @param out_terminal_inquery
 * @return RPC_ERROR_CTX error context object created in runtime mode and contents of the error code.
 */
RPC_ERROR_CTX TerminalInqueryLastTransaction(const InputTerminalInqueryLastTransaction_t *const *in_terminal_inquery, OutputTerminalInqueryLastTransaction_t **out_terminal_inquery)
{
    PROXY_INSTANCE_CHECK(g_cproxy_ctrlr);

    return (g_cproxy_ctrlr->terminal_inquery_last_transaction(in_terminal_inquery, out_terminal_inquery));
}
