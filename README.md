[你是中国人吗?](./README_CN.md)

# My Operating System

This project is a simple operating system kernel designed to help beginners learn about kernel development. It includes basic bootstrapping, a minimal Multiboot header, and a simple main function that outputs a string to the screen.

## Directory Structure

```
.
├── boot
│   └── grub
│       └── grub.cfg
├── docs
│   ├── Kernel Development Notes CN.md
│   └── Kernel Development Notes.md
├── efi
│   └── boot
├── include
│   ├── kernel
│   │   ├── drivers.h
│   │   ├── fs.h
│   │   ├── init.h
│   │   ├── kernel.h
│   │   └── mm.h
│   └── string.h
├── iso
│   ├── boot
│   │   └── grub
│   │       └── grub.cfg
│   └── efi
│       └── boot
│           └── bootx64.efi
├── linker.ld
├── Makefile
├── README_CN.md
├── README.md
└── src
    ├── boot
    │   ├── bios
    │   │   └── boot.s
    │   └── efi
    │       └── boot.c
    └── kernel
        ├── arch
        │   └── x86
        │       └── startup.S
        ├── drivers
        │   └── driver.c
        ├── fs
        │   └── fs.c
        ├── init
        │   └── init.c
        ├── kernel.c
        ├── lib
        │   └── string.c
        └── mm
            └── mm.c
```

### Explanation of Directory Structure

- **boot/**: Contains the GRUB configuration files necessary for booting the OS.
- **iso/**: Mirrors the boot directory structure needed for ISO creation. This is where the bootable ISO will be constructed.
- **src/**: Source code files are divided into:
  - **boot/**: Contains low-level assembly boot files.
  - **kernel/**: Contains kernel-related C source files.
- **include/**: Header files for the project. It separates the interface (headers) from the implementation (source files).
- **linker.ld**: The linker script used for linking the kernel.
- **Makefile**: The makefile that automates the build process.
- **README.md**: Provides information about the project, build instructions, and usage.

### Installation Tools

#### For Linux Users (e.g., Ubuntu)

1. Update the package list and install necessary tools:

    ```sh
    sudo apt update
    sudo apt install build-essential gcc g++ binutils nasm qemu-system-x86 grub-pc-bin xorriso mtools gnu-efi
    ```

2. Install cross-compilation toolchain (optional):

    ```sh
    sudo apt install gcc-multilib g++-multilib
    ```

#### For Windows Users

On Windows, you can use MSYS2 to set up a Linux-like development environment.

1. Download and install MSYS2:
    - Visit the [MSYS2 website](https://www.msys2.org/) to download and install MSYS2.
2. Update MSYS2:
    Open the MSYS2 MSYS terminal and run the following command to update the package manager:

    ```sh
    pacman -Syu
    ```

3. Install necessary tools:
    Open the MSYS2 MSYS terminal and run the following command to install GCC, NASM, and QEMU:

    ```sh
    pacman -S base-devel mingw-w64-x86_64-gcc mingw-w64-x86_64-binutils mingw-w64-x86_64-nasm mingw-w64-x86_64-qemu
    ```

## Learning Objectives

### Prerequisites
Before diving into kernel development, it's important to have a solid understanding of several foundational topics:

1. **Computer Architecture**: Understanding how a computer works, including the CPU, memory, and input/output operations.
2. **Assembly Language**: Basic knowledge of assembly language, particularly for the x86 architecture, as it helps in understanding low-level operations.
3. **C Programming**: Proficiency in C programming is essential since most kernels, including Linux, are written in C.
4. **Operating System Concepts**: Familiarity with basic OS concepts such as processes, memory management, file systems, and hardware abstraction.

### Recommended Resources

1. **Books**:
   - "Operating Systems: Design and Implementation" by Andrew S. Tanenbaum.
   - "Computer Systems: A Programmer's Perspective" by Randal E. Bryant and David R. O'Hallaron.
   - "Programming from the Ground Up" by Jonathan Bartlett (for assembly language).

2. **Online Courses**:
   - "Nand to Tetris" (Coursera): A course that takes you from basic logic gates to building a simple computer and operating system.
   - "Introduction to Computer Systems" (EDX or Coursera): Covers computer architecture and low-level programming.

3. **Documentation and Tutorials**:
   - [OSDev Wiki](https://wiki.osdev.org/): A comprehensive resource for OS development.
   - [Linux Kernel Documentation](https://www.kernel.org/doc/html/latest/): For learning how the Linux kernel is structured and functions.

## Development Guide

### Building the Project

To build the project, run:
```sh
make all
```

To clean the build files, run:
```sh
make clean
```

### Running the OS

To run the OS using QEMU:
```sh
qemu-system-i386 -cdrom os-image.iso
```

## Understanding the Source Code

### Assembly Bootloader (src/boot/bios/boot.s)

This file contains the initial bootloader code written in assembly. It sets up the Multiboot header and transfers control to the kernel's main function.

[file_link](./src/boot/bios/boot.s)

### Kernel Main Function (src/kernel/kernel.c)

The kernel's main function initializes the system and prints "Hello, World!" to the screen.

[file_link](./src/kernel/kernel.c)

### Kernel Header File (./include/kernel/kernel.h)

This header file contains the declaration for the `kernel_main` function.

[file_link](./include/kernel/kernel.h)

### Linker Script (linker.ld)

The linker script defines the memory layout for the kernel.

[file_link](./linker.ld)

### GRUB Configuration File (boot/grub/grub.cfg)

This file contains the GRUB configuration for booting the kernel.

```cfg
set timeout=0
set default=0

menuentry "My Operating System" {
    multiboot /boot/kernel.bin
    boot
}
```

### Makefile

The makefile automates the build process for the kernel.

[file_link](./Makefile)

## Challenges for Beginners

### Common Difficulties

1. **Understanding Low-Level Concepts**: Grasping how hardware and software interact at a low level can be challenging. This includes understanding CPU instructions, memory addressing, and I/O operations.
   
2. **Debugging**: Debugging kernel code is significantly harder than user-space applications because traditional debugging tools and techniques often do not work. Learning how to use tools like QEMU and GDB for debugging can be crucial.

3. **Learning Assembly Language**: Many beginners are not familiar with assembly language, which is essential for writing bootloaders and other low-level code.

### Tips for Overcoming Challenges

1. **Start with Theory**: Before diving into coding, ensure you have a solid theoretical foundation. Read books and take courses on operating systems and computer architecture.
   
2. **Incremental Learning**: Start with small, manageable projects that build upon each other. For example, begin with a simple bootloader before moving on to a more complex kernel.
   
3. **Utilize Online Resources**: Make extensive use of online communities and resources such as the OSDev Wiki and forums where you can ask questions and share knowledge.
   
4. **Practice Debugging**: Spend time learning and practicing debugging techniques. Use emulators like QEMU to run and test your kernel in a controlled environment.

By following these steps and utilizing the provided resources, beginners can gradually build up the skills and knowledge needed to understand and develop their own operating system kernel.

> The content needs to be updated, it is only the initial stage...

> In order to facilitate writing instructions, I deleted the docs folder, and detailed instructions will be written directly into the source code comments.