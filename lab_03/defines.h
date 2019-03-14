#ifndef DEFINES_H
#define DEFINES_H

#define OK               0
#define PARAMS_ERROR    -1
#define INPUT_ERROR     -2
#define MEMORY_ERROR    -3
#define OPEN_FILE_ERROR -4
#define NOT_ENOUGH_DATA -5
#define ZERO_DEVISION   -6


#define EXTRAPOLATION  -10
#define FOUND          -20


#include <math.h>
#define EPS 1e-10
#define IS_EQUAL(x, y) (fabs((x) - (y)) <= EPS)


typedef struct data_s
{
    double **data_z;
    double *data_x;
    double *data_y;
    int x_count;
    int y_count;
} data_d;

#endif // DEFINES_H
