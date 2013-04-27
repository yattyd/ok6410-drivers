ifneq ($(KERNELRELEASE),)
  obj-m := plat_btn_device.o plat_btn_driver.o

else
  KDIR	:= ~/d/works.6410/linux-2.6.36.2-v1.05

all:
	make -C $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-linux-
test:
	$(CROSS_COMPILE)gcc  plat_btn_test.c -o plat_btn_test

install:
	cp  plat_btn_device.ko /srv/tftp/rootfs/root
	cp  plat_btn_driver.ko /srv/tftp/rootfs/root
	cp  plat_btn_test /srv/tftp/rootfs/root
	echo insmod plat_btn_device.ko	> /srv/tftp/rootfs/root/mknode-led.sh
	echo insmod plat_btn_driver.ko	>> /srv/tftp/rootfs/root/mknode-led.sh
	echo chmod +x /srv/tftp/rootfs/root/mknode-led.sh
clean:
	make -C $(KDIR) M=$(shell pwd) clean
	rm -f *~ plat_btn_test

endif
