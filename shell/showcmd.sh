#!/bin/sh

var=`echo $PATH|sed "s/:/ /g"`

for list in $var;
do
	ls $list | grep -v '^d'
done
