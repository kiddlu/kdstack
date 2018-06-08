#!/bin/bash

#set -x

log_file=`mktemp --suffix=.log`
#trap "rm -f $log_file" EXIT

verbose=5
log_tag=`basename $0`

function log() {
    echo "$log_tag: `date`: $@" >> $log_file
}

function debug() {
    verbosity=$1
    shift
    if [[ "$verbose" -gt "$verbosity" ]]; then
        echo "$log_tag: `date`: $@" >> $log_file
    fi
}

function die() {
    echo "$log_tag: `date`: Fatal Error: $@" >> $log_file
    exit 1
}

log Starting script $0
uname -a || die uname not found

#bachsh -c || die bachsh not found

log $0 Finish.

cat $log_file
