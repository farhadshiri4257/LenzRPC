/**
 * @file terminalCardConsume.h
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
#ifndef __MODEL_CONSUME_CARD_H_
#define __MODEL_CONSUME_CARD_H_ 1

#include "../Interface/IModel.h"
#include "../Helper//Property.h"

NAMESPACE(proto_proxy)
{
    NAMESPACE(dataModel)
    {
        class CEntityTerminalCardConsume final : public proto_proxy::interface::IEntityModel<InputTerminalCardConsume_t, OutputTerminalCardConsume_t>
        {
        private:
            /** @attention define immutable property, in outside of class only readable
             *              if write into property, The compiler will generate a legitimate warning
             */
            /*1:*/ proto_proxy::entity::CProperty<transaction_type_t> transaction_type_;
            /*2:*/ proto_proxy::entity::CProperty<company_code_t> company_id_;
            /*3:*/ proto_proxy::entity::CProperty<user_group_t> user_group_id_;
            /*4:*/ proto_proxy::entity::CProperty<verification_status_t> verification_status_id_;
            /*5:*/ proto_proxy::entity::CProperty<issuer_type_t> issuer_id_;
            /*6:*/ proto_proxy::entity::CProperty<uint32_t> transaction_fee_;
            /*7:*/ proto_proxy::entity::CProperty<uint32_t> transaction_counter_;
            /*8:*/ proto_proxy::entity::CProperty<long double> latitude_;
            /*9:*/ proto_proxy::entity::CProperty<long double> longitude_;
            /*10:*/ proto_proxy::entity::CProperty<std::string> hash_key_block2_;
            /*11:*/ proto_proxy::entity::CProperty<std::string> expired_date_id_;
            /*12:*/ proto_proxy::entity::CProperty<std::string> transaction_date_;
            /*13:*/ proto_proxy::entity::CProperty<std::string> transaction_UID_;
            /*14:*/ proto_proxy::entity::CProperty<std::string> wallet_uid_buffer_;
            /*15:*/ proto_proxy::entity::CProperty<std::string> validation_buffer_;
            /*16:*/ proto_proxy::entity::CProperty<std::string> card_serial_;
            /*17:*/ proto_proxy::entity::CProperty<std::string> serial_hw_;
            /*18:*/ proto_proxy::entity::CProperty<std::string> serial_logic_hw_;

            void map_property_data(const InputTerminalCardConsume_t *const &input_entity) override
            {
                ((std::string)hash_key_block2_).reserve(HASH_KEY_BLOCK2_SIZE);
                ((std::string)expired_date_id_).reserve(DATE_BUFFER_SIZE);
                ((std::string)transaction_date_).reserve(DATE_BUFFER_SIZE);
                ((std::string)transaction_UID_).reserve(UID_BUFFER_SIZE);
                ((std::string)wallet_uid_buffer_).reserve(WALLET_UID_BUFFER_SIZE);
                ((std::string)validation_buffer_).reserve(VALIDATION_BUFFER_SIZE);
                ((std::string)card_serial_).reserve(CARD_SERIAL_SIZE);
                ((std::string)serial_hw_).reserve(SERIAL_HW_FIELD_SIZE);
                ((std::string)serial_logic_hw_).reserve(SERIAL_HW_FIELD_SIZE);

                transaction_type_ = std::move(input_entity->transaction_type);             /*1:*/
                company_id_ = std::move(input_entity->company_id);                         /*2:*/
                user_group_id_ = std::move(input_entity->user_group_id);                   /*3:*/
                verification_status_id_ = std::move(input_entity->verification_status_id); /*4:*/
                issuer_id_ = std::move(input_entity->issuer_id);                           /*5:*/
                transaction_fee_ = std::move(input_entity->transaction_fee);               /*6:*/
                transaction_counter_ = std::move(input_entity->transaction_counter);       /*7:*/
                latitude_ = std::move(input_entity->latitude);                             /*8:*/
                longitude_ = std::move(input_entity->longitude);                           /*9:*/
                hash_key_block2_ = (std::string)input_entity->hash_key_block2;             /*10:*/
                expired_date_id_ = (std::string)input_entity->expired_date_id;             /*11:*/
                transaction_date_ = (std::string)input_entity->transaction_date;           /*12:*/
                transaction_UID_ = (std::string)input_entity->transaction_UID;             /*13:*/
                wallet_uid_buffer_ = (std::string)input_entity->wallet_uid_buffer;         /*14:*/
                validation_buffer_ = (std::string)input_entity->validation_buffer;         /*15:*/
                card_serial_ = (std::string)input_entity->card_serial;                     /*16:*/
                serial_hw_ = (std::string)input_entity->serial_hw;                         /*17:*/
                serial_logic_hw_ = (std::string)input_entity->serial_logic_hw;             /*18:*/
            }

        public:
            explicit CEntityTerminalCardConsume(const InputTerminalCardConsume_t *const &input_entity) : transaction_type_(INITIALIZER_TRANS_TYPE),          /*1:*/
                                                                                                         company_id_(INITIALIZER_COMPANY_CODE),              /*2:*/
                                                                                                         user_group_id_(INITIALIZER_USER_GROUP),             /*3:*/
                                                                                                         verification_status_id_(INITIALIZER_VERIFY_STATUS), /*4:*/
                                                                                                         issuer_id_(INITIALIZER_ISSUER_TYPE),                /*5:*/
                                                                                                         transaction_fee_{0},                                /*6:*/
                                                                                                         transaction_counter_{0},                            /*7:*/
                                                                                                         latitude_(0.0),                                     /*8:*/
                                                                                                         longitude_(0.0),                                    /*9:*/
                                                                                                         hash_key_block2_{""},                               /*10:*/
                                                                                                         expired_date_id_{""},                               /*11:*/
                                                                                                         transaction_date_{""},                              /*12:*/
                                                                                                         transaction_UID_{""},                               /*13:*/
                                                                                                         wallet_uid_buffer_{""},                             /*14:*/
                                                                                                         validation_buffer_{""},                             /*15:*/
                                                                                                         card_serial_{""},                                   /*16:*/
                                                                                                         serial_hw_{""},                                     /*17:*/
                                                                                                         serial_logic_hw_{""}                                /*18:*/
            {
                static_assert(sizeof(InputTerminalCardConsume_t) < MAX_PROTO_BUF_MODEL_SIZE, "Input model size must be less than < 512byte .");
                static_assert(sizeof(RPC_TERMINAL_CARD_CONSUME) < REMOTE_FUNCTION_NAME_SIZE, "RPC_TERMINAL_CARD_CONSUME function name can't great than 32 byte!");

                base_protobuf_frame_ctx_.remote_function.append(RPC_TERMINAL_CARD_CONSUME);
                header_.table[RPC_FUNC_IDX] = (uint8_t)(base_protobuf_frame_ctx_.remote_function.length() & 0xFF);
                map_property_data(input_entity);
            }

            ~CEntityTerminalCardConsume() {}
            CEntityTerminalCardConsume(const CEntityTerminalCardConsume &) = delete;
            CEntityTerminalCardConsume &operator=(const CEntityTerminalCardConsume &) = delete;
            CEntityTerminalCardConsume(CEntityTerminalCardConsume &&) = delete;
            CEntityTerminalCardConsume &operator=(CEntityTerminalCardConsume &&) = delete;

            using proto_proxy::interface::IEntityModel<InputTerminalCardConsume_t, OutputTerminalCardConsume_t>::get_rx_protobuf_stream;
            using proto_proxy::interface::IEntityModel<InputTerminalCardConsume_t, OutputTerminalCardConsume_t>::get_tx_protobuf_stream;

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
                             << std::to_string(transaction_type_)       /*1:*/
                             << std::to_string(company_id_)             /*2:*/
                             << std::to_string(user_group_id_)          /*3:*/
                             << std::to_string(verification_status_id_) /*4:*/
                             << std::to_string(issuer_id_)              /*5:*/
                             << std::to_string(transaction_fee_)        /*6:*/
                             << std::to_string(transaction_counter_)    /*7:*/
                             << std::to_string(latitude_)               /*8:*/
                             << std::to_string(longitude_)              /*9:*/
                             << (std::string)hash_key_block2_           /*10:*/
                             << (std::string)expired_date_id_           /*11:*/
                             << (std::string)transaction_date_          /*12:*/
                             << (std::string)transaction_UID_           /*13:*/
                             << (std::string)wallet_uid_buffer_         /*14:*/
                             << (std::string)validation_buffer_         /*15:*/
                             << (std::string)card_serial_               /*16:*/
                             << (std::string)serial_hw_                 /*17:*/
                             << (std::string)serial_logic_hw_           /*18:*/
                             << (std::string)ACK_MAGIC_BYTE
                             << '\0';

                header_.table[MODEL_IDX_1] = (uint8_t)(std::to_string(transaction_type_).length() & 0xFF);
                header_.table[MODEL_IDX_2] = (uint8_t)(std::to_string(company_id_).length() & 0xFF);
                header_.table[MODEL_IDX_3] = (uint8_t)(std::to_string(user_group_id_).length() & 0xFF);
                header_.table[MODEL_IDX_4] = (uint8_t)(std::to_string(verification_status_id_).length() & 0xFF);
                header_.table[MODEL_IDX_5] = (uint8_t)(std::to_string(issuer_id_).length() & 0xFF);
                header_.table[MODEL_IDX_6] = (uint8_t)(std::to_string(transaction_fee_).length() & 0xFF);
                header_.table[MODEL_IDX_7] = (uint8_t)(std::to_string(transaction_counter_).length() & 0xFF);
                header_.table[MODEL_IDX_8] = (uint8_t)(std::to_string(latitude_).length() & 0xFF);
                header_.table[MODEL_IDX_9] = (uint8_t)(std::to_string(longitude_).length() & 0xFF);
                header_.table[MODEL_IDX_10] = (uint8_t)(((std::string)hash_key_block2_).length() & 0xFF);
                header_.table[MODEL_IDX_11] = (uint8_t)(((std::string)expired_date_id_).length() & 0xFF);
                header_.table[MODEL_IDX_12] = (uint8_t)(((std::string)transaction_date_).length() & 0xFF);
                header_.table[MODEL_IDX_13] = (uint8_t)(((std::string)transaction_UID_).length() & 0xFF);
                header_.table[MODEL_IDX_14] = (uint8_t)(((std::string)wallet_uid_buffer_).length() & 0xFF);
                header_.table[MODEL_IDX_15] = (uint8_t)(((std::string)validation_buffer_).length() & 0xFF);
                header_.table[MODEL_IDX_16] = (uint8_t)(((std::string)card_serial_).length() & 0xFF);
                header_.table[MODEL_IDX_17] = (uint8_t)(((std::string)serial_hw_).length() & 0xFF);
                header_.table[MODEL_IDX_18] = (uint8_t)(((std::string)serial_logic_hw_).length() & 0xFF);
                header_.table[MODEL_IDX_19] = (uint8_t)(((std::string)ACK_MAGIC_BYTE).length() & 0xFF);

                for (int32_t idx = MODEL_IDX_1; idx < MODEL_IDX_20; idx++)
                    actual_size_ += header_.table[idx];

                /*2- get offset pointer of protohdr instance*/
                return (get_tx_protobuf_stream());
            }

            /**
             * @brief Get the rx message header object
             *
             * @return const rx_msghdr_t&
             */
            inline protohdr_t &deserialize_entity(OutputTerminalCardConsume_t *&out_entity_model) override
            {
                /*1- deserialize protobuf stream to stream buffer*/
                base_cproto_buf_.deserialize_protobuf<OutputTerminalCardConsume_t, sizeof(OutputTerminalCardConsume_t)>(out_entity_model);
                /*2- get offset of protohdr */
                return (get_rx_protobuf_stream());
            }
        };
        using centity_terminal_card_consume_t = CEntityTerminalCardConsume;
    }
}

#endif