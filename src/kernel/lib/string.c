// SPDX-License-Identifier: MIT
// String library functions for MyOS
// Author: KernelKraze

#include "string.h"

// String functions
// Implement string manipulation routines here

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
