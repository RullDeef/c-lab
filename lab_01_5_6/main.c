#include <stdio.h>
#include <stdbool.h>
#include <math.h>

//#define _MAT_PRINTF 1
#include "mat.h"


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
    bool res = false;

    if (input_points(arr, 4))
    {
        printf("Invalid input. Closing program...\n");
        return 1;
    }

    a = arr[0]; b = arr[1]; c = arr[2]; d = arr[3];

    vec_printf(a);
    vec_printf(b);
    vec_printf(c);
    vec_printf(d);

    mat2x2_t mat = mat_from_vec2(vec_sub(b, a), vec_sub(d, c));

    mat_printf(mat);

    float det = mat_det(mat);

    if (fabsf(det) > _MAT_EPS)
    {
        vec2_t st = mat_apply(mat_inverse(mat), vec_sub(c, a));

        vec_printf(st);

        res = ((-_MAT_EPS < st.x && st.x < 1 + _MAT_EPS) &&
            (-1 - _MAT_EPS < st.y && st.y < _MAT_EPS)) ||
                ((-_MAT_EPS < st.y && st.y < 1 + _MAT_EPS) &&
                    (-1 - _MAT_EPS < st.x && st.x < _MAT_EPS));
    }

    printf("%d", res);

    return 0;
}
