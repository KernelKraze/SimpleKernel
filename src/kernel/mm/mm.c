// SPDX-License-Identifier: MIT
// Memory management implementation for MyOS
// Author: KernelKraze

#include "kernel/mm.h"

// 全局空闲内存块链表头
static mem_block_t* free_list = NULL;

// 内存管理初始化
void mm_init(size_t total_memory) {
    // 将整个内存初始化为一个大块的空闲内存
    free_list = (mem_block_t*) total_memory;
    free_list->size = total_memory - sizeof(mem_block_t);
    free_list->next = NULL;
}

// 内存分配
void* kmalloc(size_t size) {
    mem_block_t* current = free_list;
    mem_block_t* previous = NULL;

    // 遍历空闲链表寻找合适大小的块
    while (current) {
        if (current->size >= size) {
            // 找到合适大小的块
            if (current->size > size + sizeof(mem_block_t)) {
                // 如果块足够大，分割成两部分
                mem_block_t* new_block = (mem_block_t*)((char*)current + sizeof(mem_block_t) + size);
                new_block->size = current->size - size - sizeof(mem_block_t);
                new_block->next = current->next;
                current->size = size;
                current->next = NULL;

                if (previous) {
                    previous->next = new_block;
                } else {
                    free_list = new_block;
                }
            } else {
                // 如果块大小刚好，直接使用
                if (previous) {
                    previous->next = current->next;
                } else {
                    free_list = current->next;
                }
            }

            return (void*)((char*)current + sizeof(mem_block_t));
        }

        previous = current;
        current = current->next;
    }

    // 没有找到合适大小的块
    return NULL;
}

// 内存释放
void kfree(void* ptr) {
    if (!ptr) {
        return;
    }

    mem_block_t* block = (mem_block_t*)((char*)ptr - sizeof(mem_block_t));
    block->next = free_list;
    free_list = block;
}
