ifneq ($(KERNELRELEASE),)
  obj-m := btn2.o

else
  KDIR	:= ~/d/works.6410/linux-2.6.36.2-v1.05

all:
	make -C $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-linux-
test:
	$(CROSS_COMPILE)gcc  btn2_test.c -o btn2_test

install:
	cp  btn2.ko /srv/tftp/rootfs/root
	cp  btn2_test /srv/tftp/rootfs/root
clean:
	make -C $(KDIR) M=$(shell pwd) clean
	rm -f btn2_test *~

endif
