// SPDX-License-Identifier: MIT
// Memory management header file for MyOS
// Author: KernelKraze

#ifndef MM_H
#define MM_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// 内存块结构
typedef struct mem_block {
    size_t size;
    struct mem_block* next;
} mem_block_t;

/**
 * @brief 初始化内存管理器
 * 
 * @param start_address 内存开始地址
 * @param total_memory 内存总大小
 */
void mm_init(void* start_address, size_t total_memory);

/**
 * @brief 分配指定大小的内存
 * 
 * @param size 要分配的内存大小（字节）
 * @return 分配的内存块指针，失败时返回 NULL
 */
void* kmalloc(size_t size);

/**
 * @brief 释放先前分配的内存块
 * 
 * @param ptr 指向要释放的内存块的指针
 */
void kfree(void* ptr);

// 定义 uintptr_t 类型
typedef unsigned long uintptr_t;

#ifdef __cplusplus
}
#endif

#endif // MM_H
