/**
 * @file Proxy.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __PROXY_H_
#define __PROXY_H_ 1

#include "../Interface/IMessageProto.h"
#include "../Connection/ServerCtrlr.h"

NAMESPACE(proto_proxy)
{
    NAMESPACE(dataModel)
    {
        class CProxyCtrlr : public proto_proxy::interface::IMessageProto
        {
        private:
            using pred_t = const void *;
            using pointer_predicate_t = bool (*)(pred_t, const error_num_t, error_context_t *);
            pointer_predicate_t is_nullptr_predicate_ = [](auto object, const auto error_num, auto *error_context)
            {
                error_context->error_num = error_num;
                return (object == nullptr ? true : false);
            };
            error_context_t *error_context_;
            const proto_proxy::network::cserver_ctrlr_t &remote_endpoint_;

            template <typename IN_MODEL, typename OUT_MODEL_ENTITY, typename MODEL_CLASS>
            RPC_ERROR_CTX evaluate_request(const MODEL_CLASS &, OUT_MODEL_ENTITY *&) const;


        public:
            RPC_ERROR_CTX request_for_connect(const InputRequestForConnect_t *const *in_req_connect, OutputRequestForConnect_t **out_req_connect) const override;
            RPC_ERROR_CTX request_for_keys(const InputRequestForKeys_t *const *, OutputRequestForKeys_t **) const override;
            RPC_ERROR_CTX redirect_to_issuer(const InputRedirectToIssuer_t *const *, OutputRedirectToIssuer_t **) const override;
            RPC_ERROR_CTX terminal_consume_voucher(const InputTerminalConsumeVoucher_t *const *, OutputTerminalConsumeVoucher_t **) const override;
            RPC_ERROR_CTX terminal_card_consume(const InputTerminalCardConsume_t *const *, OutputTerminalCardConsume_t **) const override;
            RPC_ERROR_CTX terminal_group_card_consume(const InputTerminalGroupCardConsume_t *const *, OutputTerminalGroupCardConsume_t **) const override;
            RPC_ERROR_CTX terminal_QR_consume(const InputTerminalQRConsume_t *const *, OutputTerminalQRConsume_t **) const override;
            RPC_ERROR_CTX terminal_group_QR_consume(const InputTerminalGroupQRConsume_t *const *, OutputTerminalGroupQRConsume_t **) const override;
            RPC_ERROR_CTX terminal_inquery(const InputTerminalInquery_t *const *, OutputTerminalInquery_t **) const override;
            RPC_ERROR_CTX terminal_inquery_group(const InputTerminalInqueryGroup_t *const *, OutputTerminalInqueryGroup_t **) const override;
            RPC_ERROR_CTX terminal_card_charge_wallet(const InputTerminalCardChargeWallet_t *const *, OutputTerminalCardChargeWallet_t **) const override;
            RPC_ERROR_CTX terminal_inquery_last_transaction(const InputTerminalInqueryLastTransaction_t *const *, OutputTerminalInqueryLastTransaction_t **) const override;

        public:
            CProxyCtrlr(const proto_proxy::network::cserver_ctrlr_t &server) : error_context_(new (std::nothrow) error_context_t), remote_endpoint_(server) 
            {
                static_assert(std::is_base_of<proto_proxy::interface::IMessageProto, CProxyCtrlr>::value,
                              "CProxyCtrlr must be derived from base proto_proxy::interface::IMessageProto");

                if (&remote_endpoint_ == nullptr)
                    LOG_INFO("server is NULL ");
            }
            ~CProxyCtrlr()
            {
                if (error_context_ != nullptr)
                    delete error_context_;
            }
            CProxyCtrlr(const CProxyCtrlr &) = delete;
            CProxyCtrlr &operator=(const CProxyCtrlr &) = delete;
            CProxyCtrlr(CProxyCtrlr &&rhs) = delete;
            CProxyCtrlr &operator=(CProxyCtrlr &&rhs) = delete;
        };
        using cproxy_ctrlr_t = CProxyCtrlr;
    }
}

#endif