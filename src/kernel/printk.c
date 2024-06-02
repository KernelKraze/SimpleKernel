// SPDX-License-Identifier: MIT
// printk.c - Kernel printing functions for MyOS
// Author: KernelKraze

#include "kernel/printk.h"
#include "string.h"
#include <stdarg.h>

#define VIDEO_MEMORY 0xb8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define WHITE_ON_BLACK 0x07

static unsigned int cursor_pos = 0;

// Helper function to convert integer to hex string
// 辅助函数，将整数转换为十六进制字符串
static void itoh(unsigned int num, char *buffer) {
    const char *hex_digits = "0123456789ABCDEF";
    int i;
    for (i = 7; i >= 0; i--) {
        buffer[i] = hex_digits[num & 0xF];
        num >>= 4;
    }
    buffer[8] = '\0';
}

// Helper function to format and print a string to the video memory
// 辅助函数，格式化并打印字符串到视频内存
static void vprintk(const char *format, va_list args) {
    char buffer[1024];
    char *buf_ptr = buffer;
    const char *fmt_ptr = format;

    while (*fmt_ptr) {
        if (*fmt_ptr == '%') {
            fmt_ptr++;
            switch (*fmt_ptr) {
                case 'd': { // Decimal integer
                    int num = va_arg(args, int);
                    buf_ptr += snprintf(buf_ptr, sizeof(buffer) - (buf_ptr - buffer), "%d", num);
                    break;
                }
                case 'x': { // Hexadecimal integer
                    int num = va_arg(args, int);
                    buf_ptr += snprintf(buf_ptr, sizeof(buffer) - (buf_ptr - buffer), "%x", num);
                    break;
                }
                case 'p': { // Pointer
                    void *ptr = va_arg(args, void *);
                    buf_ptr += snprintf(buf_ptr, sizeof(buffer) - (buf_ptr - buffer), "0x%08x", (unsigned int)ptr);
                    break;
                }
                case 's': { // String
                    char *str = va_arg(args, char *);
                    buf_ptr += snprintf(buf_ptr, sizeof(buffer) - (buf_ptr - buffer), "%s", str);
                    break;
                }
                case '%': { // Percent sign
                    *buf_ptr++ = '%';
                    break;
                }
                default: { // Unsupported format specifier
                    *buf_ptr++ = '%';
                    *buf_ptr++ = *fmt_ptr;
                    break;
                }
            }
        } else {
            *buf_ptr++ = *fmt_ptr;
        }
        fmt_ptr++;
    }
    *buf_ptr = '\0';

    char *vidptr = (char *)VIDEO_MEMORY;
    for (char *p = buffer; *p != '\0'; p++) {
        if (*p == '\n') {
            // Move to the beginning of the next line
            // 移动到下一行的开头
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

// Kernel print function with log level
// 带有日志级别的内核打印函数
void printk(const char *level, const char *format, ...) {
    char log_buffer[1024];
    va_list args;

    // Create the log message with level prefix
    // 创建带有级别前缀的日志消息
    snprintf(log_buffer, sizeof(log_buffer), "[%s] %s", level, format);

    va_start(args, format);
    vprintk(log_buffer, args);
    va_end(args);
}
