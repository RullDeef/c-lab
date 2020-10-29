/**
 * @file mat_io.h
 * @breif Модуль для ввода-вывода динамических матриц.
 */

#ifndef __MAT_IO_H_
#define __MAT_IO_H_

#include <stdio.h>
#include "../matrix/matrix.h"
#include "../status_codes.h"
#include "../io/app_params.h"

/**
 * @brief Точность для вывода целых чисел.
 */
#define MAT_IO_INT_PRECISION 0

/**
 * @brief Точность для вывода вещественных чисел.
 */
#define MAT_IO_DOUBLE_PRECISION 6

typedef int (*input_fn_t)(FILE *, matrix_t *);
typedef int (*output_fn_t)(FILE *, const matrix_t *, int);

/**
 * @brief Считывание матрицы в обычном формате.
 */
int mat_io_input_simple(FILE *file, matrix_t *matrix);

/**
 * @brief Считывание матрицы в координатном формате.
 */
// int mat_io_input_coordinate(FILE *file, matrix_t *matrix); // not implemented

/**
 * @brief Вывод матрицы в обычном формате с заданной точностью.
 */
// int mat_io_output_simple(FILE *file, const matrix_t *matrix, int precision); // not implemented

/**
 * @brief Вывод матрицы в координатном формате с заданной точностью.
 */
int mat_io_output_coordinate(FILE *file, const matrix_t *matrix, int precision);

/**
 * @brief Считывает матрицу из файла передоваемой функцией чтения.
 */
status_code_t input_matrix(filename_t filename, matrix_t *matrix, input_fn_t input_ft);

/**
 * @brief Выводит матрицу в файл с помощью передаваемой функции записи.
 * 
 * @param pricision - кол-во цифр после точки.
 */
status_code_t output_matrix(filename_t filename, const matrix_t *matrix, output_fn_t output_fn, int precision);

#endif // __MAT_IO_H_
