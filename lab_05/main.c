#include <stdio.h>
#include <stdlib.h>
#include "math_module.h"
#define OK 0
#define INPUT_ERROR -1
#define MEMORY_ERROR -2

int read_data(input_data_t *inp_data)
{
    int rc = INPUT_ERROR;
    printf("Input T0 Tw m: ");
    if (scanf("%lf %lf %d", &(inp_data->T0), &(inp_data->Tw), &(inp_data->m)) == 3)
       rc = OK;
    return rc;
}



int main(void)
{
    input_data_t inp_data;
    double *nt_array = NULL;
    double result_p = 0.0;
    int rc = read_data(&inp_data);
    if (rc == OK)
    {
        nt_array = calloc(RANGE + 1, sizeof(double));
        if (nt_array)
        {
            result_p = calculate_p(nt_array, inp_data);
            rc = OK;
        }
        else
            rc = MEMORY_ERROR;
    }
    switch (rc) {
    case OK:
        //print_array(nt_array, RANGE + 1);
        printf("result p = %lf\n", result_p);
        free(nt_array);
        break;
    case INPUT_ERROR:
        printf("Error in input. Try again)\n");
        break;
    case MEMORY_ERROR:
        printf("Some memory error occured.\n");
        break;
    default:
        break;
    }
    return 0;
}
