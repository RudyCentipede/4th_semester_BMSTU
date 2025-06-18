#ifndef FIND_FUNC_ROOT
#define FIND_FUNC_ROOT

#include <cmath>
#include <iostream>

#define EPS 1e-6

double func(double x)
{
    double res = 0;
    double m = 7;

    __asm__(
        "fld %1\n"
        "fld %1\n"
        "fmulp\n" // x^2
        "fld %1\n"
        "fmulp\n" //x^3
        "fld %2\n"
        "faddp\n" //x^3 + 7
        "fcos\n"
        "fstp %0\n"
        : "=m"(res)
        : "m"(x), "m"(m));

    return res;
}

double mid_point(double a, double b)
{
    double res = 0;
    double div = 2;
    __asm__(
        "fld %1\n"
        "fld %2\n"
        "faddp %%ST(1), %%ST(0)\n" // результат в ST(1)

        "fld %3\n"
        "fdivp\n"

        "fstp %0\n"
        : "=m"(res)
        : "m"(a), "m"(b), "m"(div));

    return res;
}

int sign(double x)
{
    if (x > 0)
        return 1;
    else if (x < 0)
        return -1;
    return 0;
}

double find_func_root(double start, double end, int step_count)
{
    double res = 0;
    double c = 0;

    // c = mid_point(start, end);
    // std::cout << c << " " << func(c) << "*\n";

    for (int i = 0; i < step_count; ++i)
    {
        c = mid_point(start, end);

        if (fabs(func(c)) < EPS)
            return c;

        if (sign(func(c)) == sign(func(start)))
            start = c;
        else
            end = c;
    }

    return mid_point(start, end);
}

#endif