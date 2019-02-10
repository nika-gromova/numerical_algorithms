#ifndef IO_H
#define IO_H

double **allocate(int n);
void free_matrix(double **mtr, int n);
int read_file(FILE *f, double ***mtr, int *n);
void print_dots(double **mtr, int n);

#endif // IO_H
