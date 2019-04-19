#include <math.h>
#include <assert.h>
#include <stdio.h>
#include "math_module.h"

static double Q_table[][13] = { { 2000, 4000, 6000, 8000, 10000, 12000, 14000, 16000, 18000, 20000, 22000, 24000, 26000 },\
                                { 1.0000, 1.0000, 1.0000, 1.0001, 1.0025, 1.0198, 1.0895, 1.2827, 1.6973, 2.4616, 3.6552, 5.3749, 7.6838 },\
                                { 4.0000, 4.0000, 4.1598, 4.3006, 4.4392, 4.5661, 4.6817, 4.7923, 4.9099, 5.0511, 5.2354, 5.4841, 5.8181 },\
                                { 5.5000, 5.5000, 5.5116, 5.9790, 6.4749, 6.9590, 7.4145, 7.8370, 8.2289, 8.5970, 8.9509, 9.3018, 9.6621 },\
                                { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },\
                                { 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0 }
                              };
static double E_table[] = { 12.13, 20.98, 31.00, 45.00 };
static double Z_table[] = { 0.0, 1.0, 2.0, 3.0, 4.0 };

static double P_initial = 15.0;
static double T_initial = 10000.0;
static double v_intial = -1.0;
static double x_initial[] = { 2.0, -1.0, -10.0, -25.0, -35.0 };
static double alpha_initial = 0.0;
static double gamma_initial = 0.0;


double find_T(input_data_t data, double z)
{
    return (data.T0 + (data.Tw - data.T0) * pow(z, data.m));
}

double find_nt(double p, double t)
{
    return ((K * p) / t);
}

void fill_nt_array(double *nt_array, double p, double h, input_data_t data)
{
    double z = 0.0;
    double tmp_T;
    for (int i = 0; i <= RANGE; i++)
    {
        tmp_T = find_T(data, z);
        nt_array[i] = find_nt(p, tmp_T);
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

double dichotomy(double a, double b, double eps, function_param_t params, double (*function)(double, function_param_t))
{
    double f_a = function(a, params);
    double f_b = function(b, params);
    double c = (a + b) * 0.5;
    double f_c = function(c, params);
    while (fabs(f_c) > eps)
    {
        if (f_a * f_c < 0.0)
            b = c;
        else if (f_b * f_c)
            a = c;
        else
            break;
        c = (a + b) * 0.5;
        f_a = function(a, params);
        f_b = function(b, params);
        f_c = function(c, params);
    }
    return c;
}


double calculate_gamma(double gamma, double *v_x, double T)
{
    double sum = exp(v_x[0]) / (1 + 0.5 * gamma);
    double z2;
    for (int i = 2; i < 5; i++)
    {
        z2 = Z_table[i - 1] * Z_table[i - 1];
        sum += ((exp(v_x[i]) * z2) / (1 + z2 * gamma * 0.5));
    }
    double T3 = T * T *T;
    double coef = 5.87 * 10e10 / T3;
    return gamma * gamma - coef * sum;
}

double calculate_Q(double T)
{

}
