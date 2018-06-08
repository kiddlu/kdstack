#!/bin/bash

set -x

killall testlsof
gcc testlsof.c -o testlsof
./testlsof &
pid=`ps -aux | grep testlsof | head -n1 |awk '{print $2}'`

echo pid is $pid
strace -o lsof.strace lsof -p $pid
