/**
 * @file ServerCtrlr.cpp
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
#include "../../Header/Connection/ServerCtrlr.h"
#include <chrono>

using namespace proto_proxy::network;
/** @note server_ctrlr object in unique, this static memeber is safe!*/
/** @brief */
sigset_t CServerCtrlr::s_mask_;
/** @brief */
timer_t CServerCtrlr::s_tmer_id_;
/** @brief */
server_resource_ctx_t CServerCtrlr::s_server_resource_ctx_{0};
/** @brief */
int32_t CServerCtrlr::s_monitor_event_fd_ = 0;
/** @brief */
epoll_event_t CServerCtrlr::s_event_list_;

NAMESPACE(global)
{
    extern std::string g_hardware_serial;
    extern const rswConfigContext_t *g_rswConfigContext;
}

/**
 * @brief device low-level api factory loader function
 *
 * @attention because this function low-level is important,
 *            must be load explicit in memory space proxy controller library.
 *
 * @return error_num_t error context resource
 */
error_num_t CServerCtrlr::bonding_low_level_routine()
{
    rild_lib_loader_t _rild_lib_loader;
    error_num_t res = _rild_lib_loader(NETWORK_LIB_DEVICE_NAME, NETWORK_LIB_QUERY_NET_FUNC_NAME, s_server_resource_ctx_.queryDataStateOfC);
    if (res != DONE_PROC)
        return (res);

    lh_m3_lib_loader_t lh_m3_lib_loader;
    res = lh_m3_lib_loader(M3_SYSTEM_LIB_NAME, M3_SYSTEM_LIB_FUNC_NAME, s_server_resource_ctx_.sys_get_hardware_serial);
    if (res != DONE_PROC)
        return (res);

    byte8_t tem_serial[32]{0};
    if (s_server_resource_ctx_.sys_get_hardware_serial(&tem_serial[0]) == -1)
        return (ERR_SERIAL_HW);
    global::g_hardware_serial.append(tem_serial);

    return (DONE_PROC);
}

/**
 * @brief
 * IMPORTANT: Don't change this options, t
 *                   he best options are set to communicate with the server correctly.
 *
 * @return error_num_t error context resource
 */
error_num_t CServerCtrlr::set_socket_option()
{
    int32_t _rcvlowat = 64;
    int32_t _is_alive = IS_SET;
    long send_recv_bufffer_size = -1;
    socklen_t _last_send_recv_buf_size = sizeof(send_recv_bufffer_size);

    timeval_t _send_recv_timeout{0};
    _send_recv_timeout.tv_sec = (time_t)server_option_.wait_time_sec;
    _send_recv_timeout.tv_usec = 0;

    flush_spec_t _session_flush_spec;
    _session_flush_spec.l_onoff = IS_SET;
    _session_flush_spec.l_linger = (time_t)server_option_.wait_time_sec;

    if (setsockopt(s_server_resource_ctx_.socket_handle, SOL_SOCKET,
                   SO_RCVBUF,
                   (byte8_t *)&server_option_.recv_buf,
                   sizeof(long)) < 0)
    {
        return (ERR_SO_RCVBUF);
    }

    if (setsockopt(s_server_resource_ctx_.socket_handle, SOL_SOCKET,
                   SO_SNDBUF,
                   (byte8_t *)&server_option_.send_buf,
                   sizeof(long)) < 0)
    {
        return (ERR_SO_SNDBUF);
    }

    if (getsockopt(s_server_resource_ctx_.socket_handle, SOL_SOCKET,
                   SO_SNDBUF,
                   (byte8_t *)&send_recv_bufffer_size,
                   &_last_send_recv_buf_size) < 0)
    {
        return (ERR_GET_SO_SNDBUF);
    }

    const auto &send_buf_size = (server_option_.send_buf * 2);
    if (likely(send_buf_size != send_recv_bufffer_size))
    {
        return (ERR_NOT_SET_SENDBUF);
    }

    if (setsockopt(s_server_resource_ctx_.socket_handle, SOL_SOCKET,
                   SO_RCVTIMEO,
                   &_send_recv_timeout,
                   sizeof(_send_recv_timeout)) < 0)
    {
        return (ERR_SO_RCVTIMEO);
    }

    if (setsockopt(s_server_resource_ctx_.socket_handle, SOL_SOCKET,
                   SO_SNDTIMEO,
                   &_send_recv_timeout,
                   sizeof(_send_recv_timeout)) < 0)
    {
        return (ERR_SO_SNDTIMEO);
    }

    if (setsockopt(s_server_resource_ctx_.socket_handle, SOL_SOCKET,
                   SO_RCVLOWAT,
                   &_rcvlowat,
                   sizeof(int32_t)) < 0)
    {
        return (ERR_SO_RCVLOWAT);
    }

    if (setsockopt(s_server_resource_ctx_.socket_handle, SOL_SOCKET,
                   SO_KEEPALIVE,
                   &_is_alive,
                   sizeof(_is_alive)) < 0)
    {
        return (ERR_SO_RCVLOWAT);
    }

    if (setsockopt(s_server_resource_ctx_.socket_handle, SOL_SOCKET,
                   SO_LINGER,
                   &_session_flush_spec,
                   sizeof(_session_flush_spec)) < 0)
    {
        return (ERR_SO_RCVLOWAT);
    }

    return (DONE_PROC);
}

