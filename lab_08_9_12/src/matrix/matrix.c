#include <stdlib.h>
#include <memory.h>
#include "matrix.h"
#include <assert.h>

matrix_t mat_null(void)
{
    return (matrix_t) {
        .rows = 0,
        .cols = 0,
        .data = NULL
    };
}

matrix_t mat_create(size_t rows, size_t cols)
{
    assert(rows > 0 && cols > 0);

    matrix_t matrix = {
        .rows = rows,
        .cols = cols
    };

    /*           <- rows -->   <-cols->
        data = [ __ __ __ __ | __ __ __ : __ __ __ : __ __ __ : __ __ __ ]
    */
    size_t memsize = rows * (sizeof(matrix_elem_t*) + cols * sizeof(matrix_elem_t));
    matrix.data = (matrix_elem_t **)malloc(memsize);

    if (matrix.data == NULL)
        matrix = mat_null();
    else
    {
        memset(matrix.data, 0, memsize);

        // fill up row pointers
        for (size_t row = 0; row < rows; row++)
            matrix.data[row] = (matrix_elem_t *)(matrix.data + rows) + cols * row;
    }

    return matrix;
}

bool mat_is_null(const matrix_t *matrix)
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

matrix_t *mat_resize(matrix_t *matrix, size_t new_rows, size_t new_cols);

matrix_elem_t mat_get(const matrix_t *matrix, size_t row, size_t col)
{
    assert(!mat_is_null(matrix));
    assert(0 <= row && row < matrix->rows);
    assert(0 <= col && col < matrix->cols);

    return matrix->data[row][col];
}

void mat_set(matrix_t *matrix, size_t row, size_t col, matrix_elem_t value)
{
    assert(!mat_is_null(matrix));
    assert(0 <= row && row < matrix->rows);
    assert(0 <= col && col < matrix->cols);

    matrix->data[row][col] = value;
}

int mat_add(const matrix_t *mat_1, const matrix_t *mat_2, matrix_t *res)
{
    assert(!mat_is_null(mat_1) && !mat_is_null(mat_2) && !mat_is_null(res));

    int status_code = EXIT_SUCCESS;

    if (mat_1->rows != mat_2->rows || mat_1->cols != mat_2->cols ||
        res->rows != mat_1->rows || res->cols != mat_1->cols)
        status_code = EXIT_FAILURE;
    else
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
    assert(!mat_is_null(mat_1) && !mat_is_null(mat_2) && !mat_is_null(res));

    int status_code = EXIT_SUCCESS;

    if (mat_1->rows != res->rows || mat_1->cols != mat_2->rows || res->cols != mat_2->cols)
        status_code = EXIT_FAILURE;
    else
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
