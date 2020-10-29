#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "matrix.h"

/**
 * @brief Рассчитывает необходимый объем памяти
 * для хранения матрицы заданного размера.
 */
inline static size_t imp__calc_required_mat_size(size_t rows, size_t cols)
{
    return rows * (sizeof(matrix_elem_t *) + cols * sizeof(matrix_elem_t));
}

inline static void imp__reassign_row_ptrs(matrix_t matrix)
{
    for (size_t row = 0; row < matrix.rows; row++)
        matrix.data[row] = (matrix_elem_t *)(matrix.data + matrix.rows) + matrix.cols * row;
}

matrix_t mat_null(void)
{
    matrix_t null_mat = {
        .rows = 0,
        .cols = 0,
        .imp__allocated = 0,
        .data = NULL
    };

    return null_mat;
}

matrix_t mat_create(size_t rows, size_t cols)
{
    matrix_t matrix = mat_null();

    if (rows != 0 && cols != 0)
    {
        matrix.imp__allocated = imp__calc_required_mat_size(rows, cols);
        matrix.data = (matrix_elem_t **)calloc(1, matrix.imp__allocated);

        if (matrix.data == NULL)
            matrix.imp__allocated = 0U;
        else
        {
            matrix.rows = rows;
            matrix.cols = cols;
            imp__reassign_row_ptrs(matrix);
        }
    }

    return matrix;
}

inline bool mat_is_null(const matrix_t *matrix)
{
    return matrix->data == NULL;
}

void mat_free(matrix_t *matrix)
{
    if (!mat_is_null(matrix))
    {
        free(matrix->data);
        *matrix = mat_null();
    }
}

matrix_t mat_copy(const matrix_t *matrix)
{
    matrix_t result = mat_create(matrix->rows, matrix->cols);

    if (!mat_is_null(&result))
    {
        size_t mem_size = result.imp__allocated;
        memcpy(result.data, matrix->data, mem_size);
    }

    return result;
}

static void imp__copy_values(matrix_t *dest, matrix_t *src)
{
    for (size_t row = 0; row < src->rows; row++)
    {
        for (size_t col = 0; col < src->cols; col++)
        {
            matrix_elem_t value;
            mat_get(src, row, col, &value);
            mat_set(dest, row, col, value);
        }
    }
}

int mat_resize(matrix_t *matrix, size_t new_rows, size_t new_cols)
{
    int status_code = mat_success;

    if (mat_is_null(matrix))
    {
        *matrix = mat_create(new_rows, new_cols);
        status_code = mat_is_null(matrix) ? mat_bad_create : mat_success;
    }

    // действительно нужно что-то делать?
    if (status_code == mat_success && (matrix->rows != new_rows || matrix->cols != new_cols))
    {
        matrix_t old_matrix = *matrix;

        size_t new_size = imp__calc_required_mat_size(new_rows, new_cols);
        matrix_elem_t **new_data = NULL;

        if (new_size > matrix->imp__allocated)
        {
            new_size = (size_t)((float)new_size * MAT_ALLOC_MULTIPLIER);
            new_data = (matrix_elem_t**)calloc(new_size, sizeof(char));

            if (new_data == NULL)
                status_code = mat_bad_alloc;
            else
            {
                matrix->data = new_data;
                matrix->rows = new_rows;
                matrix->cols = new_cols;
            }
        } 

        if (status_code == mat_success)
        {
            imp__reassign_row_ptrs(*matrix);
            imp__copy_values(matrix, &old_matrix);
            mat_free(&old_matrix);
        }
    }

    return status_code;
}

void swap(void *a, void *b, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        char temp = *((char *)a + i);
        *((char *)a + i) = *((char *)b + i);
        *((char *)b + i) = temp;
    }
}

void mat_swap_rows(matrix_t *matrix, size_t row_1, size_t row_2)
{
    if (0 <= row_1 && row_1 < matrix->rows && 0 <= row_2 && row_2 < matrix->rows)
    {
        matrix_elem_t *temp_row = matrix->data[row_1];
        matrix->data[row_1] = matrix->data[row_2];
        matrix->data[row_2] = temp_row;
    }
}

void mat_add_row(matrix_t *matrix, size_t dest_row, size_t src_row, matrix_elem_t k)
{
    for (size_t col = 0; col < matrix->cols; col++)
        matrix->data[dest_row][col] += k * matrix->data[src_row][col];
}