/**
 * @brief device mac needed by server when sending protobuf stream
 *
 * @return error_num_t error context resource
 */
error_num_t CServerCtrlr::get_device_mac_address(const byte8_t *__nic_name, u_char (&__mac_address)[RSW_ETH_ALEN])
{
    ifreq_t _ifreq;
    strcpy(_ifreq.ifr_name, __nic_name);
    if (0 == ioctl(s_server_resource_ctx_.socket_handle, SIOCGIFHWADDR, &_ifreq))
    {
        unsigned byte8_t mac_buf[MAC_BCD_SIZE]{0};
        LOG_INFO("ifreq.ifr_addr.sa_data (%s)", _ifreq.ifr_addr.sa_data);

        for (int32_t index = 0; index < MAC_ASCI_SIZE; ++index)
        {
            sprintf((byte8_t *)mac_buf, "%02x", (unsigned byte8_t)_ifreq.ifr_addr.sa_data[index]);
            memmove(__mac_address + (index > 0 ? index << 1 : 0), mac_buf, 2);
        }
        return (DONE_PROC);
    }
    return (ERR_MAC_ADDR_NOT_FOUND);
}

/**
 * @brief bonding to low-level device function -> queryDataStateOfC(), get status from radio interface layer deamon
 * @param network status code
 * @return error_num_t error context resource
 */
error_num_t CServerCtrlr::check_device_network(int32_t &__status) const
{
    __status = s_server_resource_ctx_.queryDataStateOfC();
    return (!(__status == REGISTERED_ROAMING || __status == STATE_IN_SERVICE) ? ERR_IN_NETWORK_CONNECTION : DONE_PROC);
}

/**
 * @brief cleanup socket resource
 *
 * @return error_num_t error context resource
 */
error_num_t CServerCtrlr::close_session()
{
    if (s_server_resource_ctx_.socket_handle > 0)
    {
        unregister_event(s_monitor_event_fd_, s_server_resource_ctx_.socket_handle, &s_event_list_);

        shutdown(s_server_resource_ctx_.socket_handle, SHUT_RDWR);

        while (close(s_server_resource_ctx_.socket_handle) != 0)
            ;

        s_server_resource_ctx_.socket_handle = -1;

        freeaddrinfo(s_server_resource_ctx_.server_addr_info);
        LOG_INFO("close_session is OK!");
    }
    return (DONE_PROC);
}

