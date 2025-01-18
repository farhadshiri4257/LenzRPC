/**
 * @file Proxy.cpp
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
#include "../../Header/Proxy/Proxy.h"

#include "../../Header/Model_header.h"

using namespace proto_proxy;
using namespace proto_proxy::dataModel;

/**
 * @brief this generic function, can start and send/recv and close server.
 *        this operation same with all other API routine.
 *
 * @attention Important Note-1: (assertion)
 *            the MODEL_CLASS certainly derived from this class proto_proxy::interface::CEntityInterface<OUT_MODEL_ENTITY>,
 *            and override pure virtual member function.
 *
 * @attention Important Note-2: (thread safety)
 *            because proxy_ctrlr is singleton instance,
 *            therfore all method in class can be compatible for
 *            thread safety paradigm and so all method is immutable method,
 *            but mutable object eliminated const by const_cast to rvalue, and this situation guaranteed by,
 *            The lowest object lifetime scope paradigm with the care of c++ compiler.
 *
 * @attention Important Note-3: (restricted)
 *            stack frame in android with device LENZ-E60 is expensive and is limited size.
 *            therefore if needed large variable, only defined in the HEAP.
 *
 * @attention Important Note-4: (recommendation)
 *            is not recommended change the virtual memory size for stack frame.
 *
 *
 * @attention when router switch server received data to the client, endpoint connection teardown by the server
 *            therfore in client side can't the persistent the connection.
 *            If the possibility of persistent the connection with the client is implemented by the server,
 *            the connection building step can be done only once on the client side.
 *
 * @tparam OUT_MODEL_ENTITY
 * @tparam MODEL_CLASS
 * @param entity_instance
 * @param output_model
 * @return RPC_ERROR_CTX
 */
template <typename IN_MODEL, typename OUT_MODEL_ENTITY, typename MODEL_CLASS>
RPC_ERROR_CTX CProxyCtrlr::evaluate_request(const MODEL_CLASS &entity_instance, OUT_MODEL_ENTITY *&output_model) const
{
    static_assert(std::is_base_of<proto_proxy::interface::IEntityModel<IN_MODEL, OUT_MODEL_ENTITY>, MODEL_CLASS>::value,
                  "MODEL_CLASS must be derived from base proto_proxy::interface::CEntityInterface");

    /** @remark this generic method it has several stages as follows:
     *          1- check the availability device network connection.
     *          2- build the connection session with the router switch server(RSW).
     *          3- start ticket timer,because send(),connect() process can't non-bloking operation.
     *          4- serialize input entity model data member to protobuf stream.
     *          5- map stream buffer to iovec message
     *          6- send protobuf stream to the server
     *          7- deserialize protobuf stream to output entity model data member.
     *          8- map stream buffer to iovec message
     *          9- receive the protobuf stream from router switch server.
     *          10- emit event for close the session connection.
     */

    /*1- check network status */
    int32_t network_status = 0;
    auto res = remote_endpoint_.check_device_network(network_status);
    if (res != DONE_PROC)
    {
        error_context_->error_no = remote_endpoint_.get_error_code();
        error_context_->error_num = ERR_IN_NETWORK_CONNECTION;
        return (error_context_);
    }
    LOG_INFO("network_status(%d)", network_status);

    /*2- server init and set option */
    res = const_cast<proto_proxy::network::cserver_ctrlr_t &>(remote_endpoint_).open_session();
    if (res != DONE_PROC)
    {
        error_context_->error_no = remote_endpoint_.get_error_code();
        error_context_->error_num = res;
        return (error_context_);
    }

    /** @brief entity model to binary stream buffer */
    /*3- serialize entity model to stream buffer */
    const protohdr_t &tx_msg_hdr = const_cast<MODEL_CLASS &>(entity_instance).serialize_entity();
    
    /*4- perform send / recv stream */
    ssize_t sended_byte = 0;
    uint32_t sum_istream_bit = entity_instance.get_sum_istream_bit();
    res = const_cast<proto_proxy::network::cserver_ctrlr_t &>(remote_endpoint_).write_multiplex_v1(tx_msg_hdr, sended_byte, sum_istream_bit);
    if (res == DONE_PROC && sended_byte > 0)
    {
        /** @brief binary stream buffer to entity model */
        /*5- deserialize stream buffer into entity model */
        protohdr_t &rx_ms_hdr = const_cast<MODEL_CLASS &>(entity_instance).deserialize_entity(output_model);
        ssize_t received_byte = 0;
        res = const_cast<proto_proxy::network::cserver_ctrlr_t &>(remote_endpoint_).read_multiplex(rx_ms_hdr, received_byte);
    }
    /*if (res != DONE_PROC)
    {
        output_model->rsw_request_status = RES_RSW(REQUEST_TIMEOUT_CORE);
    }*/

    /*5- when send / recv in session is done, must be emit the close session event.*/
    emit_event(closeSession);

    /*6- check session status */
    if (res == DONE_PROC) // send & recv is ok!
    {
        error_context_->error_num = DONE_PROC;
        LOG_INFO("send / recv process is OK!");
        return (NO_ERROR);
    }
    else // send or recv is failed!
    {
        error_context_->error_no = remote_endpoint_.get_error_code();
        error_context_->error_num = res;
        LOG_INFO("send / recv process is failed!");
        return (error_context_);
    }
}

