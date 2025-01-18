/**
 * @file redirectToIssuer.h
 * @author f.shiri (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-08-15
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
#ifndef __MODEL_REDIRECT_ISSUER_H_
#define __MODEL_REDIRECT_ISSUER_H_ 1

#include "../Interface/IModel.h"
#include "../Helper//Property.h"

NAMESPACE(proto_proxy)
{
    NAMESPACE(dataModel)
    {
        class CEntityRedirectIssuer final : public proto_proxy::interface::IEntityModel<InputRedirectToIssuer_t, OutputRedirectToIssuer_t>
        {
        private:
            /** @attention define immutable property, in outside of class only readable
             *              if write into property, The compiler will generate a legitimate warning
             */
            /*1:*/ proto_proxy::entity::CProperty<int64_t> transaction_fee_;
            /*2:*/ proto_proxy::entity::CProperty<std::string> transaction_date_;
            /*3:*/ proto_proxy::entity::CProperty<std::string> transaction_UID_;
            /*4:*/ proto_proxy::entity::CProperty<std::string> serial_hw_;
            /*5:*/ proto_proxy::entity::CProperty<std::string> serial_logic_hw_;
            /*6:*/ proto_proxy::entity::CProperty<std::string> QR_buffer_;

            void map_property_data(const InputRedirectToIssuer_t *const &input_entity) override
            {
                ((std::string)transaction_date_).reserve(DATE_BUFFER_SIZE);
                ((std::string)transaction_UID_).reserve(UID_BUFFER_SIZE);
                ((std::string)serial_hw_).reserve(SERIAL_HW_FIELD_SIZE);
                ((std::string)serial_logic_hw_).reserve(SERIAL_HW_FIELD_SIZE);
                ((std::string)QR_buffer_).reserve(QR_BUFFER_SIZE);

                transaction_fee_ = std::move(input_entity->transaction_fee);
                transaction_date_ = (std::string)input_entity->transaction_date;
                transaction_UID_ = (std::string)input_entity->transaction_UID;
                serial_hw_ = (std::string)input_entity->serial_hw;
                serial_logic_hw_ = (std::string)input_entity->serial_logic_hw;
                QR_buffer_ = (std::string)input_entity->QR_buffer;
            }

        public:
            explicit CEntityRedirectIssuer(const InputRedirectToIssuer_t *const &input_entity) : transaction_fee_(0),
                                                                                                 transaction_date_{""},
                                                                                                 transaction_UID_{""},
                                                                                                 serial_hw_{""},
                                                                                                 serial_logic_hw_{""},
                                                                                                 QR_buffer_{""}
            {
                static_assert(sizeof(InputRedirectToIssuer_t) < MAX_PROTO_BUF_MODEL_SIZE, "Input model size must be less than < 512byte .");
                static_assert(sizeof(RPC_REDIRECT_TO_ISSUER) < REMOTE_FUNCTION_NAME_SIZE, "RPC_REDIRECT_TO_ISSUER function name can't great than 32 byte!");

                base_protobuf_frame_ctx_.remote_function.append(RPC_REDIRECT_TO_ISSUER);
                header_.table[RPC_FUNC_IDX] = (uint8_t)(base_protobuf_frame_ctx_.remote_function.length() & 0xFF);
                map_property_data(input_entity);
            }

            ~CEntityRedirectIssuer() {}
            CEntityRedirectIssuer(const CEntityRedirectIssuer &) = delete;
            CEntityRedirectIssuer &operator=(const CEntityRedirectIssuer &) = delete;
            CEntityRedirectIssuer(CEntityRedirectIssuer &&) = delete;
            CEntityRedirectIssuer &operator=(CEntityRedirectIssuer &&) = delete;

            using proto_proxy::interface::IEntityModel<InputRedirectToIssuer_t, OutputRedirectToIssuer_t>::get_rx_protobuf_stream;
            using proto_proxy::interface::IEntityModel<InputRedirectToIssuer_t, OutputRedirectToIssuer_t>::get_tx_protobuf_stream;

            /**
             * @brief
             *
             */
            inline const protohdr_t &serialize_entity() override
            {
                /*1- serialize all data into stream buffer */
                /*Important: Set tcp ethernet packet header,
                             therefore check this header by router switch server.!
                */
                stream_pool_ << base_protobuf_frame_ctx_.mac_address
                             << base_protobuf_frame_ctx_.crc32_buf
                             << base_protobuf_frame_ctx_.remote_function
                             << std::to_string(transaction_fee_)
                             << (std::string)transaction_date_
                             << (std::string)transaction_UID_
                             << (std::string)serial_hw_
                             << (std::string)serial_logic_hw_
                             << (std::string)QR_buffer_
                             << (std::string)ACK_MAGIC_BYTE   
                             << '\0';   

                header_.table[MODEL_IDX_1] = (uint8_t)(std::to_string(transaction_fee_).length() & 0xFF);
                header_.table[MODEL_IDX_2] = (uint8_t)(((std::string)transaction_date_).length() & 0xFF);
                header_.table[MODEL_IDX_3] = (uint8_t)(((std::string)transaction_UID_).length() & 0xFF);
                header_.table[MODEL_IDX_4] = (uint8_t)(((std::string)serial_hw_).length() & 0xFF);
                header_.table[MODEL_IDX_5] = (uint8_t)(((std::string)serial_logic_hw_).length() & 0xFF);
                header_.table[MODEL_IDX_6] = (uint8_t)(((std::string)QR_buffer_).length() & 0xFF);
                header_.table[MODEL_IDX_7] = (uint8_t)(((std::string)ACK_MAGIC_BYTE).length() & 0xFF);

                for (int32_t idx = MODEL_IDX_1; idx < MODEL_IDX_8; idx++)
                    actual_size_ += header_.table[idx];

                /*2- get offset pointer of protohdr instance*/
                return (get_tx_protobuf_stream());
            }

            /**
             * @brief Get the rx message header object
             *
             * @return const rx_msghdr_t&
             */
            inline protohdr_t &deserialize_entity(OutputRedirectToIssuer_t *&out_entity_model) override
            {
                /*1- deserialize protobuf stream to stream buffer*/
                base_cproto_buf_.deserialize_protobuf<OutputRedirectToIssuer_t, sizeof(OutputRedirectToIssuer_t)>(out_entity_model);
                /*2- get offset of protohdr */
                return (get_rx_protobuf_stream());
            }
        };
        using centity_redirect_issuer_t = CEntityRedirectIssuer;
    }
}

#endif