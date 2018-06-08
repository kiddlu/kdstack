#!/bin/bash
set -x

pkg_url="https://ftp.mirrorservice.org/sites/lsof.itap.purdue.edu/pub/tools/unix/lsof/lsof_4.89.tar.gz"
pkg_name="lsof_4.89"
cross_tool="arm-linux-gnueabi"
root_path=`readlink -f $0 | xargs dirname`/../root/

wget -N $pkg_url

tar -xvf $pkg_name.tar.gz
tar -xvf $pkg_name/"$pkg_name"_src.tar
cd "$pkg_name"_src

exit 0
./configure --prefix=$root_path --build=x86_64-linux-gnu --host=$cross_tool

make
