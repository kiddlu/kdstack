#base on sysfs

obj-m := kobject-example.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
ECFLAGS := "-I$(PWD)"

modules:
	$(MAKE) -C $(KDIR) M=$(PWD) modules EXTRA_CFLAGS=$(ECFLAGS)

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
