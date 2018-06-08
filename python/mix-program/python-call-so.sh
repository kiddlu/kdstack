#!/bin/bash

#use ctype, which is like jni

cat <<EOF > pycall.c
/** *gcc -o libpycall.so -shared -fPIC pycall.c*/
#include <stdio.h>
#include <stdlib.h>
int foo(int a, int b)
{
    printf("you input %d and %d\n", a, b);
    return a+b;
}
EOF

gcc -o libpycall.so -shared -fPIC pycall.c

cat <<EOF > pycall.py
import ctypes
ll = ctypes.cdll.LoadLibrary
lib = ll("./libpycall.so")
ret = lib.foo(1, 3)
print ret
print '***finish***'
EOF

python pycall.py
rm pycall.py pycall.c libpycall.so
