; SPDX-License-Identifier: MIT
; Startup code for MyOS on x86 architecture
; Author: KernelKraze

; Define section for text (code)
; 定义文本段（代码段）
section .text
global startup
extern kernel_main

; Multiboot header
; 多启动头
align 4
multiboot_header:
    dd 0x1BADB002                ; Magic number / 魔术数字
    dd 0x00                      ; Flags / 标志
    dd -(0x1BADB002 + 0x00)      ; Checksum / 校验和
    ; Magic number / 魔术数字：0x1BADB002 是 Multiboot 规范要求的魔术数字，用于标识 Multiboot 头
    ; Flags / 标志：当前设置为 0，表示没有特殊要求
    ; Checksum / 校验和：用于确保 Multiboot 头的有效性，魔术数字、标志和校验和的总和应为 0

; Code entry point
; 代码入口点
startup:
    ; Set up the stack
    ;
    ; Set up the stack
    ; 设置堆栈
    mov esp, stack_space + stack_size
    ; 将 esp 设置为预留的堆栈空间的顶部。
    
    ; Call the kernel main function
    ; 调用内核主函数
    extern kernel_main
    call kernel_main
    ; 调用 kernel_main 函数，这是内核的入口点。

halt:
    ; Halt the CPU
    ; 停止CPU
    hlt
    jmp halt                    ; Infinite loop / 无限循环
    ; 使用 hlt 指令停止 CPU，然后跳转到 halt 标签进入无限循环，防止返回到引导加载程序。

; Define section for uninitialized data (BSS)
; 定义未初始化数据段（BSS段）
section .bss
align 16
stack_space:
    resb 4096                   ; Reserve 4 KB for the stack / 保留4KB堆栈空间
stack_size equ $ - stack_space  ; Calculate the size of the stack / 计算堆栈大小
