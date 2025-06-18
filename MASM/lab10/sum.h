#ifndef SUM_H
#define SUM_H

template <typename T>
T sum(T a, T b)
{
    T res = 0;

    res = a + b;

    return res;
}

template <typename T>
T sum_asm(T a, T b)
{
    T res = 0;

    __asm__(
        "fld %1\n"
        "fld %2\n"
        "faddp %%ST(1), %%ST(0)\n"

        "fstp %0\n"
        : "=m"(res)
        : "m"(a),
          "m"(b)
    );

    return res;
}

#endif