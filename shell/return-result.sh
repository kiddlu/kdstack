#!/bin/bash

#if $? = 0 , echo &&
#if $? != 0, echo ||
gcc -dM -E -  < /dev/null | grep _WIN32 > null && echo "WIN32"  || echo "POSIX"
