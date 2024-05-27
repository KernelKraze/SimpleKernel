// SPDX-License-Identifier: MIT
// String library header file for MyOS
// Author: KernelKraze

#ifndef STRING_H
#define STRING_H

#include <stddef.h>

// String functions
void strcpy(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);
size_t strlen(const char *str);

#endif // STRING_H
