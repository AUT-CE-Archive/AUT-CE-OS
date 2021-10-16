KERNELDIR=/lib/modules/$(shell uname -r)/build

#MODULES = birthdayList.ko
obj-m += birthdayList.o

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean
