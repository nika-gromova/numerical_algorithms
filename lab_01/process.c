#include <stdlib.h>
#include <stdio.h>

#include "defines.h"
#include "process.h"
#include "allocate.h"


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


int search_place(double **mtr, int size, double search, int *left, int *right)
{
    int mid;
    *left = 0, *right = size - 1;
    while (*left <= *right)
    {
        mid = (*left + *right) / 2;
        if (IS_EQUAL(mtr[0][mid], search))
            return mid; // искомый элемент уже есть в массиве узлов
        if (mtr[0][mid] > search)
            *right = mid - 1;
        else
            *left = mid + 1;
    }
    if (*left >= size || *right < 0)
        return EXTRAPOLATION; // экстраполяция

    return FOUND; // найдены индексы ближайших элементов
}

int take_dots(double **mtr, int init_size, double **selected, int selected_size, int pos_l, int pos_r)
{
    selected[0][0] = mtr[0][pos_l];
    selected[1][0] = mtr[1][pos_l];
    selected[0][1] = mtr[0][pos_r];
    selected[1][1] = mtr[1][pos_r];
    int index = 2;
    pos_l--;
    pos_r++;

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

double find(double **mtr, int size, int degree, double x, int *rc)
{
    *rc = OK;
    double div;
    double res = 0;
    double *coefficients = calloc(degree + 1, sizeof(double));
    if (coefficients)
    {
        coefficients[0] = mtr[1][0];
        int n = degree;
        for (int i = 1; i < degree + 1 && *rc == OK; i++)
        {
            for (int j = 0; j < n && *rc == OK; j++)
            {
                div = mtr[0][0] - mtr[0][degree + 1 - n];
                if (fabs(div) <= EPS)
                    *rc = ZERO_DEVISION;
                else
                    mtr[1][j] = (mtr[1][j] - mtr[1][j + 1]) / (div);
            }
            coefficients[i] = mtr[1][0];
            n--;
        }
        if (*rc == OK)
        {
            res = coefficients[0];
            int c = 0;
            double mult = 1.0;
            for (int i = 1; i < degree + 1 && c < size; i++)
            {
                mult *= (x - mtr[0][c]);
                res += mult * coefficients[i];
                c++;
            }
        }
        /*for (int i = 0; i < degree + 1; i++)
            printf("%.3lf ", coefficients[i]);
        printf("\n");*/
        free(coefficients);
    }
    return res;
}


int calculate(double **mtr, int size, double x_for_search, int degree_of_polynomial, double *result)
{
    int rc;
    int left, right;
    double **mtr_selected = NULL;
    int selected_size;
    rc = search_place(mtr, size, x_for_search, &left, &right);
    if (left > right)
    {
        int tmp = left;
        left = right;
        right = tmp;
    }
    if (rc == EXTRAPOLATION)
    {
        degree_of_polynomial = size - 1;
        printf("Extrapolation occured:\n");
        rc = FOUND;
    }
    //printf("left = %d, right = %d\n", left, right);
    if (rc == FOUND)
    {
        selected_size = degree_of_polynomial + 1;
        mtr_selected = allocate(selected_size);
        if (mtr_selected)
        {
            rc = take_dots(mtr, size, mtr_selected, selected_size, left, right);
            if (rc == OK)
            {
                sort_inc(mtr_selected, selected_size);
                /*printf("\nSelected:\n");
                for (int i = 0; i < selected_size; i++)
                {
                    printf("%.3lf %.3lf\n", mtr_selected[0][i], mtr_selected[1][i]);
                }*/
                *result = find(mtr_selected, selected_size, degree_of_polynomial, x_for_search, &rc);
                if (rc == OK)
                    rc = FOUND;
            }
            free_matrix(mtr_selected);
        }
        else
            rc = MEMORY_ERROR;
    }
    return rc;
}
