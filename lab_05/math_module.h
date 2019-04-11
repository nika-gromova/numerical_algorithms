#ifndef MATH_MODULE_H
#define MATH_MODULE_H

#define K 7242
#define RANGE 40
#define EPS_DOUBLE 1e-50

#define P1 3
#define P2 25
#define EPS_DICHOTOMY 10e-4

typedef struct input_data
{
    double T0;
    double Tw;
    double P_initial;
    double T_initial;
    int m;
} input_data_t;

double calculate_p(double *nt_array, input_data_t inp_data);
#endif // MATH_MODULE_H
