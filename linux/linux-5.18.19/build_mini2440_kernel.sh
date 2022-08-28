#!/bin/bash

set -e

TOOLCHAIN_PATH=$PWD/../../buildroot/buildroot-2022.02.4/output/host/bin/arm-mini2440-linux-gnueabi-
make ARCH=arm CROSS_COMPILE=$TOOLCHAIN_PATH distclean
make ARCH=arm CROSS_COMPILE=$TOOLCHAIN_PATH mini2440_defconfig
make ARCH=arm CROSS_COMPILE=$TOOLCHAIN_PATH -j16
