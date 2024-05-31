// SPDX-License-Identifier: MIT
// String library functions for MyOS
// Author: KernelKraze

#include "string.h"
#include <stdarg.h>

// String functions
// Implement string manipulation routines here
// 字符串函数
// 在这里实现字符串操作例程

void strcpy(char *dest, const char *src) {
    while ((*dest++ = *src++));
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

size_t strlen(const char *str) {
    size_t len = 0;
    while (*str++) {
        len++;
    }
    return len;
}

// Helper function to convert integer to string
// 将整数转换为字符串的辅助函数
static void itoa(int value, char *str, int base) {
    char *rc;
    char *ptr;
    char *low;
    
    rc = ptr = str;

    // Handle negative numbers for base 10
    // 处理十进制的负数
    if (base == 10 && value < 0) {
        *ptr++ = '-';
        value = -value;
    }

    // Process digits
    // 处理数字
    low = ptr;
    do {
        *ptr++ = "0123456789abcdef"[value % base];
        value /= base;
    } while (value);
    
    // Terminate string in buffer.
    // 在缓冲区中终止字符串
    *ptr-- = '\0';

    // Reverse string
    // 反转字符串
    while (low < ptr) {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
}

int vsnprintf(char *str, size_t size, const char *format, va_list ap) {
    char *buf = str;
    const char *p = format;
    char tmp[20];
    int len = 0;

    for (; *p != '\0'; p++) {
        if (*p != '%') {
            if (len < size - 1) {
                *buf++ = *p;
                len++;
            }
            continue;
        }

        p++;

        switch (*p) {
            case 'd':
            case 'i': {
                int num = va_arg(ap, int);
                itoa(num, tmp, 10);
                for (char *t = tmp; *t != '\0'; t++) {
                    if (len < size - 1) {
                        *buf++ = *t;
                        len++;
                    }
                }
                break;
            }
            case 's': {
                char *s = va_arg(ap, char*);
                while (*s) {
                    if (len < size - 1) {
                        *buf++ = *s++;
                        len++;
                    }
                }
                break;
            }
            default:
                if (len < size - 1) {
                    *buf++ = '%';
                    len++;
                }
                if (len < size - 1) {
                    *buf++ = *p;
                    len++;
                }
                break;
        }
    }

    if (len < size) {
        *buf = '\0';
    } else {
        str[size - 1] = '\0';
    }

    return len;
}

int snprintf(char *str, size_t size, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    int result = vsnprintf(str, size, format, ap);
    va_end(ap);
    return result;
}
