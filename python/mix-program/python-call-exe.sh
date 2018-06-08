#!/bin/bash

#use ctype, which is like jni

cat <<EOF > main.cpp
#include <iostream>  
using namespace std;  
int test()  
{  
    int a = 10, b = 5;  
    return a+b;  
}  
int main()  
{  
    cout<<"---begin---"<<endl;  
    int num = test();  
    cout<<"num="<<num<<endl;  
    cout<<"---end---"<<endl;
    return num;  
}
EOF

g++ -o test main.cpp

cat <<EOF > main.py
import commands
import os

main = "./test"
if os.path.exists(main):
    ret, out = commands.getstatusoutput(main)
    print 'ret = %d, \nout = %s' % (ret, out)

print '*'*10
print '*'*10

f = os.popen(main)
data = f.readlines()
f.close()
print data

print '*'*10
print '*'*10
os.system(main)
EOF

python main.py
rm main.py main.cpp test