/**
 * @section RequestForConnect
 * @link file:///../OnlineTicket/ProtoBuf_Project/Header/API/MessageProto.h @endlink
 *
 * @param in_req_connect
 * @param out_req_connect
 * @return RPC_ERROR_CTX
 */
RPC_ERROR_CTX CProxyCtrlr::request_for_connect(const InputRequestForConnect_t *const *in_req_connect, OutputRequestForConnect_t **out_req_connect) const
{

    if (is_nullptr(in_req_connect, ERR_INPUT_MODEL_IS_NULL) ||
        is_nullptr((*in_req_connect), ERR_INPUT_MODEL_IS_NULL))
        return (error_context_);

    if (is_nullptr(out_req_connect, ERR_OUTPUT_MODEL_IS_NULL) ||
        is_nullptr((*out_req_connect), ERR_OUTPUT_MODEL_IS_NULL))
        return (error_context_);

    /* create new object from model helper class */
    const centity_request_for_connect_t req_connect_protobuf_instance(*in_req_connect);
    return (evaluate_request<InputRequestForConnect_t, OutputRequestForConnect_t>(req_connect_protobuf_instance, (*out_req_connect)));
}

/**
 * @section RequestForKeys
 * @link file:///../OnlineTicket/ProtoBuf_Project/Header/API/MessageProto.h @endlink
 *
 * @param in_req_keys
 * @param out_request_keys
 * @return RPC_ERROR_CTX
 */
RPC_ERROR_CTX CProxyCtrlr::request_for_keys(const InputRequestForKeys_t *const *in_req_keys, OutputRequestForKeys_t **out_request_keys) const
{
    if (is_nullptr(in_req_keys, ERR_INPUT_MODEL_IS_NULL) ||
        is_nullptr((*in_req_keys), ERR_INPUT_MODEL_IS_NULL))
        return (error_context_);

    if (is_nullptr(out_request_keys, ERR_OUTPUT_MODEL_IS_NULL) ||
        is_nullptr((*out_request_keys), ERR_OUTPUT_MODEL_IS_NULL))
        return (error_context_);

    /* create new object from model helper class */
    const centity_request_for_key_t req_key_protobuf_instance(*in_req_keys);
    return (evaluate_request<InputRequestForKeys_t, OutputRequestForKeys_t>(req_key_protobuf_instance, (*out_request_keys)));
}

/**
 * @section RedirectToIssuer
 * @link file:///../OnlineTicket/ProtoBuf_Project/Header/API/MessageProto.h @endlink *
 *
 * @param in_redirect_issuer
 * @param out_redirect_issuer
 * @return RPC_ERROR_CTX
 */
RPC_ERROR_CTX CProxyCtrlr::redirect_to_issuer(const InputRedirectToIssuer_t *const *in_redirect_issuer, OutputRedirectToIssuer_t **out_redirect_issuer) const
{
    if (is_nullptr(in_redirect_issuer, ERR_INPUT_MODEL_IS_NULL) ||
        is_nullptr((*in_redirect_issuer), ERR_INPUT_MODEL_IS_NULL))
        return (error_context_);

    if (is_nullptr(out_redirect_issuer, ERR_OUTPUT_MODEL_IS_NULL) ||
        is_nullptr((*out_redirect_issuer), ERR_OUTPUT_MODEL_IS_NULL))
        return (error_context_);

    /* create new object from model helper class */
    const centity_redirect_issuer_t redirect_protobuf_instance(*in_redirect_issuer);
    return (evaluate_request<InputRedirectToIssuer_t, OutputRedirectToIssuer_t>(redirect_protobuf_instance, (*out_redirect_issuer)));
}