/**
 * @brief
 *
 * @param __tx_msghdr
 * @param __sum_istream_bit
 * @return error_num_t
 */
error_num_t CServerCtrlr::protobuf_validation_data(const protohdr_t &__tx_msghdr, uint32_t __sum_istream_bit)
{

    pay_load_t _pay_load = *((pay_load_t *)(__tx_msghdr.msg_iov->iov_base));
    LOG_INFO("@ pay_load.magic_buf -> (%llx) - _pay_load.crc_chk(%llu)", _pay_load.magic_buf, _pay_load.crc_chk);
    LOG_INFO("@ pay_load_.stream -> (%s)", _pay_load.stream);
    LOG_INFO("@ pay_load_.header ->  (%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d),(%d)",
             _pay_load.header.table[MAGIC_KEY_IDX],
             _pay_load.header.table[CRC_SIZE_IDX],
             _pay_load.header.table[TABLE_SIZE_IDX],
             _pay_load.header.table[MAC_ADDR_IDX],
             _pay_load.header.table[CRC32_BUF_IDX],
             _pay_load.header.table[RPC_FUNC_IDX],
             _pay_load.header.table[MODEL_IDX_1],
             _pay_load.header.table[MODEL_IDX_2],
             _pay_load.header.table[MODEL_IDX_3],
             _pay_load.header.table[MODEL_IDX_4],
             _pay_load.header.table[MODEL_IDX_5],
             _pay_load.header.table[MODEL_IDX_6],
             _pay_load.header.table[MODEL_IDX_7],
             _pay_load.header.table[MODEL_IDX_8],
             _pay_load.header.table[MODEL_IDX_9],
             _pay_load.header.table[MODEL_IDX_10],
             _pay_load.header.table[MODEL_IDX_11],
             _pay_load.header.table[MODEL_IDX_12],
             _pay_load.header.table[MODEL_IDX_13],
             _pay_load.header.table[MODEL_IDX_14],
             _pay_load.header.table[MODEL_IDX_15],
             _pay_load.header.table[MODEL_IDX_16],
             _pay_load.header.table[MODEL_IDX_17],
             _pay_load.header.table[MODEL_IDX_18],
             _pay_load.header.table[MODEL_IDX_19],
             _pay_load.header.table[MODEL_IDX_20],
             _pay_load.header.table[MODEL_IDX_21],
             _pay_load.header.table[MODEL_IDX_22],
             _pay_load.header.table[MODEL_IDX_23],
             _pay_load.header.table[MODEL_IDX_24],
             _pay_load.header.table[MODEL_IDX_25],
             _pay_load.header.table[MODEL_IDX_26]);

    uint32_t _sum_istream_bit_tmp = check_validation_data(_pay_load.stream);
    LOG_INFO("CServerCtrlr -> 1- sum_istream_bit_tmp(%d), 2- sum_istream_bit(%d)", _sum_istream_bit_tmp, __sum_istream_bit);
    if (_sum_istream_bit_tmp != __sum_istream_bit)
        return (ERR_STREAM_CORRUPTED);
}

/**
 * @brief
 *
 * @param sig
 * @param si
 * @param uc
 */
void CServerCtrlr::session(int32_t __sig_no, siginfo_t *__signal_info, void *__arg)
{

    LOG_INFO("_____ Session is Done!!!");

    // int32_t _timer_over = timer_getoverrun(s_tmer_id_);
    // if (_timer_over == -1)
    //     LOG_INFO("timer_getoverrun");

    if (session_timer_set(s_tmer_id_, &ITIMER_SPEC_JIFFY_DISARM_INITIALIZER) == -1)
        LOG_ERR("session_timer_set");

    close_session();
}

/**
 * @brief
 *
 * @return error_num_t
 */
