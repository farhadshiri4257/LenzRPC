/**
 * @file singleTone.h
 * @author
 * @brief
 * @version 0.1
 * @date 2019-01-08
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
#ifndef SINGLETON_H_
#define SINGLETON_H_ 1
#include <functional>

NAMESPACE(proto_proxy)
{
	NAMESPACE(immutable)
	{
		template <class OBJECT>
		class CSingleton
		{

		public:
			CSingleton(){};
			~CSingleton(){};

			/**
			 * @brief
			 *
			 * @tparam Params
			 * @param params
			 * @return OBJECT*
			 */
			template <class... Params>
			static const OBJECT *newInstancePtr(Params &&...params)
			{
				static OBJECT *s_to = new OBJECT(std::forward<Params>(params)...);
				return (s_to);
			}

			/**
			 * @brief
			 *
			 * @tparam Params
			 * @param params
			 * @return OBJECT&
			 */
			template <class... Params>
			static const OBJECT &newInstanceRef(Params &&...params)
			{
				static OBJECT s_to(std::forward<Params>(params)...);
				return (std::reference_wrapper<OBJECT>(s_to));
			}

			/**
			 * @brief
			 *
			 * @param ptr
			 */
			static bool destroy(const OBJECT *ptr)
			{
				if (ptr != nullptr)
				{
					delete ptr;
					return 1;
				}
				return 0;
			}

		private:
			CSingleton(const CSingleton &) = delete;
			CSingleton &operator=(const CSingleton &) = delete;
			CSingleton(CSingleton &&) = delete;
			CSingleton &operator=(const CSingleton &&) = delete;
		};
		template <class OBJECT>
		using csingleton_t = CSingleton<OBJECT>;
	}
}
#endif
