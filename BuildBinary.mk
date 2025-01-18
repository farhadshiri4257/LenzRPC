#/**
# * @file Model_header.h
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
#befor edit, please read this user manual file:../android-ndk-r9d/docs/ANDROID-MK.html

#set project local path 
#my-dir is macro used by NDK
LOCAL_PATH := $(call my-dir)
MY_MAIN_PROJECT_PATH := /home/farhad/DeviceProject/OnlineTicket
GOOGLE_PROTOBUF_PATH := /usr/local/include/google/protobuf/
GNU_STL_FOR_NDK_PATH := /home/farhad/workspace/android-ndk-r15c/sources/cxx-stl/gnu-libstdc++/4.9/libs/armeabi-v7a/

#show info in runtime mode when build script is run
$(info written by f.shiri) 
$(info build libprotobuf1)

include $(CLEAR_VARS)

#binary module name
LOCAL_MODULE:= protobuf

#set all source code directory into temp var
MY_SRC_FILES := $(wildcard $(LOCAL_PATH)/ProtoLib.cpp)
MY_SRC_FILES += $(wildcard $(LOCAL_PATH)/Src/Connection/*.cpp)
MY_SRC_FILES += $(wildcard $(LOCAL_PATH)/Src/Model/*.cpp)
MY_SRC_FILES += $(wildcard $(LOCAL_PATH)/Src/Proxy/*.cpp)

#set temp var to LOCAL_SRC_FILES macro
LOCAL_SRC_FILES := $(MY_SRC_FILES:$(LOCAL_PATH)/%=%)

#declare all header file directory
LOCAL_C_INCLUDES += \
	$(JNI_H_INCLUDE) \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/Header/ \
	$(LOCAL_PATH)/Header/API/ \
	$(LOCAL_PATH)/Header/Interface/ \
	$(LOCAL_PATH)/Header/Connection/ \
	$(LOCAL_PATH)/Header/Model/ \
	$(LOCAL_PATH)/Header/Proxy/ \

LOCAL_CPP_INCLUDES += \
	$(JNI_H_INCLUDE) \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/Header/ \
	$(LOCAL_PATH)/Header/API/ \
	$(LOCAL_PATH)/Header/Interface/ \
	$(LOCAL_PATH)/Header/Connection/ \
	$(LOCAL_PATH)/Header/Model/ \
	$(LOCAL_PATH)/Header/Proxy/ \

LIBCXX_LIBS := $(GNU_STL_FOR_NDK_PATH)

#LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS += -fpermissive 
#set flags for c++ compiler, if need to be compiled cpp file
LOCAL_CPP_EXTENSION := .cpp
LOCAL_CPP_FEATURES := rtti 
LOCAL_STATIC_LIBRARIES := c++_static
LOCAL_CPPFLAGS :=  -I$(GNU_STL_FOR_NDK_PATH) -DEND_POINT_IS_DEVICE=1 -DVERBOSE_INFO=1 
LOCAL_CPPFLAGS +=  -lrt  -pie -fPIE  -std=c++14 -fno-elide-constructors

#set static libs used by linker
#LOCAL_LDLIBS := -L$(MY_MAIN_PROJECT_PATH)/IOManager_Project/libs/armeabi/ -llh_iomanager
#LOCAL_LDLIBS += -L$(MY_MAIN_PROJECT_PATH)/BzLib_Project/libs/armeabi/ -llh_bzlib
#LOCAL_LDLIBS += -L$(MY_MAIN_PROJECT_PATH)/Crypto_Project/libs/armeabi/ -llh_crypto
#LOCAL_LDLIBS += -L$(MY_MAIN_PROJECT_PATH)/system_lib/  -llh_posapi -lcutils -llh_m3_sam -lscaner_barcode -lrild_conn -lminui -lcurl 

#set flags for gnu linker 
#arm bineray executable only work wehn set the flag postion independent code(pie) 
#APP_ALLOW_MISSING_DEPS=true 
LOCAL_LDLIBS := -ldl -lz #-llog
LOCAL_LDLIBS += -L/home/f.shiri/DeviceProject/OnlineTicket/system_lib/ -lcutils
#LOCAL_LDFLAGS += -pie -fPIE 

#build the binary file
include $(BUILD_SHARED_LIBRARY)