error_num_t CServerCtrlr::init_session_monitoring()
{
    struct sigevent sev;
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &CServerCtrlr::session;

    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGRTMIN, &sa, nullptr) == -1)
    {
        LOG_INFO("sigaction");
        return (TIMER_EVENT_ERROR);
    }

    sigemptyset(&s_mask_);
    sigaddset(&s_mask_, SIGRTMIN);
    if (sigprocmask(SIG_SETMASK, &s_mask_, nullptr) == -1)
    {
        LOG_INFO("sigprocmask");
        return (TIMER_EVENT_ERROR);
    }

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    if (session_timer_create(&sev, &s_tmer_id_) == -1)
    {
        LOG_INFO("session_timer_create");
        return (TIMER_EVENT_ERROR);
    }
    return (DONE_PROC);
}

/**
 * @brief Because the devices use the internet network to send packets,
 *        usually sending a piece of a very large buffer stream to the RSW server
 *        may cause some bytes to be lost when passing through different network hops.
 *        or it is possible that the layer force netlink to resend some packets,
 *        which in any case creates overhead for the RSW server.
 *        therefore, packet fragmentation algorithm has been used at the time of sending.
 *
 * @param tx_msghdr
 * @param sended_byte
 * @return error_num_t
 */
error_num_t CServerCtrlr::write_multiplex_v1(const protohdr_t &__tx_msghdr, ssize_t &__sended_byte, uint32_t __sum_istream_bit)
{
    /* iovec uio stream buffer sending */
    /*Important: unfortunately ARMv7 ABI in android (linux version in device lenz-e60 is 3.10.49) is not supported MSG_FASTOPEN , MSG_ZEROCOPY in TCP/IP */
    protobuf_validation_data(__tx_msghdr, __sum_istream_bit);

    uint32_t _displacment{0};
    uint32_t _fragment_cnt = __tx_msghdr.msg_iov->iov_len / RSW_PACKET_SIZE;
    uint32_t _last_packet = __tx_msghdr.msg_iov->iov_len % RSW_PACKET_SIZE;
    uint32_t _sent_counter = 0;
    protobuf_t _protobuf[_fragment_cnt + 1]{0};
    protohdr_t _protohdr{0};
    bool _is_last_packet = false;

    while (INFINITE_LOOP)
    {
        _protobuf[_sent_counter].iov_base = PROTO_PTR_ADD(__tx_msghdr.msg_iov->iov_base, _displacment);
        _protobuf[_sent_counter].iov_len = RSW_PACKET_SIZE;
        _protohdr.msg_iov = &_protobuf[_sent_counter];
        _protohdr.msg_iovlen = 1;
        /** @note send user io vector message in kernel path, is better solution because in
         *        one syscall all data stream is sent to the server.
         */
        //__sended_byte += sendmsg(s_event_list_.data.fd, &_protohdr, MSG_WAITALL | MSG_MORE | MSG_TRUNC | MSG_CMSG_CLOEXEC);
        __sended_byte += write(s_event_list_.data.fd, _protohdr.msg_iov->iov_base, _protohdr.msg_iov->iov_len);
        /** @note because the last packet is very important, again sending  to RSW.
         *         RSW promised, if the receving packets is duplicated, drop it.
         *         therfore sending duplicate last packet is better than last packet not recevied by RSW
         */
        if (_is_last_packet)
        {
            _is_last_packet = false;
            // sendmsg(s_event_list_.data.fd, &_protohdr, MSG_WAITALL | MSG_MORE | MSG_TRUNC | MSG_CMSG_CLOEXEC);
            write(s_event_list_.data.fd, _protohdr.msg_iov->iov_base, _protohdr.msg_iov->iov_len);
        }

        if (__sended_byte < 0 && (errno > 0 && errno != EINTR))
        {
            LOG_ERR("Can't sent data to the server!!! with errorno(%d) - __sended_byte(%u)", errno, __sended_byte);
            return (ERR_SEND_BYTE_IS_ZERO);
        }

        if (_displacment == __tx_msghdr.msg_iov->iov_len)
            break;

        if (_sent_counter <= (_fragment_cnt - 1))
        {
            _sent_counter++;
            _displacment += RSW_PACKET_SIZE;
        }
        else
        {
            _displacment += _last_packet;
            _is_last_packet = true;
        }

        if (errno == EINTR)
            continue;
    }

    LOG_INFO("sended bytes: %d \n", (int32_t)__sended_byte);
    return (DONE_PROC);
}

