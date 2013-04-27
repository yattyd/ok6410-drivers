obj-m := driver_char.o
KDIR :=/work/linux-2.6.36.2-v1.05/
all:
	make -C $(KDIR) M=$(shell pwd) modules
install:
	cp driver_char.ko /tftpboot/
clean:
	make -C $(KDIR) M=$(shell pwd) clean
