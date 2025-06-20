#include "edges.h"

return_codes_t edges_alloc(edge_t *&data, const int size)
{
    if (size <= 0)
        return ERROR_EDGES_SIZE;

    data = (edge_t *) malloc(size * sizeof(edge_t));

    return_codes_t rc = SUCCESS;

    if (!data)
        rc = ERROR_MEM_ALLOC;

    return rc;
}

void edges_free(edge_t *&data)
{
    free(data);
}

edges_t edges_set_default()
{
    return edges_t{NULL, 0};
}

static return_codes_t edges_count_fread(int &count, FILE *in)
{
    if (in == NULL)
        return ERROR_FILE_OPEN;

    if (fscanf(in, "%d", &count) != 1)
        return ERROR_FILE_READ;

    if (count <= 0)
        return ERROR_EDGES_SIZE;

    return SUCCESS;
}

static return_codes_t edges_data_fread(edge_t *data, const int size, FILE *in)
{
    if (in == NULL)
        return ERROR_FILE_OPEN;

    if (size <= 0)
        return ERROR_EDGES_SIZE;

    if (!data)
        return ERROR_MEM_ALLOC;

    return_codes_t rc = SUCCESS;
    for (int i = 0; rc == SUCCESS && i < size; i++)
        rc = edge_fread(data[i], in);

    return rc;
}

return_codes_t edges_fread(edges_t &edges, FILE *in)
{
    if (in == NULL)
        return ERROR_FILE_OPEN;

    return_codes_t rc = edges_count_fread(edges.size, in);

    if (rc == SUCCESS)
    {
        rc = edges_alloc(edges.data, edges.size);

        if (rc == SUCCESS)
        {
            rc = edges_data_fread(edges.data, edges.size, in);

            if (rc != SUCCESS)
                edges_free(edges.data);
        }
    }

    return rc;
}

return_codes_t edges_fwrite(const edges_t &edges, FILE *out)
{
    if (out == NULL)
        return ERROR_FILE_OPEN;

    if (!edges.data)
        return ERROR_EMPTY_DATA;

    if (fprintf(out, "%d\n", edges.size) < 0)
        return ERROR_FILE_WRITE;

    return_codes_t rc = SUCCESS;

    for (int i = 0; rc == SUCCESS && i < edges.size; i++)
        rc = edge_fwrite(edges.data[i], out);

    return rc;
}
