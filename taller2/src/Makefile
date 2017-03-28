CFLAGS= -Wall -O0 -g -ggdb -m32
NASMFLAGS= -g -ggdb
LDFLAGS=-static -m32 -L'/usr/lib32'
CC=gcc

ARCH := $(shell uname -m)

BINS=hello mypid launch mister whoop

ifeq ($(ARCH), "x86_64")
BINS+= tinyhello_64 universe_64
NASMFLAGS+= -felf64
ASMLDFLAGS=-static
else
BINS+= tinyhello upcase nofork universe
NASMFLAGS+= -felf
ASMLDFLAGS=-static
endif

OBJS=$(BINS:=.o) syscalls.o

all: $(BINS) ptrace-code.tar.gz

ptrace-code.tar.gz: whoop hello.c tinyhello.asm tinyhello_64.asm universe.asm universe_64.asm mypid.c launch.c
	mkdir ptrace-code
	cp $^ ptrace-code/
	tar -zcvf $@ ptrace-code
	rm -r ptrace-code

# Dependencias
hello.o: hello.c
mypid.o: mypid.c
sotrace.o: sotrace.c
launch.o: launch.c

whoop: whoop.c
	$(CC) -o $@ -O2 $< $(LDFLAGS)

sotrace: sotrace.o syscalls.o

tinyhello.o: tinyhello.asm
	nasm $(NASMFLAGS) -o $@ $<

tinyhello_64.o: tinyhello_64.asm
	nasm $(NASMFLAGS) -o $@ $<

tinyhello: tinyhello.o
	ld -melf_i386 $(ASMLDFLAGS) -o $@ $<

tinyhello_64: tinyhello_64.o
	ld $(ASMLDFLAGS) -o $@ $<

clean:
	rm -rf $(OBJS) $(BINS)
