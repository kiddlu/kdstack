#!/bin/sh

g++ -c add.cpp -o add.o

gcc -c main.c -o main.o

gcc add.o main.o -o test.exe
