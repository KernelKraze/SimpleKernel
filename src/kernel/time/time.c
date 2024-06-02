// SPDX-Identifier: MIT
// time.c - Time management functions for MyOS
// Author: KernelKraze

#include "kernel/time.h"
#include "kernel/io.h"  // 包含定义outb函数的头文件

// 全局变量保存CPU频率（单位：Hz）
static uint64_t cpu_frequency_hz = 0;

// 读取TSC的函数
uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

// 64位除法函数
static uint64_t udivdi3(uint64_t n, uint64_t d) {
    uint64_t q = 0, r = 0;
    for (int i = 63; i >= 0; i--) {
        r = (r << 1) | ((n >> i) & 1);
        if (r >= d) {
            r -= d;
            q |= (1ull << i);
        }
    }
    return q;
}

// 初始化时间管理函数
void init_time(void) {
    // 使用PIT定时器进行初始化
    // PIT定时器设置为100 Hz，每10 ms触发一次中断
    uint16_t divisor = 1193; // 1193180 Hz / 100 Hz = 11931.8 ~= 1193

    // 设置PIT通道0
    outb(0x43, 0x36); // 设置控制字
    outb(0x40, divisor & 0xFF); // 设置低字节
    outb(0x40, (divisor >> 8) & 0xFF); // 设置高字节

    // 记录初始TSC值
    uint64_t start_tsc = rdtsc();
    
    // 等待10次中断（100 ms）
    for (int i = 0; i < 100000; i++) {
        // 等待PIT中断
        __asm__ volatile ("hlt");
    }

    // 记录结束TSC值
    uint64_t end_tsc = rdtsc();

    // 计算CPU频率
    cpu_frequency_hz = udivdi3(end_tsc - start_tsc, 100); // 每秒时钟周期数
}

// 延迟函数，以毫秒为单位
void delay(uint32_t milliseconds) {
    uint64_t start = rdtsc();
    uint64_t cycles_per_millisecond = udivdi3(cpu_frequency_hz, 1000);
    uint64_t end = start + cycles_per_millisecond * milliseconds;

    while (rdtsc() < end) {
        // 忙等待，直到达到目标时间
    }
}
