#!/bin/sh
gcc -shared -fPIC -o libtest.dll test.c
gcc -o main -L. -ltest main.c