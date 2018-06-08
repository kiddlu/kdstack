#!/bin/bash

#set -x

function factorial() {
    local value=$1
    if [[ "$1" -gt "1" ]]; then
        prev=`expr $value - 1`
        sum=`factorial $prev`
        ret=`expr $value \* $sum`
        echo $ret
    else
        echo 1
    fi
}

function check_int() {
    #local tmp=`echo $1 |sed 's/[0-9]//g'`
    [[ -n "`echo $1 | sed 's/[0-9]//g'`" ]] && return 1 || return 0
}

if [[ "`check_int $1`" -ne "0" ]]; then
    echo "$1 is not a integer"
    exit 1
fi

factorial $1
