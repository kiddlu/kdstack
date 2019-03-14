#!/bin/bash
set -x

pkg_url="https://github.com/ggreer/the_silver_searcher/archive/2.1.0.tar.gz"
pkg_ver="2.1.0"

wget -N $pkg_url

tar -xvf $pkg_ver.tar.gz

cd the_silver_searcher-$pkg_ver

./build.sh LDFLAGS=-static
