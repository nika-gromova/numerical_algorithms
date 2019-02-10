#ifndef IO_H
#define IO_H

double **allocate(int n);
void free_matrix(double **mtr);
int read_file(FILE *f, double ***mtr, int *n);
void print_dots(double **mtr, int n);
void sort_inc(double **mtr, int size);

#endif // IO_H
