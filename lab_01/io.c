#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "io.h"

double **allocate(int n)
{
    double **matrix = malloc(3 * sizeof(double *));
    if (matrix)
    {
        double *data = calloc(n * 2, sizeof(double));
        if (data)
        {
            for (int i = 0; i < 2; i++)
                matrix[i] = data + i * n;
            matrix[2] = data;
        }
        else
        {
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void free_matrix(double **mtr)
{
    free(mtr[2]);
    free(mtr);
}

int read_file(FILE *f, double ***mtr, int *n)
{
    double **buf = NULL;
    int rc = OK;
    if (fscanf(f, "%d", n) == 1)
    {
        if (*n > 0)
        {
            buf = allocate(*n);
            if (buf)
            {
                for (int i = 0; i < *n && rc == OK; i++)
                {
                    if (fscanf(f, "%lf %lf", &buf[0][i], &buf[1][i]) != 2)
                        rc = INPUT_ERROR;
                }
                if (rc == INPUT_ERROR)
                {
                    free_matrix(buf);
                }
                else
                    *mtr = buf;
            }
            else
                rc = MEMORY_ERROR;
        }
        else
            rc = PARAMS_ERROR;
    }
    else
        rc =INPUT_ERROR;
    return rc;
}

void print_dots(double **mtr, int n)
{
    printf("Dots:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%.3lf %.3lf\n", mtr[0][i], mtr[1][i]);
    }
}

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