/**
 * @brief
 *
 * @param tx_msghdr
 * @param sended_byte
 * @return error_num_t
 */
error_num_t CServerCtrlr::write_multiplex_v2(const protohdr_t &__tx_msghdr, ssize_t &__sended_byte, uint32_t __sum_istream_bit)
{
    /* iovec uio stream buffer sending */
    /*Important: unfortunately ARMv7 ABI in android (linux version in device lenz-e60 is 3.10.49) is not supported MSG_FASTOPEN , MSG_ZEROCOPY in TCP/IP */
    protobuf_validation_data(__tx_msghdr, __sum_istream_bit);

    byte8_t _fragment_buffer[RSW_PACKET_SIZE]{0};
    uint32_t _displacment{0};
    uint32_t _fragment_cnt = __tx_msghdr.msg_iov->iov_len / RSW_PACKET_SIZE;
    uint32_t _last_packet = __tx_msghdr.msg_iov->iov_len % RSW_PACKET_SIZE;
    uint32_t _sent_counter = 0;
    uint32_t _fragment_size = RSW_PACKET_SIZE;
    while (INFINITE_LOOP)
    {
        memmove(_fragment_buffer + 0, (byte8_t *)__tx_msghdr.msg_iov->iov_base + _displacment, _fragment_size);
        __sended_byte += send(s_server_resource_ctx_.socket_handle, _fragment_buffer, _fragment_size, MSG_WAITALL);

        if (__sended_byte < 0 || (errno > 0 && errno != EINTR))
        {
            LOG_ERR("Can't sent data to the server!!! with errorno(%d) - __sended_byte(%u)", errno, __sended_byte);
            return (ERR_SEND_BYTE_IS_ZERO);
        }

        if (_displacment == __tx_msghdr.msg_iov->iov_len)
            break;

        if (_sent_counter <= (_fragment_cnt - 1))
        {
            _sent_counter++;
            _displacment += RSW_PACKET_SIZE;
            _fragment_size = RSW_PACKET_SIZE;
        }
        else
        {
            _displacment += _last_packet;
            _fragment_size = _last_packet;
        }
        memset(_fragment_buffer, 0, RSW_PACKET_SIZE);

        if (errno == EINTR)
            continue;
    }

    LOG_INFO("sended bytes: %d \n", (int32_t)__sended_byte);
    return (DONE_PROC);
}

/**
 * @brief
 *
 * @param tx_msghdr
 * @param sended_byte
 * @return error_num_t
 */
error_num_t CServerCtrlr::write_multiplex_v3(const protohdr_t &__tx_msghdr, ssize_t &__sended_byte, uint32_t __sum_istream_bit)
{
    /* iovec uio stream buffer sending */
    /*Important: unfortunately ARMv7 ABI in android (linux version in device lenz-e60 is 3.10.49) is not supported MSG_FASTOPEN , MSG_ZEROCOPY in TCP/IP */
    protobuf_validation_data(__tx_msghdr, __sum_istream_bit);

    while (INFINITE_LOOP)
    {
        __sended_byte = sendmsg(s_server_resource_ctx_.socket_handle, &__tx_msghdr, MSG_WAITALL);

        if (__sended_byte < 0 || (errno > 0 && errno != EINTR))
        {
            LOG_ERR("Can't sent data to the server!!! with errorno(%d) - __sended_byte(%u)", errno, __sended_byte);
            return (ERR_SEND_BYTE_IS_ZERO);
        }

        if (__sended_byte == __tx_msghdr.msg_iov->iov_len)
            break;

        if (errno == EINTR)
            continue;
    }

    LOG_INFO("sended bytes: %d \n", (int32_t)__sended_byte);
    return (DONE_PROC);
}

