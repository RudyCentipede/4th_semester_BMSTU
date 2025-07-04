#include "points.h"

return_codes_t points_alloc(point_t *&data, const int size)
{
    if (size <= 0)
        return ERROR_POINTS_SIZE;

    data = (point_t *) malloc(size * sizeof(point_t));

    return_codes_t rc = SUCCESS;

    if (!data)
        rc = ERROR_MEM_ALLOC;

    return rc;
}

void points_free(point_t *&data)
{
    free(data);
}

points_t points_set_default()
{
    return points_t{NULL, 0};
}

return_codes_t points_move_all(points_t &points, const move_data_t &coeff)
{
    if (!points.data)
        return ERROR_FIGURE_NOT_LOADED;

    for (int i = 0; i < points.size; i++)
        point_move(points.data[i], coeff);

    return SUCCESS;
}

return_codes_t points_rotate_all(points_t &points, const point_t &rotate_center, const rotate_data_t &coeff)
{
    if (!points.data)
        return ERROR_FIGURE_NOT_LOADED;

    for (int i = 0; i < points.size; i++)
        point_rotate(points.data[i], rotate_center, coeff);

    return SUCCESS;
}

return_codes_t points_scale_all(points_t &points, const point_t &scale_center, const scale_data_t &coeff)
{
    if (!points.data)
        return ERROR_FIGURE_NOT_LOADED;

    for (int i = 0; i < points.size; i++)
        point_scale(points.data[i], scale_center, coeff);

    return SUCCESS;
}

static return_codes_t points_count_fread(int &count, FILE *in)
{
    if (in == NULL)
        return ERROR_FILE_OPEN;

    if (fscanf(in, "%d", &count) != 1)
        return ERROR_FILE_READ;

    if (count <= 0)
        return ERROR_POINTS_SIZE;

    return SUCCESS;
}

static return_codes_t points_data_fread(point_t *data, const int size, FILE *in)
{
    if (in == NULL)
        return ERROR_FILE_OPEN;

    if (size <= 0)
        return ERROR_POINTS_SIZE;

    if (!data)
        return ERROR_MEM_ALLOC;

    return_codes_t rc = SUCCESS;

    for (int i = 0; rc == SUCCESS && i < size; i++)
        rc = point_fread(data[i], in);

    return rc;
}

return_codes_t points_fread(points_t &points, FILE *in)
{
    if (in == NULL)
        return ERROR_FILE_OPEN;

    return_codes_t rc = points_count_fread(points.size, in);

    if (rc == SUCCESS)
    {
        rc = points_alloc(points.data, points.size);

        if (rc == SUCCESS)
        {
            rc = points_data_fread(points.data, points.size, in);

            if (rc != SUCCESS)
                points_free(points.data);
        }
    }

    return rc;
}

return_codes_t points_fwrite(const points_t &points, FILE *out)
{
    if (out == NULL)
        return ERROR_FILE_OPEN;

    if (!points.data)
        return ERROR_EMPTY_DATA;

    if (fprintf(out, "%d\n", points.size) < 0)
        return ERROR_FILE_WRITE;

    return_codes_t rc = SUCCESS;

    for (int i = 0; rc == SUCCESS && i < points.size; i++)
        rc = point_fwrite(points.data[i], out);

    return rc;
}