matrix_t mat_reduced(const matrix_t *matrix, size_t reduce_row, size_t reduce_col)
{
    matrix_t result = mat_create(matrix->rows - 1, matrix->cols - 1);

    if (!mat_is_null(&result))
    {
        for (size_t row = 0; row < matrix->rows; row++)
        {
            if (row == reduce_row)
                continue;

            size_t res_row = row > reduce_row ? row - 1 : row;
            for (size_t col = 0; col < matrix->cols; col++)
            {
                if (col == reduce_col)
                    continue;

                size_t res_col = col > reduce_col ? col - 1 : col;
                result.data[res_row][res_col] = matrix->data[row][col];
            }
        }
    }

    return result;
}

int mat_get(const matrix_t *matrix, size_t row, size_t col, matrix_elem_t *value)
{
    int status = mat_bad_index;

    if (!mat_is_null(matrix) && 0 <= row && row < matrix->rows && 0 <= col && col < matrix->cols)
    {
        *value = matrix->data[row][col];
        status = mat_success;
    }

    return status;
}

int mat_set(matrix_t *matrix, size_t row, size_t col, matrix_elem_t value)
{
    int status = mat_bad_index;

    if (!mat_is_null(matrix) && 0 <= row && row < matrix->rows && 0 <= col && col < matrix->cols)
    {
        matrix->data[row][col] = value;
        status = mat_success;
    }

    return status;
}

int mat_add(const matrix_t *mat_1, const matrix_t *mat_2, matrix_t *res)
{
    int status_code = mat_success;

    if (mat_is_null(mat_1) || mat_is_null(mat_2))
        status_code = mat_bad_matrix;
    else if (mat_1->rows != mat_2->rows || mat_1->cols != mat_2->cols)
        status_code = mat_bad_dims;
    else
        status_code = mat_resize(res, mat_1->rows, mat_2->cols);

    if (status_code == mat_success)
    {
        for (size_t row = 0; row < res->rows; row++)
        {
            for (size_t col = 0; col < res->cols; col++)
            {
                matrix_elem_t m_1, m_2;
                mat_get(mat_1, row, col, &m_1);
                mat_get(mat_2, row, col, &m_2);

                mat_set(res, row, col, m_1 + m_2);
            }
        }
    }

    return status_code;
}

int mat_mult(const matrix_t *mat_1, const matrix_t *mat_2, matrix_t *res)
{
    int status_code = EXIT_SUCCESS;
 
    if (mat_is_null(mat_1) || mat_is_null(mat_2))
        status_code = mat_bad_matrix;
    else if (mat_1->cols != mat_2->rows)
        status_code = mat_bad_dims;
    else
        status_code = mat_resize(res, mat_1->rows, mat_2->cols);

    if (status_code == mat_success)
    {
        for (size_t row = 0; row < res->rows; row++)
        {
            for (size_t col = 0; col < res->cols; col++)
            {
                matrix_elem_t r = 0;

                for (size_t mid = 0; mid < mat_1->cols; mid++)
                {
                    matrix_elem_t m_1, m_2;
                    mat_get(mat_1, row, mid, &m_1);
                    mat_get(mat_2, mid, col, &m_2);
                    r += m_1 * m_2;
                }

                mat_set(res, row, col, r);
            }
        }
    }

    return status_code;
}

/**
 * @brief Определяет позицию и модуль максимального по модулю элемента матрицы.
 */
static matrix_elem_t imp__get_prime(const matrix_t *matrix, size_t *prime_row, size_t *prime_col)
{
    matrix_elem_t prime = 0.0;

    for (size_t row = 0; row < matrix->rows; row++)
    {
        for (size_t col = 0; col < matrix->cols - 1; col++)
        {
            if (fabs(matrix->data[row][col]) > fabs(prime))
            {
                prime = matrix->data[row][col];
                *prime_row = row;
                *prime_col = col;
            }
        }
    }

    return prime;
}

static void imp__normalize_all_rows(matrix_t *matrix, size_t prime_row, size_t prime_col, matrix_elem_t prime)
{
    for (size_t row = 0; row < matrix->rows; row++)
    {
        if (row != prime_row)
        {
            // добавить главную строку к текущей
            matrix_elem_t m = -matrix->data[row][prime_col] / prime;
            mat_add_row(matrix, row, prime_row, m);
        }
    }
}

