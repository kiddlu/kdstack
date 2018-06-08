#!/bin/bash
set -x

pkg_url="https://github.com/strace/strace/archive/v4.19.tar.gz"
pkg_name="v4.19"
dir_name="strace-4.19"

wget -N $pkg_url

tar -xvf $pkg_name.tar.gz

cd $dir_name

CFLAGS="-Os\
 -fomit-frame-pointer\
 -static\
 -static-libgcc\
 -ffunction-sections -fdata-sections\
 -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1\
 -fno-unwind-tables\
 -fno-asynchronous-unwind-tables\
 -Wl,--gc-sections\
 -Wl,-Map=strace.mapfile\
"

./bootstrap
./configure 
make CFLAGS="$CFLAGS"
