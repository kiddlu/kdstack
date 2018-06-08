#!/bin/bash

#Addresses and Ranges of Text
#Restricting to a line number
#Patterns
#Ranges by line number
#Ranges by patterns

txt_file=`mktemp --suffix=.xtx`
trap "rm -f $txt_file " EXIT

cat <<'TXTEND' > $txt_file
hahahahaha     WORD
dudulo
TXTEND

#Append with 'a'
#The "a" command appends a line after the range or pattern. This example will add a line after every line with "WORD:"
sed '
    /WORD/a\
Add this line after every line with WORD\nNow TWo line\
Now Three
' $txt_file

echo ""
#insert a new line before the pattern with the "i" command
sed '
    /WORD/i\
Insert this line before every line with WORD\nNow TWo line\
Now Three
' $txt_file

echo ""
#You can change the current line with a new line.
#A "d" command followed by a "a" command won't work
sed '
    /WORD/c\
Insert this line before every line with WORD\nNow TWo line\
Now Three
' $txt_file


#Sed ignores leading tabs and spaces in all commands. However these white space characters may or may not be ignored if they start the text following a "a," "c" or "i" command
#Just end each line with a "\"

# add a blank line after every line
sed '1,$ {
	a\

}' $txt_file
