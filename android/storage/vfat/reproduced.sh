#!/bin/bash

set -x

#disable every events
echo 0 > /sys/kernel/debug/tracing/events/enable

#echo 1 > /sys/kernel/debug/tracing/events/writeback/enable
#echo 1 > /sys/kernel/debug/tracing/events/f2fs/enable
#echo 1 > /sys/kernel/debug/tracing/events/block/enable
echo 1 > /sys/kernel/debug/tracing/events/mmc/enable
#echo 1 > /sys/kernel/debug/tracing/events/sync/enable
echo 1 > /sys/kernel/debug/tracing/events/syscalls/sys_enter_mkdirat/enable
echo 1 > /sys/kernel/debug/tracing/events/syscalls/sys_exit_mkdirat/enable

echo 0 > /sys/kernel/debug/tracing/trace


rm -rf  /storage/sdcard1/test-*

while true
do
  timeout 0.8s mkdir -m 777  /storage/sdcard1/test-$RANDOM-$RANDOM
  if [[ "$?" -ne "0" ]]; then
    echo "mkdirat error; check"
    cat /sys/kernel/debug/tracing/trace > ftrace.log
    exit
  fi
  sleep 1
done