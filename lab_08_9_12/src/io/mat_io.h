#ifndef __MAT_IO_H_
#define __MAT_IO_H_

#include <stdio.h>
#include "../matrix/matrix.h"

#define MAT_IO_INT_PRECISION 0
#define MAT_IO_DOUBLE_PRECISION 6

typedef int (*input_fn_t)(FILE *, matrix_t *);
typedef int (*output_fn_t)(FILE *, const matrix_t *, int);

int mat_io_input_simple(FILE *file, matrix_t *matrix);
int mat_io_input_coordinate(FILE *file, matrix_t *matrix); // not implemented

int mat_io_output_simple(FILE *file, const matrix_t *matrix, int precision); // not implemented
int mat_io_output_coordinate(FILE *file, const matrix_t *matrix, int precision);

#endif // __MAT_IO_H_
