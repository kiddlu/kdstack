#!/bin/sh

# partial match and returns the number of characters matched.
expr linux : lin
#3

# the condition is string 2 entirely should match in string 1.
expr linux : linx
#0

# regular expression to match any number of characters
expr linux : '.*'
#5

# to print the matched characters instead of number of matching positions
expr linux : '..\(...\)'
#nux
