// SPDX-License-Identifier: MIT
// -*- linux-c -*- -------------------------------------------------------
// 
//   Copyright (C) 2024 KernelKraze
//   All Rights Reserved
// 
// -----------------------------------------------------------------------

/*
 * This is a simple kernel entry point for a custom OS.
 * 这是一个自定义操作系统的简单内核入口点。
 */

void kernel_main(void)
{
    const char *str = "Hello, World!";
    char *vidptr = (char *)0xb8000;  /* Start of video memory / 视频内存起始地址 */
    unsigned int i = 0;
    unsigned int j = 0;

    /* Clear the screen */
    /* 清屏 */
    while (j < 80 * 25 * 2) {
        vidptr[j] = ' ';       /* Set character to space / 设置字符为空格 */
        vidptr[j + 1] = 0x07;  /* Set attribute byte - white text on black background / 设置属性字节 - 白色文本，黑色背景 */
        j += 2;
    }

    j = 0;
    /* Print the string */
    /* 打印字符串 */
    while (str[i] != '\0') {
        vidptr[j] = str[i];    /* Set character / 设置字符 */
        vidptr[j + 1] = 0x07;  /* Set attribute byte / 设置属性字节 */
        ++i;
        j += 2;
    }

    while (1) /* Infinite loop to prevent kernel from exiting / 无限循环，防止内核退出 */
        ;
}
