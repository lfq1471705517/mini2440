#!/bin/bash

set -e

OUTPUT_DIR=output/

rm -rf output/*

echo "-----------start compiling rootfs-----------"
cd buildroot/buildroot-2022.02.4/
./build_mini2440_rootfs.sh
cp output/images/rootfs.tar.bz2 ../../$OUTPUT_DIR
cd -
echo "-----------end rootfs compiling-----------"


echo "-----------start compiling kernel-----------"
cd linux/linux-5.18.19/
./build_mini2440_kernel.sh
cp arch/arm/boot/zImage ../../$OUTPUT_DIR
cd -
echo "-----------end kernel compiling-----------"


echo "-----------start compiling u-boot-----------"
cd u-boot/u-boot-2016.11
./build_mini2440_uboot.sh
cp u-boot.bin ../../$OUTPUT_DIR
cd -
echo "-----------end u-boot compiling-----------"

