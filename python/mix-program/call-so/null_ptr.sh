#!/bin/bash

#use ctype, which is like jni

cat <<EOF > pycall.c
/* gcc -o libpycall.so -shared -fPIC pycall.c */
#include <stdio.h>
#include <stdlib.h>

struct cstruct
{
    int i;
    float d;
    struct cstruct *self;
};

int add(int a, int b)
{
    //printf("you input %d and %d\n", a, b);
    return a+b;
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
    return;
}

struct cstruct *init(void)
{
    struct cstruct *p = malloc(sizeof(struct cstruct));
    p->i = 6;
    p->d = 3.0;
    p->self = p;
    return p;
}

void final(struct cstruct *p)
{
    free(p);
}
EOF

gcc -o libpycall.so -shared -fPIC pycall.c

cat <<EOF > pycall.py
from ctypes import *

class Cstruct(Structure):
    pass

ll = cdll.LoadLibrary
lib = ll("./libpycall.so")
a = c_int(3)
b = c_int(7)

ret = lib.add(a, b)
print(ret)

lib.swap(byref(a), byref(b))
print(a, b)
print(a.value, b.value)

Cstruct._fields_ = [("i", c_int),
                    ("d", c_float),
                    ("self", POINTER(Cstruct))]
lib.init.restype = POINTER(Cstruct)

cs = lib.init()
print(cs.contents.i, cs.contents.d, cs.contents.self)
cs_self = cs.contents.self
print(cs_self.contents.i, cs_self.contents.d, cs_self.contents.self)

lib.final(cs)
EOF

python3 pycall.py
rm pycall.py pycall.c libpycall.so