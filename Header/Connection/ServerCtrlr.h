/**
 * @file ServerCtrlr.h
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
#ifndef __SERVER_CTRLR_H_
#define __SERVER_CTRLR_H_ 1

#include <sys/epoll.h>

#include "../Common.h"

#include "../../../static_header/rild_util.h"
#include "../plugin/PluginLoader.h"

#define MAC_BCD_SIZE 3
#define MAC_ASCI_SIZE 6
#define RSW_IP_PROTOCOL IPPROTO_TCP           // RSW -> Router Switch Namespace
#define RSW_SOCKET_COMMINUTE_TYPE SOCK_STREAM // RSW -> Router Switch Namespace
#define SERVER_ADDRESS_BUF_SIZE 32
#define SERVER_POST_BUF_SIZE 16
#define SUCCESS_CONNECTED_TO_SERVER 0
#define INVALID_SERVER_PORT -1
#define INVALID_CONNECTION -1

#define NETWORK_LIB_DEVICE_NAME "./system/lib/librild_conn.so"
#define NETWORK_LIB_QUERY_NET_FUNC_NAME "queryDataStateOfC"

#define M3_SYSTEM_LIB_NAME "./system/lib/liblh_m3_sam.so"
#define M3_SYSTEM_LIB_FUNC_NAME "sys_get_hardware_serial"

#define ANDROID_LOG_LIB_NAME "./system/lib/liblog.so"
#define ANDROID_LOG_FUNC_NAME "__android_log_write"

NAMESPACE(proto_proxy)
{
    NAMESPACE(network)
    {
        using socket_handle_t = int32_t;
        using timeval_t = struct timeval;
        using ifreq_t = struct ifreq;
        using addrinfo_t = struct addrinfo;
        using query_data_state_of_c_ptr_t = int32_t (*)();
        using sys_get_hardware_serial_t = int32_t (*)(byte8_t *);
        using log_func_ptr_t = int32_t (*)(int32_t prio, const char *, const char *);
        using rild_lib_loader_t = proto_proxy::network::cplugin_loader_t<query_data_state_of_c_ptr_t>;
        using lh_m3_lib_loader_t = proto_proxy::network::cplugin_loader_t<sys_get_hardware_serial_t>;
        using epoll_event_t = struct epoll_event;

        typedef struct
        {
            byte8_t port[SERVER_POST_BUF_SIZE];
            byte8_t server[SERVER_ADDRESS_BUF_SIZE];
        } clientData_t;

        typedef struct
        {
            long recv_buf;
            long send_buf;
            double wait_time_sec; // Important: this value must be less than the router server switch ticket timer expired value.
            clientData_t client;
        } server_option_t;

        typedef struct
        {
            socket_handle_t socket_handle;
            error_num_t server_error_ctx;
            query_data_state_of_c_ptr_t queryDataStateOfC;
            sys_get_hardware_serial_t sys_get_hardware_serial;
            addrinfo_t *server_addr_info;
        } server_resource_ctx_t;

        class CServerCtrlr final
        {
        private:
            pthread_t timer_thread_id_;
            int32_t error_code_;
            server_option_t server_option_;

            static epoll_event_t s_event_list_;
            static int32_t s_monitor_event_fd_;
            static server_resource_ctx_t s_server_resource_ctx_;
            static timer_t s_tmer_id_;
            static sigset_t s_mask_;

            error_num_t set_socket_option();
            error_num_t get_device_mac_address(const byte8_t *nic_name, u_char (&mac_address)[RSW_ETH_ALEN]);
            error_num_t protobuf_validation_data(const protohdr_t &__tx_msghdr, uint32_t __sum_istream_bit);
            int32_t check_connect_status(socket_handle_t __connected_socket);

            static void session(int32_t , siginfo_t *, void *);
            static error_num_t close_session();

        public:
            CServerCtrlr(int32_t __monitor_event_fd) : error_code_(0),
                                                       server_option_{0}
            {
                s_monitor_event_fd_ = __monitor_event_fd;
                s_server_resource_ctx_.socket_handle = INVALID_SERVER_PORT;
                if (sigprocmask(SIG_UNBLOCK, &s_mask_, nullptr) == -1)
                    LOG_INFO("sigprocmask can't unblock!");
            }
            ~CServerCtrlr()
            {
                emit_event(closeSession);
            }
            CServerCtrlr(const CServerCtrlr &) = delete;
            CServerCtrlr &operator=(CServerCtrlr &) = delete;
            CServerCtrlr(CServerCtrlr &&) = delete;
            CServerCtrlr &operator=(CServerCtrlr &&) = delete;

            error_num_t check_device_network(int32_t &status) const;
            error_num_t open_session();
            error_num_t write_multiplex_v1(const protohdr_t &tx_msghdr, ssize_t &sended_byte, uint32_t sum_istream_bit);
            error_num_t write_multiplex_v2(const protohdr_t &tx_msghdr, ssize_t &sended_byte, uint32_t sum_istream_bit);
            error_num_t write_multiplex_v3(const protohdr_t &__tx_msghdr, ssize_t &__sended_byte, uint32_t __sum_istream_bit);

            error_num_t read_multiplex(protohdr_t &rx_msghdr, ssize_t &received_byte);
            error_num_t bonding_low_level_routine();
            int32_t get_session();
            error_num_t init_session_monitoring();

            /**
             * @brief Get the error code object
             *
             * @return int
             */
            int32_t get_error_code() const
            {
                return (error_code_);
            }
            /**
             * @brief Get the socket h object
             *
             * @return int
             */
            int get_server_port() const
            {
                LOG_INFO("get_server_port() socket handle(%d)", s_server_resource_ctx_.socket_handle);

                return (s_server_resource_ctx_.socket_handle);
            }
        };
        using cserver_ctrlr_t = CServerCtrlr;
    }
}

#endif
