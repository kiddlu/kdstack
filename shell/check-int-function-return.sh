#!/bin/bash 

#set -x
#https://www.linuxjournal.com/content/return-values-bash-functions

#return string must check, do not echo something else
function check_int_expr() {
        expr $1 + 0 &>/dev/null
        if [[ $? -ne 0 ]]; then
            echo "1"
        else
            echo "0"
        fi
        return
}

#return code only 0~255 to $?
function check_int_sed() {
    local tmp=`echo $1 |sed 's/[0-9]//g'`
    if [[ -n "${tmp}" ]] ;then
        return 1
    else
        return 0
    fi
}

[[ $# -ne 2 ]] && echo "`basename $0` arg1 arg2"
args1=$1
args2=$2

if [[ "`check_int_expr $args1`" -ne "0" ]]; then
    echo "$args1 is not a integer"
    exit 1
fi

check_int_sed $args2
if [[ $? -ne "0" ]]; then
    echo "$args2 is not a integer"
    exit 1
fi

if [[ $args1 -gt $args2 ]];then
    echo "$args1 greate than $args2"
else
    echo "$args1 less than $args2"
fi
