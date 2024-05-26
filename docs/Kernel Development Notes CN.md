# 内核开发笔记

## 项目概述
该项目旨在开发一个简单的操作系统内核。内核包括基本的引导、一个最小的Multiboot头和一个简单的主函数，该函数将字符串输出到屏幕上。本文件详细介绍了开发过程，包括目录结构、源代码和构建配置。

## 目录结构
项目目录组织如下：

```
myos/
├── boot/
│   └── grub/
│       └── grub.cfg
├── iso/
│   ├── boot/
│   │   └── grub/
├── src/
│   ├── boot/
│   │   └── boot.s
│   └── kernel/
│       └── kernel.c
├── include/
│   └── kernel.h
├── linker.ld
├── Makefile
└── README.md
```

### 目录结构说明
- **boot/**: 包含引导操作系统所需的GRUB配置文件。
- **iso/**: 镜像了ISO创建所需的boot目录结构。这是构建可引导ISO的地方。
- **src/**: 源代码文件分为：
  - **boot/**: 包含低级汇编引导文件。
  - **kernel/**: 包含与内核相关的C源文件。
- **include/**: 项目的头文件。它将接口（头文件）与实现（源文件）分开，提高了可管理性。
- **linker.ld**: 用于链接内核的链接脚本。
- **Makefile**: 自动化构建过程的makefile。
- **README.md**: 提供有关项目、构建说明和使用的信息。

## 源代码

### 汇编引导程序 (src/boot/boot.s)
```assembly
/* SPDX-License-Identifier: MIT */

/* 
 * Assembly bootloader for My Operating System
 * Author: KernelKraze
 *
 * This code sets up the Multiboot header and starts the kernel.
 * 该代码设置了Multiboot头并启动内核。
 */

section .multiboot_header
    align 4
    dd 0x1BADB002                /* Magic number / 魔术数字 */
    dd 0x00000003                /* Flags / 标志 */
    dd -(0x1BADB002 + 0x00000003)/* Checksum / 校验和 */

section .text
    global _start
    extern kernel_main

_start:
    /* Jump to the kernel entry point */
    /* 跳转到内核入口点 */
    call kernel_main
    hlt

section .note.GNU-stack
    dd 0, 0, 0
```

### 内核主函数 (src/kernel/kernel.c)
```c
/* SPDX-License-Identifier: MIT */

/*
 * Simple kernel main function for My Operating System
 * Author: KernelKraze
 *
 * This is a simple kernel entry point for a custom OS.
 * 这是一个自定义操作系统的简单内核入口点。
 */

void kernel_main(void)
{
    const char *str = "Hello, World!";
    char *vidptr = (char*)0xb8000;  /* Start of video memory / 视频内存起始地址 */
    unsigned int i = 0;
    unsigned int j = 0;

    /* Clear the screen */
    /* 清屏 */
    while (j < 80 * 25 * 2) {
        vidptr[j] = ' ';       /* Set character to space / 设置字符为空格 */
        vidptr[j + 1] = 0x07;  /* Set attribute byte - white text on black background / 设置属性字节 - 白色文本，黑色背景 */
        j += 2;
    }

    j = 0;
    /* Print the string */
    /* 打印字符串 */
    while (str[i] != '\0') {
        vidptr[j] = str[i];    /* Set character / 设置字符 */
        vidptr[j + 1] = 0x07;  /* Set attribute byte / 设置属性字节 */
        ++i;
        j += 2;
    }

    while (1) /* Infinite loop to prevent kernel from exiting / 无限循环，防止内核退出 */
        ;
}
```

### 内核头文件 (include/kernel.h)
```c
/* SPDX-License-Identifier: MIT */

/*
 * Header file for kernel functions
 * Author: KernelKraze
 *
 * This file contains declarations for kernel functions.
 * 该文件包含内核函数的声明。
 */

#ifndef KERNEL_H
#define KERNEL_H

void kernel_main(void);

#endif /* KERNEL_H */
```

### 链接脚本 (linker.ld)
```ld
/* SPDX-License-Identifier: MIT */

/* 
 * Linker script for My Operating System kernel
 * Author: KernelKraze
 *
 * This script defines the memory layout of the kernel, specifying where
 * various sections of the code should be loaded into memory.
 *
 * 该链接脚本用于我的操作系统内核
 * 作者: KernelKraze
 *
 * 此脚本定义了内核的内存布局，指定了代码的各个部分应该加载到内存中的位置。
 */

ENTRY(_start)

SECTIONS
{
    /* The kernel will be loaded at 1MB in memory */
    /* 内核将加载到内存中的1MB处 */
    . = 0x00100000;

    /* The text section, which includes the Multiboot header and code */
    /* 文本段，包括Multiboot头和代码 */
    .text :
    {
        *(.multiboot_header)  /* Multiboot header section / Multiboot头段 */
        *(.text)              /* Code section / 代码段 */
    }

    /* The read-only data section */
    /* 只读数据段 */
    .rodata :
    {
        *(.rodata)            /* Read-only data section / 只读数据段 */
    }

    /* The data section */
    /* 数据段 */
    .data :
    {
        *(.data)              /* Initialized data section / 已初始化数据段 */
    }

    /* The BSS section, which includes uninitialized data */
    /* BSS段，包括未初始化数据 */
    .bss :
    {
        *(.bss)               /* Uninitialized data section / 未初始化数据段 */
        *(COMMON)             /* Common symbols / 公共符号 */
    }
}
```

### GRUB配置文件 (boot/grub/grub.cfg)
```cfg
# SPDX-License-Identifier: MIT
# 
# GRUB configuration file for booting My Operating System
# GRUB配置文件，用于启动我的操作系统
# 
# Author: KernelKraze
#

set timeout=0
set default=0

menuentry "My Operating System" {
    multiboot /boot/kernel.bin
    boot
}
```

## Makefile

```makefile
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
```

## 构建和运行操作系统

### 构建项目
要构建项目，运行：
```sh
make all
```

要清理构建文件，运行：
```sh
make clean
```

### 运行操作系统
使用QEMU运行操作系统：
```sh
qemu-system-i386 -cdrom os-image.iso
```

## 结论
本文档详细介绍了设置一个简单操作系统内核开发环境的过程。涵盖了目录结构、源代码、构建过程和执行方法。通过遵循这些笔记，您可以构建和运行一个最小内核，深入了解低级系统编程和操作系统开发。