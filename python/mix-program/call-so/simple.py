#!/usr/bin/python3
import os

#use ctype, which is like jni
src_code='''
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
    //printf("you input %d and %d\\n", a, b);
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

void change(struct cstruct *p)
{
    p->i = 9;
    p->d = 29.0;
    p->self = NULL;
    return;
}

void final(struct cstruct *p)
{
    free(p);
    return;
}
'''
txt = open("pycall.c", "w")
txt.write(src_code)
txt.close()
os.system("gcc -o libpycall.so -shared -fPIC pycall.c")


##main
import _ctypes
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

lib.change(cs)
print(cs.contents.i, cs.contents.d, cs.contents.self)

lib.final(cs)

_ctypes.FreeLibrary(lib._handle)
os.system("rm pycall.c libpycall.so")