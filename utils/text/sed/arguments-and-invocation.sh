#!/bin/bash

#Arguments and invocation of sed
#Multiple commands with -e command
#Filenames on the command line
#sed -n: no printing
#Using 'sed /pattern/'
#Using 'sed -n /pattern/p' to duplicate the function of grep
#sed -f scriptname
#sed in shell scripts
#Quoting multiple sed lines in the C shell
#Quoting multiple sed lines in the Bourne shell
#sed -V
#sed -h
#A sed interpreter script
#Sed Comments
#Passing arguments into a sed script
#Using sed in a shell here-is document
#Multiple commands and order of execution

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

#Quoting multiple sed lines in the Bourne shell
echo 'aeoiu' | sed '
    s/a/A/g
    s/e/E/g
    s/i/I/g
    s/o/O/g
    s/u/U/g
'

sed_file=`mktemp --suffix=.sed`
trap "rm -f $sed_file " EXIT

#txt
cat <<'SEDEND' > $sed_file
#!/bin/sed -f
s/a/A/g
s/e/E/g
s/i/I/g
s/o/O/g
s/u/U/g
SEDEND

echo 'uuiia' | sed -f $sed_file
