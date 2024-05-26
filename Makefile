# SPDX-License-Identifier: MIT
# Makefile for building the My Operating System kernel
# Author: KernelKraze

CFLAGS=-m32 -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -fno-stack-protector -Iinclude

# Targets
all: os-image

os-image: iso/boot/grub/grub.cfg kernel.bin
	mkdir -p iso/boot/grub
	cp boot/grub/grub.cfg iso/boot/grub/grub.cfg
	cp kernel.bin iso/boot/kernel.bin
	grub-mkrescue -o os-image.iso iso

kernel.bin: src/boot/boot.o src/kernel/kernel.o
	ld -m elf_i386 -T linker.ld -o kernel.bin src/boot/boot.o src/kernel/kernel.o

src/boot/boot.o: src/boot/boot.s
	nasm -f elf32 src/boot/boot.s -o src/boot/boot.o

src/kernel/kernel.o: src/kernel/kernel.c include/kernel.h
	gcc $(CFLAGS) -c src/kernel/kernel.c -o src/kernel/kernel.o

# Phony targets
.PHONY: clean

clean:
	rm -rf src/boot/*.o src/kernel/*.o *.bin *.iso kernel.bin iso/boot/kernel.bin
