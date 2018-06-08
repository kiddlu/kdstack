#!/usr/bin/awk -f

BEGIN {
#initialize a counter

    counter=0
    while(counter <= 10) {
        print counter
        counter += 1
    }

    counter=0
    do {
        print counter
        counter += 1
    } while(counter <= 10)
}

#*= : multiplication assignment operator
#+= : addition assignment operator
#/= : division assignment operator
#-= : subtraction assignment operator
#%= : modulus assignment operator
#^= : exponentiation assignment operator
#
#* : multiplication operator
#+ : addition operator
#/ : division operator
#- : subtraction operator
#% : modulus operator
#^ : exponentiation operator
