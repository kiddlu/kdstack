#!/bin/bash 

#set -x
function square_cube() {
    local value=$1
    local output=$2

    echo "$value * $value" | bc > $output
    echo "$value * $value * $value" | bc >> $output
}

tmp_file=`mktemp --suffix=.tmp`
trap "rm -f $tmp_file" EXIT

for i in `seq 5`; do
    square_cube $i $tmp_file
    square=`head -1 $tmp_file`
    cube=`tail -1 $tmp_file`
    echo "The Square of $i is $square"
    echo "The Cube of $i is $cube"
done
