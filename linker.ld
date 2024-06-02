/* SPDX-License-Identifier: MIT */
/* Linker script for MyOS kernel */
/* Author: KernelKraze */

ENTRY(_start)

SECTIONS
{
    /* 内核加载地址 */
    . = 1M;

    /* .multiboot 段必须放在 .text 段的开头 */
    .text : {
        *(.multiboot)
        *(.text)
    }

    .data : { *(.data) }

    .bss : {
        *(.bss COMMON)
        *(COMMON)
    }

    . = ALIGN(4);
    _end = .;
}
