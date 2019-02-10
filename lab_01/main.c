#include <stdio.h>

#include "defines.h"
#include "io.h"
#include "process.h"

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
    int size = 0;
    double x_for_search;
    int degree_of_polynomial;
    double **mtr_selected = NULL;
    int selected_size = 0;
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
                printf("Input degree of the polynomial n:\n");
                if (scanf("%d", &degree_of_polynomial) == 1)
                {
                    selected_size = degree_of_polynomial + 1;
                    mtr_selected = allocate(selected_size);
                    if (mtr_selected)
                    {
                        rc = calculate(mtr, size, mtr_selected, selected_size, x_for_search, degree_of_polynomial, &result);
                        if (rc == -10)
                        {
                            printf("Searching argument is already in initial data.\n"); //: (%.3lf, %.3lf)\n")
                        }
                        else if (rc == -20)
                        {
                            printf("Extrapolation occured. Calculation stopped.\n");
                        }
                        else if (rc != OK)
                        {
                            printf("There is not enough data for making polynomial of needed degree!\n");
                        }
                        else
                        {
                            printf("result = %.3lf\n", result);
                        }
                        free_matrix(mtr_selected);
                    }
                    else
                        rc = MEMORY_ERROR;
                    // add find root

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
    return rc;
}



