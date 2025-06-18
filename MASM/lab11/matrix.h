#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    double **data;
    size_t n;
    size_t m;
} matrix_t;


void matrix_free(matrix_t *matrix)
{
    for (size_t i = 0; i < matrix->n; i++)
        if (matrix->data[i])
            free(matrix->data[i]);
    if (matrix->data)
        free(matrix->data);
}


int matrix_alloc(matrix_t *matrix, size_t n, size_t m)
{
    matrix->data = calloc(n, sizeof(double*));
    if (matrix->data == NULL)
        return 1;

    matrix->n = n;
    matrix->m = m;

    for (size_t i = 0; i < n; i++)
    {
        matrix->data[i] = calloc(m, sizeof(double));
        if (matrix->data[i] == NULL)
        {
            matrix_free(matrix);
            return 1;
        }
    }
    return 0;
}

int matrix_scan(matrix_t *matrix)
{
    for (int i = 0; i < matrix->n; i++)
        for (int j = 0; j < matrix->m; j++)
            if(scanf("%lf", &(matrix->data[i][j])) != 1)
                return 1;
    return 0;
}

void matrix_print(matrix_t matrix)
{
    for (int i = 0; i < matrix.n; i++)
    {
        for (int j = 0; j < matrix.m; j++)
        {
            printf("%lf ", matrix.data[i][j]);
        }
        printf("\n");
    }
}


matrix_t matrix_transpose(matrix_t matrix)
{
    matrix_t res = { 0 };
    matrix_alloc(&res, matrix.m, matrix.n);

    for (int i = 0; i < matrix.n; i++)
        for (int j = 0; j < matrix.m; j++)
            res.data[j][i] = matrix.data[i][j];

    return res;
}

static double mul_and_sum_sse(double *sa, double *sb, int n)
{
    double tmp = 0.0, res = 0.0;
    __float128 *a = (__float128 *)sa;
    __float128 *b = (__float128 *)sb;
    for (size_t i = 0; i < n; i += 2, a++, b++)
    {
        __asm__(
                "movapd xmm0, %1\n"
                "movapd xmm1, %2\n"
                "mulpd xmm0, xmm1\n"
                "haddpd xmm0, xmm0\n"
                "movsd %0, xmm0\n"
                : "=m"(tmp)
                : "m"(*a), "m"(*b)
                : "xmm0", "xmm1");

        res += tmp;
    }


    return res;
}

matrix_t matrix_mul(matrix_t a, matrix_t b)
{
    matrix_t trans_b = matrix_transpose(b);

    matrix_t res = { 0 };
    matrix_alloc(&res, a.n, b.m);

    for (int i = 0; i < res.n; i++)
        for (int j = 0; j < res.m; j++)
            res.data[i][j] = mul_and_sum_sse(a.data[i], trans_b.data[j], a.m);

    matrix_free(&trans_b);

    return res;
}


#endif //MATRIX_H
