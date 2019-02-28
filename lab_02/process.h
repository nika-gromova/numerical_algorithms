#ifndef PROCESS_H
#define PROCESS_H

void sort_inc(double **mtr, int size);
int calculate_coefficients(double **mtr, double *a, double *b, double *c, double *d, int size);
int calculate(double *result, double **mtr, double *a, double *b, double *c, double *d, int size, double x);

#endif // PROCESS_H
