#!/bin/bash

#open
#CONFIG_MAGIC_SYSRQ=y
cat /proc/sys/kernel/sysrq
echo  1 > /proc/sys/kernel/sysrq

echo X > /proc/sysrq-trigger

#   0 - disable sysrq completely
#   1 - enable all functions of sysrq
#  >1 - bitmask of allowed sysrq functions (see below for detailed function description):
#          2 =   0x2 - enable control of console logging level
#          4 =   0x4 - enable control of keyboard (SAK, unraw)
#          8 =   0x8 - enable debugging dumps of processes etc.
#         16 =  0x10 - enable sync command
#         32 =  0x20 - enable remount read-only
#         64 =  0x40 - enable signalling of processes (term, kill, oom-kill)
#        128 =  0x80 - allow reboot/poweroff
#        256 = 0x100 - allow nicing of all RT tasks
#
#
#'b'     - Will immediately reboot the system without syncing or unmounting
#          your disks.
#
#'c'	- Will perform a system crash by a NULL pointer dereference.
#          A crashdump will be taken if configured.
#
#'d'	- Shows all locks that are held.
#
#'e'     - Send a SIGTERM to all processes, except for init.
#
#'f'	- Will call oom_kill to kill a memory hog process.
#
#'g'	- Used by kgdb (kernel debugger)
#
#'h'     - Will display help (actually any other key than those listed
#          here will display help. but 'h' is easy to remember :-)
#
#'i'     - Send a SIGKILL to all processes, except for init.
#
#'j'     - Forcibly "Just thaw it" - filesystems frozen by the FIFREEZE ioctl.
#
#'k'     - Secure Access Key (SAK) Kills all programs on the current virtual
#          console. NOTE: See important comments below in SAK section.
#
#'l'     - Shows a stack backtrace for all active CPUs.
#
#'m'     - Will dump current memory info to your console.
#
#'n'	- Used to make RT tasks nice-able
#
#'o'     - Will shut your system off (if configured and supported).
#
#'p'     - Will dump the current registers and flags to your console.
#
#'q'     - Will dump per CPU lists of all armed hrtimers (but NOT regular
#          timer_list timers) and detailed information about all
#          clockevent devices.
#
#'r'     - Turns off keyboard raw mode and sets it to XLATE.
#
#'s'     - Will attempt to sync all mounted filesystems.
#
#'t'     - Will dump a list of current tasks and their information to your
#          console.
#
#'u'     - Will attempt to remount all mounted filesystems read-only.
#
#'v'	- Forcefully restores framebuffer console
#'v'	- Causes ETM buffer dump [ARM-specific]
#
#'w'	- Dumps tasks that are in uninterruptable (blocked) state.
#
#'x'	- Used by xmon interface on ppc/powerpc platforms.
#          Show global PMU Registers on sparc64.
#
#'y'	- Show global CPU Registers [SPARC-64 specific]
#
#'z'	- Dump the ftrace buffer
#
#'0'-'9' - Sets the console log level, controlling which kernel messages
#          will be printed to your console. ('0', for example would make
#          it so that only emergency messages like PANICs or OOPSes would
#          make it to your console.)
