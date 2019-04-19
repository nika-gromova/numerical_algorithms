#include "interpolation.h"
int search_place(double *row, int len, double search, int *left, int *right)
{
    int mid;
    *left = 0, *right = len;
    while (*left <= *right)
    {
        mid = (*left + *right) / 2;
        if (IS_EQUAL(row[mid], search))
            return mid; // искомый элемент уже есть в массиве узлов
        if (row[mid] > search)
            *right = mid - 1;
        else
            *left = mid + 1;
    }
    if (*left > *right)
    {
        int tmp = *left;
        *left = *right;
        *right = tmp;
    }
    if (*right >= len || *left < 0)
        return EXTRAPOLATION; // экстраполяция

    return FOUND; // найдены индексы ближайших элементов
}

int get_indexes(int *left, int *right, int init_size, int selected_size)
{
    int index = 0;
    // один большой костыль )0)))0)
    while ((index < selected_size) && (*left >= 0 || *right < init_size))
    {
        if (*right < init_size)
        {
            index++;
            if (index < selected_size)
                (*right)++;
        }
        if (*left >= 0 && index < selected_size)
        {
            index++;
            if (((*right < init_size && index < selected_size - 1) || (*right >= init_size && index < selected_size)) && (*left) != 0)
                (*left)--;
        }
    }
    if (*right - *left != selected_size)
        (*right)++;
    if (index != selected_size)
        return NOT_ENOUGH_DATA;
    return OK;
}

void place_by_indexes(data_d *d_init, data_d *d_select, int pos_l_y, int pos_r_y, int pos_l_x, int pos_r_x)
{
    int index_i = 0;
    int index_j = 0;
    for (int i = pos_l_x; i < pos_r_x && index_i < d_select->x_count; i++, index_i++)
        d_select->data_x[index_i] = d_init->data_x[i];
    for (int i = pos_l_y; i < pos_r_y && index_j < d_select->y_count; i++, index_j++)
        d_select->data_y[index_j] = d_init->data_y[i];
    index_i = 0;
    index_j = 0;
    for (int i = pos_l_x; i < pos_r_x && index_i < d_select->x_count; i++, index_i++)
    {
        index_j = 0;
        for (int j = pos_l_y; j < pos_r_y && index_j < d_select->y_count; j++, index_j++)
        {
            d_select->data_z[index_i][index_j] = d_init->data_z[i][j];
        }
    }
}

int take_dots(data_d *d_init, data_d *d_select, int pos_l_y, int pos_r_y, int pos_l_x, int pos_r_x)
{
    int rc = OK;
    rc = get_indexes(&pos_l_x, &pos_r_x, d_init->x_count, d_select->x_count);
    if (rc == OK)
    {
        rc = get_indexes(&pos_l_y, &pos_r_y, d_init->y_count, d_select->y_count);
        if (rc == OK)
            place_by_indexes(d_init, d_select, pos_l_y, pos_r_y, pos_l_x, pos_r_x);
    }
    return rc;
}

double find(double *argument, double *value, int size, int degree, double search, int *rc)
{
    *rc = OK;
    double div;
    double res = 0.0;
    double *coefficients = calloc(degree + 1, sizeof(double));
    if (coefficients)
    {
        coefficients[0] = value[0];
        int n = degree;
        for (int i = 1; i < degree + 1 && *rc == OK; i++)
        {
            for (int j = 0; j < n && *rc == OK; j++)
            {
                div = argument[0] - argument[degree + 1 - n];
                if (fabs(div) <= EPS)
                    *rc = ZERO_DEVISION;
                else
                    value[j] = (value[j] - value[j + 1]) / (div);
            }
            coefficients[i] = value[0];
            n--;
        }
        if (*rc == OK)
        {
            res = coefficients[0];
            int c = 0;
            double mult = 1.0;
            for (int i = 1; i < degree + 1 && c < size; i++)
            {
                mult *= (search - argument[c]);
                res += mult * coefficients[i];
                c++;
            }
        }
        free(coefficients);
    }
    else
        *rc = MEMORY_ERROR;
    return res;
}
