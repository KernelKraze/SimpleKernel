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

// Helper function to format and print a string to the video memory
static void vprintk(const char *format, va_list args) {
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);

    char *vidptr = (char *)VIDEO_MEMORY;

    for (char *p = buffer; *p != '\0'; p++) {
        if (*p == '\n') {
            // Move to the beginning of the next line
            cursor_pos = ((cursor_pos / (SCREEN_WIDTH * 2)) + 1) * SCREEN_WIDTH * 2;
            continue;
        }

        vidptr[cursor_pos] = *p;
        vidptr[cursor_pos + 1] = WHITE_ON_BLACK;
        cursor_pos += 2;

        // Move to the next line if end of screen width is reached
        if (cursor_pos >= SCREEN_WIDTH * 2 * SCREEN_HEIGHT) {
            cursor_pos = 0;
        }
    }
}

// Kernel print function with log level
void printk(const char *level, const char *format, ...) {
    char log_buffer[1024];
    va_list args;

    // Create the log message with level prefix
    snprintf(log_buffer, sizeof(log_buffer), "[%s] %s", level, format);

    va_start(args, format);
    vprintk(log_buffer, args);
    va_end(args);
}
