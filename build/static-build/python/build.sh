#!/bin/bash
#https://wiki.python.org/moin/BuildStatically
set -x

pkg_url="https://www.python.org/ftp/python/2.7.14/Python-2.7.14.tgz"
pkg_name="Python-2.7.14"

wget -N $pkg_url

tar -xvf $pkg_name.tgz

cd $pkg_name

./configure --enable-optimizations LDFLAGS=-static --disable-shared

make LDFLAGS=-static LINKFORSHARED=""
