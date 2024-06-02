// SPDX-Identifier: MIT
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
#include "kernel/printk.h"  // 包含内核打印头文件
#include "kernel/time.h"

#define VIDEO_MEMORY 0xb8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define WHITE_ON_BLACK 0x07
#define MEMORY_SIZE_MB 4
#define MEMORY_SIZE_BYTES (MEMORY_SIZE_MB * 1024 * 1024)

// 假设这是内存的起始地址
extern char _end; // 链接脚本中定义的内核结束位置
void* memory_start = &_end;


// 清屏函数
void clear_screen(void) {
    char *vidptr = (char *)VIDEO_MEMORY;
    for (unsigned int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * 2; i += 2) {
        vidptr[i] = ' ';
        vidptr[i + 1] = WHITE_ON_BLACK;
    }
}

// 打印ASCII艺术字函数
void print_ascii_art(void) {
    const char *art[] = {
        "    __  __________    __    ____ _       ______  ____  __    ____ ",
        "   / / / / ____/ /   / /   / __ \\ |     / / __ \\/ __ \\/ /   / __ \\",
        "  / /_/ / __/ / /   / /   / / / / | /| / / / / / /_/ / /   / / / /",
        " / __  / /___/ /___/ /___/ /_/ /| |/ |/ / /_/ / _, _/ /___/ /_/ / ",
        "/_/ /_/_____/_____/_____ /\\____/ |__/|__/\\____/_/ |_/_____/_//_/  ",
        "                                                                  "
    };

    for (int i = 0; i < 6; i++) {
        printk(LOG_LEVEL_TEST, "%s\n", art[i]);
    }
}

// QEMU关机函数
#define QEMU_SHUTDOWN_PORT 0x604
#define QEMU_SHUTDOWN_CODE 0x2000

static inline void outw(uint16_t port, uint16_t data) {
    __asm__ volatile ("outw %0, %1" : : "a"(data), "Nd"(port));
}

void qemu_shutdown() {
    outw(QEMU_SHUTDOWN_PORT, QEMU_SHUTDOWN_CODE);
}

// ACPI关机函数
#define PM1A_CNT 0x404  // 假设这是PM1a控制端口地址
#define SLP_TYPa 0x2000 // SLP_TYPa值
#define SLP_EN 0x2000   // SLP_EN值

void acpi_shutdown() {
    outw(PM1A_CNT, SLP_TYPa | SLP_EN);
}

// 关机函数，选择合适的方法
void shutdown() {
    qemu_shutdown();
    acpi_shutdown();
}

// 内核主函数
void kernel_main(void) {
    printk(LOG_LEVEL_INFO, "Kernel started\n");

    // 初始化内存管理系统，假设总内存大小为4MB
    printk(LOG_LEVEL_INFO, "Initializing Memory Management\n");
    mm_init(memory_start, MEMORY_SIZE_BYTES);
    delay(5000);
    // 清屏
    clear_screen();

    // 打印内核初始化信息
    printk(LOG_LEVEL_INFO, "Screen Cleared\n");
    print_ascii_art();
    printk(LOG_LEVEL_INFO, "Memory Management Initialized\n");

    // 测试内存分配和释放
    void* test_alloc = kmalloc(256); // 分配256字节内存
    if (test_alloc != NULL) {
        printk(LOG_LEVEL_INFO, "Memory Allocated: 256 bytes\n");
        // 用于测试，假设test_alloc是一个字符串
        snprintf((char*)test_alloc, 256, "This is a test string stored in allocated memory.");
        printk(LOG_LEVEL_INFO, "Allocated Memory Content: %s\n", (char*)test_alloc);
        kfree(test_alloc); // 释放分配的内存
        printk(LOG_LEVEL_INFO, "Memory Freed: 256 bytes\n");
    } else {
        printk(LOG_LEVEL_ERROR, "Memory Allocation Failed\n");
    }

    // 添加延迟以查看输出
    //printk(LOG_LEVEL_INFO, "Delaying shutdown for 5 seconds...\n");
    //delay(5000);

    // 打印关机信息并调用关机函数
    //printk(LOG_LEVEL_INFO, "Shutting down the kernel\n");
    //shutdown();

    // 无限循环，防止内核退出
    while (1)
        ;
}
