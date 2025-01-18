/**
 * @file property.h
 * @author f.shiri (you@domain.com)
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
#ifndef PROPERTY_H_
#define PROPERTY_H_ 1

#include <typeinfo>
#include <string>

NAMESPACE(proto_proxy)
{
	NAMESPACE(entity)
	{
		template <typename TP>
		struct CProperty
		{
			using property_type = TP;
			CProperty() = delete;
			explicit CProperty(const CProperty<property_type> &rhs) : /*length_(0),*/ property_(rhs.property_) {}
			const CProperty &operator=(const CProperty<property_type> &rhs)
			{
				const_cast<property_type &>(property_) = rhs.property_;
				return *this;
			}
			/*Preorder:
			 * when create new property must pass the default value of
			 * property!
			 */
			explicit CProperty(const property_type &rhs) : /*length_(0),*/ property_(rhs)
			{
				operator=(rhs);
			}

			/**
			 * @brief use in default value initilize in CTOR
			 *
			 * @param rhs
			 */
			inline void operator=(const property_type &rhs)
			{
				const_cast<property_type &>(property_) = rhs;
				/**
				 * @brief if property_type is string, estimate string size.
				 *
				 */
				// if (strcmp(typeid(property_type).name(), "Ss") == 0) 
				//	setLength(const_cast<property_type &>(rhs));
			}

			/**
			 * @brief use when assignment rvalue operator
			 *
			 * @param rhs
			 */
			inline void operator=(const property_type &&rhs) 
			{
				const_cast<property_type &>(property_) = std::move(rhs);

				// if (strcmp(typeid(property_type).name(), "Ss") == 0) 
				//	setLength(const_cast<property_type &>(rhs));
			}

			/**
			 * @brief used when need compare the property by none const object
			 * 
			 * @param rhs 
			 * @return true 
			 * @return false 
			 */
			inline bool operator==(const property_type &rhs)
			{
				return get().compare(rhs) == 0;
			}

			/**
			 * @brief used when need compare the property by const object
			 * 
			 * @param rhs 
			 * @return true 
			 * @return false 
			 */
			inline bool operator==(const property_type &rhs) const
			{
				return get().compare(rhs) == 0;
			}

			/**
			 * @brief none const cast operator
			 * 
			 * @return TP 
			 */
			operator TP() 
			{
				return property_;
			}

			/**
			 * @brief const cast operator
			 * 
			 * @return TP 
			 */
			operator TP() const
			{
				return property_;
			}

			/**
			 * @brief get none const refernce property
			 * 
			 * @return property_type& 
			 */
			property_type &get()
			{
				return *(reinterpret_cast<property_type *>(&const_cast<property_type &>(property_)));
			}

			/**
			 * @brief get const refernce property
			 * 
			 * @return const property_type& 
			 */
			const property_type &get() const
			{
				return *(reinterpret_cast<property_type *>(&const_cast<property_type &>(property_)));
			}

			/**
			 * @brief 
			 * 
			 * @return const property_type& 
			 */
			const property_type &operator->() const
			{
				return (get());
			}

		private:
			/*inline void setLength(property_type &rhs)
			{
				const_cast<size_t &>(length_) = (reinterpret_cast<std::string *>(&rhs))->length();
			}*/

			/*const size_t length_;*/

			const property_type property_;
		};

		typedef struct CProperty<bool> property_bool;
		typedef struct CProperty<int> property_int;
		typedef struct CProperty<std::string> property_string;
	}
}
#endif /* PROPERTY_H_ */