int32_t CServerCtrlr::check_connect_status(socket_handle_t __connected_socket)
{
    /* try to finish the connect()
     *(should be called after connect() only when fd is writable (EPOLLOUT))
     */
    int32_t _option{0};
    socklen_t _socket_len = sizeof(_option);
    return (0 == getsockopt(__connected_socket, SOL_SOCKET, SO_ERROR, &_option, &_socket_len)) ? _option : errno;
}

/**
 * @brief
 *
 * @param __rx_msghdr
 * @param __received_byte
 * @return error_num_t
 */
error_num_t CServerCtrlr::read_multiplex(protohdr_t &__rx_msghdr, ssize_t &__received_byte)
{
    auto _start_recv_timeout = std::chrono::high_resolution_clock::now();
    while (INFINITE_LOOP)
    {

        /** @attention
         *          Important note:
         *          If the recvmsg() blocks the current thread, it is possible that no data will be sent to receive from the server.
         *          therefore, the operation of receiving data should be non-block, and we should control this process using a real-time time stamp counter.
         *          and the value you define for this timer should not be less than RSW expired connection life-time in seconds.
         * */
        auto _end_recv_timeout = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> timeout = _end_recv_timeout - _start_recv_timeout;
        if (timeout.count() > server_option_.wait_time_sec)
        {
            LOG_ERR("Can't receive data from server, Timeout!!!");
            return (ERR_RECV_TIMEOUT);
        }

        int32_t _triggered_event = dispatch_event(s_monitor_event_fd_, &s_event_list_, 1, 1);
        if (_triggered_event != -1)
        {
            if (_triggered_event == 0)
                continue;

            if (raised_event(s_event_list_.events, EPOLLIN))
            {

                __received_byte = recvmsg(s_event_list_.data.fd, &__rx_msghdr, MSG_DONTWAIT | MSG_PEEK);
                if (errno == EAGAIN)
                    continue;
                if (__received_byte <= 0 && errno > 0)
                {
                    LOG_ERR("Can't receive data from server!!! with errorno(%d)", errno);
                    if (errno == ECONNRESET)
                        return (ERR_RSW_PEER_RESET);
                    return (ERR_RECV_BYTE_IS_ZERO);
                }

                if (__received_byte == __rx_msghdr.msg_iov->iov_len)
                {
                    LOG_INFO("expected size(%u) - received size(%u) ", __rx_msghdr.msg_iov->iov_len, __received_byte);
                    break;
                }
            }
        }
    }

    return (DONE_PROC);
}

/**
 * @brief
 *
 * @return error_ctx_t
 */
