#include <stdio.h>
#include <stdbool.h>
#include <math.h>

//#define _MAT_PRINTF 1
#include "mat.h"


/**
 * returns a <= x <= b with epsilon from mat.h
 */
bool in_range(float x, float a, float b)
{
    return (a - _MAT_EPS < x) && (x < b + _MAT_EPS);
}


bool input_points(vec2_t arr[], const int size)
{
    char c;
    int argc;

    for(int i = 0; i < size; i++)
    {
        argc = scanf("%f%c%f%c", &(arr[i].x), &c, &(arr[i].y), &c);
        if (argc < 3 || (argc == 4 && c != '\n' && c != ' '))
            return true;
    }

    return false;
}


int main()
{
    vec2_t a, b, c, d;
    vec2_t arr[4];

    if (input_points(arr, 4))
    {
        printf("Invalid input. Closing program...\n");
        return 1;
    }

    a = arr[0];
    b = arr[1];
    c = arr[2];
    d = arr[3];

    vec_printf(a);
    vec_printf(b);
    vec_printf(c);
    vec_printf(d);

    vec2_t u = vec_sub(b, a);
    vec2_t v = vec_sub(d, c);
    mat2x2_t mat = mat_from_vec2(u, v);
    mat_printf(mat);

    float det = mat_det(mat);

    if (vec_length(u) < _MAT_EPS || vec_length(v) < _MAT_EPS)
    {
        // printf("Some pair of points are the same.");
        return 1;
    }
    else if (fabsf(det) < _MAT_EPS)
    {
        // printf("Lines are parallel.");
        return 1;
    }
    else
    {
        vec2_t st = mat_apply(mat_inverse(mat), vec_sub(c, a));
        vec_printf(st);

        bool res = (in_range(st.x, 0, 1) && in_range(st.y, -1, 0)) ||
            (in_range(st.x, -1, 0) && in_range(st.y, 0, 1));
        
        printf("%d", res);
        return 0;
    }
}
