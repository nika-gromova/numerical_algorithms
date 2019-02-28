#include "defines.h"
#include <stdlib.h>
#include <stdio.h>

void sort_inc(double **mtr, int size)
{
    double tmp; // сортировка с флагом по возрастанию
    int flag = 1;
    while (flag)
    {
        flag = 0;
        for (int i = 1; i < size; i++)
        {
            if (mtr[0][i] < mtr[0][i - 1])
            {
               tmp = mtr[0][i];
               mtr[0][i] = mtr[0][i - 1];
               mtr[0][i - 1] = tmp;
               tmp = mtr[1][i];
               mtr[1][i] = mtr[1][i - 1];
               mtr[1][i - 1] = tmp;
               flag = 1;
            }
        }
    }
}


int search_place(double **mtr, int size, double search, int *result)
{
    int mid;
    int left = 0, right = size - 1;
    while (left <= right)
    {
        mid = (left + right) / 2;
        if (IS_EQUAL(mtr[0][mid], search))
        {
            *result = mid;
            return FOUND; // искомый элемент уже есть в массиве узлов
        }
        if (mtr[0][mid] > search)
            right = mid - 1;
        else
            left = mid + 1;
    }
    if (left >= size || right < 0)
        return EXTRAPOLATION; // экстраполяция
    if (left > right)
    {
        int tmp = left;
        left = right;
        right = tmp;
    }
    *result = right;
    return OK; // найден индекс ближайшего справа известного значения
}

void filling_h(double **mtr, int size, double *h_arr)
{
    // размер массива h_arr равен size = N + 1
    h_arr[0] = 0;
    for (int i = 1; i < size; i++)
    {
        h_arr[i] = mtr[0][i] - mtr[0][i - 1];
    }
}

void filling_A(double *h_arr, int size, double *A_arr)
{
    // размер массива A_arr равен size
    A_arr[0] = 0;
    A_arr[1] = 0;
    for (int i = 2; i < size; i++)
    {
        A_arr[i] = h_arr[i - 1];
    }
}

void filling_B(double *h_arr, int size, double *B_arr)
{
    // размер массива B_arr равен size
    B_arr[0] = 0;
    B_arr[1] = 0;
    for (int i = 2; i < size; i++)
    {
        B_arr[i] = (-1) * 2 * (h_arr[i - 1] + h_arr[i]);
    }
}

void filling_D(double *h_arr, int size, double *D_arr)
{
    // размер массива D_arr равен size
    D_arr[0] = 0;
    D_arr[1] = 0;
    for (int i = 2; i < size; i++)
    {
        D_arr[i] =h_arr[i];
    }
}

void filling_F(double *h_arr, int size, double *F_arr, double **mtr)
{
    // размер массива F_arr равен size
    F_arr[0] = 0;
    F_arr[1] = 0;
    for (int i = 2; i < size; i++)
    {
        F_arr[i] = (-1) * 3 * ((mtr[1][i] - mtr[1][i - 1]) / h_arr[i] - (mtr[1][i - 1] - mtr[1][i - 2]) / h_arr[i - 1]);
    }
}

void filling_ksi(double *A_arr, double *D_arr, double *B_arr, double *ksi_arr, int size)
{
    // размер массива ksi_arr равен size + 1
    ksi_arr[0] = 0;
    ksi_arr[1] = 0;
    ksi_arr[2] = 0;
    for (int i = 3; i <= size; i++)
    {
        ksi_arr[i] = D_arr[i - 1] / (B_arr[i - 1] - A_arr[i - 1] * ksi_arr[i - 1]);
    }
}

void filling_eta(double *A_arr, double *F_arr, double *B_arr, double *eta_arr, double *ksi_arr, int size)
{
    // размер массива eta_arr равен size + 1
    eta_arr[0] = 0;
    eta_arr[1] = 0;
    eta_arr[2] = 0;
    for (int i = 3; i <= size; i++)
    {
        eta_arr[i] = (A_arr[i - 1] * eta_arr[i - 1] + F_arr[i - 1]) / (B_arr[i - 1] - A_arr[i - 1] * ksi_arr[i - 1]);
    }
}

