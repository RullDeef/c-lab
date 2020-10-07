#include "mat_io.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define TEMP_STR_LENGTH 256

enum
{
    mat_io_success = 0,
    mat_io_failed,
    mat_io_invalid_input_file,
};

int mat_io_input_simple(FILE *file, matrix_t *matrix)
{
    char temp[TEMP_STR_LENGTH];
    int status_code = mat_io_success;

    if (fgets(temp, TEMP_STR_LENGTH, file) == NULL)
        status_code = mat_io_failed;
    else
    {
        size_t rows;
        size_t cols;

        if (sscanf(temp, "%lu %lu ", &rows, &cols) != 2 || rows == 0 || cols == 0)
            status_code = mat_io_invalid_input_file;
        else
        {
            *matrix = mat_create(rows, cols);

            for (size_t row = 0; row < rows && status_code == mat_io_success; row++)
            {
                if (fgets(temp, TEMP_STR_LENGTH, file) == NULL)
                    status_code = mat_io_failed;
                else
                {
                    char *begin_ptr = temp;
                    char *end_ptr = NULL;

                    for (size_t col = 0; col < cols && strlen(begin_ptr) > 0 && status_code == mat_io_success; col++)
                    {
                        matrix_elem_t value = strtol(begin_ptr, &end_ptr, 10);

                        if ((value == 0 && errno == EINVAL) || value == LONG_MAX || value == LONG_MIN)
                            status_code = mat_io_invalid_input_file;
                        else
                            mat_set(matrix, row, col, value);
                    }
                }
            }
        }
    }

    if (status_code != mat_io_success)
        mat_free(matrix);

    return status_code;
}

static size_t imp__get_nonzero_amount(const matrix_t *matrix)
{
    size_t amount = 0;

    for (size_t row = 0; row < matrix->rows; row++)
        for (size_t col = 0; col < matrix->cols; col++)
            if (mat_get(matrix, row, col) != 0)
                amount++;

    return amount;
}

int mat_io_output_coordinate(FILE *file, const matrix_t *matrix)
{
    int status_code = mat_io_success;

    size_t nonzero_amount = imp__get_nonzero_amount(matrix);
    fprintf(file, "%lu %lu %lu\n", matrix->rows, matrix->cols, nonzero_amount);

    for (size_t row = 0; row < matrix->rows; row++)
    {
        for (size_t col = 0; col < matrix->cols; col++)
        {
            matrix_elem_t value = mat_get(matrix, row, col);
            if (value != 0)
                fprintf(file, "%lu %lu %ld\n", row + 1, col + 1, value);
        }
    }

    return status_code;
}
