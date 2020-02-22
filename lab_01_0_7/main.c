#include <stdio.h>
#include <stdbool.h>
#include <math.h>


float f(float x)
{
    return expf(x);
}

float s(float x, float eps)
{
    int n = 1;
    float term = 1;
    float res = 0;

    do
    {
        res += term;
        term *= x / n;
        n++;
    }
    while(fabsf(term) > eps);

    return res;
}

bool input_float(float* x)
{
    char c;
    int argc = scanf("%f%c", x, &c);

    return (argc != 2 && argc != EOF) || c != '\n';
}


int main()
{
    float x;
    float eps;

    printf("Input x: ");

    if(input_float(&x))
    {
        printf("Incorrect input. Closing program...\n");
        return 1;
    }

    printf("Input eps: ");

    if(input_float(&eps))
    {
        printf("Incorrect input. Closing program...\n");
        return 1;
    }

    if(eps <= 0)
    {
        printf("Eps must be positive number. Closing program...\n");
        return 1;
    }

    float f_x = f(x);
    float s_x = s(x, eps);

    printf("\ns(x) = %.6f\n", s_x);
    printf("f(x) = %.6f\n", f_x);
    printf("\nAbs error: %.6f\n", fabsf(s_x - f_x));
    printf("Rel error: %.6f\n", fabsf(1 - s_x / f_x));

    return 0;
}
