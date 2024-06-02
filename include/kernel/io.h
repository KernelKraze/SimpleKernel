// SPDX-Identifier: MIT
// io.h - I/O port functions for MyOS
// Author: KernelKraze

#ifndef IO_H
#define IO_H

#include "kernel/types.h"

// 写端口函数
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

#endif // IO_H
