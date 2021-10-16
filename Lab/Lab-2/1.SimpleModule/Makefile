KERNELDIR=/lib/modules/$(shell uname -r)/build

#MODULES = simpleModule.ko
obj-m += simpleModule.o

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean
