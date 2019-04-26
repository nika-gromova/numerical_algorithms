#ifndef MATH_MODULE_H
#define MATH_MODULE_H

#define K 7242
#define RANGE 3

#define P1 3
#define P2 25
#define EPS 10e-4

typedef struct input_data
{
    double T0;
    double Tw;
    int m;
} input_data_t;

typedef struct function_param
{
    double param;
    double array[6];
}function_param_t;

double calculate_p(double *nt_array, input_data_t inp_data);
void print_array(double *array, int n);
#endif // MATH_MODULE_H
