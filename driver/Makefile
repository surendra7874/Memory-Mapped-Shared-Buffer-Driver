obj-m += mmap_demo.o

KDIR := $(HOME)/kernel/linux-ti-4.19
ARCH := arm
CROSS_COMPILE := $(HOME)/toolchains/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-

PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean