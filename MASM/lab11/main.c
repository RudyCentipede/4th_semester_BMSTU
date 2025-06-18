#include <stdio.h>
#include "matrix.h"

#define OK 0
#define SIZES_ERR 1
#define MATRIX_ALLOC_ERR 2
#define MATRIX_SCAN_ERR 3

int main(void)
{
    matrix_t a = { 0 };
    matrix_t b = { 0 };
    size_t n = 0;
    size_t m = 0;

    printf("Enter num of rows for matrix A: ");
    if (scanf("%zu", &n) != 1 || n == 0)
    {
        printf("Wrong num of rows\n");
        return SIZES_ERR;
    }

    printf("Enter num of columns for matrix A: ");
    if (scanf("%zu", &m) != 1 || m == 0)
    {
        printf("Wrong num of columns\n");
        return SIZES_ERR;
    }

    if (matrix_alloc(&a, n, m) != 0)
    {
        printf("Matrix A allocation error\n");
        return MATRIX_ALLOC_ERR;
    }

    printf("Enter matrix A:\n");
    if (matrix_scan(&a) != 0)
    {
        matrix_free(&a);
        printf("Matrix A scan error\n");
        return MATRIX_SCAN_ERR;
    }

    printf("Enter num of rows for matrix B: ");
    if (scanf("%zu", &n) != 1 || n == 0)
    {
        matrix_free(&a);
        printf("Wrong num of rows\n");
        return SIZES_ERR;
    }

    printf("Enter num of columns for matrix B: ");
    if (scanf("%zu", &m) != 1 || m == 0)
    {
        matrix_free(&a);
        printf("Wrong num of columns\n");
        return SIZES_ERR;
    }

    if (matrix_alloc(&b, n, m) != 0)
    {
        matrix_free(&a);
        printf("Matrix B allocation error\n");
        return MATRIX_ALLOC_ERR;
    }

    printf("Enter matrix B:\n");
    if (matrix_scan(&b) != 0)
    {
        matrix_free(&a);
        matrix_free(&b);
        printf("Matrix B scan error\n");
        return MATRIX_SCAN_ERR;
    }

    if (a.m != b.n)
        printf("Impossible to multiply matrices A and B. Num of columns of A have to be equal to num of rows of B");
    else
    {
        matrix_t c = matrix_mul(a, b);
        matrix_print(c);
    }


    matrix_free(&a);
    matrix_free(&b);
}
