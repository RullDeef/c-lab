#ifndef _MAT_H
#define _MAT_H 1

#ifndef _MAT_EPS
#define _MAT_EPS 0.0000001f
#endif

#ifdef _MAT_PRINTF
#include <stdio.h>
#endif

typedef float __num_t;


typedef struct
{
    __num_t x, y;
} vec2_t;


#define zero_vec2  (vec2_t) { 0, 0 };
#define one_vec2   (vec2_t) { 1, 1 };


typedef struct
{
    __num_t data[4];
} mat2x2_t;


#define zero_mat     (mat2x2_t) {{ 0, 0, 0, 0 }};
#define identity_mat (mat2x2_t) {{ 1, 0, 0, 1 }};


vec2_t vec_add(vec2_t vec1, vec2_t vec2)
{
    vec2_t res = (vec2_t) { vec1.x + vec2.x, vec1.y + vec2.y };
    return res;
}

vec2_t vec_sub(vec2_t vec1, vec2_t vec2)
{
    vec2_t res = (vec2_t) { vec1.x - vec2.x, vec1.y - vec2.y };
    return res;
}

__num_t mat_det(mat2x2_t mat)
{
    return mat.data[0] * mat.data[3] - mat.data[1] * mat.data[2];
}


mat2x2_t mat_mult(mat2x2_t mat1, mat2x2_t mat2)
{
    mat2x2_t res = (mat2x2_t) {{
        mat1.data[0] * mat2.data[0] + mat1.data[1] * mat2.data[2],
        mat1.data[0] * mat2.data[1] + mat1.data[1] * mat2.data[3],
        mat1.data[2] * mat2.data[0] + mat1.data[3] * mat2.data[2],
        mat1.data[2] * mat2.data[1] + mat1.data[3] * mat2.data[3] }};
    return res;
}


vec2_t mat_apply(mat2x2_t mat, vec2_t vec)
{
    vec2_t res = (vec2_t) {
        mat.data[0] * vec.x + mat.data[1] * vec.y,
        mat.data[2] * vec.x + mat.data[3] * vec.y };
    return res;
}


mat2x2_t mat_inverse(mat2x2_t mat)
{
    __num_t det = mat_det(mat);

    if (-_MAT_EPS < det && det < _MAT_EPS)
        return zero_mat;
    
    mat2x2_t res = (mat2x2_t) {{ mat.data[3] / det, -mat.data[1] / det,
        -mat.data[2] / det, mat.data[0] / det }};
    return res;
}


mat2x2_t mat_from_vec2(vec2_t vec1, vec2_t vec2)
{
    mat2x2_t res = (mat2x2_t) {{ vec1.x, vec2.x, vec1.y, vec2.y }};
    return res;
}


void vec_printf(vec2_t vec)
{
#ifdef _MAT_PRINTF
    printf("{%5.3f %5.3f}\n", vec.x, vec.y);
#endif
}


void mat_printf(mat2x2_t mat)
{
#ifdef _MAT_PRINTF
    printf("[%5.3f %5.3f\n %5.3f %5.3f]\n", mat.data[0], mat.data[1], mat.data[2], mat.data[3]);
#endif
}


#endif