#include <stdio.h>

#include "defines.h"
#include "io.h"
#include "allocate.h"

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
            rc = INPUT_ERROR;
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