void filling_C(double *eta_arr, double *ksi_arr, double *C_arr, int size)
{
    // размер массива C_arr равен size + 1
    C_arr[size] = 0;
    C_arr[0] = 0;
    C_arr[1] = 0;
    for (int i = size - 1; i >= 2; i--)
    {
        C_arr[i] = ksi_arr[i + 1] * C_arr[i + 1] + eta_arr[i + 1];
    }
}

int calculate_C(double **mtr, int size, double *C_arr, double *h_arr)
{
    int rc = OK;
    double *A_arr = calloc(size, sizeof(double));
    double *B_arr = calloc(size, sizeof(double));
    double *D_arr = calloc(size, sizeof(double));
    double *F_arr = calloc(size, sizeof(double));
    double *eta_arr = calloc(size + 1, sizeof(double));
    double *ksi_arr = calloc(size + 1, sizeof(double));
    if (A_arr && B_arr && D_arr && F_arr && eta_arr && ksi_arr)
    {
        filling_h(mtr, size, h_arr);
        filling_A(h_arr, size, A_arr);
        filling_B(h_arr, size, B_arr);
        filling_D(h_arr, size, D_arr);
        filling_F(h_arr, size, F_arr, mtr);
        filling_ksi(A_arr, D_arr, B_arr, ksi_arr, size);
        filling_eta(A_arr, F_arr, B_arr, eta_arr, ksi_arr, size);
        filling_C(eta_arr, ksi_arr, C_arr, size);
        /*for (int i = 0; i < size; i++)
        {
            printf("h[%d] = %.4lf\n", i, h_arr[i]);
            printf("A[%d] = %.4lf\n", i, A_arr[i]);
            printf("B[%d] = %.4lf\n", i, B_arr[i]);
            printf("D[%d] = %.4lf\n", i, D_arr[i]);
            printf("F[%d] = %.4lf\n", i, F_arr[i]);
            printf("\n");
        }
        for (int i = 0; i < size + 1; i++)
        {
            printf("ksi[%d] = %.4lf\n", i, ksi_arr[i]);
            printf("eta[%d] = %.4lf\n", i, eta_arr[i]);

            printf("\n");
        }*/
        free(A_arr);
        free(B_arr);
        free(D_arr);
        free(F_arr);
        free(eta_arr);
        free(ksi_arr);
    }
    else
        rc = MEMORY_ERROR;
    return rc;
}

int calculate_coefficients(double **mtr, double *a, double *b, double *c, double *d, int size)
{
    int rc = OK;

    double *h_arr = calloc(size, sizeof(double));
    if (h_arr)
    {
        rc = calculate_C(mtr, size, c, h_arr);
        if (rc == OK)
        {
            for (int i = 1; i < size; i++)
            {
                a[i] = mtr[1][i - 1];
                b[i] = ((mtr[1][i] - mtr[1][i - 1]) / h_arr[i]) - ((h_arr[i] * (c[i + 1] + 2 * c[i])) / 3);
                d[i] = (c[i + 1] - c[i]) / (3 * h_arr[i]);
            }
        }
        free(h_arr);
    }
    else
        rc = MEMORY_ERROR;
    return rc;
}

int calculate(double *result, double **mtr, double *a, double *b, double *c, double *d, int size, double x)
{
    int index;
    int rc = search_place(mtr, size, x, &index);
    if (rc == OK)
    {
        double tmp = x - mtr[0][index - 1];
        *result = a[index] + b[index] * tmp + c[index] * SQUARE(tmp) + d[index] * CUBE(tmp);
    }
    else if (rc == FOUND)
    {
        *result = mtr[1][index];
    }
    return rc;
}
