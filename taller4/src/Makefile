obj-m := ej1.o ej2.o
KVERSION := $(shell uname -r)
all:
	make -C /lib/modules/$(KVERSION)/build SUBDIRS=$(shell pwd) modules
clean:
	make -C /lib/modules/$(KVERSION)/build SUBDIRS=$(shell pwd) clean