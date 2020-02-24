#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535
#endif

int main()
{
    float a, b;
    float alpha;
    float area;

    printf("Base A: ");
    scanf("%f", &a);

    printf("Base B: ");
    scanf("%f", &b);

    printf("Angle: ");
    scanf("%f", &alpha);
    
    alpha *= M_PI / 180;
    area = fabsf(a * a - b * b) / 4 * tanf(alpha);
    
    printf("Area = %.5f", area);
    
    return 0;
}
