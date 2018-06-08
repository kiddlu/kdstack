#!/bin/sh

g++ -Wall -g -fPIC -c hello.cpp -o hello.o
g++ -shared -Wl,-soname,libhello.so -o libhello.so hello.o

g++ -Wall -g -c main.cpp
g++ -Wall -Wl,-rpath,. -o main main.o libhello.so
