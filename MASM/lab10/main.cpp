#include <iostream>

#include "time_measuring.h"
#include "sum.h"
#include "mul.h"
#include "sin_compare.h"
#include "find_func_root.h"

int main()
{
#ifdef TIME
    srand(time(NULL));
//
//    double a =  static_cast<double>(rand()) / 50.0;
//    double b =  static_cast<double>(rand()) / 500.0;

    cpu_fence();
    uint64_t total_start = rdtsc();
    cpu_fence();

    std::cout << "float compare:" << std::endl;
    measure<float>(sum<float>, "add");
    measure<float>(sum_asm<float>, "asm add");
    measure<float>(mul<float>, "mul");
    measure<float>(mul_asm<float>, "asm mul");

    std::cout << "\ndouble compare:" << std::endl;
    measure<double>(sum<double>, "add");
    measure<double>(sum_asm<double>, "asm add");
    measure<double>(mul<double>, "mul");
    measure<double>(mul_asm<double>, "asm mul");


    cpu_fence();
    uint64_t total_end = rdtsc();
    cpu_fence();

    std::cout << "\nTotal ticks: " << (total_end - total_start) << std::endl;
#endif

#ifdef SIN
    std::cout << "\nSin compare:" << std::endl;
    c_sin();
    std::cout << std::endl;
    asm_sin();
#endif

#ifdef ROOT
    std::cout << "\nRoot of cos(x^3 + 7): " << find_func_root(0.5, 1, 10) << std::endl;
#endif
}
