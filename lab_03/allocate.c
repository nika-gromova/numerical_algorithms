#include <stdlib.h>
#include "allocate.h"

double **allocate(int n, int m)
{
    double **matrix = malloc((n + 1) * sizeof(double *));
    if (matrix)
    {
        double *data = calloc(n * m, sizeof(double));
        if (data)
        {
            for (int i = 0; i < n; i++)
                matrix[i] = data + i * m;
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
