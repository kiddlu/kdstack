#!/bin/bash

#Addresses and Ranges of Text
#Restricting to a line number
#Patterns
#Ranges by line number
#Ranges by patterns

txt_file=`mktemp --suffix=.xtx`
trap "rm -f $txt_file " EXIT

#txt
cat <<'TXTEND' > $txt_file
1 s/a/A/g 1
2 s/e/E/g 2
3 s/i/I/g 3
#44 s/o/O/g 4 START
#555 s/u/U/g 5
#6666 s/x/X/g 6 STOP
#77777 7
TXTEND

#Restricting to a line number
sed '3 s/[0-9][0-9]*//g' $txt_file
echo ""
sed '1,3 s/s/(&)/g' $txt_file
#The "$" is one of those conventions that mean "last" in utilities like
echo ""
sed '3,$ s/g/(&)/g' $txt_file

#Patterns
#sed '\,^#, s/[0-9][0-9]*//'
echo ""
sed '/^#/ s/[0-9][0-9]*//' $txt_file

#use the backslash to escape the slash:
echo ""
sed '/^#/ s/\//\\/g' $txt_file

#remove comments everywhere except the lines between the two keywords:
echo ""
sed -e '1,/.*START/ s/#.*//' $txt_file
echo ""
sed -e '1,/.*START/ s/#.*//' -e '/STOP/,$ s/#.*//' $txt_file

#Delete with d
echo ""
sed '3,$ d' $txt_file
echo ""
sed '3,/^$/ d' $txt_file
#print last 10 lines of file
# First argument is the filename
lines=`wc -l $1 | awk '{print $1}' `
start=`expr $lines - 10`
sed "1,$start d" $1