/**
 * @section TerminalConsumeVoucher
 * @link file:///../OnlineTicket/ProtoBuf_Project/Header/API/MessageProto.h @endlink
 *
 * @param in_terminal_consume_voucher
 * @param out_terminal_consume_voucher
 * @return RPC_ERROR_CTX
 */
RPC_ERROR_CTX CProxyCtrlr::terminal_consume_voucher(const InputTerminalConsumeVoucher_t *const *in_terminal_consume_voucher, OutputTerminalConsumeVoucher_t **out_terminal_consume_voucher) const
{
    if (is_nullptr(in_terminal_consume_voucher, ERR_INPUT_MODEL_IS_NULL) ||
        is_nullptr((*in_terminal_consume_voucher), ERR_INPUT_MODEL_IS_NULL))
        return (error_context_);

    if (is_nullptr(out_terminal_consume_voucher, ERR_OUTPUT_MODEL_IS_NULL) ||
        is_nullptr((*out_terminal_consume_voucher), ERR_OUTPUT_MODEL_IS_NULL))
        return (error_context_);

    /* create new object from model helper class */
    const centity_terminal_consume_voucher_t consume_voucher_protobuf_instance(*in_terminal_consume_voucher);
    return (evaluate_request<InputTerminalConsumeVoucher_t, OutputTerminalConsumeVoucher_t>(consume_voucher_protobuf_instance, (*out_terminal_consume_voucher)));
}

/**
 * @section TerminalCardConsume
 * @link file:///../OnlineTicket/ProtoBuf_Project/Header/API/MessageProto.h @endlink
 *
 * @param in_terminal_card_consume
 * @param out_terminal_card_consume
 * @return RPC_ERROR_CTX
 */
RPC_ERROR_CTX CProxyCtrlr::terminal_card_consume(const InputTerminalCardConsume_t *const *in_terminal_card_consume, OutputTerminalCardConsume_t **out_terminal_card_consume) const
{
    if (is_nullptr(in_terminal_card_consume, ERR_INPUT_MODEL_IS_NULL) ||
        is_nullptr((*in_terminal_card_consume), ERR_INPUT_MODEL_IS_NULL))
        return (error_context_);

    if (is_nullptr(out_terminal_card_consume, ERR_OUTPUT_MODEL_IS_NULL) ||
        is_nullptr((*out_terminal_card_consume), ERR_OUTPUT_MODEL_IS_NULL))
        return (error_context_);

    /* create new object from model helper class */
    const centity_terminal_card_consume_t card_consume_protobuf_instance(*in_terminal_card_consume);
    return (evaluate_request<InputTerminalCardConsume_t, OutputTerminalCardConsume_t>(card_consume_protobuf_instance, (*out_terminal_card_consume)));
}

/**
 * @section TerminalGroupCardConsume
 * @link file:///../OnlineTicket/ProtoBuf_Project/Header/API/MessageProto.h @endlink
 *
 * @param in_terminal_gr_card_consume
 * @param out_terminal_gr_card_consume
 * @return RPC_ERROR_CTX
 */
RPC_ERROR_CTX CProxyCtrlr::terminal_group_card_consume(const InputTerminalGroupCardConsume_t *const *in_terminal_gr_card_consume, OutputTerminalGroupCardConsume_t **out_terminal_gr_card_consume) const
{
    if (is_nullptr(in_terminal_gr_card_consume, ERR_INPUT_MODEL_IS_NULL) ||
        is_nullptr((*in_terminal_gr_card_consume), ERR_INPUT_MODEL_IS_NULL))
        return (error_context_);

    if (is_nullptr(out_terminal_gr_card_consume, ERR_OUTPUT_MODEL_IS_NULL) ||
        is_nullptr((*out_terminal_gr_card_consume), ERR_OUTPUT_MODEL_IS_NULL))
        return (error_context_);

    /* create new object from model helper class */
    const centity_terminal_group_card_consume_t card_consume_protobuf_instance(*in_terminal_gr_card_consume);
    return (evaluate_request<InputTerminalGroupCardConsume_t, OutputTerminalGroupCardConsume_t>(card_consume_protobuf_instance, (*out_terminal_gr_card_consume)));
}

