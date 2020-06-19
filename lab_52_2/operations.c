#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <assert.h>
#include "product.h"
#include "operations.h"

#define MAX_PRODUCT_ARRAY_SIZE 100

status_code_t loc_read_products_array(FILE *file, product_t products_array[], size_t *size);
status_code_t loc_sort_products(FILE *file_in, FILE *file_out);
void loc_sort_products_array(product_t products_array[], size_t size);
status_code_t loc_write_products_array(FILE *file, product_t products_array[], size_t size);

bool ends_with(char *str, char *substr);
status_code_t loc_find_products(FILE *file_in, char *substr);

status_code_t sort_products(char *filename_in, char *filename_out)
{
    assert(filename_in != NULL && filename_out != NULL);
    assert(strlen(filename_in) != 0 && strlen(filename_out) != 0);

    FILE *file_in, *file_out;
    status_code_t status_code;

    file_in = fopen(filename_in, "r");
    if (file_in == NULL)
        status_code = cant_open_input_file;
    else
    {
        file_out = fopen(filename_out, "w");
        if (file_out == NULL)
            status_code = cant_open_output_file;
        else
        {
            status_code = loc_sort_products(file_in, file_out);
            fclose(file_out);
        }

        fclose(file_in);
    }

    return status_code;
}

status_code_t find_products(char *filename_in, char *substr)
{
    FILE *file_in;
    status_code_t status_code;

    file_in = fopen(filename_in, "r");
    if (file_in == NULL)
        status_code = cant_open_input_file;
    else
    {
        status_code = loc_find_products(file_in, substr);
        fclose(file_in);
    }

    return status_code;
}

status_code_t loc_read_products_array(FILE *file, product_t products_array[], size_t *size)
{
    status_code_t status_code = exit_success;
    *size = 0;

    while (true)
    {
        status_code = product_read(file, &products_array[*size]);
        if (status_code == end_of_file)
        {
            status_code = exit_success;
            break;
        }
        else if (status_code != exit_success)
            break;
        
        *size += 1;

        if (*size > MAX_PRODUCT_ARRAY_SIZE)
        {
            status_code = too_many_products;
            break;
        }
    }

    if (status_code == exit_success && *size == 0)
        status_code = empty_file;

    return status_code;
}

status_code_t loc_sort_products(FILE *file_in, FILE *file_out)
{
    assert(file_in != NULL && file_out != NULL);

    product_t products_array[MAX_PRODUCT_ARRAY_SIZE];
    size_t size;
    status_code_t status_code;

    status_code = loc_read_products_array(file_in, products_array, &size);
    if (status_code == exit_success)
    {
        loc_sort_products_array(products_array, size);
        status_code = loc_write_products_array(file_out, products_array, size);
    } 

    return status_code;
}

void loc_sort_products_array(product_t products_array[], size_t size)
{
    for (size_t j = 0; j < size; j++)
    {
        for (size_t i = 0; i + 1 < size; i++)
        {
            if (!product_ordered(&products_array[i], &products_array[i + 1]))
            {
                product_swap(&products_array[i], &products_array[i + 1]);
            }
        }
    }
}

status_code_t loc_write_products_array(FILE *file, product_t products_array[], size_t size)
{
    status_code_t status_code = exit_success;

    for (size_t i = 0; i < size; i++)
    {
        status_code = product_write(file, &products_array[i]);
        if (status_code != exit_success)
            break;
    }

    return status_code;
}

bool ends_with(char *str, char *substr)
{
    if (strcmp(str, substr) == 0)
        return true;
    else if (strlen(str) > strlen(substr))
    {
        int offset = strlen(str) - strlen(substr);
        if (strcmp(str + offset, substr) == 0)
            return true;
        return false;
    }
    
    return false;
}

status_code_t loc_find_products(FILE *file_in, char *substr)
{
    product_t products_array[MAX_PRODUCT_ARRAY_SIZE];
    size_t size;
    status_code_t status_code;

    status_code = loc_read_products_array(file_in, products_array, &size);
    if (status_code == exit_success)
    {
        for (size_t i = 0; i < size; i++)
        {
            if (ends_with(products_array[i].name, substr))
                product_write(stdout, &products_array[i]);
        }
    }

    return status_code;
}
