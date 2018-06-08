#!/bin/bash

#Addresses and Ranges of Text
#Restricting to a line number
#Patterns
#Ranges by line number
#Ranges by patterns

txt_file=`mktemp --suffix=.xtx`
trap "rm -f $txt_file " EXIT

cat <<'TXTEND' > $txt_file
1 s/a/A/g 1
#44 s/o/O/g 4 START
#555 s/u/U/g 5    
hahahahaha     

#########    
#6666 s/x/X/g 6 STOP
#77777 7
TXTEND

#print last 2 lines of file
# first argument is the filename
lines=`wc -l $txt_file | awk '{print $1}' `
start=`expr $lines - 4`
sed "1,$start d" $txt_file

#The first removes every character from the "#" to the end of the line, and the second deletes all blank lines, third one should be added to remove all blanks and tabs immediately before the end of line:
echo ""
sed -e 's/#.*//' -e '/^$/ d' -e 's/[ ^I]*$//' $txt_file

#Printing with p
echo ""
sed -n '1,3 p' $txt_file
echo ""
sed -n '1,3 !p' $txt_file


#Relationships between d, p, and !
#sed -n	1,10	p  	Print first 10 lines
#sed -n	11,$	!p	Print first 10 lines
#sed    1,10	!d	Print first 10 lines
#sed	  11,$	d	  Print first 10 lines
#sed -n	1,10	!p	Print last 10 lines
#sed -n	11,$	p	  Print last 10 lines
#sed	  1,10	d	  Print last 10 lines
#sed   	11,$	!d	Print last 10 lines
#sed -n	1,10	d	  Nothing printed
#sed -n	1,10	!d	Nothing printed
#sed -n	11,$	d	  Nothing printed
#sed -n	11,$	!d	Nothing printed
#sed    1,10	p	  Print first 10 lines twice, then next 10 lines once
#sed    11,$	!p	Print first 10 lines twice, then last 10 lines once
#sed    1,10	!p	Print first 10 lines once, then last 10 lines twice
#sed    11,$	p	  Print first 10 lines once, then last 10 lines twice
