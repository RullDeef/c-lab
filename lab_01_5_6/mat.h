#ifndef _MAT_H
#define _MAT_H 1

#ifndef _MAT_EPS
#define _MAT_EPS 0.0000001f
#endif

#include <math.h>

#ifdef _MAT_PRINTF
#include <stdio.h>
#endif

typedef float mat_num_t;


typedef struct
{
    mat_num_t x, y;
} vec2_t;


typedef struct
{
    mat_num_t data[4];
} mat2x2_t;


mat_num_t vec_length(vec2_t vec)
{
    mat_num_t x2 = vec.x * vec.x;
    mat_num_t y2 = vec.y * vec.y;
    return sqrtf(x2 + y2);
}

/*
vec2_t vec_add(vec2_t vec1, vec2_t vec2)
{
    vec2_t res;
    res.x = vec1.x + vec2.x;
    res.y = vec1.y + vec2.y;
    return res;
}
*/

vec2_t vec_sub(vec2_t vec1, vec2_t vec2)
{
    vec2_t res;
    res.x = vec1.x - vec2.x;
    res.y = vec1.y - vec2.y;
    return res;
}


mat_num_t mat_det(mat2x2_t mat)
{
    return mat.data[0] * mat.data[3] - mat.data[1] * mat.data[2];
}

/*
mat2x2_t mat_mult(mat2x2_t mat1, mat2x2_t mat2)
{
    mat2x2_t res;
    res.data[0] = mat1.data[0] * mat2.data[0] + mat1.data[1] * mat2.data[2];
    res.data[1] = mat1.data[0] * mat2.data[1] + mat1.data[1] * mat2.data[3];
    res.data[2] = mat1.data[2] * mat2.data[0] + mat1.data[3] * mat2.data[2];
    res.data[3] = mat1.data[2] * mat2.data[1] + mat1.data[3] * mat2.data[3];
    return res;
}
*/

vec2_t mat_apply(mat2x2_t mat, vec2_t vec)
{
    vec2_t res;
    res.x = mat.data[0] * vec.x + mat.data[1] * vec.y;
    res.y = mat.data[2] * vec.x + mat.data[3] * vec.y;
    return res;
}


mat2x2_t mat_inverse(mat2x2_t mat)
{
    mat_num_t det = mat_det(mat);
    mat2x2_t res;

    res.data[0] = 0;
    res.data[1] = 0;
    res.data[2] = 0;
    res.data[3] = 0;

    if (det < -_MAT_EPS || _MAT_EPS < det)
    {
        res.data[0] = mat.data[3] / det;
        res.data[1] = -mat.data[1] / det;
        res.data[2] = -mat.data[2] / det;
        res.data[3] = mat.data[0] / det;
    }
    
    return res;
}


mat2x2_t mat_from_vec2(vec2_t vec1, vec2_t vec2)
{
    mat2x2_t res;
    res.data[0] = vec1.x;
    res.data[1] = vec2.x;
    res.data[2] = vec1.y;
    res.data[3] = vec2.y;
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