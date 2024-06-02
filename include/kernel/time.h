// SPDX-Identifier: MIT
// time.h - Time management functions for MyOS
// Author: KernelKraze

#ifndef TIME_H
#define TIME_H

#include "kernel/types.h"

// 读取TSC的函数
uint64_t rdtsc(void);

// 初始化时间管理函数
void init_time(void);

// 延迟函数，以毫秒为单位
void delay(uint32_t milliseconds);

#endif // TIME_H
