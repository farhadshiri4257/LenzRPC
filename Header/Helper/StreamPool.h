/**
 * @file StringBuilder.h
 * @author f.shiri (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2018-03-01
 *
 * @copyright Copyright (c) 2018-2025
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
#ifndef STRINGBUILDER_H_
#define STRINGBUILDER_H_

#include <string>
#include <iostream>
#include <memory>

NAMESPACE(proto_proxy)
{
	NAMESPACE(immutable)
	{
		class CStringStreamPool
		{
		private:
			class CBufferImplement
			{
			private:
				std::string buffer_pool_;

			public:
				explicit CBufferImplement(size_t reserved_size)
				{
					buffer_pool_.reserve(reserved_size);
				}
				~CBufferImplement()
				{
					buffer_pool_.clear();
				};
				CBufferImplement(const CBufferImplement &) = delete;
				CBufferImplement &operator=(const CBufferImplement &) = delete;
				CBufferImplement(CBufferImplement &&) = delete;
				CBufferImplement &operator=(CBufferImplement &&) = delete;

				template <typename T>
				inline const std::string &operator<<(const T &inputStr)
				{
					append(inputStr);
					return str();
				}

				inline void append(const byte8_t *inputStr)
				{
					if (inputStr != nullptr)
						buffer_pool_ += inputStr;
				}

				inline void append(const std::string &inputStr)
				{
					append(inputStr.c_str());
				}

				inline void setString(const std::string &i_str)
				{
					setString(i_str.c_str());
				}

				inline void setString(const byte8_t *i_str)
				{
					buffer_pool_ = i_str;
				}

				inline const std::string &str() const
				{
					return (buffer_pool_);
				}

				inline const byte8_t *str_buf()
				{
					return (buffer_pool_.c_str());
				}
			};
			std::unique_ptr<CBufferImplement> string_impl_ptr_;

			CStringStreamPool &append(const std::string &inputStr)
			{
				string_impl_ptr_->operator<<(inputStr);
				return *this;
			}

			CStringStreamPool &append(const byte8_t *inputStr)
			{
				string_impl_ptr_->operator<<(inputStr);
				return *this;
			}

			const std::string &raw_string()
			{
				return string_impl_ptr_->str();
			}

		public:
			CStringStreamPool(size_t reserved_size) : string_impl_ptr_(new CBufferImplement(reserved_size))
			{
				// string_impl_ptr_ = std::make_unique<CBufferImplement>(reserved_size);
			}
			~CStringStreamPool() {}
			explicit CStringStreamPool(const byte8_t *i_str, size_t reserved_size) : string_impl_ptr_(new CBufferImplement(reserved_size))
			{
				string_impl_ptr_->setString(i_str);
			}

			explicit CStringStreamPool(const std::string &i_str, size_t reserved_size) : string_impl_ptr_(new CBufferImplement(reserved_size))
			{
				string_impl_ptr_->setString(i_str);
			}

			explicit CStringStreamPool(const CStringStreamPool &rhs)
			{
				if (this != &rhs)
				{
					/*because unique_ptr is mutable!*/
					string_impl_ptr_ = std::make_unique<CBufferImplement>(MAX_BUF_FOR_STREAM);
					string_impl_ptr_->setString(const_cast<CStringStreamPool &>(rhs).raw_string());
					const_cast<CStringStreamPool &>(rhs).string_impl_ptr_.release();
				}
			}

			CStringStreamPool &operator=(const CStringStreamPool &rhs)
			{
				if (this != &rhs)
				{
					/*because unique_ptr is mutable!*/
					string_impl_ptr_ = std::make_unique<CBufferImplement>(MAX_BUF_FOR_STREAM);
					string_impl_ptr_->setString(const_cast<CStringStreamPool &>(rhs).raw_string());
					const_cast<CStringStreamPool &>(rhs).string_impl_ptr_.release();
				}
				return *this;
			}

			explicit CStringStreamPool(CStringStreamPool &&rhs) : string_impl_ptr_(std::move(rhs.string_impl_ptr_))
			{
			}

			const CStringStreamPool &operator<<(const std::string &inputStr) const
			{
				string_impl_ptr_->operator<<(inputStr);
				return *this;
			}

			const CStringStreamPool &operator<<(const byte8_t *inputStr) const
			{
				string_impl_ptr_->operator<<(inputStr);
				return *this;
			}

			const CStringStreamPool &operator=(const byte8_t *rhs) const
			{
				string_impl_ptr_->append(rhs);
				return *this;
			}

			const CBufferImplement *rdbuf() const
			{
				return string_impl_ptr_.get();
			}

			inline bool clear_buf()
			{
				memset((void*)string_impl_ptr_.get()->str_buf() , 0 , rdbuf()->str().length());
				return (true);
			}
		};
		using cstring_stream_pool_t = CStringStreamPool;
	}
}
#endif /* STRINGBUILDER_H_ */
