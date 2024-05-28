// SPDX-License-Identifier: MIT
// Memory management header file for MyOS
// Author: KernelKraze

#ifndef MM_H
#define MM_H

#include <stddef.h>

// 内存块结构
typedef struct mem_block {
    size_t size;
    struct mem_block* next;
} mem_block_t;

// 内存管理初始化
void mm_init(size_t total_memory);

// 内存分配和释放
void* kmalloc(size_t size);
void kfree(void* ptr);

#endif // MM_H
