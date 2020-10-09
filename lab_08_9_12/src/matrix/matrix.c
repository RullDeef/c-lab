#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "matrix.h"
#include <assert.h>

inline static size_t imp__calc_required_mat_size(size_t rows, size_t cols)
{
    return rows * (sizeof(matrix_elem_t *) + cols * sizeof(matrix_elem_t));
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
    // assert(rows > 0 && cols > 0);
    if (rows == 0 || cols == 0)
        return mat_null();

    matrix_t matrix = {
        .rows = rows,
        .cols = cols
    };

    /*           <- rows -->   <-cols->
        data = [ __ __ __ __ | __ __ __ : __ __ __ : __ __ __ : __ __ __ ]
    */
    matrix.imp__allocated = imp__calc_required_mat_size(rows, cols);

    // take more space for future manipulations
    matrix.imp__allocated = (size_t)((float)matrix.imp__allocated * 1.5f);

    matrix.data = (matrix_elem_t **)malloc(matrix.imp__allocated);

    if (matrix.data == NULL)
        matrix = mat_null();
    else
    {
        memset(matrix.data, 0, matrix.imp__allocated);

        // fill up row pointers
        for (size_t row = 0; row < rows; row++)
            matrix.data[row] = (matrix_elem_t *)(matrix.data + rows) + cols * row;
    }

    return matrix;
}

bool mat_is_null(const matrix_t *matrix)
{
    assert(matrix != NULL);
    return matrix->data == NULL;
}

void mat_free(matrix_t *matrix)
{
    assert(matrix != NULL);
    
    if (!mat_is_null(matrix))
    {
        free(matrix->data);
        *matrix = mat_null();
    }
}

matrix_t mat_copy(const matrix_t *matrix)
{
    assert(matrix != NULL);

    matrix_t result = mat_create(matrix->rows, matrix->cols);

    if (!mat_is_null(&result))
    {
        size_t mem_size = result.imp__allocated;
        memcpy(result.data, matrix->data, mem_size);
    }

    return result;
}

#include <stdio.h>

int mat_resize(matrix_t *matrix, size_t new_rows, size_t new_cols)
{
    int status_code = mat_success;

    if (mat_is_null(matrix))
    {
        *matrix = mat_create(new_rows, new_cols);
        if (mat_is_null(matrix))
            status_code = mat_bad_create;
    }

    // действительно нужно что-то делать?
    if (status_code == mat_success && (matrix->rows != new_rows || matrix->cols != new_cols))
    {
        matrix_t old_matrix = *matrix;
        
        size_t new_size = imp__calc_required_mat_size(new_rows, new_cols);
        matrix_elem_t **new_data = NULL;

        // check if realloc needed
        if (new_size > matrix->imp__allocated)
        {
            // allocate much more
            new_size = (size_t)((float)new_size * 1.5f);
            // warning - after realloc will be reading from memory that does not belong us
            new_data = (matrix_elem_t**)calloc(new_size, sizeof(char));

            if (new_data == NULL)
            {
                // fprintf(stderr, "resize: bad realloc for %lu bytes.\n", new_size);
                status_code = mat_bad_alloc;
            }
            else
            {
                // fprintf(stderr, "resize: reallocated %lu bytes.\n", new_size);
                matrix->data = new_data;

                matrix->rows = new_rows;
                matrix->cols = new_cols;
            }
        } 

        if (status_code == mat_success)
        {
            // reassign pointers
            for (size_t row = 0; row < new_rows; row++)
                matrix->data[row] = (matrix_elem_t *)(matrix->data + new_rows) + new_cols * row;

            // fprintf(stderr, "resize: pointers has been updated.\n");
            // fprintf(stderr, "resize: old matrix dims: %lu x %lu.\n", old_matrix.rows, old_matrix.cols);

            // copy old values to newly allocated memory
            // warning - will be reading from memory that does not belong us
            for (size_t row = 0; row < old_matrix.rows; row++)
            {
                for (size_t col = 0; col < old_matrix.cols; col++)
                {
                    matrix_elem_t value = mat_get(&old_matrix, row, col);
                    mat_set(matrix, row, col, value);
                }
            }

            mat_free(&old_matrix);
        }
    }

    return status_code;
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
    assert(0 <= dest_row && dest_row < matrix->rows);
    assert(0 <= src_row && src_row < matrix->rows);

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

matrix_elem_t mat_get(const matrix_t *matrix, size_t row, size_t col)
{
    if (!mat_is_null(matrix) && 0 <= row && row < matrix->rows && 0 <= col && col < matrix->cols)
        return matrix->data[row][col];
    else
        return 0.0;
}

void mat_set(matrix_t *matrix, size_t row, size_t col, matrix_elem_t value)
{
    if (!mat_is_null(matrix) && 0 <= row && row < matrix->rows && 0 <= col && col < matrix->cols)
        matrix->data[row][col] = value;
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
                matrix_elem_t m_1 = mat_get(mat_1, row, col);
                matrix_elem_t m_2 = mat_get(mat_2, row, col);

                matrix_elem_t r = m_1 + m_2;

                mat_set(res, row, col, r);
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
                    matrix_elem_t m_1 = mat_get(mat_1, row, mid);
                    matrix_elem_t m_2 = mat_get(mat_2, mid, col);

                    r += m_1 * m_2;
                }

                mat_set(res, row, col, r);
            }
        }
    }

    return status_code;
}

