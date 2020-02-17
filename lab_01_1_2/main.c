#include <stdio.h>
#include <math.h>

int main()
{
    float a, b;
    float alpha;
    float perimeter;

    scanf("%f %f %f", &a, &b, &alpha);
    perimeter = a + b + (b - a) / cos(alpha);
    printf("Perimeter = %.3f", perimeter);
    return 0;
}
