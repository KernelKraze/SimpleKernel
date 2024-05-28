// SPDX-License-Identifier: MIT
// String library header file for MyOS
// Author: KernelKraze

#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdarg.h>

// Function to copy a string from src to dest
// 将源字符串复制到目标缓冲区
void strcpy(char *dest, const char *src);

// Function to compare two strings
// 比较两个字符串
int strcmp(const char *s1, const char *s2);

// Function to calculate the length of a string
// 计算字符串的长度
size_t strlen(const char *str);

// Function to format a string and place it in a buffer
// 格式化字符串并将其放入缓冲区
int vsnprintf(char *str, size_t size, const char *format, va_list ap);

// Function to print a formatted string to the video memory
// 打印格式化字符串到视频内存
void vprintf(const char *format, va_list args);

// Function to print a formatted string
// 打印格式化字符串
void printf(const char *format, ...);

#endif // STRING_H
