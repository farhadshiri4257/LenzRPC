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
#ifndef __CONSTS_H_
#define __CONSTS_H_ 1

#define NAMESPACE(name) namespace name

NAMESPACE(proto_proxy)
{

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "e60demo"
#endif
#define OsLog(TYPE_LOG, ...) ALOG(TYPE_LOG, LOG_TAG, __VA_ARGS__)
#define TAG_ERR "[[ProtoLib:INFO]] :"
#define TAG_INFO "[[ProtoLib:INFO]] :"

#ifdef VERBOSE_INFO
#define LOG_INFO(...) OsLog(LOG_DEBUG, TAG_INFO __VA_ARGS__)
#define LOG_ERR(...) OsLog(LOG_DEBUG, TAG_ERR __VA_ARGS__)
#else 
#define LOG_INFO(...)
#define LOG_ERR(...) 
#endif 


#define __NAKED__ __attribute__((naked))

#define __rte_unused __attribute__((__unused__))

#define __rte_noreturn __attribute__((noreturn))

#define is_nullptr(A, B) is_nullptr_predicate_(A, B, error_context_)
#define IS_NULL_PTR(A) (A == NULL)

#define INFINITE_LOOP 1

#define PROXY_INSTANCE_CHECK(A)                            \
    if (IS_NULL_PTR(A))                                    \
    {                                                      \
        g_error_context->error_num = ERR_INSTANCE_IS_NULL; \
        return (g_error_context);                          \
    }
}

/**
 * Check if a branch is likely to be taken.
 *
 * This compiler builtin allows the developer to indicate if a branch is
 * likely to be taken. Example:
 *
 *   if (likely(x > 1))
 *      do_stuff();
 *
 */
#define likely(x) __builtin_expect(!!(x), 1)

/**
 * Check if a branch is unlikely to be taken.
 *
 * This compiler builtin allows the developer to indicate if a branch is
 * unlikely to be taken. Example:
 *
 *   if (unlikely(x < 1))
 *      do_stuff();
 *
 */
#define unlikely(x) __builtin_expect(!!(x), 0)

#define SIZEOF_FIELD(type, field) (sizeof(((type *)0)->field))

typedef enum
{
    STATE_OUT_OF_SERVICE = 0,
    STATE_IN_SERVICE = 1,
    SEARCHING_FALL_THROUGH = 2,
    REGISTRATION_DENIED = 3,
    UNKNOWN = 4,
    REGISTERED_ROAMING = 5
} network_status_t;


#endif