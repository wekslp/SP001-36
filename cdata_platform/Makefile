obj-m := cdata_platform_dev.o cdata_platform_drv.o


KDIR := /usr/src/linux-headers-3.16.0-77-generic
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.o *.ko .*cmd modules.* Module.* .tmp_versions *.mod.c test
