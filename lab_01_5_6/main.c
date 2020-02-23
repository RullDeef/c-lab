#include <stdio.h>
#include <stdbool.h>
#include <math.h>


typedef struct {
    float x, y;
} point_t;


point_t subtract(point_t a, point_t b)
{
    point_t res;
    res.x = a.x - b.x;
    res.y = a.y - b.y;
    return res;
}


float slant_product(point_t a, point_t b)
{
    return a.x * b.y - b.x * a.y;
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
    return (argc != 4 && argc != EOF) || c != '\n';
}


int main()
{
    point_t a, b, c, d;
    bool res;

    printf("Input point A(x, y): ");

    if (input_point(&a))
    {
        printf("Incorrect input. Closing program...\n");
        return 1;
    }

    printf("Input point B(x, y): ");

    if (input_point(&b))
    {
        printf("Incorrect input. Closing program...\n");
        return 1;
    }

    printf("Input point C(x, y): ");

    if (input_point(&c))
    {
        printf("Incorrect input. Closing program...\n");
        return 1;
    }

    printf("Input point D(x, y): ");

    if (input_point(&d))
    {
        printf("Incorrect input. Closing program...\n");
        return 1;
    }

    res = intersect_line(a, b, c, d) && intersect_line(c, d, a, b);

    printf("Intersect: %d", res);

    return 0;
}
