#include "mat_io.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>

#define TEMP_STR_LENGTH 256

enum
{
    mat_io_success = 0,
    mat_io_failed,
    mat_io_invalid_input_file,
    mat_io_garbage_in_file,
    mat_io_bad_precision
};

static bool imp__has_garbage_next(FILE *file)
{
    char temp;

    do
    {
        temp = fgetc(file);
    } while (temp != EOF && isspace(temp));

    // fprintf(stderr, "temp char is '%c'\n", temp == EOF ? '.' : temp);

    return temp != EOF; // expect EOF if no garbage
}

static int imp__read_matrix_row(const char *str, size_t row, matrix_t *matrix)
{
    int status_code = mat_io_success;

    const char *begin_ptr = str;
    char *end_ptr = NULL;

    for (size_t col = 0; col < matrix->cols && strlen(begin_ptr) > 0 && status_code == mat_io_success; col++)
    {
        matrix_elem_t value = strtod(begin_ptr, &end_ptr);
        begin_ptr = end_ptr + 1;

        if ((value == 0 && errno == EINVAL) || value == LONG_MAX || value == LONG_MIN)
            status_code = mat_io_invalid_input_file;
        else
            mat_set(matrix, row, col, value);
    }

    return status_code;
}

static int imp__safe_read_mat(FILE *file, matrix_t *matrix)
{
    int status_code = mat_io_success;

    for (size_t row = 0; row < matrix->rows && status_code == mat_io_success; row++)
    {
        char temp[TEMP_STR_LENGTH];
    
        if (fgets(temp, TEMP_STR_LENGTH, file) == NULL)
            status_code = mat_io_failed;
        else
            status_code = imp__read_matrix_row(temp, row, matrix);
    }

    // check for ongoing garbage in file
    if (imp__has_garbage_next(file))
    {
        // fprintf(stderr, "file has garbage.\n");
        status_code = mat_io_garbage_in_file;
    }

    return status_code;
}

static bool imp__file_is_empty(FILE* file)
{
    fseek(file, 0, SEEK_END);
    bool empty = ftell(file) == 0;
    fseek(file, 0, SEEK_SET);

    return empty;
}

int mat_io_input_simple(FILE *file, matrix_t *matrix)
{
    char temp[TEMP_STR_LENGTH];
    int status_code = mat_io_success;

    if (imp__file_is_empty(file) || fgets(temp, TEMP_STR_LENGTH, file) == NULL)
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
            status_code = imp__safe_read_mat(file, matrix);
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

int mat_io_output_simple(FILE *file, const matrix_t *matrix, int precision)
{
    int status_code = mat_io_success;

    if (precision < 0)
        status_code = mat_io_bad_precision;
    else
    {
        fprintf(file, "%lu %lu\n", matrix->rows, matrix->cols);

        for (size_t row = 0; row < matrix->rows; row++)
        {
            for (size_t col = 0; col < matrix->cols; col++)
            {
                matrix_elem_t value = mat_get(matrix, row, col);
                fprintf(file, "% .*lf", precision, value);
                if (col + 1 < matrix->cols)
                    fprintf(file, " ");
            }
            fprintf(file, "\n");
        }
    }

    return status_code;
}

int mat_io_output_coordinate(FILE *file, const matrix_t *matrix, int precision)
{
    int status_code = mat_io_success;

    if (precision < 0)
        status_code = mat_io_bad_precision;
    else
    {
        size_t nonzero_amount = imp__get_nonzero_amount(matrix);
        fprintf(file, "%lu %lu %lu\n", matrix->rows, matrix->cols, nonzero_amount);

        for (size_t row = 0; row < matrix->rows; row++)
        {
            for (size_t col = 0; col < matrix->cols; col++)
            {
                matrix_elem_t value = mat_get(matrix, row, col);
                if (value != 0)
                    fprintf(file, "%lu %lu %.*lf\n", row + 1, col + 1, precision, value);
            }
        }
    }

    return status_code;
}