static void imp__copy_reduced_mat_values(matrix_t *matrix, matrix_t *reduced, size_t prime_row, size_t prime_col)
{
    for (size_t row = 0; row < matrix->rows; row++)
    {
        if (row != prime_row)
        {
            size_t reduced_row = row > prime_row ? row - 1 : row;
            for (size_t col = 0; col < matrix->cols; col++)
            {
                if (col != prime_col)
                {
                    size_t reduced_col = col > prime_col ? col - 1 : col;
                    matrix->data[row][col] = reduced->data[reduced_row][reduced_col];
                }
            }
        }
    }
}

/**
 * @brief Удаляет остальные элементы в столбце с главным элементом используя главную строку.
 */
static void imp__reset_prime_row(matrix_t *matrix, size_t prime_row, size_t prime_col, matrix_elem_t prime)
{
    for (size_t col = 0; col < matrix->cols - 1; col++)
    {
        if (col != prime_col)
        {
            for (size_t row = 0; row < matrix->rows; row++)
            {
                if (row != prime_row && matrix->data[row][col] != 0.0)
                {
                    matrix_elem_t m = matrix->data[prime_row][col] / matrix->data[row][col];
                    // matrix->data[prime_row][col] -= m * matrix->data[row][col];
                    matrix->data[prime_row][col] = 0.0;
                    matrix->data[prime_row][matrix->cols - 1] -= m * matrix->data[row][matrix->cols - 1];
                    break;
                }
            }
        }
    }

    // обработать последний элемент в главной строке
    matrix->data[prime_row][matrix->cols - 1] /= matrix->data[prime_row][prime_col];
    matrix->data[prime_row][prime_col] = 1.0;
}

static int imp__gauss_transform(matrix_t *matrix)
{
    int status_code = mat_success;

    // базовый случай рекурсии
    if (matrix->rows == 1)
    {
        // adjust last bias
        if (fabs(matrix->data[0][0]) < 1e-6)
            status_code = mat_singular_matrix;
        else
        {
            matrix->data[0][1] /= matrix->data[0][0];
            matrix->data[0][0] = 1.0;
        }
        return status_code;
    }

    // определить главную строку в матрице
    size_t prime_row = 0;
    size_t prime_col = 0;
    matrix_elem_t prime = imp__get_prime(matrix, &prime_row, &prime_col);

    if (fabs(prime) < 1.0e-6)
        status_code = mat_zero_matrix;
    else
    {
        imp__normalize_all_rows(matrix, prime_row, prime_col, prime);
        matrix_t reduced = mat_reduced(matrix, prime_row, prime_col);

        if (!mat_is_null(&reduced))
        {
            status_code = imp__gauss_transform(&reduced);

            if (status_code == mat_success)
            {
                // вернуть значения из обработанной матрицы в исходную
                imp__copy_reduced_mat_values(matrix, &reduced, prime_row, prime_col);
                imp__reset_prime_row(matrix, prime_row, prime_col, prime);
            }
        }

        mat_free(&reduced);
    }

    return status_code;
}

/**
 * @brief Переставляет строки матрицы местами, приводя её к диагональному виду.
 */
static void imp__rearrange_rows(matrix_t *matrix)
{
    for (size_t col = 0; col < matrix->cols - 2; col++)
    {
        for (size_t row = col; row < matrix->rows; row++)
        {
            if (fabs(matrix->data[row][col] - 1.0) < MAT_EPSILON)
            {
                swap((void *)(matrix->data + col), (void *)(matrix->data + row), sizeof(matrix_elem_t *));
                break;
            }
        }
    }
}

int mat_solve_sle(matrix_t *matrix, matrix_t *result)
{
    int status_code = mat_success;

    if (matrix->rows + 1 != matrix->cols)
        status_code = mat_bad_dims;
    else
    {
        status_code = mat_resize(result, matrix->rows, 1);

        if (status_code == mat_success)
        {
            // создать временную копию матрицы matrix для обработки
            matrix_t temp = mat_copy(matrix);

            if (mat_is_null(&temp))
                status_code = mat_bad_copy;
            else
            {
                status_code = imp__gauss_transform(&temp);
                if (status_code == mat_success)
                {
                    imp__rearrange_rows(&temp);
                    for (size_t row = 0; row < temp.rows; row++)
                        result->data[row][0] = temp.data[row][temp.cols - 1];
                }
            }

            // очистить память под временную матрицу
            mat_free(&temp);
        }

        if (status_code != mat_success)
            mat_free(result);
    }

    return status_code;
}
