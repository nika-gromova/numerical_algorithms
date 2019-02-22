#include "defines.h"

int search_place(double **mtr, int size, double search, int *result)
{
    int mid;
    int left = 0, right = size - 1;
    while (left <= right)
    {
        mid = (left + right) / 2;
        if (IS_EQUAL(mtr[0][mid], search))
            return FOUND; // искомый элемент уже есть в массиве узлов
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

int filling_h(double **mtr, int size, double *h_arr)
{
    h_arr[0] = 0; // размер массива h_arr равен size
    for (int i = 1; i < size; i++)
    {
        h_arr[i] = mtr[0][i] - mtr[0][i - 1];
    }
}

int filling_A(double *h_arr, int size, double *A_arr)
{
    for (int i = 1; i < size; i++)
    {
        A[i + 1] =
    }
}
{

}
