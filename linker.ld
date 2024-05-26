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
