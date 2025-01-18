/**
 * @file PluginLoader.h
 * @author f.shiri (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2017-08-15
 *
 * @copyright Copyright (c) 2017-2025
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
#ifndef __PLUGIN_LOADER_H_
#define __PLUGIN_LOADER_H_ 1

#include "../Common.h"
#include <dlfcn.h>

/*not worked in linux android*/
//#define __CleanupLibClose __attribute__((cleanup(closeModule)))
using lib_off_t = void *;

NAMESPACE(proto_proxy)
{
    NAMESPACE(network)
    {
        template <typename T>
        class CPlugin_Loader
        {
        private:
            using lib_func_type = T;
            static inline int32_t closeModule(lib_off_t handle)
            {
                int32_t tmp = -1;
                if (handle)
                {
                    tmp = dlclose(handle);
                }
                handle = nullptr;
                return tmp;
            }

        public:
            CPlugin_Loader() {}
            ~CPlugin_Loader() {}
            CPlugin_Loader(const CPlugin_Loader &) = delete;
            CPlugin_Loader &operator=(const CPlugin_Loader &) = delete;
            CPlugin_Loader(CPlugin_Loader &&) = delete;
            CPlugin_Loader &operator=(CPlugin_Loader &&) = delete;

            error_num_t operator()(const byte8_t *libName, const byte8_t *delegate_func_name, lib_func_type &delegate_func_type)
            {
                lib_off_t lib_handle = dlopen(libName, RTLD_LAZY); // RTLD_NOW | RTLD_LOCAL
                if (unlikely(lib_handle == nullptr))
                    return (ERR_DEVICE_LIBRARY_LOAD);

                delegate_func_type = (lib_func_type)dlsym(lib_handle, delegate_func_name);
                if (unlikely(delegate_func_type == nullptr))
                    return (ERR_DEVICE_LIBRARY_SYMBOL);

                closeModule(lib_handle);
                return (DONE_PROC);
            }
        };
        template <typename T>
        using cplugin_loader_t = CPlugin_Loader<T>;
    }
}

#endif