#!/bin/sh

rm -f -r -d libs
rm -f -r -d obj
ndk-build NDK_PROJECT_PATH=./ APP_BUILD_SCRIPT=./BuildBinary.mk NDK_APPLICATION_MK=./Application.mk NDK_TOOLCHAIN_VERSION=4.8 clean
clear
./build