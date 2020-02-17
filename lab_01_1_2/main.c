#include <stdio.h>
#include <math.h>

int main()
{
    float a, b;
    float alpha;
    float perimeter;

    scanf("%f %f %f", &a, &b, &alpha);
    alpha *= 180 / M_PI;
    perimeter = a + b + (b - a) / cos(alpha);
    printf("Perimeter = %.5f", perimeter);
    return 0;
}