error_num_t CServerCtrlr::open_session()
{

    server_option_.recv_buf = global::g_rswConfigContext->receive_buffer_size;
    server_option_.send_buf = global::g_rswConfigContext->send_buffer_size;
    server_option_.wait_time_sec = global::g_rswConfigContext->interval_transaction_timeout;
    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(48000);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    memcpy(server_option_.client.port, global::g_rswConfigContext->rsw_port_main, 6);
    // memcpy(io_clientData.server, "46.209.140.1", SERVER_ADDRESS_BUF_SIZE); // 192.168.140.30
    memcpy(server_option_.client.server, global::g_rswConfigContext->rsw_host_main, SERVER_ADDRESS_BUF_SIZE); // 192.168.140.31

    addrinfo_t _addr_criteria{0};
    memset(&_addr_criteria, 0, sizeof(_addr_criteria));

    _addr_criteria.ai_family = AF_INET;
    _addr_criteria.ai_socktype = RSW_SOCKET_COMMINUTE_TYPE;
    _addr_criteria.ai_protocol = RSW_IP_PROTOCOL;
    _addr_criteria.ai_flags = 0; // AI_V4MAPPED; //in linux for android not needed for set!

    int status = getaddrinfo(server_option_.client.server, server_option_.client.port, &_addr_criteria, &s_server_resource_ctx_.server_addr_info);
    if (status != 0)
    {
        LOG_ERR("Resolve DNS Failed: Can't get ip address! (%s) -- %s\n", server_option_.client.server, gai_strerror(status));
        error_code_ = status;
        return (ERR_ADDR_INFO_TRANSLATE);
    }

    for (addrinfo_t *addr = s_server_resource_ctx_.server_addr_info; addr != nullptr; addr = addr->ai_next)
    {
        s_server_resource_ctx_.socket_handle = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (s_server_resource_ctx_.socket_handle < INVALID_SERVER_PORT)
            continue;

        /* because MAC address in lenz device is not available, comment this section */
        /*u_char(mac_address)[RSW_ETH_ALEN]{0};
          get_device_mac_address("rmnet_data0", mac_address);
          LOG_INFO("mac_address (%s)", mac_address);
        */
        set_socket_option();

        /** @attention because Connect() and Send() method operation can't set the non-blocking style,
         *             and when request is send by device, if Connect() method can't connect to the RSW
         *             or Send() can't send data stream to the RSW, the caller thread is blocked.!!
         *             therefore when request is arrived with this object,request bounding with expiration ticket timer controller object.
         *
         *             Ticket Timer state:
         *                  1) if send / recv is compelete, timer is explicit disarm with proxy_ctrlr object.
         *                  2) if send / recv can't completed, example can't to the RSW,
         *                     and waiting request time-out is finished, timer is implicit disarm with kernel.
         *                  3) if can't connect to the RSW, timer is implicit disarm with kernel.
         */
        if (session_timer_set(s_tmer_id_, &ITIMER_SPEC_0IT_INTERVAL_SEC_5IT_VALUE_SEC_INITIALIZER) == -1)
            LOG_ERR("session_timer_set");

        /*if(bind(s_server_resource_ctx_.socket_handle, (struct sockaddr *)&local_addr, sizeof(local_addr)))
        {
            emit_event(closeSession);
            return (ERR_IN_CONNECT_SERVER);
        }*/

        const auto _connect_status = connect(s_server_resource_ctx_.socket_handle, addr->ai_addr, addr->ai_addrlen);
        if (_connect_status == SUCCESS_CONNECTED_TO_SERVER)
        {
            s_event_list_.events = EPOLLIN | EPOLLOUT | EPOLLET;
            s_event_list_.data.fd = s_server_resource_ctx_.socket_handle;
            register_event(s_monitor_event_fd_, s_server_resource_ctx_.socket_handle, &s_event_list_);

            break;
        }
        else
        {
            emit_event(closeSession);
            return (ERR_IN_CONNECT_SERVER);
        }
    }

    LOG_INFO("socket handle(%d)", s_server_resource_ctx_.socket_handle);

    if (s_server_resource_ctx_.socket_handle <= INVALID_SERVER_PORT)
        return (ERR_IN_SOCKET_HANDLE);

    return (DONE_PROC);
}

/**
 * @brief
 *
 * @return int32_t
 */
int32_t CServerCtrlr::get_session()
{
    for (addrinfo_t *addr = s_server_resource_ctx_.server_addr_info; addr != nullptr; addr = addr->ai_next)
    {
        const auto _connect_status = connect(s_server_resource_ctx_.socket_handle, addr->ai_addr, addr->ai_addrlen);
        LOG_ERR("connect status (%d)", _connect_status);
        if (_connect_status == SUCCESS_CONNECTED_TO_SERVER)
        {
            return (_connect_status);
        }
    }
    emit_event(closeSession);
    s_server_resource_ctx_.socket_handle = INVALID_SERVER_PORT;
    return (INVALID_CONNECTION);
}