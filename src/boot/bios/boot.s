; SPDX-Identifier: MIT
; This code sets up the Multiboot header and starts the kernel.
; 该代码设置了Multiboot头并启动内核。

section .multiboot_header
    align 4
    dd 0x1BADB002                ; Magic number / 魔数
    dd 0x00000003                ; Flags / 标志
    dd -(0x1BADB002 + 0x00000003); Checksum / 校验和

section .text
    global _start
    extern kernel_main

_start:
    ; Enable A20 line
    call enable_a20

    ; Initialize protected mode
    cli                             ; Disable interrupts / 禁用中断
    lgdt [gdt_descriptor]           ; Load GDT / 加载GDT
    mov eax, cr0
    or eax, 1                       ; Set PE bit (Protection Enable) / 设置PE位（启用保护）
    mov cr0, eax
    jmp CODE_SEG:init_pm32          ; Far jump to flush prefetch queue / 远跳转以刷新指令预取队列

enable_a20:
    in al, 0x64
    and al, 0xFD
    out 0x64, al
    .loop:
        in al, 0x64
        test al, 2
        jnz .loop
    mov al, 0xD1
    out 0x60, al
    ret

; 32-bit protected mode initialization
init_pm32:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, STACK_TOP             ; Set stack pointer / 设置堆栈指针

    call kernel_main               ; Call kernel_main / 调用 kernel_main
    hlt                            ; Halt the CPU / 停止CPU

section .note.GNU-stack
    dd 0, 0, 0

section .gdt
gdt_start:
    dq 0x0000000000000000         ; Null segment / 空段
    dq 0x00cf9a000000ffff         ; Code segment descriptor / 代码段描述符
    dq 0x00cf92000000ffff         ; Data segment descriptor / 数据段描述符
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ 0x08
DATA_SEG equ 0x10
STACK_TOP equ 0x7C00
