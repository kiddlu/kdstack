#!/bin/sh
source ~/.winixrc

if [[ ! -f "$1" ]]; then 
	echo "no $1 file"
fi

FILE_TYPE=`file $1`
SUFFIX=none

echo $FILE_TYPE | grep -e "POSIX shell script" > /dev/null
if [[ $? -eq 0 ]]; then
	echo "$1 is POSIX shell script"
	SUFFIX=sh
fi

if [[ "$SUFFIX" != "none" ]]; then
	echo "add suffix .$SUFFIX"
	mv $1 $1.$SUFFIX
fi

exit 0
