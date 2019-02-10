#ifndef PROCESS_H
#define PROCESS_H

int search_place(double **mtr, int size, double search);
int take_dots(double **mtr, int init_size, double **selected, int selected_size, int pos);
double find(double **mtr, int size, int degree, double x);
void copy_mtr(double **src, double **dst, int size);
void sort_inc(double **mtr, int size);
int calculate(double **mtr, int size, double **selected, int selected_size, double x_for_search, int degree_of_polynomial, double *result);

#endif // PROCESS_H
