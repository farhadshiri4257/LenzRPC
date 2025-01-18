#/**
# * @file Application.mk
# * @author f.shiri (you@domain.com)
# * @brief
# * @version 0.1
# * @date 2019-08-21
# *
# * @copyright Copyright (c) 2019-2025
# * 
# * Redistribution and use in source and binary forms, with or without
# * modification, are permitted provided that the following conditions
# * are met:
# * 1. Redistributions of source code must retain the above copyright
# *    notice, this list of conditions and the following disclaimer.
# * 2. Redistributions in binary form must reproduce the above copyright
# *    notice, this list of conditions and the following disclaimer in the
# *    documentation and/or other materials provided with the distribution.
# *
# * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# * SUCH DAMAGE.
# */
#if need to be edited this script,
#befor edit please read this user manual file:../android-ndk-r9d/docs/APPLICATION-MK.html

#set flags for C compiler
APP_CPPFLAGS += -fexceptions

#set the Application Binary Interface   
APP_ABI := armeabi-v7a

#set optimization level 
#release = -O3
#debug   = -O0
APP_OPTIM := release	

#set the gnu stl if needed 
APP_STL := c++_static 
#APP_STL := gnustl_static 
#APP_STL := gnustl_shared

#set the compiler toolchain, this value can be set 4.6 or 4.7 or 4.8
#this toolchain folder ../android-ndk-r(X)d/toolchains
NDK_TOOLCHAIN_VERSION:=4.9

#set the Build Scrip file
APP_BUILD_SCRIPT=./BuildBinary.mk