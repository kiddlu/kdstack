#!/bin/bash

echo 'abcd123' | sed 's/\([a-z]*\).*/\1/'

#Note the space between the two remembered patterns.
#This is used to make sure two words are found.
#However, this will do nothing if a single word is found, or any lines with no letters.
#You may want to insist that words have at least one letter by using
echo 'ab cde 123' | sed 's/\([a-z][a-z]*\) \([a-z][a-z]*\)/\2 \1/'

#using extended regular expressions (note that '(' and ')' no longer need to have a backslash)
echo 'ab cde 123' | sed -r 's/([a-z]+) ([a-z]+)/\2 \1/'

echo 'ab abcde 123' | sed 's/\([a-z]*\)/\1/'
echo 'ab abcde 123' | sed 's/\([a-z]*\).*/\1/'
echo 'ab abcde 123' | sed 's/\([a-z]*\) \1/\1/'


echo 'abc abcde 123' | sed -rn '/([a-z]+) \1/p'
echo 'ab abcde 123' | sed 's/\([a-z]*\)/\1/'

echo 'abc abcde 123' | sed -r 's/^(.)(.)(.)/\3\2\1/'
echo 'abc abcde 123' | sed 's/^\(.\)\(.\)\(.\)/\3\2\1/'
