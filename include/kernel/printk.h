// SPDX-License-Identifier: MIT
// printk.h - Kernel printing functions for MyOS
// Author: KernelKraze

#ifndef PRINTK_H
#define PRINTK_H

#include <stdarg.h>

// Log levels / 日志级别
#define LOG_LEVEL_INFO  "INFO"
#define LOG_LEVEL_ERROR "ERROR"

// Kernel print function / 声明打印函数
void printk(const char *level, const char *format, ...);

#endif // PRINTK_H
