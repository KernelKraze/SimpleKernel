; SPDX-License-Identifier: MIT */
; -*- linux-assembly -*- ------------------------------------------------ */
; 
;   This code sets up the Multiboot header and starts the kernel.
;   该代码设置了Multiboot头并启动内核。
; 
;   Author: KernelKraze
;
; -----------------------------------------------------------------------

section .multiboot_header
    align 4
    dd 0x1BADB002                ; Magic number / 魔数
    dd 0x00000003                ; Flags / 标志
    dd -(0x1BADB002 + 0x00000003); Checksum / 校验和

section .text
    global _start
    extern kernel_main

_start:
    ; Jump to the kernel entry point
    ; 跳转到内核入口点
    call kernel_main
    hlt

section .note.GNU-stack
    dd 0, 0, 0
