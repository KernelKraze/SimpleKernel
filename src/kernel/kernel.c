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

#define VIDEO_MEMORY 0xb8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define WHITE_ON_BLACK 0x07

void kernel_main(void)
{
    // 初始化内存管理系统，假设总内存大小为4MB
    mm_init(4 * 1024 * 1024);

    const char *str = "Hello, World!";
    char *vidptr = (char *)VIDEO_MEMORY;  /* Start of video memory / 视频内存起始地址 */
    unsigned int i = 0;
    unsigned int screen_size = SCREEN_WIDTH * SCREEN_HEIGHT * 2;

    /* Clear the screen */
    /* 清屏 */
    for (i = 0; i < screen_size; i += 2) {
        vidptr[i] = ' ';       /* Set character to space / 设置字符为空格 */
        vidptr[i + 1] = WHITE_ON_BLACK;  /* Set attribute byte - white text on black background / 设置属性字节 - 白色文本，黑色背景 */
    }

    /* Print the string */
    /* 打印字符串 */
    i = 0;
    unsigned int j = 0;
    while (str[i] != '\0') {
        vidptr[j] = str[i];    /* Set character / 设置字符 */
        vidptr[j + 1] = WHITE_ON_BLACK;  /* Set attribute byte / 设置属性字节 */
        ++i;
        j += 2;
    }

    // 测试内存分配和释放
    void* test_alloc = kmalloc(256); // 分配256字节内存
    if (test_alloc) {
        kfree(test_alloc); // 释放分配的内存
    }

    /* Infinite loop to prevent kernel from exiting */
    /* 无限循环，防止内核退出 */
    while (1)
        ;
}
