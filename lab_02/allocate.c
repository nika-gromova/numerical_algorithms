#include <stdlib.h>
#include "allocate.h"

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
