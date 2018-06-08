#!/bin/bash  

while getopts "a:b:cdef" opt; do  
  case $opt in  
    a)  
      echo "this is -a the arg is ! $OPTARG"   
      ;;  
    b)  
      echo "this is -b the arg is ! $OPTARG"   
      ;;  
    c)  
      echo "this is -c the arg is ! $OPTARG"   
      ;;  
    \?)  
      echo "Invalid option: -$OPTARG"   
      ;;  
  esac
done