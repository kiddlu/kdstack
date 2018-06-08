#!/bin/bash

#config IKCONFIG
#	tristate "Kernel .config support"
#	---help---
#	  This option enables the complete Linux kernel ".config" file
#	  contents to be saved in the kernel. It provides documentation
#	  of which kernel options are used in a running kernel or in an
#	  on-disk kernel.  This information can be extracted from the kernel
#	  image file with the script scripts/extract-ikconfig and used as
#	  input to rebuild the current kernel or to build another kernel.
#	  It can also be extracted from a running kernel by reading
#	  /proc/config.gz if enabled (below).
#
#config IKCONFIG_PROC
#	bool "Enable access to .config through /proc/config.gz"
#	depends on IKCONFIG && PROC_FS
#	---help---
#	  This option enables access to the kernel configuration file
#	  through /proc/config.gz.
#
#config LOG_BUF_SHIFT
#	int "Kernel log buffer size (16 => 64KB, 17 => 128KB)"
#	range 12 21
#	default 17
#	help
#	  Select kernel log buffer size as a power of 2.
#	  Examples:
#	  	     17 => 128 KB
#		     16 => 64 KB
#            15 => 32 KB
#            14 => 16 KB
#		     13 =>  8 KB
#		     12 =>  4 KB

#config.gz
zcat /proc/config.gz
