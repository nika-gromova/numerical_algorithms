#include <stdlib.h>

#include "defines.h"
#include "process.h"

void sort_inc(double **mtr, int size)
{
    double tmp;
    for (int i = 0; i < size - 1; i++)
        for (int j = i + 1; j < size; j++)
        {
            if (mtr[0][i] > mtr[0][j])
            {
               tmp = mtr[0][i];
               mtr[0][i] = mtr[0][j];
               mtr[0][j] = tmp;
               tmp = mtr[1][i];
               mtr[1][i] = mtr[1][j];
               mtr[1][j] = tmp;
            }
        }
}


int search_place(double **mtr, int size, double search)
{
    int mid, left = 0, right = size - 1;
    while (left <= right)
    {
        mid = (left + right) / 2;
        if (IS_EQUAL(mtr[0][mid], search))
            return -10; // искомый элемент уже есть в массиве узлов
        if (mtr[0][mid] > search)
            right = mid - 1;
        else
            left = mid + 1;
    }
    if (left >= size || right < 0)
        return -20; // экстраполяция
    mid = (left + right) / 2;
    return mid;
}

int take_dots(double **mtr, int init_size, double **selected, int selected_size, int pos)
{
    int pos_l = pos - 1;
    int pos_r = pos + 1;

    selected[0][0] = mtr[0][pos];
    selected[1][0] = mtr[1][pos];
    int index = 1;
    while ((index < selected_size) && (pos_l >= 0 || pos_r < init_size))
    {
        if (pos_l >= 0)
        {
            selected[0][index] = mtr[0][pos_l];
            selected[1][index] = mtr[1][pos_l];
            pos_l--;
            index++;
        }
        if (pos_r < init_size && index < selected_size)
        {
            selected[0][index] = mtr[0][pos_r];
            selected[1][index] = mtr[1][pos_r];
            pos_r++;
            index++;
        }
    }
    if (index != selected_size)
        return NOT_ENOUGH_DATA;
    return OK;
}

double find(double **mtr, int size, int degree, double x)
{
    double res = 0;
    double *coefficients = calloc(degree + 1, sizeof(double));
    if (coefficients)
    {
        coefficients[0] = mtr[1][0];
        int n = degree;
        for (int i = 1; i < degree + 1; i++)
        {
            for (int j = 0; j < n; j++)
            {
                mtr[1][j] = (mtr[1][j] - mtr[1][j + 1]) / (mtr[0][0] - mtr[0][degree + 1 - n]);
            }
            coefficients[i] = mtr[1][0];
            n--;
        }
        res = coefficients[0];
        int c = 0;
        double mult = 1.0;
        for (int i = 1; i < degree + 1 && c < size; i++)
        {
            mult *= (x - mtr[0][c]);
            res += mult * coefficients[i];
            c++;
        }
        /*for (int i = 0; i < degree + 1; i++)
            printf("%.3lf ", coefficients[i]);
        printf("\n");*/
        free(coefficients);
    }
    return res;
}


int calculate(double **mtr, int size, double **selected, int selected_size, double x_for_search, int degree_of_polynomial, double *result)
{
    int rc;
    rc = search_place(mtr, size, x_for_search);
    if (rc != -10 && rc != -20)
    {
        rc = take_dots(mtr, size, selected, selected_size, rc);
        if (rc == OK)
        {
            sort_inc(selected, selected_size);
            /*printf("\nSelected:\n");
            for (int i = 0; i < selected_size; i++)
            {
                printf("%.3lf %.3lf\n", selected[0][i], selected[1][i]);
            }*/
            *result = find(selected, selected_size, degree_of_polynomial, x_for_search);
        }
    }
    return rc;
}
