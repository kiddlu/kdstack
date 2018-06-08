#!/bin/bash

#Single char tr

#-c, -C, --complement
#use the complement of SET1
#
#-d, --delete
#delete characters in SET1, do not translate
#
#-s, --squeeze-repeats
#replace each sequence of a repeated character that is listed in the last specified SET, with a single occurrence of that character
#
#-t, --truncate-set1
#first truncate SET1 to length of SET2


echo 'linux' | tr "[:lower:]" "[:upper:]"
echo 'linux' | tr "a-z" "A-Z"
echo 'I LovE linuX. one is better Than 2' | tr "a-z" "A-Z"
echo 'I LovE linuX. one is better Than 2' | tr "a-z" "A-Z"


echo 'A ha? any help?... lalaal' | tr -cs "[:alpha:]" "\n"

echo -e "oh\tmy\texamples\r\n" | tee /dev/stdout | tr -cd "[:print:]"

echo ''
echo '{I love tr}' | tr '{}' '()'

echo ''
echo 'This is for testing' | tr [:space:] '\t'
echo ''
echo 'This   is   for testing' | tr -s [:space:] '\t'
echo ''
echo 'This   is   for testing' | tr -s [:space:]  ' '

#
#\NNN   character with octal value NNN (1 to 3 octal digits)
#
#\\     backslash
#
#\a     audible BEL
#
#\b     backspace
#
#\f     form feed
#
#\n     new line
#
#\r     return
#
#\t     horizontal tab
#
#\v     vertical tab
#
#CHAR1-CHAR2
#all characters from CHAR1 to CHAR2 in ascending order
#
#[CHAR*]
#in SET2, copies of CHAR until length of SET1
#
#[CHAR*REPEAT]
#REPEAT copies of CHAR, REPEAT octal if starting with 0
#
#[:alnum:]
#all letters and digits
#
#[:alpha:]
#all letters
#
#[:blank:]
#all horizontal whitespace
#
#[:cntrl:]
#all control characters
#
#[:digit:]
#all digits
#
#[:graph:]
#all printable characters, not including space
#
#[:lower:]
#all lower case letters
#
#[:print:]
#all printable characters, including space
#
#[:punct:]
#all punctuation characters
#
#[:space:]
#all horizontal or vertical whitespace
#
#[:upper:]
#all upper case letters
#
#[:xdigit:]
#all hexadecimal digits
#
#[=CHAR=]
#all characters which are equivalent to CHAR
