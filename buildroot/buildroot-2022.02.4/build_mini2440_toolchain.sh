#!/bin/bash

set -e

make clean
make mini2440_toolchain_defconfig
make sdk
