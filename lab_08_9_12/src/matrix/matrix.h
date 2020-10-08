#ifndef __MATRIX_H_
#define __MATRIX_H_

#include <stdbool.h>

enum {
    mat_success = 0,
    mat_bad_create,
    mat_bad_dims,
    mat_bad_copy,
    mat_bad_alloc,
    mat_bad_resize,
    mat_zero_matrix
};

typedef double matrix_elem_t;

/*
    Объединенный подход 2:
        вся матрица и указатели на строки располагаются одним блоком.
*/
typedef struct
{
    size_t rows;
    size_t cols;
    size_t __allocated;
    matrix_elem_t **data;
} matrix_t;

matrix_t mat_null(void);
matrix_t mat_create(size_t rows, size_t cols);

bool mat_is_null(const matrix_t *matrix);
void mat_free(matrix_t *matrix);

matrix_t mat_copy(const matrix_t *matrix);
int mat_resize(matrix_t *matrix, size_t new_rows, size_t new_cols);

void mat_swap_rows(matrix_t *matrix, size_t row_1, size_t row_2);

// dest_row += k * src_row
void mat_add_row(matrix_t *matrix, size_t dest_row, size_t src_row, matrix_elem_t k);

matrix_t mat_reduced(const matrix_t *matrix, size_t reduce_row, size_t reduce_col);

matrix_elem_t mat_get(const matrix_t *matrix, size_t row, size_t col);
void mat_set(matrix_t *matrix, size_t row, size_t col, matrix_elem_t value);

int mat_add(const matrix_t *mat_1, const matrix_t *mat_2, matrix_t *res);
int mat_mult(const matrix_t *mat_1, const matrix_t *mat_2, matrix_t *res);

bool mat_is_square(const matrix_t *matrix);

/**
 * @brief Решает систему линейных уравнений с коэффициентами в
 * квадратной матрице matrix.
 * 
 * @param matrix - квадратная матрица коэффициентов
 * @param result - матрица-столбец решений (если найдено)
 * 
 * @return status_code  0 - успешное нахождение решения,
 *                      не 0 - ошибки при вычислении.
 */
int mat_solve_sle(matrix_t *matrix, matrix_t *result);

#endif // __MATRIX_H_
