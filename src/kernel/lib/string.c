// SPDX-License-Identifier: MIT
// String library functions for MyOS
// Author: KernelKraze

#include "string.h"
#include <stdarg.h>

#define VIDEO_MEMORY 0xb8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define WHITE_ON_BLACK 0x07

static unsigned int cursor_pos = 0;

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

// Print a formatted string to the video memory
// 打印格式化字符串到视频内存
void vprintf(const char *format, va_list args) {
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);

    char *vidptr = (char *)VIDEO_MEMORY;

    for (char *p = buffer; *p != '\0'; p++) {
        if (*p == '\n') {
            // Move to the beginning of the next line
            // 移动到下一行的开头位置
            cursor_pos = ((cursor_pos / (SCREEN_WIDTH * 2)) + 1) * SCREEN_WIDTH * 2;
            continue;
        }

        vidptr[cursor_pos] = *p;
        vidptr[cursor_pos + 1] = WHITE_ON_BLACK;
        cursor_pos += 2;

        // Move to the next line if end of screen width is reached
        // 如果到达屏幕宽度的末尾，则移动到下一行
        if (cursor_pos >= SCREEN_WIDTH * 2 * SCREEN_HEIGHT) {
            cursor_pos = 0;
        }
    }
}


// Print a formatted string
// 打印格式化字符串
void printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}
