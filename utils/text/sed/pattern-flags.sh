#!/bin/bash

#Sed Pattern Flags
#/g - Global replacement
#Is sed recursive?
#/1, /2, etc. Specifying which occurrence
#/p - print
#Write to a file with /w filename
#/I - Ignore Case
#Combining substitution flags

#By default, sed prints every line. If it makes a substitution, the new text is printed instead of the old one. If you use an optional argument to sed, "sed -n," it will not, by default, print any new lines. When the "-n" option is used, the "p" flag will cause the modified line to be printed.
#sed /pattern/
echo '
Hello World
Hello Stream EDitor
' | sed -n '/World/p'

txt_file=`mktemp --suffix=.txt`
trap "rm -f $txt_file" EXIT

#Write to a file with /w filename
#Combining substitution flags
echo '9asd90340' | sed -n "s/^[0-9]*/&/pw $txt_file"
cat $txt_file

#/I - Ignore Case
echo 'ABC###' | sed -n 's/abc/DDD/Ip'

#Multiple commands with -e command
#sed --expression=
echo 'I a b U' | sed -e 's/a/A/' -e 's/b/B/'

#sed -n 's/'$1'/&/p'
#However - there is a problem with this script. If you have a space as an argument, the script would cause a syntax error A better version would protect from this happening:
echo "$0.hahaha" | sed  's/'"$0"'/(&)/'
echo "$0.hahaha" | sed  "s/$0/(&)/"

#echo -n 'what is the value?'
#read value
value=`whoami`
sed 's/XYZ/'"$value"'/' <<EOF
The whoami is XYZ
EOF
