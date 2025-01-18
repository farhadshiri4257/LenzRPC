/**
 * @file Assert.h
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
/* Die with fatal error. */
#ifndef _ASSERT_H_
#define _ASSERT_H_ 1

#include "../Consts.h"

NAMESPACE(proto_proxy)
{

#define FATAL(msg, error_no)                      \
  do                                              \
  {                                               \
    fprintf(stderr,                               \
            "Fatal Err in %s on line %d: %s()\n", \
            __FILE__,                             \
            __LINE__,                             \
            msg);                                 \
    fflush(stderr);                               \
    abort();                                      \
  } while (0)

/* Have our own assert, so we are sure it does not get optimized away in
 * a release build.
 */
#define ASSERT(expr)                                     \
  do                                                     \
  {                                                      \
    if (!(expr))                                         \
    {                                                    \
      fprintf(stderr,                                    \
              "Assertion failed in %s on line %d: %s\n", \
              __FILE__,                                  \
              __LINE__,                                  \
              #expr);                                    \
      abort();                                           \
    }                                                    \
  } while (0)

#define ASSERT_BASE(a, operator, b, type, conv)                \
  do                                                           \
  {                                                            \
    volatile type eval_a = (type)(a);                          \
    volatile type eval_b = (type)(b);                          \
    if (!(eval_a operator eval_b))                             \
    {                                                          \
      fprintf(stderr,                                          \
              "Assertion failed in %s on line %d: `%s %s %s` " \
              "(%" #conv " %s %" #conv ")\n",                  \
              __FILE__,                                        \
              __LINE__,                                        \
              #a,                                              \
              #operator,                                       \
              #b,                                              \
              eval_a,                                          \
              #operator,                                       \
              eval_b);                                         \
      abort();                                                 \
    }                                                          \
  } while (0)

#define ASSERT_BASE_STR(expr, a, operator, b, type, conv)      \
  do                                                           \
  {                                                            \
    if (!(expr))                                               \
    {                                                          \
      fprintf(stderr,                                          \
              "Assertion failed in %s on line %d: `%s %s %s` " \
              "(%" #conv " %s %" #conv ")\n",                  \
              __FILE__,                                        \
              __LINE__,                                        \
              #a,                                              \
              #operator,                                       \
              #b,                                              \
              (type)a,                                         \
              #operator,(type) b);                             \
      abort();                                                 \
    }                                                          \
  } while (0)

#define ASSERT_BASE_LEN(expr, a, operator, b, conv, len)       \
  do                                                           \
  {                                                            \
    if (!(expr))                                               \
    {                                                          \
      fprintf(stderr,                                          \
              "Assertion failed in %s on line %d: `%s %s %s` " \
              "(%.*" #conv " %s %.*" #conv ")\n",              \
              __FILE__,                                        \
              __LINE__,                                        \
              #a,                                              \
              #operator,                                       \
              #b,                                              \
              (int)len,                                        \
              a,                                               \
              #operator,(int) len,                             \
              b);                                              \
      abort();                                                 \
    }                                                          \
  } while (0)

#define ASSERT_BASE_HEX(expr, a, operator, b, size)              \
  do                                                             \
  {                                                              \
    if (!(expr))                                                 \
    {                                                            \
      int i;                                                     \
      unsigned char *a_ = (unsigned char *)a;                    \
      unsigned char *b_ = (unsigned char *)b;                    \
      fprintf(stderr,                                            \
              "Assertion failed in %s on line %d: `%s %s %s` (", \
              __FILE__,                                          \
              __LINE__,                                          \
              #a,                                                \
              #operator,                                         \
              #b);                                               \
      for (i = 0; i < size; ++i)                                 \
      {                                                          \
        if (i > 0)                                               \
          fprintf(stderr, ":");                                  \
        fprintf(stderr, "%02X", a_[i]);                          \
      }                                                          \
      fprintf(stderr, " %s ", #operator);                        \
      for (i = 0; i < size; ++i)                                 \
      {                                                          \
        if (i > 0)                                               \
          fprintf(stderr, ":");                                  \
        fprintf(stderr, "%02X", b_[i]);                          \
      }                                                          \
      fprintf(stderr, ")\n");                                    \
      abort();                                                   \
    }                                                            \
  } while (0)

#define ASSERT_EQ_64(a, b) ASSERT_BASE(a, ==, b, int64_t, PRId64)
#define ASSERT_GE_64(a, b) ASSERT_BASE(a, >=, b, int64_t, PRId64)
#define ASSERT_GT_64(a, b) ASSERT_BASE(a, >, b, int64_t, PRId64)
#define ASSERT_LE_64(a, b) ASSERT_BASE(a, <=, b, int64_t, PRId64)
#define ASSERT_LT_64(a, b) ASSERT_BASE(a, <, b, int64_t, PRId64)
#define ASSERT_NE_64(a, b) ASSERT_BASE(a, !=, b, int64_t, PRId64)

#define ASSERT_UINT64_EQ(a, b) ASSERT_BASE(a, ==, b, uint64_t, PRIu64)
#define ASSERT_UINT64_GE(a, b) ASSERT_BASE(a, >=, b, uint64_t, PRIu64)
#define ASSERT_UINT64_GT(a, b) ASSERT_BASE(a, >, b, uint64_t, PRIu64)
#define ASSERT_UINT64_LE(a, b) ASSERT_BASE(a, <=, b, uint64_t, PRIu64)
#define ASSERT_UINT64_LT(a, b) ASSERT_BASE(a, <, b, uint64_t, PRIu64)
#define ASSERT_UINT64_NE(a, b) ASSERT_BASE(a, !=, b, uint64_t, PRIu64)

#define ASSERT_EQ_32(a, b) ASSERT_BASE(a, ==, b, int32_t, PRId32)
#define ASSERT_GE_32(a, b) ASSERT_BASE(a, >=, b, int32_t, PRId32)
#define ASSERT_GT_32(a, b) ASSERT_BASE(a, >, b, int32_t, PRId32)
#define ASSERT_LE_32(a, b) ASSERT_BASE(a, <=, b, int32_t, PRId32)
#define ASSERT_LT_32(a, b) ASSERT_BASE(a, <, b, int32_t, PRId32)
#define ASSERT_NE_32(a, b) ASSERT_BASE(a, !=, b, int32_t, PRId32)

#define ASSERT_UINT32_EQ(a, b) ASSERT_BASE(a, ==, b, uint32_t, PRIu64)
#define ASSERT_UINT32_GE(a, b) ASSERT_BASE(a, >=, b, uint32_t, PRIu32)
#define ASSERT_UINT32_GT(a, b) ASSERT_BASE(a, >, b, uint32_t, PRIu32)
#define ASSERT_UINT32_LE(a, b) ASSERT_BASE(a, <=, b, uint32_t, PRIu32)
#define ASSERT_UINT32_LT(a, b) ASSERT_BASE(a, <, b, uint32_t, PRIu32)
#define ASSERT_UINT32_NE(a, b) ASSERT_BASE(a, !=, b, uint32_t, PRIu32)

#define ASSERT_DOUBLE_EQ(a, b) ASSERT_BASE(a, ==, b, double, f)
#define ASSERT_DOUBLE_GE(a, b) ASSERT_BASE(a, >=, b, double, f)
#define ASSERT_DOUBLE_GT(a, b) ASSERT_BASE(a, >, b, double, f)
#define ASSERT_DOUBLE_LE(a, b) ASSERT_BASE(a, <=, b, double, f)
#define ASSERT_DOUBLE_LT(a, b) ASSERT_BASE(a, <, b, double, f)
#define ASSERT_DOUBLE_NE(a, b) ASSERT_BASE(a, !=, b, double, f)

#define ASSERT_STR_EQ(a, b) \
  ASSERT_BASE_STR(strcmp(a, b) == 0, a, ==, b, char *, "s")

#define ASSERT_STR_NE(a, b) \
  ASSERT_BASE_STR(strcmp(a, b) != 0, a, !=, b, char *, "s")

#define ASSERT_MEM_EQ(a, b, size) \
  ASSERT_BASE_LEN(memcmp(a, b, size) == 0, a, ==, b, s, size)

#define ASSERT_MEM_NE(a, b, size) \
  ASSERT_BASE_LEN(memcmp(a, b, size) != 0, a, !=, b, s, size)

#define ASSERT_MEM_HEX_EQ(a, b, size) \
  ASSERT_BASE_HEX(memcmp(a, b, size) == 0, a, ==, b, size)

#define ASSERT_MEM_HEX_NE(a, b, size) \
  ASSERT_BASE_HEX(memcmp(a, b, size) != 0, a, !=, b, size)

#define ASSERT_NULL(a) \
  ASSERT_BASE(a, ==, nullptr, void *, p)

#define ASSERT_NOT_NULL(a) \
  ASSERT_BASE(a, !=, nullptr, void *, p)

#define ASSERT_PTR_EQ(a, b) \
  ASSERT_BASE(a, ==, b, void *, p)

#define ASSERT_PTR_NE(a, b) \
  ASSERT_BASE(a, !=, b, void *, p)
}
#endif
