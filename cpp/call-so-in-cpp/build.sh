#!/bin/sh

g++ -Wall -g -fPIC -c cat.cpp -o cat.o
g++ -shared -Wl,-soname,libcat.so -o libcat.so cat.o

g++ -Wall -g -c main.cpp
g++ -Wall -Wl,-rpath,. -o main main.o libcat.so
