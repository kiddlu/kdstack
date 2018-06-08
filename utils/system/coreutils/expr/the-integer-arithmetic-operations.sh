#!/bin/sh

z=5
#---- Need spaces around + sign.
z=`expr $z+1`
echo $z
#5+1

z=5
z=`expr $z + 1`
echo $z
#6

expr 5 \* 2
#10
