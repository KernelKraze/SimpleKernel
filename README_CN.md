# 我的操作系统

这是一个简单的操作系统项目，旨在帮助初学者学习内核开发。项目包括基本的引导、多重引导头部和一个简单的主函数，该函数将字符串输出到屏幕上。

## 目录结构

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
- **include/**: 项目的头文件。它将接口（头文件）与实现（源文件）分开。
- **linker.ld**: 用于链接内核的链接脚本。
- **Makefile**: 自动化构建过程的makefile。
- **README.md**: 提供有关项目、构建说明和使用的信息。

### 安装工具

#### 对于Linux用户（例如Ubuntu）

1. 更新包列表并安装必要的工具：

    ```sh
    sudo apt update
    sudo apt install build-essential gcc g++ binutils nasm qemu-system-x86 grub-pc-bin xorriso mtools
    ```

2. 安装交叉编译工具链（可选）：

    ```sh
    sudo apt install gcc-multilib g++-multilib
    ```

#### 对于Windows用户

在Windows上，可以通过安装MSYS2来获得类似Linux的开发环境。

1. 下载并安装MSYS2：
    - 前往 [MSYS2官网](https://www.msys2.org/) 下载并安装MSYS2。
2. 更新MSYS2：
    打开MSYS2 MSYS终端，运行以下命令以更新包管理器：

    ```sh
    pacman -Syu
    ```

3. 安装必要的工具：
    打开MSYS2 MSYS终端，运行以下命令安装GCC、NASM和QEMU：

    ```sh
    pacman -S base-devel mingw-w64-x86_64-gcc mingw-w64-x86_64-binutils mingw-w64-x86_64-nasm mingw-w64-x86_64-qemu
    ```

## 学习目标

### 前提条件
在深入进行内核开发之前，了解以下基础知识非常重要：

1. **计算机体系结构**：了解计算机的工作原理，包括CPU、内存和输入/输出操作。
2. **汇编语言**：掌握基本的汇编语言，特别是x86架构，这有助于理解低级操作。
3. **C语言编程**：精通C语言编程，因为大多数内核（包括Linux）都是用C语言编写的。
4. **操作系统概念**：熟悉基本的操作系统概念，如进程、内存管理、文件系统和硬件抽象。

### 推荐资源

1. **书籍**：
   - 《操作系统：设计与实现》作者：Andrew S. Tanenbaum
   - 《深入理解计算机系统》作者：Randal E. Bryant 和 David R. O'Hallaron
   - 《从零开始学编程》作者：Jonathan Bartlett（汇编语言）

2. **在线课程**：
   - "Nand to Tetris" (Coursera): 课程从基本的逻辑门到构建一个简单的计算机和操作系统。
   - "Introduction to Computer Systems" (EDX或Coursera): 涵盖计算机体系结构和低级编程。

3. **文档和教程**：
   - [OSDev Wiki](https://wiki.osdev.org/): 一个全面的操作系统开发资源。
   - [Linux Kernel Documentation](https://www.kernel.org/doc/html/latest/): 了解Linux内核的结构和功能。

## 开发指南

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

## 理解源代码

### 汇编引导程序 (src/boot/boot.s)

这个文件包含用汇编语言编写的初始引导程序代码。它设置了多重引导头并将控制权转移到内核的主函数。

```assembly
section .multiboot_header
    align 4
    dd 0x1BADB002                ; Magic number / 魔数
    dd 0x00000003                ; Flags / 标志
    dd -(0x1BADB002 + 0x00000003); Checksum / 校验和

section .text
    global _start
    extern kernel_main

_start:
    call kernel_main
    hlt
```

### 内核主函数 (src/kernel/kernel.c)

内核的主函数初始化系统并在屏幕上打印“Hello, World!”。

```c
void kernel_main(void)
{
    const char *str = "Hello, World!";
    char *vidptr = (char*)0xb8000;  /* Start of video memory / 视频内存起始地址 */
    unsigned int i = 0;
    unsigned int j = 0;

    /* 清屏 */
    while (j < 80 * 25 * 2) {
        vidptr[j] = ' ';       /* 设置字符为空格 */
        vidptr[j + 1] = 0x07;  /* 设置属性字节 - 白色文本，黑色背景 */
        j += 2;
    }

    j = 0;
    /* 打印字符串 */
    while (str[i] != '\0') {
        vidptr[j] = str[i];    /* 设置字符 */
        vidptr[j + 1] = 0x07;  /* 设置属性字节 */
        ++i;
        j += 2;
    }

    while (1); /* 无限循环，防止内核退出 */
}
```

### 内核头文件 (include/kernel.h)

这个头文件包含 `kernel_main` 函数的声明。

```c
#ifndef KERNEL_H
#define KERNEL_H

void kernel_main(void);

#endif /* KERNEL_H */
```

### 链接脚本 (linker.ld)

链接脚本定义了内核的内存布局。

```ld
ENTRY(_start)

SECTIONS
{
    . = 0x00100000;

    .text :
    {
        *(.multiboot_header)
        *(.text)
    }

    .rodata :
    {
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
        *(COMMON)
    }
}
```

### GRUB配置文件 (boot/grub/grub.cfg)

这个文件包含用于引导内核的GRUB配置。

```cfg
set timeout=0
set default=0

menuentry "My Operating System" {
    multiboot /boot/kernel.bin
    boot
}
```

### Makefile

Makefile自动化内核的构建过程。

```makefile
CFLAGS=-m32 -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -fno-stack-protector -Iinclude

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

clean:
	rm -rf src/boot/*.o src/kernel/*.o *.bin *.iso kernel.bin iso/boot/kernel.bin

.PHONY: clean
```

## 初学者的挑战

### 常见困难

1. **理解低级概念**：掌握硬件和软件在低级别的交互可能会很困难。这包括理解CPU指令、内存寻址和I/O操作。
2. **调试**：调试内核代码比用户空间应用程序难得多，因为传统的调试工具和技术往往不起作用。学习使用QEMU和GDB等工具进行调试至关重要。
3. **学习汇编语言**：许多初学者不熟悉汇编语言，而这对编写引导程序和其他低级代码是必不可少的。

### 克服挑战的建议

1. **从理论开始**：在开始编写代码之前，确保你有扎实的理论基础。阅读相关书籍并参加操作系统和计算机体系结构课程。
2. **逐步学习**：从小的、易管理的项目开始，逐步积累。例如，先编写一个简单的引导程序，然后再逐步增加复杂的内核功能。
3. **利用在线资源**：充分利用在线社区和资源，如OSDev Wiki和论坛，在那里你可以提问和分享知识。
4. **练习调试**：花时间学习和练习调试技巧。使用像QEMU这样的模拟器在受控环境中运行和测试你的内核。

通过遵循这些步骤并利用提供的资源，初学者可以逐步建立起理解和开发自己操作系统内核的技能和知识。

# 内容待更新,现在仅仅只是初始阶段..