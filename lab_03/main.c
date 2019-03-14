#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "allocate.h"
#include "process.h"

void info(void)
{
    printf("Please input in command line:\n");
    printf("app.exe input.txt\n");
    printf("\n'input.txt' - is a file, where the initial information about dots is stored.\n");
}

int read_file(FILE *f, data_d *data)
{
    int rc = OK;
    double **buf = NULL;
    double *x = NULL;
    double *y = NULL;
    double tmp_x;
    double tmp_y;
    double tmp_z;
    int n, m;

    if (fscanf(f, "%d %d", &n, &m) == 2 && n > 0 && m > 0)
    {
        buf = allocate(n, m);
        x = calloc(n, sizeof(double));
        y = calloc(m, sizeof(double));
        if (buf && x && y)
        {
            for (int i = 0; i < n && rc == OK; i++)
            {
                for (int j = 0; j < m && rc == OK; j++)
                {
                    if (fscanf(f, "%lf %lf %lf", &tmp_x, &tmp_y, &tmp_z) == 3)
                    {
                        if (i == 0)
                            y[j] = tmp_y;
                        x[i] = tmp_x;
                        buf[i][j] = tmp_z;
                    }
                    else
                        rc = INPUT_ERROR;
                }
            }
            if (rc == OK)
            {
                data->data_x = x;
                data->data_y = y;
                data->data_z = buf;
                data->x_count = n;
                data->y_count = m;
            }
            else
            {
                free(x);
                free(y);
                free_matrix(buf, n);
            }
        }
        else
            rc = MEMORY_ERROR;
    }
    else
        rc = INPUT_ERROR;
    return rc;
}

void print_data(data_d data)
{
    printf("%9s ", "x\\y| ");
    for (int i = 0; i < data.y_count; i++)
        printf("%7.3lf ", (data.data_y)[i]);
    printf("\n");
    for (int i = 0; i < data.y_count * 3; i++)
        printf(" - ");
    printf("\n");
    for (int i = 0; i < data.x_count; i++)
    {
        for (int j = 0; j < data.y_count; j++)
        {
            if (j == 0)
                printf("%7.3lf| ", (data.data_x)[i]);
            printf("%7.3lf ", (data.data_z)[i][j]);
        }
        printf("\n");
    }
}

double function(double x, double y)
{
    return x * x + y * y;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        info();
        return PARAMS_ERROR;
    }
    int rc = OK;
    data_d input_data;
    input_data.data_x = NULL;
    input_data.data_y = NULL;
    input_data.data_z = NULL;
    input_data.x_count = 0;
    input_data.y_count = 0;
    data_d selected;
    selected.data_x = NULL;
    selected.data_y = NULL;
    selected.data_z = NULL;
    int nx;
    int ny;
    double x_inp;
    double y_inp;
    double result = 0;

    FILE *input_file = fopen(argv[1], "r");
    if (input_file)
    {
        rc = read_file(input_file, &input_data);
        if (rc == OK)
        {
            print_data(input_data);
            printf("input nx ny: ");
            if (scanf("%d %d", &nx, &ny) == 2 && nx >= 0 && ny >= 0)
            {
                if (nx < input_data.x_count && ny < input_data.y_count)
                {
                    printf("input x y: ");
                    if (scanf("%lf %lf", &x_inp, &y_inp) == 2)
                    {
                        selected.x_count = nx + 1;
                        selected.y_count = ny + 1;
                        selected.data_x = calloc(selected.x_count, sizeof(double));
                        selected.data_y = calloc(selected.y_count, sizeof(double));
                        selected.data_z = allocate(selected.x_count, selected.y_count);
                        if (selected.data_x && selected.data_y && selected.data_z && rc == OK)
                        {
                            rc = calculate(&input_data, &selected, x_inp, y_inp, &result);
                            switch (rc)
                            {
                            case OK:
                                printf("result:\ninterpolation: z = %.6lf\nreal function: z = %.6lf\n", result, function(x_inp, y_inp));
                                break;
                            case MEMORY_ERROR:
                                printf("memory errors occured, please try again!\n");
                                break;
                            case EXTRAPOLATION:
                                printf("extrapolation occured, calculations stopped!\n");
                                break;
                            case ZERO_DEVISION:
                                printf("this method isn't suitable for such calculations!\n");
                                break;
                            default:
                                break;
                            }
                            free_matrix(selected.data_z, nx + 1);
                            free(selected.data_x);
                            free(selected.data_y);
                        }
                    }
                    else
                        rc = INPUT_ERROR;
                }
                else
                {
                    printf("there is no enough data for calculations!\n");
                    rc = NOT_ENOUGH_DATA;
                }
            }
            else
                rc = INPUT_ERROR;
            free_matrix(input_data.data_z, input_data.x_count);
            free(input_data.data_x);
            free(input_data.data_y);
        }
        else
            printf("error (%d)\n", rc);
        fclose(input_file);
    }
    else
    {
        printf("error in openning file, please try again\n");
        rc = OPEN_FILE_ERROR;
    }
    if (rc == INPUT_ERROR)
        printf("input error :(\n");
    return rc;
}
