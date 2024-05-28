// SPDX-License-Identifier: MIT
// -*- linux-c -*- -------------------------------------------------------
// 
//   Copyright (C) 2024 KernelKraze
//   All Rights Reserved
// 
// -----------------------------------------------------------------------

/*
 * This is a simple kernel entry point for a custom OS.
 * 这是一个自定义操作系统的简单内核入口点。
 */

#include "kernel/mm.h"  // 包含内存管理头文件
#include "string.h"     // 包含字符串处理头文件

#define VIDEO_MEMORY 0xb8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define WHITE_ON_BLACK 0x07

static unsigned int cursor_pos = 0;  // 声明 cursor_pos 变量并初始化为 0

void kernel_main(void)
{
    // 初始化内存管理系统，假设总内存大小为4MB
    mm_init(4 * 1024 * 1024);

    // 清屏
    char *vidptr = (char *)VIDEO_MEMORY;
    for (unsigned int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * 2; i += 2) {
        vidptr[i] = ' ';
        vidptr[i + 1] = WHITE_ON_BLACK;
    }
    cursor_pos = 0;
    printf("Hello, World!\n");

    // 打印内核初始化信息
    printf("[Kernel INFO] %s\n", "Kernel started\n");
    printf("[Kernel INFO] %s\n", "Memory Management Initialized\n");

    // 测试内存分配和释放
    void* test_alloc = kmalloc(256); // 分配256字节内存
    if (test_alloc) {
        printf("[Kernel INFO] %s\n", "Memory Allocated: 256 bytes\n");
        kfree(test_alloc); // 释放分配的内存
        printf("[Kernel INFO] %s\n", "Memory Freed: 256 bytes\n");
    } else {
        printf("[Kernel ERROR] %s\n", "Memory Allocation Failed\n");
    }

    // 无限循环，防止内核退出
    while (1)
        ;
}
