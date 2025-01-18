/**
 * @file IModel.h
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
#ifndef __INTERFACE_MODEL_H_
#define __INTERFACE_MODEL_H_ 1

#include "../Common.h"
#include "../Connection/ProtocolBuf.h"
#include "../Helper/Property.h"
#include "../Helper/StreamPool.h"

NAMESPACE(global)
{
    extern std::string g_hardware_serial;
    extern std::string g_mac_address;
}

NAMESPACE(proto_proxy)
{
    NAMESPACE(interface)
    {
        typedef struct __attribute__((__aligned__(RTE_CACHELINE))) protobuf_frame_ctx
        {
            std::string mac_address;
            std::string crc32_buf;
            std::string remote_function;
            protobuf_frame_ctx()
            {
                mac_address.reserve(RSW_ETH_ALEN);
                crc32_buf.reserve(CRC32_BUFFER_LEN);
                remote_function.reserve(REMOTE_FUNCTION_NAME_SIZE);
            }
        } protobuf_frame_ctx_t;

        template <typename IN_MODEL, typename OUT_MODEL>
        class IEntityModel
        {
        private:
        protected:
            uint32_t sum_istream_bit_;
            size_t actual_size_;
            proto_proxy::immutable::cstring_stream_pool_t stream_pool_ __attribute__((__aligned__(RTE_CACHELINE))); // 8b
            table_size_t header_ __attribute__((__aligned__(RTE_CACHELINE)));                                       // 32b
            proto_proxy::network::cproto_buf_t base_cproto_buf_ __attribute__((__aligned__(RTE_CACHELINE)));        // 72b
            protobuf_frame_ctx_t base_protobuf_frame_ctx_ __attribute__((__aligned__(RTE_CACHELINE)));              // 128b

            /** @bug Important:
             *          When using std::istreamstring for stream pool, at runtime sometimes the first 8 bytes of the stream buffer
             *          in the specified class created an ambiguous data overlap.
             *          that's why this was observed in Android NDKr15 - C++14,
             *          so it was replaced by <StreamPool.h> helper class.
             */
            // std::istringstream base_entity_istream_ __attribute__((__aligned__(RTE_CACHELINE)));             // 392b

            virtual const protohdr_t &serialize_entity() = 0;
            virtual protohdr_t &deserialize_entity(OUT_MODEL *&) = 0;
            virtual void map_property_data(const IN_MODEL *const &input_entity) = 0;

            /**
             * @brief Get the tx message header object
             *
             * @return const tx_msghdr_t&
             */
            inline const protohdr_t &get_tx_protobuf_stream()
            {
                /*1- serialize stream buffer to protobuf stream */
                sum_istream_bit_ = check_validation_data(stream_pool_.rdbuf()->str());
                // stream_pool_.rdbuf()->str().shrink_to_fit();

                /*2- estimated CRC32 for input para values */
                unsigned long evaluated_crc32 = 0;
                evaluated_crc32 = crc32_generator((const Bytef *)stream_pool_.rdbuf()->str().c_str(),
                                                    stream_pool_.rdbuf()->str().length(),
                                                    header_.table[MAC_ADDR_IDX] + header_.table[CRC32_BUF_IDX]);
                const auto crc_buf = std::to_string(evaluated_crc32);
                memcpy((void *)&stream_pool_.rdbuf()->str()[header_.table[MAC_ADDR_IDX]],
                       crc_buf.c_str(),
                       crc_buf.length());

                base_cproto_buf_.serialize_protobuf(header_,
                                                    stream_pool_.rdbuf()->str().c_str(),
                                                    stream_pool_.rdbuf()->str().length(),
                                                    SIZEOF_FIELD(pay_load_t, magic_buf) +
                                                        SIZEOF_FIELD(pay_load_t, crc_chk) +
                                                        SIZEOF_FIELD(pay_load_t, header) +
                                                        header_.table[MAC_ADDR_IDX] +
                                                        header_.table[CRC32_BUF_IDX] +
                                                        header_.table[RPC_FUNC_IDX] +
                                                        actual_size_);
                /*2- get offset of tx_msghdr */
                return (base_cproto_buf_.get_tx_protobuf_stream());
            }

            /**
             * @brief Get the rx message header object
             *
             * @return const rx_msghdr_t&
             */
            inline protohdr_t &get_rx_protobuf_stream()
            {
                return (base_cproto_buf_.get_rx_protobuf_stream());
            }

        public:
            /*const CEntityInterface &get_instance() const
            {
                return (static_cast<const Derived &>(*this));
            }*/

            IEntityModel() : sum_istream_bit_(0), actual_size_(0), stream_pool_(MAX_BUF_FOR_STREAM)
            {
                memset(&header_.table, 0, sizeof(table_size_t));

                header_.table[MAGIC_KEY_IDX] = (uint8_t)(SIZEOF_FIELD(pay_load_t, magic_buf) & 0xFF);
                header_.table[CRC_SIZE_IDX] = (uint8_t)(SIZEOF_FIELD(pay_load_t, crc_chk) & 0xFF);
                header_.table[TABLE_SIZE_IDX] = (uint8_t)(SIZEOF_FIELD(pay_load_t, header) & 0xFF);

                base_protobuf_frame_ctx_.mac_address.append(global::g_mac_address);
                header_.table[MAC_ADDR_IDX] = (uint8_t)(base_protobuf_frame_ctx_.mac_address.length() & 0xFF);

                base_protobuf_frame_ctx_.crc32_buf.append(CRC32_PATTERN);
                header_.table[CRC32_BUF_IDX] = (uint8_t)(base_protobuf_frame_ctx_.crc32_buf.length() & 0xFF);
            }

            virtual ~IEntityModel() {}
            IEntityModel(const IEntityModel &) = delete;
            IEntityModel &operator=(const IEntityModel &) = delete;
            IEntityModel(IEntityModel &&) = delete;
            IEntityModel &operator=(IEntityModel &) = delete;

            /**
             * @brief
             *
             * @return uint32_t
             */
            uint32_t get_sum_istream_bit() const
            {
                return (sum_istream_bit_);
            }

            /**
             * @brief
             *
             * @return true
             * @return false
             */
            inline bool clear_buffer()
            {
                return (stream_pool_.clear_buf());
            }
        };
    }
}

#endif
