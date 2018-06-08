#!/bin/bash
set -x

pkg_url="https://ftp.gnu.org/gnu/bash/bash-4.4.tar.gz"
pkg_name="bash-4.4"

wget -N $pkg_url

tar -xvf $pkg_name.tar.gz

cd $pkg_name

./configure --enable-static-link --enable-history --without-bash-malloc

make

make install
