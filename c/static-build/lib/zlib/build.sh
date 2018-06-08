#!/bin/bash
set -x

pkg_url="http://zlib.net/zlib-1.2.11.tar.gz"
pkg_name="zlib-1.2.11"
cross_tool="arm-linux-gnueabi"
root_path=`readlink -f $0 | xargs dirname`/../../root/

wget -N $pkg_url

tar -xvf $pkg_name.tar.gz
cd $pkg_name

CC="$cross_tool"-gcc ./configure --prefix=$root_path

make

make install
