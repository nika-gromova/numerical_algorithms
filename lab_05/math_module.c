#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "interpolation.h"
#include "math_module.h"

#define CONST_SIZE 5

#define TABLE_VALUE_SIZE 13
#define DEGREE 2

static double Q_table[][TABLE_VALUE_SIZE] = { { 2000, 4000, 6000, 8000, 10000, 12000, 14000, 16000, 18000, 20000, 22000, 24000, 26000 },\
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
static double v_initial = -1.0;
static double x_initial[] = { 2.0, -1.0, -10.0, -25.0, -35.0 };
static double alpha_initial = 0.0;
static double gamma_initial = 0.0;
static double alpha_coef = 0.285 * 10e-11;


// ############################################

double dichotomy(double a, double b, function_param_t params, double (*function)(double, function_param_t))
{
    double f_a = function(a, params);
    double f_b = function(b, params);
    double c = (a + b) * 0.5;
    double f_c = function(c, params);
    while (fabs(f_c) > EPS)
    {
        if (f_a * f_c < 0.0)
            b = c;
        else if (f_b * f_c < 0.0)
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


double calculate_gamma(double gamma, function_param_t params)
{
    double *v_x = params.array;
    double T = params.param;

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

void calculate_Q(double *Q, double T)
{
    for (int i = 0; i < CONST_SIZE; i++)
        Q[i] = interpolate(Q_table[0], Q_table[i + 1], TABLE_VALUE_SIZE, DEGREE, T);
}

void calculate_delta_E(double *delta_E, double T, double gamma)
{
    double coef = 8.61 * 10e-5 * T;
    double tmp;
    gamma *= 0.5;
    for (int i = 0; i < 4; i++)
    {
        tmp = Z_table[i + 1] * Z_table[i + 1] * gamma;
        delta_E[i] = coef * log(((1 + tmp) * (1 + gamma)) / (1 + tmp));
    }
}

void calculate_K(double *K_table, double T, double gamma)
{
    double cur_Q[CONST_SIZE] = { 0 };
    double cur_delta_E[4] = { 0 };
    double power = 11603.0 / T;
    double coef = 4.83 * 10e-3;
    double T_32 = pow(T, 1.5);
    calculate_Q(cur_Q, T);
    calculate_delta_E(cur_delta_E, T, gamma);
    for (int i = 0; i < 4; i++)
        K_table[i] = coef * (cur_Q[i + 1] / cur_Q[i]) * T_32 * exp((-1) * (E_table[i] - cur_delta_E[i]) * power);
}

void gauss(double *result, double mtr[][6], double *r_vector, int n)
{
    int i = 0;
    while ( i < n)
    {
        double max_elem = 0.0;
        int max_index = 0;
        for (int k = i; k < n; k++)
        {
            if (fabs(mtr[k][i]) >= max_elem)
            {
                max_elem = mtr[k][i];
                max_index = k;
            }
        }
        if (max_index != i)
        {
            double tmp;
            for (int k = 0; k < n; k++)
            {
                tmp = mtr[i][k];
                mtr[i][k] = mtr[max_index][k];
                mtr[max_index][k] = tmp;
            }
            tmp = r_vector[i];
            r_vector[i] = r_vector[max_index];
            r_vector[max_index] = tmp;
       }
        double tmp = mtr[i][i];
        for (int k = i + 1; k < n; k++)
        {
            double coef = mtr[k][i] / tmp;
            for (int h = i; h < n; h++)
                mtr[k][h] -= coef * mtr[i][h];
            r_vector[k] -= coef * r_vector[i];
        }
        i++;
    }
    for (int i = n - 1; i > -1; i--)
    {
        for (int j = n - 1; j > i; j--)
            r_vector[i] -= result[j] * mtr[i][j];
        result[i] = r_vector[i] / mtr[i][i];
    }
}

void fill_A(double mtr[][6], double *x, double v)
{
    mtr[4][0] = exp(v);
    for (int i = 2; i < CONST_SIZE + 1; i++)
        mtr[4][i] = -Z_table[i - 1] * exp(x[i - 1]);
    mtr[5][0] = -exp(v);
    for (int i = 1; i < CONST_SIZE + 1; i++)
        mtr[5][i] = -exp(x[i - 1]);
}

void fill_r(double *r_vector, double *x, double *k, double v, double coef, double alpha)
{
    for (int i = 0; i < 4; i++)
        r_vector[i] = (-1) * (v + x[i + 1] - x[i] - log(k[i]));
    double tmp = 0;
    for (int i = 1; i < CONST_SIZE; i++)
        tmp += Z_table[i] * exp(x[i]);
    r_vector[4] = -exp(v) + tmp;
    r_vector[5] = coef + exp(v) + tmp + Z_table[0] * exp(x[0]) - alpha;
}

char xi_bigger_eps(double *dx, double *x)
{
    char ret = 1;
    for (int i = 0; i < CONST_SIZE; i++)
        if (fabs(dx[i] / x[i]) >= EPS)
            ret = 0;
    return ret;
}

double calculate_system(double T, double P)
{
    double A_mtr[][6] = { { 1, -1, 1, 0, 0, 0 },\
                          { 1, 0, -1, 1, 0, 0 },\
                          { 1, 0, 0, -1, 1, 0 },\
                          { 1, 0, 0, 0, -1, 1 },\
                          { exp(v_initial), 0 },\
                          {-exp(v_initial) }
                        };
    fill_A(A_mtr, x_initial, v_initial);

    double K_table[4] = { 0 };
    calculate_K(K_table, T, gamma_initial);

    double right_vector[6] = { 0 };
    double coef = -K * P / T;
    fill_r(right_vector, x_initial, K_table, v_initial, coef, alpha_initial);

    double v = v_initial;
    double x[5] = { 0 };
    for (int i = 0 ; i < 5; i++)
        x[i] = x_initial[i];

    double delta[6] = { 0 };
    double gamma = 0;
    double alpha = 0;

    function_param_t gamma_params = {x, T};

    do
    {
        gauss(delta, A_mtr, right_vector, 6);
        v += delta[0];
        for (int i = 0; i < 5; i++)
            x[i] += delta[i + 1];
        gamma = dichotomy(0.0, 3.0, gamma_params, calculate_gamma);
        alpha = alpha_coef * pow(gamma * T, 3);
        calculate_K(K_table, T, gamma);
        fill_A(A_mtr, x, v);
        fill_r(right_vector, x, K_table, v, coef, alpha);
    }
    while (fabs(delta[0] / v) >= EPS && xi_bigger_eps(delta + 1, x));

    double result = 0;
    for (int i = 0; i < 5; i++)
        result += exp(delta[i + 1]);
    return result;
}

//#######################################################

double find_T(input_data_t data, double z)
{
    return (data.T0 + (data.Tw - data.T0) * pow(z, data.m));
}

double find_nt(double p, double t)
{
    return calculate_system(t, p);
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
    double coef = K * (P_initial / T_initial);
    double a = P1;
    double b = P2;
    double c = (double)(a + b) / 2.0;
    double f_a = f(nt_array, coef, inp_data, a);
    double f_b = f(nt_array, coef, inp_data, b);
    double f_c = f(nt_array, coef, inp_data, c);
    while (fabs(f_c) > EPS)// && ind < 10)
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
