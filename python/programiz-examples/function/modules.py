#!/usr/bin/python

# import statement example
# to import standard module math

import math
print("The value of pi is", math.pi)


# import module by renaming it

import math as m
print("The value of pi is", m.pi)


# import only pi from math module

from math import pi
print("The value of pi is", pi)


# import all names form
# the standard module math

from math import *
print("The value of pi is", pi)


import sys
print(sys.path)
