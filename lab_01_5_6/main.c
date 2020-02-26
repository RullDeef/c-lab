#include <stdio.h>
#include <stdbool.h>
#include <math.h>


#define EPS 1e-8


typedef struct
{
    float x, y;
} point_t;


point_t subtract(point_t a, point_t b)
{
    point_t res;
    res.x = a.x - b.x;
    res.y = a.y - b.y;
    return res;
}


float length(point_t vect)
{
    return sqrtf(vect.x * vect.x + vect.y * vect.y);
}


float slant_product(point_t a, point_t b)
{
    return a.x * b.y - b.x * a.y;
}


bool is_too_close(point_t a, point_t b)
{
    return length(subtract(a, b)) < EPS;
}


bool has_same_ends(point_t a, point_t b, point_t c, point_t d)
{
    bool a_c = is_too_close(a, c);
    bool a_d = is_too_close(a, d);
    bool b_c = is_too_close(b, c);
    bool b_d = is_too_close(b, d);

    return a_c || a_d || b_c || b_d;
}


bool in_between(point_t a, point_t b, point_t c)
{
    point_t v = subtract(b, a);
    point_t u = subtract(c, a);

    bool xx = (c.x - a.x) * (c.x - b.x) < 0;
    bool yy = (c.y - a.y) * (c.y - b.y) < 0;
    
    return fabsf(slant_product(u, v)) < EPS && xx && yy;
}


/**
 * Checks if points c and d lays on different sides of line ab
 */
bool intersect_line(point_t a, point_t b, point_t c, point_t d)
{
    point_t v1 = subtract(b, a);
    point_t v2 = subtract(c, a);
    point_t v3 = subtract(d, a);

    float s1 = slant_product(v2, v1);
    float s2 = slant_product(v1, v3);

    return s1 * s2 > 0;
}


bool input_point(point_t *p)
{
    char c;
    int argc = scanf("%f%c%f%c", &p->x, &c, &p->y, &c);
    return argc < 3 || (argc == 4 && c != '\n' && c != ' ');
}


int main()
{
    point_t a, b, c, d;
    bool res;

    //printf("Input point A(x, y): ");

    if (input_point(&a))
    {
        printf("Incorrect input. Closing program...\n");
        return 1;
    }

    //printf("Input point B(x, y): ");

    if (input_point(&b))
    {
        printf("Incorrect input. Closing program...\n");
        return 1;
    }

    //printf("Input point C(x, y): ");

    if (input_point(&c))
    {
        printf("Incorrect input. Closing program...\n");
        return 1;
    }

    //printf("Input point D(x, y): ");

    if (input_point(&d))
    {
        printf("Incorrect input. Closing program...\n");
        return 1;
    }

    res = has_same_ends(a, b, c, d);

    if (!res)
    {
        res = in_between(a, b, c) || in_between(a, b, d);
        res = res || in_between(c, d, a) || in_between(c, d, b);
    }

    if (!res)
        res = intersect_line(a, b, c, d) && intersect_line(c, d, a, b);

    printf("%d", res);

    return 0;
}
