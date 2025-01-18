/**
 * @file terminalConsumeVoucher.h
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
#ifndef __MODEL_CONSUME_VOUCHER_H_
#define __MODEL_CONSUME_VOUCHER_H_ 1

#include "../Interface/IModel.h"
#include "../Helper//Property.h"

NAMESPACE(proto_proxy)
{
    NAMESPACE(dataModel)
    {
        class CEntityTerminalConsumeVoucher final : public proto_proxy::interface::IEntityModel<InputTerminalConsumeVoucher_t, OutputTerminalConsumeVoucher_t>
        {
        private:
            /** @attention define immutable property, in outside of class only readable
             *              if write into property, The compiler will generate a legitimate warning
             */
            /*1:*/ proto_proxy::entity::CProperty<voucher_type_t> voucher_type_;
            /*2:*/ proto_proxy::entity::CProperty<int64_t> voucher_serial_;
            /*3:*/ proto_proxy::entity::CProperty<int64_t> voucher_fee_;
            /*4:*/ proto_proxy::entity::CProperty<double> latitude_;
            /*5:*/ proto_proxy::entity::CProperty<double> longitude_;
            /*6:*/ proto_proxy::entity::CProperty<std::string> transaction_UID_;
            /*7:*/ proto_proxy::entity::CProperty<std::string> serial_logic_hw_;
            /*8:*/ proto_proxy::entity::CProperty<std::string> serial_hw_;
            /*9:*/ proto_proxy::entity::CProperty<std::string> voucher_message_buffer_;

            void map_property_data(const InputTerminalConsumeVoucher_t *const &input_entity) override
            {
                ((std::string)transaction_UID_).reserve(UID_BUFFER_SIZE);
                ((std::string)serial_hw_).reserve(SERIAL_HW_FIELD_SIZE);
                ((std::string)serial_logic_hw_).reserve(SERIAL_HW_FIELD_SIZE);
                ((std::string)voucher_message_buffer_).reserve(VOUCHER_BUFFER_SIZE);

                /*1:*/ voucher_type_ = std::move(input_entity->voucher_type);
                /*2:*/ voucher_serial_ = std::move(input_entity->voucher_serial);
                /*3:*/ voucher_fee_ = std::move(input_entity->voucher_fee);
                /*4:*/ latitude_ = std::move(input_entity->latitude);
                /*5:*/ longitude_ = std::move(input_entity->longitude);
                /*6:*/ transaction_UID_ = (std::string)input_entity->transaction_UID;
                /*7:*/ serial_logic_hw_ = (std::string)input_entity->serial_logic_hw;
                /*8:*/ serial_hw_ = (std::string)input_entity->serial_hw;
                /*9:*/ voucher_message_buffer_ = (std::string)input_entity->voucher_message_buffer;
            }

        public:
            explicit CEntityTerminalConsumeVoucher(const InputTerminalConsumeVoucher_t *const &input_entity) : voucher_type_(INITIALIZER_VOUCHER_TYPE), /*1:*/
                                                                                                               voucher_serial_(0),                      /*2:*/
                                                                                                               voucher_fee_(0),                         /*3:*/
                                                                                                               latitude_(0.0),                          /*4:*/
                                                                                                               longitude_(0.0),                         /*5:*/
                                                                                                               transaction_UID_{""},                    /*6:*/
                                                                                                               serial_logic_hw_{""},                    /*7:*/
                                                                                                               serial_hw_{""},                          /*8:*/
                                                                                                               voucher_message_buffer_{""}              /*9:*/
            {
                static_assert(sizeof(InputTerminalConsumeVoucher_t) < MAX_PROTO_BUF_MODEL_SIZE, "Input model size must be less than < 512byte .");
                static_assert(sizeof(RPC_TERMINAL_CONSUME_VOUCHER) < REMOTE_FUNCTION_NAME_SIZE, "RPC_TERMINAL_CONSUME_VOUCHER function name can't great than 32 byte!");

                base_protobuf_frame_ctx_.remote_function.append(RPC_TERMINAL_CONSUME_VOUCHER);
                header_.table[RPC_FUNC_IDX] = (uint8_t)(base_protobuf_frame_ctx_.remote_function.length() & 0xFF);
                map_property_data(input_entity);
            }

            ~CEntityTerminalConsumeVoucher() {}
            CEntityTerminalConsumeVoucher(const CEntityTerminalConsumeVoucher &) = delete;
            CEntityTerminalConsumeVoucher &operator=(const CEntityTerminalConsumeVoucher &) = delete;
            CEntityTerminalConsumeVoucher(CEntityTerminalConsumeVoucher &&) = delete;
            CEntityTerminalConsumeVoucher &operator=(CEntityTerminalConsumeVoucher &&) = delete;

            using proto_proxy::interface::IEntityModel<InputTerminalConsumeVoucher_t, OutputTerminalConsumeVoucher_t>::get_rx_protobuf_stream;
            using proto_proxy::interface::IEntityModel<InputTerminalConsumeVoucher_t, OutputTerminalConsumeVoucher_t>::get_tx_protobuf_stream;

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
                             << std::to_string(voucher_type_)         /*1:*/
                             << std::to_string(voucher_serial_)       /*2:*/
                             << std::to_string(voucher_fee_)          /*3:*/
                             << std::to_string(latitude_)             /*4:*/
                             << std::to_string(longitude_)            /*5:*/
                             << (std::string)transaction_UID_         /*6:*/
                             << (std::string)serial_logic_hw_         /*7:*/
                             << (std::string)serial_hw_               /*8:*/
                             << (std::string)voucher_message_buffer_  /*9:*/
                             << (std::string)ACK_MAGIC_BYTE
                             << '\0';

                header_.table[MODEL_IDX_1] = (uint8_t)(std::to_string(voucher_type_).length() & 0xFF);
                header_.table[MODEL_IDX_2] = (uint8_t)(std::to_string(voucher_serial_).length() & 0xFF);
                header_.table[MODEL_IDX_3] = (uint8_t)(std::to_string(voucher_fee_).length() & 0xFF);
                header_.table[MODEL_IDX_4] = (uint8_t)(std::to_string(latitude_).length() & 0xFF);
                header_.table[MODEL_IDX_5] = (uint8_t)(std::to_string(longitude_).length() & 0xFF);
                header_.table[MODEL_IDX_6] = (uint8_t)(((std::string)transaction_UID_).length() & 0xFF);
                header_.table[MODEL_IDX_7] = (uint8_t)(((std::string)serial_logic_hw_).length() & 0xFF);
                header_.table[MODEL_IDX_8] = (uint8_t)(((std::string)serial_hw_).length() & 0xFF);
                header_.table[MODEL_IDX_9] = (uint8_t)(((std::string)voucher_message_buffer_).length() & 0xFF);
                header_.table[MODEL_IDX_10] = (uint8_t)(((std::string)ACK_MAGIC_BYTE).length() & 0xFF);

                for (int32_t idx = MODEL_IDX_1; idx < MODEL_IDX_11; idx++)
                    actual_size_ += header_.table[idx];

                /*2- get offset pointer of protohdr instance*/
                return (get_tx_protobuf_stream());
            }

            /**
             * @brief Get the rx message header object
             *
             * @return const rx_msghdr_t&
             */
            inline protohdr_t &deserialize_entity(OutputTerminalConsumeVoucher_t *&out_entity_model) override
            {
                /*1- deserialize protobuf stream to stream buffer*/
                base_cproto_buf_.deserialize_protobuf<OutputTerminalConsumeVoucher_t, sizeof(OutputTerminalConsumeVoucher_t)>(out_entity_model);
                /*2- get offset of protohdr */
                return (get_rx_protobuf_stream());
            }
        };
        using centity_terminal_consume_voucher_t = CEntityTerminalConsumeVoucher;
    }
}

#endif