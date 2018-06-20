#!/bin/bash

#gcc -o test test.c

#gcc -o hook.so hook.c -fPIC --shared -Os

#chmod a+x test

#LD_PRELOAD=./hook.so ./test

gcc -o ./test test.c hook.c
chmod a+x ./test
./test

rm ./test
