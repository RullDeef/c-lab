#include "mat_io.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>

#define TEMP_STR_LENGTH 256

enum
{
    mat_io_success = 0,
    mat_io_failed,
    mat_io_invalid_input_file,
    mat_io_bad_dims,
    mat_io_garbage_in_file,
    mat_io_bad_precision
};

/**
 * @brief Проверяет файл на наличие непробельных символов.
 */
static bool imp__has_garbage_next(FILE *file)
{
    char temp;

    do
    {
        temp = fgetc(file);
    } while (temp != EOF && isspace(temp));

    return temp != EOF; // expect EOF if no garbage
}

/**
 * @brief Проверяет строку на наличие непробельных символов.
 */
static int imp__check_row_ending(char *str)
{
    int status_code = mat_io_success;

    while (*str != '\0' && isspace((int)*str))
        str++;
    
    if (*str != '\0')
        status_code = mat_io_invalid_input_file;

    return status_code;
}

/**
 * @brief Считывает строку матрицы из символьной строки
 */
static int imp__read_matrix_row(char *str, size_t row, matrix_t *matrix)
{
    int status_code = mat_io_success;

    for (size_t col = 0; col < matrix->cols && strlen(str) > 0 && status_code == mat_io_success; col++)
    {
        matrix_elem_t value = strtod(str, &str);

        if (errno == ERANGE)
            status_code = mat_io_invalid_input_file;
        else
            mat_set(matrix, row, col, value);
    }

    if (status_code == mat_io_success)
        status_code = imp__check_row_ending(str);

    return status_code;
}

/**
 * @brief Считывает матрицу из проверенного файла.
 */
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
    if (status_code == mat_io_success && imp__has_garbage_next(file))
        status_code = mat_io_garbage_in_file;

    return status_code;
}

/**
 * @brief Проверяет, пуст ли файл.
 */
static bool imp__file_is_empty(FILE* file)
{
    fseek(file, 0, SEEK_END);
    bool empty = ftell(file) == 0;
    fseek(file, 0, SEEK_SET);

    return empty;
}

/**
 * @brief Считывает размеры матрицы из строки.
 */
static int imp__scan_mat_dims(char *str, size_t *rows, size_t *cols)
{
    int status_code = mat_io_success;

    *rows = strtoul(str, &str, 10);
    if (*rows == 0 || errno == ERANGE)
        status_code = mat_io_bad_dims;
    else
    {
        *cols = strtoul(str, &str, 10);
        if (*cols == 0 || errno == ERANGE)
            status_code = mat_io_bad_dims;
        else
        {
            while (*str != '\0' && isspace((int)*str))
                str++;
            
            if (*str != '\0')
                status_code = mat_io_garbage_in_file;
        }
    }

    return status_code;
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

        status_code = imp__scan_mat_dims(temp, &rows, &cols);
        if (status_code == mat_io_success)
            status_code = mat_resize(matrix, rows, cols);
        if (status_code == mat_io_success)
            status_code = imp__safe_read_mat(file, matrix);
    }

    if (status_code != mat_io_success)
        mat_free(matrix);

    return status_code;
}

/**
 * @brief Возвращает число ненулевых (с точностью 1.0е-6) элементов матрицы
 */
static size_t imp__get_nonzero_amount(const matrix_t *matrix)
{
    size_t amount = 0;

    for (size_t row = 0; row < matrix->rows; row++)
        for (size_t col = 0; col < matrix->cols; col++)
            if (fabs(mat_get(matrix, row, col)) >= 1.0e-6)
                amount++;

    return amount;
}

int mat_io_output_coordinate(FILE *file, const matrix_t *matrix, int precision)
{
    int status_code = mat_io_success;

    if (mat_is_null(matrix))
        status_code = mat_io_failed;
    else if (precision < 0)
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
                if (fabs(value) >= 1.0e-6)
                    fprintf(file, "%lu %lu %.*lf\n", row + 1, col + 1, precision, value);
            }
        }
    }

    return status_code;
}
