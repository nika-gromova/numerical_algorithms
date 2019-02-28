#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "allocate.h"
#include "process.h"

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


void info(void)
{
    printf("Please input in command line:\n");
    printf("app.exe input.txt\n");
    printf("\n'input.txt' - is a file, where the initial information about dots is stored.\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        info();
        return PARAMS_ERROR;
    }
    int rc = OK;
    double **mtr = NULL;
    double *a = NULL;
    double *b = NULL;
    double *c = NULL;
    double *d = NULL;
    int size = 0;
    double x_for_search;
    double result;
    FILE *f = fopen(argv[1], "r");
    if (f)
    {
        rc = read_file(f, &mtr, &size); // size - сколько всего точек (N + 1)
        if (rc == OK)
        {
            sort_inc(mtr, size);
            print_dots(mtr, size);
            a = calloc(size, sizeof(double));
            b = calloc(size, sizeof(double));
            d = calloc(size, sizeof(double));
            c = calloc(size + 1, sizeof(double));
            if (a && b && c && d)
            {
                rc = calculate_coefficients(mtr, a, b, c, d, size);
                if (rc == OK)
                {
                    /*for (int i = 0; i < size; i++)
                    {
                        printf("a[%d] = %.4lf\n", i, a[i]);
                        printf("b[%d] = %.4lf\n", i, b[i]);
                        printf("c[%d] = %.4lf\n", i, c[i]);
                        printf("d[%d] = %.4lf\n", i, d[i]);
                        printf("\n");
                    }*/
                    printf("Input X:\n");
                    if (scanf("%lf", &x_for_search) == 1)
                    {
                        rc = calculate(&result, mtr, a, b, c, d, size, x_for_search);
                        if (rc == OK)
                        {
                            printf("Result: %.6lf\n", result);
                        }
                        else if (rc == FOUND)
                        {
                            printf("Searching argument is already in initial data: (%.3lf, %.3lf).\n", x_for_search, result);
                            rc = OK;
                        }
                        else if (rc == EXTRAPOLATION)
                            printf("Extrapolation occured. Calculation stopped.\n");
                    }
                    else
                        rc = INPUT_ERROR;
                }
                free(a);
                free(b);
                free(c);
                free(d);
            }
            else
                rc = MEMORY_ERROR;
            free_matrix(mtr);
        }
        switch(rc)
        {
        case MEMORY_ERROR:
            printf("Some memory errors occured!\n");
            break;
        case INPUT_ERROR:
            printf("Input error!\n");
            break;
        }
        fclose(f);
    }
    else
    {
        printf("errors while openning file %s\n", argv[1]);
        rc = OPEN_FILE_ERROR;
    }
    return rc;
}
