#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "interpolation.h"
#include "math_module.h"

#define CONST_SIZE 5

#define TABLE_Q_SIZE 13
#define DEGREE 2

static double Q_table[][TABLE_Q_SIZE] = { { 2000, 4000, 6000, 8000, 10000, 12000, 14000, 16000, 18000, 20000, 22000, 24000, 26000 },\
                                { 1.0000, 1.0000, 1.0000, 1.0001, 1.0025, 1.0198, 1.0895, 1.2827, 1.6973, 2.4616, 3.6552, 5.3749, 7.6838 },\
                                { 4.0000, 4.0000, 4.1598, 4.3006, 4.4392, 4.5661, 4.6817, 4.7923, 4.9099, 5.0511, 5.2354, 5.4841, 5.8181 },\
                                { 5.5000, 5.5000, 5.5116, 5.9790, 6.4749, 6.9590, 7.4145, 7.8370, 8.2289, 8.5970, 8.9509, 9.3018, 9.6621 },\
                                { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },\
                                { 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0 }
                              };

static double E_table[] = { 12.13, 20.98, 31.00, 45.00 };
static double Z_table[] = { 0.0, 1.0, 2.0, 3.0, 4.0 };

static double P_initial = 0.5;
static double T_initial = 300;
static double v_initial = -1.0;
static double x_initial[] = { 2.0, -1.0, -10.0, -25.0, -35.0 };
static double alpha_initial = 0.0;
static double gamma_initial = 0.0;
static double alpha_coef = 0.285 * 10e-11;

double res_gamma;
double res_n[6];

void print_array(double *array, int n)
{
    for (int i = 0; i < n; i++)
        printf("%e ", array[i]);
    printf("\n");
}

double calculate_gamma(double gamma, double *v_x, double T)
{
    double sum = exp(v_x[0]) / (1 + 0.5 * gamma);
    double z2;
    for (int i = 1; i < CONST_SIZE; i++)
    {
        z2 = Z_table[i] * Z_table[i];
        sum += ((exp(v_x[i + 1]) * z2) / (1 + z2 * gamma * 0.5));
    }
    double T3 = T * T * T;
    double coef = 5.87 * 10e10 / T3;
    return gamma * gamma - coef * sum;
}


double dichotomy(double a, double b, double *v_x, double T, double (*function)(double, double *, double))
{
    double f_a = function(a, v_x, T);
    double f_b = function(b, v_x, T);
    double c = (a + b) * 0.5;
    double f_c = function(c, v_x, T);
    while (fabs(f_c) > EPS)
    {
        if (f_a * f_c < 0.0)
            b = c;
        else if (f_b * f_c < 0.0)
            a = c;
        else
            break;
        c = (a + b) * 0.5;
        f_a = function(a, v_x, T);
        f_b = function(b, v_x, T);
        f_c = function(c, v_x, T);
    }
    return c;
}

void calculate_Q(double *Q, double T)
{
    for (int i = 0; i < CONST_SIZE; i++)
    {
        double tmp = interpolate(Q_table[0], Q_table[i + 1], TABLE_Q_SIZE, DEGREE, T);
        Q[i] = tmp;
    }
}

void calculate_delta_E(double *delta_E, double T, double gamma)
{
    double coef = 8.61 * 10e-5 * T;
    double tmp_1, tmp_2;
    gamma *= 0.5;
    for (int i = 0; i < 4; i++)
    {
        tmp_1 = Z_table[i + 1] * Z_table[i + 1] * gamma;
        tmp_2 = Z_table[i] * Z_table[i] * gamma;
        delta_E[i] = coef * log(((1 + tmp_1) * (1 + gamma)) / (1 + tmp_2));
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

void gauss(double *result, double (*mtr)[6], double *r_vector, int n)
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
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (j == 0)
                mtr[i][j] = 1;
            else
                mtr[i][j] = 0;
        }
    }
    int j = 1;
    for (int i = 0; i < 4; i++)
    {
        mtr[i][j] = -1;
        mtr[i][j + 1] = 1;
        j++;
    }
    mtr[4][0] = exp(v);
    mtr[4][1] = 0.0;
    for (int i = 1; i < CONST_SIZE; i++)
        mtr[4][i + 1] = -Z_table[i] * exp(x[i]);
    mtr[5][0] = -exp(v);
    for (int i = 0; i < CONST_SIZE; i++)
        mtr[5][i + 1] = -exp(x[i]);
}