/**
 * @section TerminalQRConsume
 * @link file:///../OnlineTicket/ProtoBuf_Project/Header/API/MessageProto.h @endlink
 *
 * @param in_terminal_QR_consume
 * @param out_terminal_QR_consume
 * @return RPC_ERROR_CTX
 */
RPC_ERROR_CTX CProxyCtrlr::terminal_QR_consume(const InputTerminalQRConsume_t *const *in_terminal_QR_consume, OutputTerminalQRConsume_t **out_terminal_QR_consume) const
{
    if (is_nullptr(in_terminal_QR_consume, ERR_INPUT_MODEL_IS_NULL) ||
        is_nullptr((*in_terminal_QR_consume), ERR_INPUT_MODEL_IS_NULL))
        return (error_context_);

    if (is_nullptr(out_terminal_QR_consume, ERR_OUTPUT_MODEL_IS_NULL) ||
        is_nullptr((*out_terminal_QR_consume), ERR_OUTPUT_MODEL_IS_NULL))
        return (error_context_);

    /* create new object from model helper class */
    const centity_terminal_QR_consume_t QR_consume_protobuf_instance(*in_terminal_QR_consume);
    return (evaluate_request<InputTerminalQRConsume_t, OutputTerminalQRConsume_t>(QR_consume_protobuf_instance, (*out_terminal_QR_consume)));
}

/**
 * @section TerminalGroupQRConsume
 * @link file:///../OnlineTicket/ProtoBuf_Project/Header/API/MessageProto.h @endlink
 *
 * @param in_terminal_gr_QR_consume
 * @param out_terminal_gr_QR_consume
 * @return RPC_ERROR_CTX
 */
RPC_ERROR_CTX CProxyCtrlr::terminal_group_QR_consume(const InputTerminalGroupQRConsume_t *const *in_terminal_gr_QR_consume, OutputTerminalGroupQRConsume_t **out_terminal_gr_QR_consume) const
{
    if (is_nullptr(in_terminal_gr_QR_consume, ERR_INPUT_MODEL_IS_NULL) ||
        is_nullptr((*in_terminal_gr_QR_consume), ERR_INPUT_MODEL_IS_NULL))
        return (error_context_);

    if (is_nullptr(out_terminal_gr_QR_consume, ERR_OUTPUT_MODEL_IS_NULL) ||
        is_nullptr((*out_terminal_gr_QR_consume), ERR_OUTPUT_MODEL_IS_NULL))
        return (error_context_);

    /* create new object from model helper class */
    const centity_terminal_group_QR_consume_t group_QR_consume_protobuf_instance(*in_terminal_gr_QR_consume);
    return (evaluate_request<InputTerminalGroupQRConsume_t, OutputTerminalGroupQRConsume_t>(group_QR_consume_protobuf_instance, (*out_terminal_gr_QR_consume)));
}

/**
 * @section TerminalInquery
 * @link file:///../OnlineTicket/ProtoBuf_Project/Header/API/MessageProto.h @endlink
 *
 * @param in_terminal_inquery
 * @param out_terminal_inquery
 * @return RPC_ERROR_CTX
 */
RPC_ERROR_CTX CProxyCtrlr::terminal_inquery(const InputTerminalInquery_t *const *in_terminal_inquery, OutputTerminalInquery_t **out_terminal_inquery) const
{
    if (is_nullptr(in_terminal_inquery, ERR_INPUT_MODEL_IS_NULL) ||
        is_nullptr((*in_terminal_inquery), ERR_INPUT_MODEL_IS_NULL))
        return (error_context_);

    if (is_nullptr(out_terminal_inquery, ERR_OUTPUT_MODEL_IS_NULL) ||
        is_nullptr((*out_terminal_inquery), ERR_OUTPUT_MODEL_IS_NULL))
        return (error_context_);

    /* create new object from model helper class */
    const centity_terminal_inquery_t inquery_protobuf_instance(*in_terminal_inquery);
    return (evaluate_request<InputTerminalInquery_t, OutputTerminalInquery_t>(inquery_protobuf_instance, (*out_terminal_inquery)));
}

