#!/bin/bash

#g++ -o libpycallclass.so -shared -fPIC pycallclass.cpp
cat <<EOF > pycallclass.cpp
#include <iostream>
using namespace std;

class TestLib
{
    public:
        void display();
        void display(int a);
};
void TestLib::display() {
    cout<<"First display"<<endl;
}

void TestLib::display(int a) {
    cout<<"Second display:"<<a<<endl;
}

//wrap clacc method to c function
extern "C" {
    TestLib obj;
    void display() {
        obj.display();
    }
    void display_int() {
        obj.display(2);
    }
}
EOF

g++ -o libpycallclass.so -shared -fPIC pycallclass.cpp

cat <<EOF > pycallclass.py
import ctypes
ll = ctypes.cdll.LoadLibrary
lib = ll("./libpycallclass.so")

print 'display()'
lib.display()

print 'display(100)'
lib.display_int(100)
EOF

python pycallclass.py
rm pycallclass.py pycallclass.cpp libpycallclass.so
