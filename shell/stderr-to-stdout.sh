#!/bin/bash

tmp_file=`mktemp --suffix=.tmp`
trap "rm -f $tmp_file" EXIT

gcc -v 2>&1 | tee $tmp_file > /dev/null
cat $tmp_file