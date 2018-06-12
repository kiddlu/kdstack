#!/usr/bin/python3
import os

src_code='''
#include <stdio.h>
#include <stdlib.h>

void *valid_pt(void *p)
{
    if(p == NULL)
        printf("pt is NULL\\n");
    else {
        int *ap = p;
        *ap = 0;
        printf("pt is %p\\n", p);
    }
}

void *return_null(void)
{
    return NULL;
}
'''
txt = open("pycall.c", "w")
txt.write(src_code)
txt.close()
os.system("gcc -o libpycall.so -shared -fPIC pycall.c")

#main
import _ctypes
from ctypes import *
class Cstruct(Structure):
    pass

ll = cdll.LoadLibrary
lib = ll("./libpycall.so")
lib.valid_pt(None)
a = c_int(8)
lib.valid_pt(pointer(a))
print(a)
print(pointer(a))

lib.return_null.restype = c_void_p
null_pt = lib.return_null()
if null_pt == None:
    print("clib return is NULL")
else:
    print("clib return is not NULL")

_ctypes.FreeLibrary(lib._handle)
os.system("rm pycall.c libpycall.so")