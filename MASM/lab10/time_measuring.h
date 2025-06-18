#ifndef TIME_MEASURING_H
#define TIME_MEASURING_H

#include <iostream>
#include <cstdint>
#include <string>

#define ITER_COUNT 1000000

inline uint64_t rdtsc() {
    uint32_t lo, hi;
    asm volatile (
            "rdtsc"
            : "=a" (lo), "=d" (hi)
            );
    return ((uint64_t)hi << 32) | lo;
}

inline void cpu_fence() {
    asm volatile ("" ::: "memory");
}

template <typename T>
void measure(T (*func)(T a, T b), const std::string &name) {
    uint64_t sum = 0;
    for (int i = 0; i < ITER_COUNT; i++) {
        T a = static_cast<T>(rand()) / 50.0;
        T b = static_cast<T>(rand()) / 500.0;

        cpu_fence();
        uint64_t start = rdtsc();
        cpu_fence();

        func(a, b);

        cpu_fence();
        uint64_t end = rdtsc();
        cpu_fence();

        sum += (end - start);
    }
    std::cout << name << " ticks = " << sum / ITER_COUNT << std::endl;
}

#endif