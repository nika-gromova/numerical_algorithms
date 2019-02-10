#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "io.h"

double **allocate(int n)
{
    double **matrix = malloc((n + 1) * sizeof(double *));
    if (matrix)
    {
        double *data = calloc(n * 2, sizeof(double));
        if (data)
        {
            for (int i = 0; i < n; i++)
                matrix[i] = data + i * 2;
            matrix[n] = data;
        }
        else
        {
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void free_matrix(double **mtr, int n)
{
    free(mtr[n]);
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
                    if (fscanf(f, "%lf %lf", &buf[i][0], &buf[i][1]) != 2)
                        rc = INPUT_ERROR;
                }
                if (rc == INPUT_ERROR)
                {
                    free_matrix(buf, *n);
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
        printf("%.3lf %.3lf\n", mtr[i][0], mtr[i][1]);
    }
}
