obj-m := driver_led.o
KDIR  := ~/d/works.6410/linux-2.6.36.2-v1.05

all:
	make -C $(KDIR) M=$(shell pwd) modules
test:
	$(CROSS_COMPILE)gcc  test.c  -o	 test

install:
	cp driver_led.ko /srv/tftp/rootfs/root
	cp test /srv/tftp/rootfs/root
	echo mknod /dev/my_led	c 240 0 > /srv/tftp/rootfs/root/mknode-led.sh
	echo insmod driver_led.ko  >> /srv/tftp/rootfs/root/mknode-led.sh
	echo chmod +x /srv/tftp/rootfs/root/mknode-led.sh

clean:
	make -C $(KDIR) M=$(shell pwd) clean
	rm -f *~ test
