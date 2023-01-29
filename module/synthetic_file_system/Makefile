obj-m += test.o
#ccflags-y += -g -DDEBUG

all:
	make -C /lib/modules/5.4.0-135-generic/build M=$(PWD) modules
clean:
	make -C /lib/modules/5.4.0-135-generic/build M=$(PWD) clean
