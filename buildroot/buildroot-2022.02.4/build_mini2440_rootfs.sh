#!/bin/bash

set -e

make clean
make mini2440_defconfig
make
