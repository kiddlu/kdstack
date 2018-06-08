#!/bin/sh

function showsize(){
    if [[ $diskbyte -ge 1099511627776 ]];then echo size:$(($diskbyte/1024/1024/1024/1024))T;exit;fi
    if [[ $diskbyte -ge 1073741824 ]];then echo size:$(($diskbyte/1024/1024/1024))G;exit;fi
    if [[ $diskbyte -ge 1048576 ]];then echo size:$(($diskbyte/1024/1024))M;exit;fi
    if [[ $diskbyte -ge 1024 ]];then echo $(($diskbyte/1024))K;exit;fi
    if [[ $diskbyte -lt 1024 ]];then echo $diskbyte;exit;fi
}

diskbyte=$1
showsize
