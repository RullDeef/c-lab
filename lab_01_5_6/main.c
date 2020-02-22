#include <stdio.h>
#include <stdbool.h>
#include <math.h>


float area(float x1, float y1, float x2, float y2, float x3, float y3)
{
    return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
}

bool sub_intersect(float a, float b, float c, float d)
{
    if (a > b)
    {
        b += a;
        a = b - a;
        b -= a;
    }

    if (c > d)
    {
        d += c;
        c = d - c;
        d -= c;
    }

    return fmaxf(a, c) <= fminf(b, d);
}

bool intersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    return sub_intersect(x1, x2, x3, x4) && sub_intersect(y1, y2, y3, y4)
        && area(x1, y1, x2, y2, x3, y3) * area(x1, y1, x2, y2, x4, y4) <= 0
        && area(x3, y3, x4, y4, x1, y1) * area(x3, y3, x4, y4, x2, y2) <= 0;
}


bool input_point(float *x, float *y)
{
    char c;
    int argc = scanf("%f%c%f%c", x, &c, y, &c);

    return (argc != 4 && argc != EOF) || c != '\n';
}


int main()
{
    float x1, y1;
    float x2, y2;
    float x3, y3;
    float x4, y4;

    printf("Input point A(x, y): ");

    if (input_point(&x1, &y1))
    {
        printf("Incorrect input. Closing program...\n");
        return 1;
    }

    printf("Input point B(x, y): ");

    if (input_point(&x2, &y2))
    {
        printf("Incorrect input. Closing program...\n");
        return 1;
    }

    printf("Input point C(x, y): ");

    if (input_point(&x3, &y3))
    {
        printf("Incorrect input. Closing program...\n");
        return 1;
    }

    printf("Input point D(x, y): ");

    if (input_point(&x4, &y4))
    {
        printf("Incorrect input. Closing program...\n");
        return 1;
    }

    printf("Intersect: %d", intersect(x1, y1, x2, y2, x3, y3, x4, y4));

    return 0;
}
