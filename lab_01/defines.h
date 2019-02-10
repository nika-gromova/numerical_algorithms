#ifndef DEFINES_H
#define DEFINES_H

#define OK               0
#define PARAMS_ERROR    -1
#define INPUT_ERROR     -2
#define MEMORY_ERROR    -3
#define OPEN_FILE_ERROR -4
#define NOT_ENOUGH_DATA -5

#include <math.h>
#define EPS 1e-10
#define IS_EQUAL(x, y) (fabs((x) - (y)) <= EPS)


#endif // DEFINES_H
