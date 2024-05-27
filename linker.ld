/* SPDX-License-Identifier: MIT */
/* Linker script for MyOS kernel */
/* Author: KernelKraze */

ENTRY(_start)

SECTIONS
{
    . = 1M;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss  : { *(.bss) }
}
