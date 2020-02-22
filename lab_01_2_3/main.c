#include <stdio.h>

int main()
{
    float r1, r2, r3;
    float r;

    printf("Enter three numbers: ");
    scanf("%f %f %f", &r1, &r2, &r3);

    r = r1 * r2 * r3 / (r1 * r2 + r1 * r3 + r2 * r3);

    printf("Total resistance = %.5f", r);

    return 0;
}
