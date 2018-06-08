#!/bin/bash

#system info
cat /proc/cpuinfo
cat /proc/cmdline
cat /proc/devices
cat /proc/version
cat /proc/uptime

#cpu
cat /proc/cgroups
cat /proc/loadavg
cat /proc/schedstat

#kernel
cat /proc/kallsyms
cat /proc/kmsg
cat /proc/modules

#time
cat /proc/timer_list
cat /proc/timer_stats

#irq
cat /proc/interrupts
cat /proc/softirqs
cat /proc/irq/<num>/*

#fs
cat /proc/filesystems
cat /proc/partitions

#disk
cat /proc/swaps
cat /proc/diskstats

#mem
cat /proc/dma
cat /proc/buddyinfo
cat /proc/pagetypeinfo
cat /proc/meminfo
cat /proc/iomem
cat /proc/slabinfo
cat /proc/vmallocinfo
cat /proc/vmstat
cat /proc/zoneinfo