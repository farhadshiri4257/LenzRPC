/**
 * @file IMessageProto.h
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
#ifndef __IMESSAGE_H__
#define __IMESSAGE_H__ 1

#include "../Common.h"

NAMESPACE(proto_proxy)
{
    NAMESPACE(interface)
    {
        class IMessageProto
        {
        protected:
            IMessageProto(){};
            virtual ~IMessageProto(){};

            virtual RPC_ERROR_CTX request_for_connect(const InputRequestForConnect_t *const *, OutputRequestForConnect_t **) const = 0;
            virtual RPC_ERROR_CTX request_for_keys(const InputRequestForKeys_t *const *, OutputRequestForKeys_t **) const = 0;
            virtual RPC_ERROR_CTX redirect_to_issuer(const InputRedirectToIssuer_t *const *, OutputRedirectToIssuer_t **) const = 0;
            virtual RPC_ERROR_CTX terminal_consume_voucher(const InputTerminalConsumeVoucher_t *const *, OutputTerminalConsumeVoucher_t **) const = 0;
            virtual RPC_ERROR_CTX terminal_card_consume(const InputTerminalCardConsume_t *const *, OutputTerminalCardConsume_t **) const = 0;
            virtual RPC_ERROR_CTX terminal_group_card_consume(const InputTerminalGroupCardConsume_t *const *, OutputTerminalGroupCardConsume_t **) const = 0;
            virtual RPC_ERROR_CTX terminal_QR_consume(const InputTerminalQRConsume_t *const *, OutputTerminalQRConsume_t **) const = 0;
            virtual RPC_ERROR_CTX terminal_group_QR_consume(const InputTerminalGroupQRConsume_t *const *, OutputTerminalGroupQRConsume_t **) const = 0;
            virtual RPC_ERROR_CTX terminal_inquery(const InputTerminalInquery_t *const *, OutputTerminalInquery_t **) const = 0;
            virtual RPC_ERROR_CTX terminal_inquery_group(const InputTerminalInqueryGroup_t *const *, OutputTerminalInqueryGroup_t **) const = 0;
            virtual RPC_ERROR_CTX terminal_card_charge_wallet(const InputTerminalCardChargeWallet_t *const *, OutputTerminalCardChargeWallet_t **) const = 0;
            virtual RPC_ERROR_CTX terminal_inquery_last_transaction(const InputTerminalInqueryLastTransaction_t *const *, OutputTerminalInqueryLastTransaction_t **) const = 0;
        };
    }
}

#endif