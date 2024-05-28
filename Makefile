# SPDX-License-Identifier: MIT
# Makefile for building the My Operating System kernel
# Author: KernelKraze

# 是否包含调试信息，调试信息默认关闭（默认不包含DEBUG信息）
# Include debug information? Disabled by default.
DEBUG ?= no

# 编译器选项
# Compiler options
ifeq ($(DEBUG),yes)
CFLAGS=-m32 -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -fno-stack-protector -Iinclude -g
EFI_CFLAGS=-fno-stack-protector -fpic -fshort-wchar -mno-red-zone -Iinclude -I/usr/include/efi -I/usr/include/efi/x86_64 -g
else
CFLAGS=-m32 -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -fno-stack-protector -Iinclude -s -g0
EFI_CFLAGS=-fno-stack-protector -fpic -fshort-wchar -mno-red-zone -Iinclude -I/usr/include/efi -I/usr/include/efi/x86_64 -s -g0
endif

# 链接器选项
# Linker options
LDFLAGS=-m elf_i386 -T linker.ld

# 定义目录变量
# Define directory variables
SRCDIR = src
BOOTDIR = $(SRCDIR)/boot
BOOTDIR_BIOS = $(BOOTDIR)/bios
BOOTDIR_EFI = $(BOOTDIR)/efi
KERNELDIR = $(SRCDIR)/kernel
INCDIR = include
ISODIR = iso
EFIDIR = efi
ISOEFIDIR=iso/efi

# 定义对象文件列表
# Define object file lists
BOOT_OBJS_BIOS = $(BOOTDIR_BIOS)/boot.o
BOOT_OBJS_EFI = $(BOOTDIR_EFI)/boot.o
KERNEL_OBJS = $(KERNELDIR)/kernel.o $(KERNELDIR)/init/init.o $(KERNELDIR)/mm/mm.o $(KERNELDIR)/fs/fs.o $(KERNELDIR)/drivers/driver.o $(KERNELDIR)/lib/string.o $(KERNELDIR)/arch/x86/startup.o

# 目标文件：'all' 是默认目标，这里定义了最终生成的ISO映像
# Target file: 'all' is the default target, which builds the final ISO image
all: echo_mode os-image

echo_mode:
	@if [ "$(DEBUG)" = "yes" ]; then \
		echo "编译模式：调试模式启用 (Debug mode enabled)"; \
	else \
		echo "编译模式：发布模式，不包含调试信息 (Release mode, debug information not included)"; \
	fi

# 生成ISO映像
# Build ISO image
os-image: $(ISODIR)/boot/grub/grub.cfg kernel.bin $(EFIDIR)/boot/bootx64.efi
	mkdir -p $(ISODIR)/boot/grub
	mkdir -p $(ISODIR)/efi/boot
	cp boot/grub/grub.cfg $(ISODIR)/boot/grub/grub.cfg
	cp kernel.bin $(ISODIR)/boot/kernel.bin
	cp $(EFIDIR)/boot/bootx64.efi $(ISODIR)/efi/boot/bootx64.efi
	grub-mkrescue -o os-image.iso $(ISODIR)
	@echo "ISO 映像已生成: os-image.iso (ISO image created: os-image.iso)"

# 生成内核二进制文件
# Build kernel binary
kernel.bin: $(BOOT_OBJS_BIOS) $(KERNEL_OBJS)
	ld $(LDFLAGS) -o kernel.bin $(BOOT_OBJS_BIOS) $(KERNEL_OBJS)
	@echo "内核二进制文件已生成: kernel.bin (Kernel binary created: kernel.bin)"

# 编译BIOS引导代码
# Compile BIOS boot code
$(BOOTDIR_BIOS)/boot.o: $(BOOTDIR_BIOS)/boot.s
	nasm -f elf32 $(BOOTDIR_BIOS)/boot.s -o $(BOOTDIR_BIOS)/boot.o

# 编译EFI引导代码
# Compile EFI boot code
$(BOOTDIR_EFI)/boot.o: $(BOOTDIR_EFI)/boot.c
	gcc $(EFI_CFLAGS) -c $(BOOTDIR_EFI)/boot.c -o $(BOOTDIR_EFI)/boot.o

# 生成EFI引导程序
# Build EFI bootloader
$(EFIDIR)/boot/bootx64.efi: $(BOOTDIR_EFI)/boot.o
	mkdir -p $(EFIDIR)/boot
	ld -nostdlib -znocombreloc --warn-common --warn-section-align -shared -Bsymbolic -o $(EFIDIR)/boot/bootx64.efi $(BOOTDIR_EFI)/boot.o

# 编译内核主文件
# Compile kernel main file
$(KERNELDIR)/kernel.o: $(KERNELDIR)/kernel.c $(INCDIR)/kernel/kernel.h
	gcc $(CFLAGS) -c $(KERNELDIR)/kernel.c -o $(KERNELDIR)/kernel.o

# 编译初始化模块
# Compile initialization module
$(KERNELDIR)/init/init.o: $(KERNELDIR)/init/init.c $(INCDIR)/kernel/init.h
	gcc $(CFLAGS) -c $(KERNELDIR)/init/init.c -o $(KERNELDIR)/init/init.o

# 编译内存管理模块
# Compile memory management module
$(KERNELDIR)/mm/mm.o: $(KERNELDIR)/mm/mm.c $(INCDIR)/kernel/mm.h
	gcc $(CFLAGS) -c $(KERNELDIR)/mm/mm.c -o $(KERNELDIR)/mm/mm.o

# 编译文件系统模块
# Compile file system module
$(KERNELDIR)/fs/fs.o: $(KERNELDIR)/fs/fs.c $(INCDIR)/kernel/fs.h
	gcc $(CFLAGS) -c $(KERNELDIR)/fs/fs.c -o $(KERNELDIR)/fs/fs.o

# 编译设备驱动模块
# Compile device driver module
$(KERNELDIR)/drivers/driver.o: $(KERNELDIR)/drivers/driver.c $(INCDIR)/kernel/drivers.h
	gcc $(CFLAGS) -c $(KERNELDIR)/drivers/driver.c -o $(KERNELDIR)/drivers/driver.o

# 编译库函数模块
# Compile library functions module
$(KERNELDIR)/lib/string.o: $(KERNELDIR)/lib/string.c $(INCDIR)/string.h
	gcc $(CFLAGS) -c $(KERNELDIR)/lib/string.c -o $(KERNELDIR)/lib/string.o

# 编译架构相关启动代码
# Compile architecture-specific startup code
$(KERNELDIR)/arch/x86/startup.o: $(KERNELDIR)/arch/x86/startup.S
	nasm -f elf32 $(KERNELDIR)/arch/x86/startup.S -o $(KERNELDIR)/arch/x86/startup.o

# 虚假目标：clean，用于清理编译生成的文件
# Phony target: clean, used to clean up generated files
.PHONY: clean

clean:
	rm -rf $(BOOTDIR_BIOS)/*.o $(BOOTDIR_EFI)/*.o $(KERNELDIR)/**/*.o $(KERNELDIR)/*.o $(KERNELDIR)/arch/x86/startup.o *.bin *.iso kernel.bin $(ISODIR)/boot/kernel.bin $(EFIDIR)/boot/bootx64.efi $(ISOEFIDIR)/boot/bootx64.efi