void fill_r(double *r_vector, double *x, double *k, double v, double coef, double alpha)
{
    for (int i = 0; i < 4; i++)
        r_vector[i] = -(v + x[i + 1] - x[i] - log(k[i]));
    double tmp = 0;
    for (int i = 1; i < CONST_SIZE; i++)
        tmp += Z_table[i] * exp(x[i]);
    r_vector[4] = -exp(v) + tmp;
    tmp = 0;
    for (int i = 0; i < CONST_SIZE; i++)
        tmp += exp(x[i]);
    r_vector[5] = coef + exp(v) + tmp - alpha;
}

char xi_bigger_eps(double *dx, double *x)
{
    char ret = 1;
    for (int i = 0; i < CONST_SIZE; i++)
        if (fabs(dx[i] / x[i]) >= EPS)
            ret = 0;
    return ret;
}

void print_mtr(double mtr[][6])
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
            printf("%e ", mtr[i][j]);
        printf("\n");
    }
}

double calculate_system(double T, double P)
{
    double coef = -K * P / T;
    double A_mtr[6][6];
    double K_table[4];
    double right_vector[6];

    double v_x[6];
    v_x[0] = v_initial;
    for (int i = 1 ; i < 6; i++)
        v_x[i] = x_initial[i - 1];

    double delta[6] = { 0 };
    double gamma = 0;
    double alpha = 0;

    fill_A(A_mtr, x_initial, v_initial);
    calculate_K(K_table, T, gamma_initial);
    fill_r(right_vector, x_initial, K_table, v_initial, coef, alpha_initial);

    do
    {
        gauss(delta, A_mtr, right_vector, 6);
        for (int i = 0; i < 6; i++)
            v_x[i] += delta[i];
        gamma = dichotomy(0.0, 3.0, v_x, T, calculate_gamma);
        alpha = alpha_coef * pow(gamma * T, 3);
        calculate_K(K_table, T, gamma);
        fill_A(A_mtr, v_x + 1, v_x[0]);
        fill_r(right_vector, v_x + 1, K_table, v_x[0], coef, alpha);
/*
        printf("v_x:\n");
        print_array(v_x, 6);
        printf("delta_v_x:\n");
        print_array(delta, 6);
        printf("gamma = %lf\n", gamma);
        printf("A:\n");
        print_mtr(A_mtr);
        printf("r_vector = ");
        print_array(right_vector, 6);
*/
    }
    while (fabs(delta[0] / v_x[0]) >= EPS && xi_bigger_eps(delta + 1, v_x + 1));

    double result = 0;
    double tmp;
    res_n[0] = exp(v_x[0]);
    for (int i = 1; i < 6; i++)
    {
        tmp = exp(v_x[i]);
        result += tmp;
        res_n[i] = tmp;
    }
    res_gamma = gamma;
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
    while (fabs(f_c) > EPS)
    {
        if (f_a * f_c < 0.0)
            b = c;
        else if (f_b * f_c < 0.0)
            a = c;
        else
        {
            printf("ERRORRRRR\n");
            break; // корня нет
        }
        c = (a + b) / 2.0;
        f_a = f(nt_array, coef, inp_data, a);
        f_b = f(nt_array, coef, inp_data, b);
        f_c = f(nt_array, coef, inp_data, c);
    }
    printf("gamma = %e\nn:\n", res_gamma);
    print_array(res_n, 6);

    return c;
}
