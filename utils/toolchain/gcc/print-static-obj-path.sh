#!/bin/sh

# -print-file-name=library
#Print the full absolute name of the library file library that would be used when linking—and don't do anything else. With this option, GCC does not compile or link anything; it just prints the file name. 

gcc -print-file-name=libc.a
