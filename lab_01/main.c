#include <stdio.h>

#include "defines.h"
#include "io.h"

void info(void)
{
    printf("Please input in command line:\n");
    printf("app.exe input.txt\n");
    printf("\n'input.txt' - is a file, where the initial information about dots is stored.\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        info();
        return PARAMS_ERROR;
    }
    int rc = OK;
    double **mtr = NULL;
    int size = 0;
    FILE *f = fopen(argv[1], "r");
    if (f)
    {
        rc = read_file(f, &mtr, &size);
        if (rc == OK)
        {
            print_dots(mtr, size);
            free_matrix(mtr, size);
        }
        else
        {
            printf("Error: %d\n", rc);
        }
        fclose(f);
    }
    else
    {
        printf("Error in opening file. Please try again\n");
        return OPEN_FILE_ERROR;
    }
    return OK;
}
