/**
 * @file Common.h
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
#ifndef __COMMON_H_
#define __COMMON_H_ 1

/*C++ standard header*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <utility>
//#include <strstream>
#include <type_traits>

/*C standard header*/
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/if_link.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <time.h>

//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <stdint.h>

/*Android API standard header*/
//#include <android/log.h>
#include "../../static_header/cutils/log.h"

/*Library API standard header*/
#include "Consts.h"
#include "API/MessageProto.h"
#include "Helper/Assert.h"

#define RSW_PACKET_SIZE 64
#define PACKET_ALIGNMENT 16
#define PACKET_ALIGN(x) (((x) + PACKET_ALIGNMENT - 1) & ~(PACKET_ALIGNMENT - 1))
#define MAX_EVENT_IO 8

#define init_event_handler(A) epoll_create(A)
#define deinit_event_handler(FD) close(FD)
#define dispatch_event(A, B, C, D) epoll_wait(A, B, C, D)
#define register_event(A, B, C) epoll_ctl(A, EPOLL_CTL_ADD, B, C)
#define unregister_event(A, B, C) epoll_ctl(A, EPOLL_CTL_DEL, B, C)
#define raised_event(events, type) (events & type) == type
#define emit_event(A) pthread_kill(A, SIGRTMIN)
#define closeSession pthread_self()
#define session_timer_set(A,B) timer_settime(A, 0, B, nullptr)
#define session_timer_create(A,B)  timer_create(CLOCK_REALTIME, A, B)
#define EVENT_READY_READ EPOLLIN
#define INFINITE_WAIT -1
#define WAIT_TIME_FOR_REQUEST_DONE 2.0f
#define IS_SET 1

using flush_spec_t = struct linger;
using itimerspec_t = struct itimerspec;
/** @attention 
*            This timeout sec must be at least 3 times the WAIT_TIME_FOR_REQUEST_DONE.
*            Important:
*            if this value is smaller than WAIT_TIME_FOR_REQUEST_DONE, we will have undefined behaviour!!!
*/
static constexpr itimerspec_t ITIMER_SPEC_0IT_INTERVAL_SEC_5IT_VALUE_SEC_INITIALIZER = {{0, 0}, {(int32_t)(WAIT_TIME_FOR_REQUEST_DONE * 3), 0}};
static constexpr itimerspec_t ITIMER_SPEC_JIFFY_DISARM_INITIALIZER = {{0, 0}, {0, 0}};

/**
 * add a byte-value offset to a pointer
 */
#define PROTO_PTR_ADD(ptr, x) ((void *)((uintptr_t)(ptr) + (x)))

static uint32_t
ret_pop_cnt32(uint32_t v)
{
    return (uint32_t)__builtin_popcount(v);
}

static uint64_t check_validation_data(const std::string &entity_istream)
{
    uint64_t sum_bit = 0;
    for (const auto &byte : entity_istream)
    {
        uint32_t bit_count = ret_pop_cnt32((uint32_t)byte);
        sum_bit += bit_count;
    }
    return (sum_bit);
}

#endif