/**
 * @section TerminalInqueryGroup
 * @link file:///../OnlineTicket/ProtoBuf_Project/Header/API/MessageProto.h @endlink
 *
 * @param in_terminal_inquery_gr
 * @param out_terminal_inquery_gr
 * @return RPC_ERROR_CTX
 */
RPC_ERROR_CTX CProxyCtrlr::terminal_inquery_group(const InputTerminalInqueryGroup_t *const *in_terminal_inquery_gr, OutputTerminalInqueryGroup_t **out_terminal_inquery_gr) const
{
    if (is_nullptr(in_terminal_inquery_gr, ERR_INPUT_MODEL_IS_NULL) ||
        is_nullptr((*in_terminal_inquery_gr), ERR_INPUT_MODEL_IS_NULL))
        return (error_context_);

    if (is_nullptr(out_terminal_inquery_gr, ERR_OUTPUT_MODEL_IS_NULL) ||
        is_nullptr((*out_terminal_inquery_gr), ERR_OUTPUT_MODEL_IS_NULL))
        return (error_context_);

    /* create new object from model helper class */
    const centity_terminal_inquery_group_t inquery_group_protobuf_instance(*in_terminal_inquery_gr);
    return (evaluate_request<InputTerminalInqueryGroup_t, OutputTerminalInqueryGroup_t>(inquery_group_protobuf_instance, (*out_terminal_inquery_gr)));
}

/**
 * @section TerminalCardChargeWallet
 * @link file:///../OnlineTicket/ProtoBuf_Project/Header/API/MessageProto.h @endlink
 *
 * @param in_terminal_card_charge_Wallet
 * @param out_terminal_card_charge_Wallet
 * @return RPC_ERROR_CTX
 */
RPC_ERROR_CTX CProxyCtrlr::terminal_card_charge_wallet(const InputTerminalCardChargeWallet_t *const *in_terminal_card_charge_Wallet, OutputTerminalCardChargeWallet_t **out_terminal_card_charge_Wallet) const
{
    if (is_nullptr(in_terminal_card_charge_Wallet, ERR_INPUT_MODEL_IS_NULL) ||
        is_nullptr((*in_terminal_card_charge_Wallet), ERR_INPUT_MODEL_IS_NULL))
        return (error_context_);

    if (is_nullptr(out_terminal_card_charge_Wallet, ERR_OUTPUT_MODEL_IS_NULL) ||
        is_nullptr((*out_terminal_card_charge_Wallet), ERR_OUTPUT_MODEL_IS_NULL))
        return (error_context_);

    /* create new object from model helper class */
    const centity_terminal_charg_card_wallet_consume_t inquery_group_protobuf_instance(*in_terminal_card_charge_Wallet);
    return (evaluate_request<InputTerminalCardChargeWallet_t, OutputTerminalCardChargeWallet_t>(inquery_group_protobuf_instance, (*out_terminal_card_charge_Wallet)));
}

/**
 * @section TerminalInqueryLastTransaction
 * @link file:///../OnlineTicket/ProtoBuf_Project/Header/API/MessageProto.h @endlink
 *
 * @param in_terminal_inquery_last_transaction
 * @param out_terminal_inquery_last_transaction
 * @return RPC_ERROR_CTX
 */
RPC_ERROR_CTX CProxyCtrlr::terminal_inquery_last_transaction(const InputTerminalInqueryLastTransaction_t *const *in_terminal_inquery_last_transaction, OutputTerminalInqueryLastTransaction_t **out_terminal_inquery_last_transaction) const
{
    if (is_nullptr(in_terminal_inquery_last_transaction, ERR_INPUT_MODEL_IS_NULL) ||
        is_nullptr((*in_terminal_inquery_last_transaction), ERR_INPUT_MODEL_IS_NULL))
        return (error_context_);

    if (is_nullptr(out_terminal_inquery_last_transaction, ERR_OUTPUT_MODEL_IS_NULL) ||
        is_nullptr((*out_terminal_inquery_last_transaction), ERR_OUTPUT_MODEL_IS_NULL))
        return (error_context_);

    /* create new object from model helper class */
    const centity_terminal_inquery_last_trans_t inquery_last_protobuf_instance(*in_terminal_inquery_last_transaction);
    return (evaluate_request<InputTerminalInqueryLastTransaction_t, OutputTerminalInqueryLastTransaction_t>(inquery_last_protobuf_instance, (*out_terminal_inquery_last_transaction)));
}
