ifneq ($(KERNELRELEASE),)
  obj-m := button.o

else
  KDIR	:= ~/d/works.6410/linux-2.6.36.2-v1.05

all:
	make -C $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-linux-
test:
	$(CROSS_COMPILE)gcc  button_test.c -o button_test

install:
	cp  button.ko /srv/tftp/rootfs/root
	cp  button_test /srv/tftp/rootfs/root
	echo insmod button.ko	> /srv/tftp/rootfs/root/mknode-led.sh
	echo chmod +x /srv/tftp/rootfs/root/mknode-led.sh
clean:
	make -C $(KDIR) M=$(shell pwd) clean
	rm -f *~ button_test

endif
