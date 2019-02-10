#include <defines.h>

int search_place(double **mtr, int size, double search)
{
    for (int i = 0; i < size; i++)
    {
        if (IS_EQUAL(mtr[0][i], search))
            return -10;
        // bynary search!
    }
}
