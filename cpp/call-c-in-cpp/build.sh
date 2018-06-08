#!/bin/sh

gcc -c add.c -o add.o

g++ -c main.cpp -o main.o

g++ add.o main.o -o test.exe