static matrix_elem_t imp__get_max_abs(const matrix_t *matrix, size_t *prime_row, size_t *prime_col)
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

// recursive function
static int imp__gauss_transform(matrix_t *matrix)
{
    int status_code = mat_success;

    // base case for recursion
    if (matrix->rows == 1)
    {
        // adjust last bias
        if (matrix->data[0][0] == 0.0)
            status_code = mat_singular_matrix;
        else
        {
            matrix->data[0][1] /= matrix->data[0][0];
            matrix->data[0][0] = 1.0;
        }
        return status_code;
    }

    // determine prime row
    size_t prime_row = 0;
    size_t prime_col = 0;
    matrix_elem_t prime = imp__get_max_abs(matrix, &prime_row, &prime_col);

    if (prime == 0)
        status_code = mat_zero_matrix;
    else
    {
        // normalize all rows
        for (size_t row = 0; row < matrix->rows; row++)
        {
            if (row == prime_row)
                continue;

            // add prime row to current
            matrix_elem_t m = -matrix->data[row][prime_col] / prime;
            mat_add_row(matrix, row, prime_row, m);
        }

        // reduce matrix
        matrix_t reduced = mat_reduced(matrix, prime_row, prime_col);

        if (!mat_is_null(&reduced))
        {
            status_code = imp__gauss_transform(&reduced);

            if (status_code == mat_success)
            {
                // copy matrix values back
                for (size_t row = 0; row < matrix->rows; row++)
                {
                    if (row == prime_row)
                        continue;

                    size_t reduced_row = row > prime_row ? row - 1 : row;
                    for (size_t col = 0; col < matrix->cols; col++)
                    {
                        if (col == prime_col)
                            continue;

                        size_t reduced_col = col > prime_col ? col - 1 : col;
                        matrix->data[row][col] = reduced.data[reduced_row][reduced_col];
                    }
                }

                // elliminate non-primes in prime's row
                for (size_t col = 0; col < matrix->cols - 1; col++)
                {
                    if (col == prime_col)
                        continue;
                    
                    for (size_t row = 0; row < matrix->rows; row++)
                    {
                        if (row == prime_row)
                            continue;

                        if (matrix->data[row][col] != 0.0)
                        {
                            matrix_elem_t m = matrix->data[prime_row][col] / matrix->data[row][col];
                            matrix->data[prime_row][col] -= m * matrix->data[row][col];
                            // adjust biases too
                            matrix->data[prime_row][matrix->cols - 1] -= m * matrix->data[row][matrix->cols - 1];
                            break;
                        }
                    }
                }

                // adjust last bias
                matrix->data[prime_row][matrix->cols - 1] /= matrix->data[prime_row][prime_col];
                matrix->data[prime_row][prime_col] = 1.0;
            }
        }

        mat_free(&reduced);
    }

    return status_code;
}

static void imp__rearrange_rows(matrix_t *matrix)
{
    for (size_t col = 0; col < matrix->cols - 2; col++)
    {
        for (size_t row = col; row < matrix->rows; row++)
        {
            if (matrix->data[row][col] == 1.0)
            {
                mat_swap_rows(matrix, col, row);
                break;
            }
        }
    }
}

// необходимое условие - rows + 1 == cols
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
