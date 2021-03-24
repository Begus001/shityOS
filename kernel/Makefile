ARCH = x86

SRCS = $(shell find . -name *.c) $(shell find . -name *.asm)
OBJS = $(addsuffix .o,$(basename $(SRCS)))
OBJDIR = $(ARCH)/o
INCLUDEDIR = ./include

TOOLCHAIN = /home/main/os/gcc-cross-i686/bin
CC = $(TOOLCHAIN)/i686-elf-gcc
LD = $(TOOLCHAIN)/i686-elf-ld
NASM = nasm

CFLAGS = -c -ffreestanding -nostdlib -std=gnu17 -O2 -Wall -Wextra -I $(INCLUDEDIR) -ggdb
LDFLAGS = -nostdlib -O2
NASMFLAGS = -felf32

LINKERSCRIPT = $(ARCH)/kernel.ld

KERNEL_ELF = $(ARCH)/kernel.elf

GDBDBGCFG = gdbconf

$(KERNEL_ELF): $(OBJDIR) $(OBJS)
	$(LD) -T $(LINKERSCRIPT) $(LDFLAGS) $(OBJS) -o $(KERNEL_ELF)
	mv -t $(OBJDIR) $(OBJS)

$(OBJDIR):
	mkdir $(OBJDIR) 2>/dev/null

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.asm
	$(NASM) $(NASMFLAGS) -o $@ $^

launch: $(KERNEL_ELF)
	qemu-system-i386 -kernel $(KERNEL_ELF)

debug: $(KERNEL_ELF)
	gnome-terminal -- gdb -x $(GDBDBGCFG)
	qemu-system-i386 -kernel $(KERNEL_ELF) -S -s -singlestep 

clean:
	rm -fv $(OBJS) $(OBJDIR)/* $(KERNEL_ELF)

.PHONY: kernel launch debug clean
