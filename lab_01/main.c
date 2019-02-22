#include <stdio.h>

#include "defines.h"
#include "io.h"
#include "process.h"
#include "allocate.h"

void info(void)
{
    printf("Please input in command line:\n");
    printf("app.exe input.txt\n");
    printf("\n'input.txt' - is a file, where the initial information about dots is stored.\n");
}

void messages(int rc, double result, double **mtr)
{
    if (rc == FOUND)
    {
        printf("result = %.3lf\n", result);
    }
    else if (rc == EXTRAPOLATION)
    {
        printf("Extrapolation occured. Calculation stopped.\n");
    }
    else if (rc == NOT_ENOUGH_DATA)
    {
        printf("There is not enough data for making polynomial of needed degree!\n");
    }
    else if (rc == ZERO_DEVISION)
    {
        printf("This method isn't suitable for such calculations!:(\n");
    }
    else
    {
        printf("Searching argument is already in initial data: (%.3lf, %.3lf).\n",  mtr[0][rc], mtr[1][rc]);
    }
}

void swap(double **mtr)
{
    double *tmp = mtr[0];
    mtr[0] = mtr[1];
    mtr[1] = tmp;
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
    int size = 0;
    double x_for_search;
    int degree_of_polynomial;
    double result;
    FILE *f = fopen(argv[1], "r");
    if (f)
    {
        rc = read_file(f, &mtr, &size);
        if (rc == OK)
        {
            sort_inc(mtr, size);
            print_dots(mtr, size);
            printf("\n");
            printf("Input X:\n");
            if (scanf("%lf", &x_for_search) == 1)
            {
                printf("Input degree of the polynomial n (positive integer):\n");
                if (scanf("%d", &degree_of_polynomial) == 1)
                {
                    if (degree_of_polynomial < 0)
                    {
                        printf("Only positive integer numbers!\n");
                        rc = INPUT_ERROR;
                    }
                    else if (degree_of_polynomial == 0)
                    {
                        printf("result is only %.3lf. Try to input bigger degree.\n", mtr[1][0]);
                    }
                    else if (degree_of_polynomial >= size)
                    {
                        printf("Not enough data\n");
                    }
                    else
                    {
                        rc = calculate(mtr, size, x_for_search, degree_of_polynomial, &result);
                        messages(rc, result, mtr);
                        printf("Finding roots:\n");
                        swap(mtr);
                        //sort_inc(mtr, size);
                        rc = calculate(mtr, size, 0.0, degree_of_polynomial, &result);
                        swap(mtr);
                        messages(rc, result, mtr);
                    }
                }
                else
                    rc = INPUT_ERROR;
            }
            else
                rc = INPUT_ERROR;
            if (rc == INPUT_ERROR)
                printf("Incorrect input!\n");
            free_matrix(mtr);
        }
        else
        {
            printf("Error: %d\n", rc);
        }
        fclose(f);
    }
    else
    {
        printf("Error in opening file. Please try again\n");
        return OPEN_FILE_ERROR;
    }
    if (rc == FOUND)
        rc = OK;
    return rc;
}



