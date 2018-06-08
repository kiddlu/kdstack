#ref:https://github.com/OpenXT/input/blob/e75fcda4fde8345247a4247ea940dea5f40baf57/superhid/Makefile

obj-m := kprobe_example.o jprobe_example.o
obj-m += kretprobe_example.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
ECFLAGS := "-DCONFIG_X86"

modules:
	$(MAKE) -C $(KDIR) M=$(PWD) modules EXTRA_CFLAGS=$(ECFLAGS)

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
