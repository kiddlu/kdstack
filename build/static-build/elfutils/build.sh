#!/bin/bash
set -x

pkg_url="https://sourceware.org/elfutils/ftp/0.170/elfutils-0.170.tar.bz2"
pkg_name="elfutils-0.170"
cross_tool="arm-linux-gnueabi"
root_path=`readlink -f $0 | xargs dirname`/../root/

wget -N $pkg_url

rm -rf $pkg_name
tar -xvf $pkg_name.tar.bz2

cd $pkg_name

#src/Makefile.in (LINK) -static
./configure --prefix=$root_path --exec-prefix=$root_path --build=x86_64-linux-gnu --host=$cross_tool LDFLAGS="$LDFLAGS -L$root_path/lib" LIBS="$LIBS -lz -ldl" --enable-gcov --enable-gprof

make -j4

make install
