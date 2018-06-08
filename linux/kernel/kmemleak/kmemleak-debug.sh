#!/bin/bash

#CONFIG_DEBUG_KERNEL=y

#arch/xxx/Kconfig select HAVE_DEBUG_KMEMLEAK
#CONFIG_HAVE_DEBUG_KMEMLEAK=y

#CONFIG_DEBUG_KMEMLEAK=y
#CONFIG_DEBUG_KMEMLEAK_EARLY_LOG_SIZE=4000

#CONFIG_DEBUG_KMEMLEAK_DEFAULT_OFF=n

#make -C kernel O=../out/target/product/icesky_msm8992/obj/KERNEL_OBJ ARCH=arm64 CROSS_COMPILE=aarch64-linux-android- KCFLAGS=-mno-android menuconfig defconfig

mount -t debugfs nodev /sys/kernel/debug
cat /sys/kernel/debug/kmemleak

#To trigger an intermediate memory scan:
echo scan > /sys/kernel/debug/kmemleak

#To clear the list of all current possible memory leaks:
echo clear > /sys/kernel/debug/kmemleak

#Memory scanning parameters can be modified at run-time by writing to the /sys/kernel/debug/kmemleak file. The following parameters are supported:

#off
#disable kmemleak (irreversible)

#stack=on
#enable the task stacks scanning (default)

#stack=off
#disable the tasks stacks scanning

#scan=on
#start the automatic memory scanning thread (default)

#scan=off
#stop the automatic memory scanning thread

#scan=<secs>
#set the automatic memory scanning period in seconds (default 600, 0 to stop the automatic scanning)

#scan
#trigger a memory scan

#clear
#clear list of current memory leak suspects, done by marking all current reported unreferenced objects grey, or free all kmemleak objects if kmemleak has been disabled.

#dump=<addr>
#dump information about the object found at <addr>


#Kmemleak can also be disabled at boot-time by passing kmemleak=off on the kernel command line.
#Memory may be allocated or freed before kmemleak is initialised and these actions are stored in an early log buffer.
#The size of this buffer is configured via the CONFIG_DEBUG_KMEMLEAK_EARLY_LOG_SIZE option.
#If CONFIG_DEBUG_KMEMLEAK_DEFAULT_OFF are enabled, the kmemleak is disabled by default.
#Passing kmemleak=on on the kernel command line enables the function.

#  基础算法
# kmemleak通过追踪kmalloc(), vmalloc(), kmem_cache_alloc()等函数，把分配内存的指针和大小、时间、stack trace等信息记录在一个rbtree中，
# 等到调用free释放内存时就把相应的记录从rbtree中删除，
# 也就是说rbtree中的记录就是已经分配出去但尚未释放的内存。
# 其中有些内存尚未释放是因为还在被使用，这属于正常情况，而不正常的情况即内存尚未释放但又不会再被使用，就是“泄漏”的内存。
# 那么如何找出泄漏的内存呢？kmemleak缺省每10分钟对内存做一次扫描，在内存中寻找rbtree中记录的地址(数值匹配法比较)，
# 如果某个rbtree记录的地址在整个内存中都找不到，就认为这个地址是无人引用的，以后也不可能再被用到，是“泄漏”的内存，
# 然后，把这些泄漏的内存地址以及rbtree中记录的时间、大小、strack trace等相关信息通过 /sys/kernel/debug/kmemleak 这个接口展现给我们。
# Page allocations 和 ioremap 无法跟踪。

# 注：
# kmemleak的扫描算法存在误报的可能，
# 比如内存中碰巧有一个数据与rbtree中的某个地址相同，
# 但它只是数据而非指针，kmemleak是无法分辨的，会把它当作访问内存的指针；
# 再比如rbtree中的某个地址在内存中找不到，但程序可能还在用它，只是因为程序并没有直接保存访问指针地址数值，
# 而是通过某种方式[临时计算访问地址]，这种情况kmemleak也无法分辨，会认为是泄漏。

# https://www.kernel.org/doc/html/latest/dev-tools/kmemleak.html
# http://linuxperf.com/?p=188