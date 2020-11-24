#ifndef __MATRIX_H_
#define __MATRIX_H_

#include <stdio.h>
#include <stdbool.h>

struct matrix
{
    int rows;
    int cols;
    int **data;
};

struct matrix mat_create(int rows, int cols);
struct matrix mat_invalid(void);
void mat_destroy(struct matrix mat);

bool mat_is_valid(struct matrix mat);

struct matrix mat_read(FILE *file);
int mat_print(FILE *file, struct matrix mat);

struct matrix mat_make_square(struct matrix mat);
struct matrix mat_expand(struct matrix mat, int z);

struct matrix mat_mult(struct matrix left, struct matrix right);
struct matrix mat_pow(struct matrix mat, int q);

#endif // __MATRIX_H_
