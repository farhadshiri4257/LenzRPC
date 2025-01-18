/**
 * @file ProtocolBuf.h
 * @author f.shiri (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-08-16
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
#ifndef __PROTOCOL_BUF_H_
#define __PROTOCOL_BUF_H_ 1

#include "../Common.h"

#define TRANSACTION_MSG_IDX_TX 0
#define TX_MSG_IO_VECTOR_SIZE 1
#define TRANSACTION_MSG_IDX_RX 0
#define RX_MSG_IO_VECTOR_SIZE 1


NAMESPACE(proto_proxy)
{
    NAMESPACE(network)
    {
        /*typedef struct
        {
            protobuf_t tx_iovec;
            protobuf_t rx_iovec;
            protohdr_t tx_msghdr;
            protohdr_t rx_msghdr;
        } iovec_context_t;
        */
        class CProtoBuf final
        {
        private:
            pay_load_t pay_load_;
            protobuf_t tx_iovec_;
            protobuf_t rx_iovec_;
            protohdr_t tx_msghdr_;
            protohdr_t rx_msghdr_;

        public:
            CProtoBuf() : pay_load_{0}
            {
                memset(&tx_iovec_, 0, sizeof(protobuf_t));
                memset(&rx_iovec_, 0, sizeof(protobuf_t));
                memset(&tx_msghdr_, 0, sizeof(protohdr_t));
                memset(&rx_msghdr_, 0, sizeof(protohdr_t));
            }

            ~CProtoBuf() {}
            CProtoBuf(const CProtoBuf &) = delete;
            CProtoBuf &operator=(const CProtoBuf &) = delete;
            CProtoBuf(CProtoBuf &&) = delete;
            CProtoBuf &operator=(CProtoBuf &&) = delete;

            /**
             * @brief Set the tx proto buf stream object
             *
             */
            inline void serialize_protobuf(const table_size_t &header, const byte8_t *pay_load_stream, std::size_t pay_load_stream_size, size_t pay_load_size)
            {
                if (pay_load_stream == nullptr)
                    LOG_ERR("input stream protobuf is NULL!");

                /*1- set the magic buffer into header */
                pay_load_.magic_buf = MAGIC_BYTE;

                /*2- set the data body length into header */
                memmove(&pay_load_.header, (table_size_t *)&header, sizeof(table_size_t));

                /*3- set the data body stream into header */
                memmove(pay_load_.stream, pay_load_stream, pay_load_stream_size);

                /*4- set the crc chk for data body stream into header */
                pay_load_.crc_chk = check_validation_data(pay_load_.stream);

                tx_iovec_.iov_base = (void *)&pay_load_;
                tx_iovec_.iov_len = pay_load_size;//PACKET_ALIGN(pay_load_size); 
                tx_msghdr_.msg_name = nullptr;
                tx_msghdr_.msg_namelen = 0;
                tx_msghdr_.msg_iov = &tx_iovec_;
                tx_msghdr_.msg_iovlen = TX_MSG_IO_VECTOR_SIZE;
                LOG_ERR("tx_iovec_.iov_len(%d)", tx_iovec_.iov_len);
            }


            /**
             * @brief Set the RX proto buf stream object
             *
             */
            template <typename OUT_MODEL, size_t OUT_MODEL_SIZE>
            inline void deserialize_protobuf(OUT_MODEL *&out_entity_model)
            {
                if (out_entity_model == nullptr)
                    LOG_ERR("output entity model stream protobuf is NULL!");

                rx_iovec_.iov_base = (void *)out_entity_model;
                rx_iovec_.iov_len = OUT_MODEL_SIZE;

                rx_msghdr_.msg_name = nullptr;
                rx_msghdr_.msg_namelen = 0;
                rx_msghdr_.msg_iov = &rx_iovec_;
                rx_msghdr_.msg_iovlen = RX_MSG_IO_VECTOR_SIZE;
            }

            /**
             * @brief Get the tx message header object
             *
             * @return const tx_msghdr_t&
             */
            inline const protohdr_t &get_tx_protobuf_stream() const
            {
                return (tx_msghdr_);
            }

            /**
             * @brief Get the rx message header object
             *
             * @return const rx_msghdr_t&
             */
            inline protohdr_t &get_rx_protobuf_stream()
            {
                return (rx_msghdr_);
            }
        };
        using cproto_buf_t = CProtoBuf;
    }
}
#endif
