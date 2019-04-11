#include <math.h>
#include <assert.h>
#include <stdio.h>
#include "math_module.h"

double find_T(input_data_t data, double z)
{
    return (data.T0 + (data.Tw - data.T0) * pow(z, data.m));
}

void fill_nt_array(double *nt_array, double p, double h, input_data_t data)
{
    double z = 0.0;
    double tmp_T;
    for (int i = 0; i <= RANGE; i++)
    {
        tmp_T = find_T(data, z);
        nt_array[i] = ((K * p) / tmp_T);
        z += h;
    }
}

double find_integral(double *nt_array, double p, input_data_t data)
{
    double step = 1.0 / (double)RANGE;
    fill_nt_array(nt_array, p, step, data);
    double result = 0.0;
    double z = step;
    for (int i = 1; i < RANGE; i++, z += step)
    {
        result += 4.0 * nt_array[i] * z;
        i += 1;
        z += step;
        if (i < RANGE)
            result += 2.0 * nt_array[i] * z;
    }
    result += nt_array[RANGE];
    result *= (step / 3.0);
    return result;
}

double f(double *nt_array, double coef, input_data_t inp_data, double cur_p)
{
    return coef - 2.0 * find_integral(nt_array, cur_p, inp_data);
}

double calculate_p(double *nt_array, input_data_t inp_data)
{
    double coef = K * (inp_data.P_initial / inp_data.T_initial);
    double a = P1;
    double b = P2;
    double c = (double)(a + b) / 2.0;
    double f_a = f(nt_array, coef, inp_data, a);
    double f_b = f(nt_array, coef, inp_data, b);
    double f_c = f(nt_array, coef, inp_data, c);
    while (fabs(f_c) > EPS_DICHOTOMY)// && ind < 10)
    {
        if (f_a * f_c < 0.0)
            b = c;
        else if (f_b * f_c < 0.0)
            a = c;
        else
            break; // корня нет
        c = (a + b) / 2.0;
        f_a = f(nt_array, coef, inp_data, a);
        f_b = f(nt_array, coef, inp_data, b);
        f_c = f(nt_array, coef, inp_data, c);
    }
    return c;
}



