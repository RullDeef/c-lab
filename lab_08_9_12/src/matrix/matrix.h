#ifndef __MATRIX_H_
#define __MATRIX_H_

#include <stdbool.h>

enum {
    mat_success = 0,
    mat_bad_dims
};

typedef long matrix_elem_t;

/*
    Объединенный подход 2:
        вся матрица и указатели на строки располагаются одним блоком.
*/
typedef struct
{
    size_t rows;
    size_t cols;
    matrix_elem_t **data;
} matrix_t;

matrix_t mat_null(void);
matrix_t mat_create(size_t rows, size_t cols);

bool mat_is_null(const matrix_t *matrix);
void mat_free(matrix_t *matrix);

matrix_t* mat_resize(matrix_t *matrix, size_t new_rows, size_t new_cols);

matrix_elem_t mat_get(const matrix_t *matrix, size_t row, size_t col);
void mat_set(matrix_t *matrix, size_t row, size_t col, matrix_elem_t value);

int mat_add(const matrix_t *mat_1, const matrix_t *mat_2, matrix_t *res);
int mat_mult(const matrix_t *mat_1, const matrix_t *mat_2, matrix_t *res);

#endif // __MATRIX_H_
