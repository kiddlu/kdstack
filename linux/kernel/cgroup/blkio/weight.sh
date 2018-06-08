#!/bin/bash

# Enable Block IO controller
#       CONFIG_BLK_CGROUP=y
#
# Enable group scheduling in CFQ
#       CONFIG_CFQ_GROUP_IOSCHED=y
#
# Compile and boot into kernel and mount IO controller (blkio); see
# cgroups.txt, Why are cgroups needed?.
#
#       mount -t tmpfs cgroup_root /sys/fs/cgroup
#       mkdir /sys/fs/cgroup/blkio
#       mount -t cgroup -o blkio none /sys/fs/cgroup/blkio


if [ "$UID" -eq 0 ]
then
  echo "You are root.Good~"
else
  echo "Please run as root"
  exit
fi

#set DD Flag
FLAG=noatime

# Create two cgroups
mkdir -p /sys/fs/cgroup/blkio/test1/ /sys/fs/cgroup/blkio/test2/

# Set weights of group test1 and test2
echo 1000 > /sys/fs/cgroup/blkio/test1/blkio.weight
echo 10 > /sys/fs/cgroup/blkio/test2/blkio.weight

# Create two same size files (say 512MB each) on same disk (file1, file2) and
#  launch two dd threads in different cgroup to read those files.
dd if=/dev/zero of=~/zerofile1 bs=512MB count=1
dd if=/dev/zero of=~/zerofile2 bs=512MB count=1
dd if=/dev/zero of=~/zerofile3 bs=512MB count=1

sync
echo 3 > /proc/sys/vm/drop_caches

dd if=~/zerofile1 of=~/zerofile1.cp oflag=$FLAG 2>1000.weight &
echo $! > /sys/fs/cgroup/blkio/test1/tasks
cat  /sys/fs/cgroup/blkio/test1/tasks

dd if=~/zerofile2 of=~/zerofile2.cp oflag=$FLAG 2>10.weight &
echo $! > /sys/fs/cgroup/blkio/test2/tasks
cat  /sys/fs/cgroup/blkio/test2/tasks

dd if=~/zerofile3 of=~/zerofile3.cp oflag=$FLAG 2>no.weight &

ps
